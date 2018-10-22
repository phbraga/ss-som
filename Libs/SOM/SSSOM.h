/*
 * SSSOM.h
 *
 *  Created on: 2017
 *      Author: Pedro Magalhaes
 */

#ifndef SSSOM_H_
#define SSSOM_H_

#include <set>
#include <map>
#include <vector>
#include "Mesh.h"
#include "MatUtils.h"
#include "MatVector.h"
#include "MatMatrix.h"
#include "SOM.h"
#include "SSSOMNode.h"

#define qrt(x) ((x)*(x))

using namespace std;

class SSSOM : public SOM<SSSOMNode> {
public:
    uint maxNodeNumber;
    int epochs;
    float minwd;
    float e_b;
    float e_n;

    float push_rate;
    
    TNumber dsbeta; //Taxa de aprendizagem
    TNumber epsilon_ds; //Taxa de aprendizagem
    float age_wins;       //period to remove nodes
    float lp;           //remove percentage threshold
    float a_t;

    int nodeID;
    
    inline float activation(TNode &node, const TVector &w) {

        float distance = 0;

        for (uint i = 0; i < w.size(); i++) {
            distance += node.ds[i] * qrt((w[i] - node.w[i]));
        }

        float sum = node.ds.sum();
        return (sum / (sum + distance + 0.0000001));
    }
    
    inline float wdist(const TNode &node1, const TNode &node2) {
        float distance = 0;

        for (uint i = 0; i < node1.ds.size(); i++) {
            distance +=  qrt((node1.ds[i] - node2.ds[i]));
        }
        
        return sqrt(distance);
    }

    inline void updateNode(TNode &node, const TVector &w, TNumber e) {
        //update averages
        for (uint i = 0; i < node.a.size(); i++) {
            //update neuron weights
            float distance = fabs(w[i] - node.w[i]);
            node.a[i] = e*dsbeta* distance + (1 - e*dsbeta) * node.a[i];
        }

        float max = node.a.max();
        float min = node.a.min();
        float average = node.a.mean();


        //update neuron ds weights
        for (uint i = 0; i < node.a.size(); i++) {
            if ((max - min) != 0) {
                //node.ds[i] = 1 - (node.a[i] - min) / (max - min);
                node.ds[i] = 1/(1+exp((node.a[i]-average)/((max - min)*epsilon_ds)));
            } else
                node.ds[i] = 1;
        }

        //Passo 6.1: Atualiza o peso do vencedor
        //Atualiza o nó vencedor
        node.w = node.w + e * (w - node.w);
    }

    SSSOM& updateConnections(TNode *node) {
        
        TPNodeSet::iterator itMesh = meshNodeSet.begin();
            
        while (itMesh != meshNodeSet.end()) {
            if (*itMesh != node) {
                if (checkConnectivity(node, *itMesh)) {
                    if (!isConnected(node, *itMesh))
                    connect(node, *itMesh);
                } else {
                    if (isConnected(node, *itMesh))
                        disconnect(node, *itMesh);
                }
            }
            itMesh++;
        }
        return *this;
    }
    
    bool checkConnectivity(TNode *node1, TNode *node2) {
        
        if((node1->cls == noCls || node2->cls == noCls || node1->cls == node2->cls) && wdist(*node1, *node2)<minwd) {
            return true;
        }
        
        return false;  
    }
    
    SSSOM& updateAllConnections() {

        //Conecta todos os nodos semelhantes
        TPNodeSet::iterator itMesh1 = meshNodeSet.begin();
        while (itMesh1 != meshNodeSet.end()) {
            TPNodeSet::iterator itMesh2 = meshNodeSet.begin();
            
            while (itMesh2 != meshNodeSet.end()) {
                if (*itMesh1!= *itMesh2) {
                    if (checkConnectivity(*itMesh1, *itMesh2)) {
                        if (!isConnected(*itMesh1, *itMesh2))
                        connect(*itMesh1, *itMesh2);
                    } else {
                        if (isConnected(*itMesh1, *itMesh2))
                            disconnect(*itMesh1, *itMesh2);
                    }
                }
                itMesh2++;
            }
            
            itMesh1++;
        }

        return *this;
    }

    SSSOM& removeLoosers() {
        TPNodeSet::iterator itMesh = meshNodeSet.begin();
        while (itMesh != meshNodeSet.end()) {
            if (meshNodeSet.size()<2)
                break;

            if ((*itMesh)->wins < step*lp) {
                eraseNode((*itMesh));
                itMesh = meshNodeSet.begin();
                
            } else {
                itMesh++;
                
            }
        }

        return *this;
    }
    
    SSSOM& trainningStep(int row,  std::vector<int> groups, std::map<int, int> &groupLabels) {
        TVector v(data.cols());
        for (uint l = 0; l < data.cols(); l++)
                v[l] = data[row][l];
        
        chooseTrainingType(v, groups[row]);
        
        return *this;
    }
    
    void runTrainingStep(bool sorted, std::vector<int> groups, std::map<int, int> &groupLabels) {
        if (sorted) {
            trainningStep(step%data.rows(), groups, groupLabels);
        } else {
            trainningStep(rand()%data.rows(), groups, groupLabels);
        }
    }
    
    void chooseTrainingType(TVector &v, int cls) {
        
        if (cls != noCls) { 
            updateMapSup(v, cls);
        } else { 
            updateMap(v); 
        }
    }
    
    SSSOM& finishMapFixed(bool sorted, std::vector<int> groups, std::map<int, int> &groupLabels) {

        dbgOut(1) << "Finishing map with: " << meshNodeSet.size() << endl;
        while (step!=1) { // finish the previous iteration
            runTrainingStep(sorted, groups, groupLabels);
        }
        maxNodeNumber = meshNodeSet.size(); //fix mesh max size
        
        dbgOut(1) << "Finishing map with: " << meshNodeSet.size() << endl;
        
        //step equal to 2
        runTrainingStep(sorted, groups, groupLabels);
        
        while (step!=1) {
            runTrainingStep(sorted, groups, groupLabels);
        }
        
        dbgOut(1) << "Finishing map with: " << meshNodeSet.size() << endl;
        
        return *this;
    }

    SSSOM& resetWins() {

        //Remove os perdedores
        TPNodeSet::iterator itMesh = meshNodeSet.begin();
        while (itMesh != meshNodeSet.end()) {
             (*itMesh)->wins = 0;
             itMesh++;
        }

        return *this;
    }

    TNode* createNodeMap (const TVector& w, int cls) {
        // cria um novo nodo na posição da amostra
        TVector wNew(w);
        TNode *nodeNew = createNode(nodeID, wNew);
        nodeID++;
        nodeNew->cls = cls;
        nodeNew->wins = 0;

        updateConnections(nodeNew);
        
        return nodeNew;
    }
    
    void ageWinsCriterion(){
        
        //Passo 9:Se atingiu age_wins
        if (step >= age_wins) {
            
            removeLoosers();
            resetWins();
            updateAllConnections();
            
            step = 0;
        }
    }
    
    SSSOM& updateMap(const TVector &w) {

        using namespace std;
        
        if (meshNodeSet.empty()) {
            createNodeMap(w, noCls);
            
        } else {
            TNode *winner1 = 0;

            //Passo 3 : encontra o nó vencedor
            winner1 = getFirstWinner(w); //winner
        
            //Passo 6: Calcula a atividade do nó vencedor
            TNumber a = activation(*winner1, w); //DS activation
            //Se a ativação obtida pelo primeiro vencedor for menor que o limiar
            //e o limite de nodos não tiver sido atingido
                      
            if ((a < a_t) && (meshNodeSet.size() < maxNodeNumber)) {
                createNodeMap(w, noCls);
                
            } else if (a >= a_t) { // caso contrário
                
                winner1->wins++;
                // Atualiza o peso do vencedor
                updateNode(*winner1, w, e_b);

                //Passo 6.2: Atualiza o peso dos vizinhos
                TPNodeConnectionMap::iterator it;
                for (it = winner1->nodeMap.begin(); it != winner1->nodeMap.end(); it++) {            
                    TNode* node = it->first;
                    updateNode(*node, w, e_n);
                }
            }
        }

        ageWinsCriterion();
        
        step++;
        
        return *this;
    }
    
    SSSOM& updateMapSup(const TVector& w, int cls) {
        using namespace std;
        
        if (meshNodeSet.empty()) { // mapa vazio, primeira amostra
            createNodeMap(w, cls);
            
        } else {
            TNode *winner1 = 0;
            winner1 = getFirstWinner(w); // encontra o nó vencedor
 
            if (winner1->cls == cls || winner1->cls == noCls) { // winner1 representativo e da mesma classe da amostra 
                    
                if ((winner1->act < a_t) && (meshNodeSet.size() < maxNodeNumber)) {
                    // cria um novo nodo na posição da amostra
                    createNodeMap(w, cls);
                    
                } else if (winner1->act >= a_t){
                    winner1->wins++;
                    winner1->cls = cls;
                    updateConnections(winner1);
                    updateNode(*winner1, w, e_b);
                    
                    TPNodeConnectionMap::iterator it;
                    for (it = winner1->nodeMap.begin(); it != winner1->nodeMap.end(); it++) {            
                        TNode* node = it->first;
                        updateNode(*node, w, e_n);
                    }
                } 
            } else { // winner tem classe diferente da amostra
                // caso winner seja de classe diferente, checar se existe algum
                // outro nodo no mapa que esteja no raio a_t da nova amostra e
                // que pertença a mesma classe da mesma

                handleDifferentClass(winner1, w, cls);
            }   
        }

        ageWinsCriterion();
        
        step++;
        
        return *this;
    }
    
    void handleDifferentClass(TNode *winner1, const TVector& w, int cls) {
        TNode *newWinner = winner1;
        while((newWinner = getNextWinner(newWinner)) != NULL) { // saiu do raio da ativação -> não há um novo vencedor
            if (newWinner->cls == noCls || newWinner->cls == cls) { // novo vencedor valido encontrado
                break;
            }
        }

        if (newWinner != NULL) { // novo winner de acordo com o raio de a_t
            // empurrar o primeiro winner que tem classe diferente da amostra
            updateNode(*winner1, w, -push_rate);
            
            newWinner->wins++;
            
            // puxar o novo vencedor
            updateNode(*newWinner, w, e_b);
            
            TPNodeConnectionMap::iterator it;
            for (it = newWinner->nodeMap.begin(); it != newWinner->nodeMap.end(); it++) {            
                TNode* node = it->first;
                updateNode(*node, w, e_n);
            }
            
        } else if (meshNodeSet.size() < maxNodeNumber) {
            // cria um novo nodo na posição da amostra
            createNodeMap(w, cls);
        } 
    }

    virtual TNode *getFirstWinner(const TVector &w){
        TNode *winner = 0;
        
        winner = (*Mesh<TNode>::meshNodeSet.begin());
        winner->act = activation(*winner, w);

        TNumber act = winner->act;
        
        TPNodeSet::iterator it;
        it = Mesh<TNode>::meshNodeSet.begin();
        it++;
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {
            (*it)->act = activation(*(*it), w);
            if ((*it)->act > act) {
                act = (*it)->act;
                winner = (*it);
            }
        }

        return winner;
    }

    virtual TNode *getNextWinner(TNode *previowsWinner) {
        previowsWinner->act = 0;
        
        TNode *winner = 0;
        winner = (*Mesh<TNode>::meshNodeSet.begin());
        TNumber winnerAct = winner->act;

        TPNodeSet::iterator it;
        it = Mesh<TNode>::meshNodeSet.begin();
        it++;
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {

            if ((*it)->act > winnerAct) {
                winnerAct = (*it)->act;
                winner = (*it);
            }
        }
        
        if (winnerAct < a_t)
            return NULL;

        return winner;
    }
    
    virtual inline SSSOMNode* getWinnerResult(const TVector &w) {
        TNode *winner = getFirstWinner(w);
        
        if (winner->cls == noCls) {
            TNode *newWinner = winner;
            while((newWinner = getNextWinner(newWinner)) != NULL) { // saiu do raio da ativação -> não há um novo vencedor
                if (newWinner->cls != noCls) { // novo vencedor valido encontrado
                    break;
                }
            }

            if (newWinner != NULL && newWinner->cls != noCls) 
                return newWinner;
        }

        return winner;
    }
    
    bool isNoise(float activation) {
        return activation < a_t;
    }

    void reset(int dimw) {
        SSSOM::dimw = dimw;
        step = 0;

        counter_i = 0;
        aloc_node = 0;
        aloc_con = 0;
        nodeID = 0;

        destroyMesh();
    }
  
    void reset(void) {
        reset(dimw);
    }

    SSSOM(int dimw) {
        reset(dimw);
    };

    ~SSSOM() {
    }
};

#endif /* SSSOM_H_ */
