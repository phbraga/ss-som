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

class GDSNodeMW;

class GDSConnectionMW : public Connection<GDSNodeMW> {
public:
    int age;

    GDSConnectionMW(TNode *node0, TNode *node1) : Connection<GDSNodeMW>(node0, node1), age(0) {
    }
};

class GDSNodeMW : public SSSOMNode {
public:

    typedef GDSConnectionMW TConnection;
    typedef std::map<GDSNodeMW*, TConnection*> TPNodeConnectionMap; //Para mapeamento local dos n�s e conex�es ligadas a this

    int wins;
    TPNodeConnectionMap nodeMap;
    TNumber act;

    inline int neighbors() {
        return nodeMap.size();
    }

    GDSNodeMW(int idIn, const TVector &v) : SSSOMNode(idIn, v), wins(0), act(0) {
    };

    ~GDSNodeMW() {
    };
};

class SSSOM : public SOM<GDSNodeMW> {
public:
    uint maxNodeNumber;
    int epochs;
    float minwd;
    float e_b;
    float e_b0;
    float e_n;
    float tau;
    int nodesCounter;

    float push_rate;
    
    TNumber dsbeta; //Taxa de aprendizagem
    TNumber epsilon_ds; //Taxa de aprendizagem
    float age_wins;       //period to remove nodes
    float lp;           //remove percentage threshold
    float a_t;

    int nodesLeft;
    int nodeID;
    
    int supCountDupNodes; 
    int supCountNewWinnersFound; 
    int supCountNewNodeUnderAt; 
    int supCountRightWinner; 
    int supCountPushOnly; 
     
    int unsupCountNewNode; 
    int unsupCountUpdate; 
    
    inline float activation(const TNode &node, const TVector &w) {

        float distance = 0;

        for (uint i = 0; i < w.size(); i++) {
            distance += node.ds[i] * qrt((w[i] - node.w[i]));
            //dbgOut(3) <<  node.a[i] << "\t" << w[i] << "\t" << node.w[i] << endl;
        }

        float sum = node.ds.sum();
        return (sum / (sum + distance + 0.0000001));

        //float r = node.ds.sum();
//        float e = (1/(qrt(node.ds.norm()) + 0.0000001));
//        float r = distance;
//        return (1 / (1 + e*r));
        
        //return exp(-qrt(e*r));
        //return (1 / (1 + qrt(e*r)));
        //return (1 / sqrt(1 + qrt(e*r)));
    }

    inline float getWinnerActivation(const TVector &w) {
        TNode* winner = getWinner(w);        
        float a = activation(*winner, w);
        dbgOut(2) << winner->getId() << "\t" << a << endl;
        return a;
    }
    
    inline float dist2(const TNode &node, const TVector &w) {
        /*float distance = 0;

        for (uint i = 0; i < w.size(); i++) {
            distance += node.ds[i] * qrt((w[i] - node.w[i]));
        }

        return distance / (node.ds.sum() + 0.0000001);*/
        return 1/activation(node, w);
    }

    inline float dist(const TNode &node, const TVector &w) {
        return sqrt(dist2(node, w));
    }
    
    inline float wdist(const TNode &node1, const TNode &node2) {
        float distance = 0;

        for (uint i = 0; i < node1.ds.size(); i++) {
            distance +=  qrt((node1.ds[i] - node2.ds[i]));
        }
        
        return sqrt(distance);
    }

    inline void updateNode(TNode &node, const TVector &w, TNumber e) {
        
        TVector newA(node.a);
        
        //update averages
        for (uint i = 0; i < node.a.size(); i++) {
            //update neuron weights
            float distance = fabs(w[i] - node.w[i]);
            node.a[i] = e*dsbeta* distance + (1 - e*dsbeta) * node.a[i];
        }

        float max = node.a.max();
        float min = node.a.min();
        float average = node.a.mean();
        //float dsa = node.ds.mean();


        //update neuron ds weights
        for (uint i = 0; i < node.a.size(); i++) {
            if ((max - min) != 0) {
                //node.ds[i] = 1 - (node.a[i] - min) / (max - min);
                node.ds[i] = 1/(1+exp((node.a[i]-average)/((max - min)*epsilon_ds)));
            }
            else
                node.ds[i] = 1;

//            if (node.ds[i] < 0.95*dsa)
//                node.ds[i] = epsilon_ds;

//            if (node.ds[i] < epsilon_ds)
//                node.ds[i] = epsilon_ds;
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

//        enumerateNodes();

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

        //printWinners();
        return *this;
    }

    SSSOM& finishMap(bool sorted, std::vector<int> groups, std::map<int, int> &groupLabels) {

        dbgOut(1) << "Finishing map..." << endl;
        do {
            resetWins();
            maxNodeNumber = meshNodeSet.size();
            
            if (!sorted) {
                trainning(age_wins, groups, groupLabels);
            } else {
                orderedTrainning(age_wins, groups, groupLabels);
            }
            
            resetWins();

            TVector v;
            for (int i=0; i<data.rows(); i++) {
                data.getRow(i, v);
                TNode *winner = getWinner(v);
                if (activation(*winner, v)>= a_t) {
                    winner->wins++;
                    //step++;
                }
            }
            step = data.rows();

            int prefMeshSize = meshNodeSet.size();
            removeLoosers();
            updateAllConnections();
            dbgOut(1) << "Finishing: " << prefMeshSize << "\t->\t" << meshNodeSet.size() << endl;

            if (maxNodeNumber == meshNodeSet.size() || meshNodeSet.size()==1)
                break;
        } while (true);
        
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
    
    void printWinners() {

        TPNodeSet::iterator itMesh = meshNodeSet.begin();
        while (itMesh != meshNodeSet.end()) {
            
            int count = 0;
            for (int i=0; i<data.rows(); i++) {
                TVector row;
                data.getRow(i, row);
                TNumber a = activation(*(*itMesh), row);
                
                if (a>=a_t) {
                        dbgOut(1) << i << " ";
                        count++;
                }
            }
            dbgOut(1) << "\t" << (*itMesh)->getId() << "\t" << (*itMesh)->wins << "\t" << count << endl;
            itMesh++;
        }
    }

    /*
    LARFDSSOM& finishMap() {
        resetWins();

        TVector v;
        for (int i=0; i<data.rows(); i++) {
            data.getRow(i, v);
            TNode *winner = getWinner(v);
            if (activation(*winner, v)>= a_t) {
                winner->wins++;
                //step++;
            }
        }

        step = data.rows();
        //if (step==0) step = 1;

        removeLoosers();
        updateAllConnections();

        maxNodeNumber = meshNodeSet.size();
        trainning(age_wins);
    }/**/

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
            winner1 = getWinner(w); //winner
        
            //Passo 6: Calcula a atividade do nó vencedor
            TNumber a = activation(*winner1, w); //DS activation
            //Se a ativação obtida pelo primeiro vencedor for menor que o limiar
            //e o limite de nodos não tiver sido atingido
                      
            if ((a < a_t) && (meshNodeSet.size() < maxNodeNumber)) {
                createNodeMap(w, noCls);
                unsupCountNewNode++; 
                
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
                
                unsupCountUpdate++; 
            }
            
//            e_b = learningDecay(e_b0, step + 1);
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
                    supCountNewNodeUnderAt++;
                    
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
                    
                    supCountRightWinner++; 
                } 
            } else { // winner tem classe diferente da amostra
                // caso winner seja de classe diferente, checar se existe algum
                // outro nodo no mapa que esteja no raio a_t da nova amostra e
                // que pertença a mesma classe da mesma

                handleDifferentClass(winner1, w, cls);
            }   
            
//            e_b_sup = learningDecay(e_b_sup0, step + 1);
        }

        ageWinsCriterion();
        
        step++;
        
        return *this;
    }
    
    float learningDecay(float base_lr, int step) {
        float new_lr = (base_lr)/(1 + tau * step); //Equacao (Nova e Estevez, 2013 Neural Comput&Applic)
        if (new_lr <= 0) 
            new_lr = 0.000001;
        
        return new_lr;
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
            
            supCountNewWinnersFound++; 
            
        } else if (meshNodeSet.size() < maxNodeNumber) {
            
            // cria um novo nodo na posição da amostra
            createNodeMap(w, cls);
            
//            TVector wNew(winner1->w);
//            TNode *nodeNew = createNodeMap(wNew, cls);
//            
//            TVector aNew(winner1->a);
//            nodeNew->a = aNew;
//            
//            TVector dsNew(winner1->ds);
//            nodeNew->ds = dsNew;   
//            
//            updateNode(*nodeNew, w, e_b_sup);
//
//            updateNode(*winner1, w, -push_rate);
            
            supCountDupNodes++; 
        } 
    }

    virtual TNode *getFirstWinner(const TVector &w){
        TNode *winner = 0;
        TNumber temp = 0;
        
        TNumber d = dist(*(*Mesh<TNode>::meshNodeSet.begin()), w);
        winner = (*Mesh<TNode>::meshNodeSet.begin());
        winner->act = activation(*winner, w);

        TPNodeSet::iterator it;
        it = Mesh<TNode>::meshNodeSet.begin();
        it++;
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {
            (*it)->act = activation(*(*it), w);
            temp = dist(*(*it), w);
            if (d > temp) {
                d = temp;
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
    
    inline TNode* getWinner(const TVector &w) {
        TNode *winner = 0;
        TNumber temp = 0;
        
        TNumber d = dist(*(*Mesh<TNode>::meshNodeSet.begin()), w);
        winner = (*Mesh<TNode>::meshNodeSet.begin());

        TPNodeSet::iterator it;
        it = Mesh<TNode>::meshNodeSet.begin();
        it++;
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {
            temp = dist(*(*it), w);
            if (d > temp) {
                d = temp;
                winner = (*it);
            }
        }

        return winner;
    }

    inline SSSOM& getWinners(const TVector &w, TNode* &winner1, TNode* &winner2) {
        TPNodeSet::iterator it = Mesh<TNode>::meshNodeSet.begin();
        TNumber minDist = dist2(*(*it), w);
        
        //find first winner
        winner1 = (*it);
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {
            TNumber dist = dist2(*(*it), w);
            if (dist<minDist) {
                minDist = dist;
                winner1 = (*it);
            }
        }
        
        //find second winner
        it = Mesh<TNode>::meshNodeSet.begin();
        winner2 = (*it);
        minDist = dist2(*(*it), w);
        TNode* distWinner = NULL;
        for (; it != Mesh<TNode>::meshNodeSet.end(); it++) {
            if (*it!=winner1) {
                TNumber dist = dist2(*(*it), w);
                if (dist<minDist) {
                    minDist = dist;
                    winner2 = (*it);
                    if (wdist(*winner1, *(*it)) <= minwd)
                        distWinner = winner2;
                }
            }
        }
        
        if (distWinner!=NULL)
            winner2 = distWinner;

        return *this;
    }
    
    void getActivationVector(const TVector &sample, TVector &actVector) {
        actVector.size(Mesh<TNode>::meshNodeSet.size());
        
        int i=0;
        for (it = Mesh<TNode>::meshNodeSet.begin(); it != Mesh<TNode>::meshNodeSet.end(); it++) {
            actVector[i] = activation(*(*it), sample);
            i++;
        }
    }
    
    bool isNoise(const TVector &w) {
        TNode *winner = getWinner(w);
        double a = activation(*winner, w);
        return (a<a_t);
    }

    void resetToDefault(int dimw = 2) {
        SSSOM::dimw = dimw;
        step = 0;
        nodesLeft = 1;

        maxNodeNumber = 100;
        e_b = 0.05;
        e_n = 0.0006;
        counter_i = 0;
        aloc_node = 0;
        aloc_con = 0;
        nodesCounter = 1;
        nodeID = 0;

        destroyMesh();
        TVector v(dimw);
        v.random();
        TVector wNew(v);
        createNode(0, wNew);
    }

    void reset(int dimw) {
        SSSOM::dimw = dimw;
        step = 0;
        nodesLeft = 1;

        counter_i = 0;
        aloc_node = 0;
        aloc_con = 0;
        nodesCounter = 1;
        nodeID = 0;

        destroyMesh();
//        TVector v(dimw);
//        v.random();
//        TVector wNew(v);
//        createNodeMap(wNew, noCls);
    }
    
    void resetSize(int dimw) {
        SSSOM::dimw = dimw;
    }
    
    void binarizeRelevances() {
        
        TPNodeSet::iterator it;
        for (it = Mesh<TNode>::meshNodeSet.begin(); it != Mesh<TNode>::meshNodeSet.end(); it++) {
            TNode *node = *it;
            float average = node->ds.mean();
            for (int i=0; i<node->ds.size(); i++) {
                if (node->ds[i]>average)
                    node->ds[i] = 1;
                else
                    node->ds[i] = 0;
            }
        }
    }

    void reset(void) {
        reset(dimw);
    }

    SSSOM(int dimw) {
        resetToDefault(dimw);
    };

    ~SSSOM() {
    }

    template<class Number> SSSOM& outputCentersDs(MatMatrix<Number> &m) {
        using namespace std;

        uint wSize = (*meshNodeSet.begin())->ds.size();
        uint meshNodeSetSize = meshNodeSet.size();
        m.size(meshNodeSetSize, wSize);

        int i = 0;
        typename TPNodeSet::iterator it;
        for (it = meshNodeSet.begin(); it != meshNodeSet.end(); it++) {
            for (uint j = 0; j < wSize; j++)
                m[i][j] = (*it)->ds[j];
            i++;
        }

        return *this;
    }
    
    virtual bool saveParameters(std::ofstream &file) {
        
        file << maxNodeNumber << "\t";
        file << minwd << "\t";
        file << e_b << "\t";
        file << e_n << "\t";
        file << dsbeta << "\t"; //Taxa de aprendizagem
        file << epsilon_ds << "\t"; //Taxa de aprendizagem
        file << age_wins << "\t";       //period to remove nodes
        file << lp << "\t";          //remove percentage threshold
        file << a_t << "\n";
        return true;
    }
    
    virtual bool readParameters(std::ifstream &file) {
        
        file >> maxNodeNumber;
        file >> minwd;
        file >> e_b;
        file >> e_n;
        file >> dsbeta; //Taxa de aprendizagem
        file >> epsilon_ds; //Taxa de aprendizagem
        file >> age_wins;       //period to remove nodes
        file >> lp;           //remove percentage threshold
        file >> a_t;
        file.get();//skip line end
        
        return true;
    }
};

#endif /* SSSOM_H_ */
