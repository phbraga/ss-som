/* 
 * File:   Mesh.h
 * Author: daniel
 *
 * Created on 21 de Fevereiro de 2009, 19:33
 */

#ifndef _MESH_H
#define	_MESH_H

#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include "MatMatrix.h"
#include "MatVectorList.h"


template<class T>class Connection {
public:
    typedef T TNode;
    int step;
    int touched;
    TNode *node[2];

    inline std::pair<int, int> getId() {
        return std::pair<int, int>(node[0]->getId(), node[1]->getId());
    }

    inline void getId(int &node0, int &node1) {
        node0 = node[0]->getId();
        node1 = node[1]->getId();
    }

    Connection(TNode *node0, TNode *node1) {
        node[0] = node0;
        node[1] = node1;
    }
};

class Node {
public:
    typedef float TNumber;
    typedef MatVector<float> TVector;
    typedef Connection<Node> TConnection;

    inline int getId() {
        return 0;
    }

    inline void setId(int idIn) { };
    int step;
    int touched;
};

class NodeLb : public Node {
private:
    int id;
public:

    inline int getId() {
        return id;
    }
    inline void setId(int idIn) { id=idIn; };
    NodeLb(int idIn) : id(idIn) {
    };
    
    virtual void write(std::ofstream &file){};
    
    virtual void read(std::istream &file){};
};

template<class T> class Mesh {
public:
    typedef T TNode;
    typedef typename T::TNumber TNumber;
    typedef typename T::TVector TVector;
    typedef typename T::TConnection TConnection;
    typedef typename T::TPNodeConnectionMap TPNodeConnectionMap;
    typedef std::set<T*> TPNodeSet; // mapeamento global dos n�s
    typedef std::set<TConnection*> TPConnectionSet; //Para mapeamento global das conex�es da mesh

    TPNodeSet meshNodeSet; //Com informa��es de n�s de toda a rede
    TPConnectionSet meshConnectionSet; //Com a informa��o de conex�es de toda a rede
    int aloc_node;
    int aloc_con;

    void connect(TNode *node0, TNode *node1)//Aloca conex�es
    {
        if ((node0 != 0) && (node1 != 0) && (node0 != node1)) {
            TConnection *con = new TConnection(node0, node1);
            aloc_con++;
            meshConnectionSet.insert(con); //Adiciona a conex�o a lista de conex�es do mapa.
            node0->nodeMap[node1] = con;
            node1->nodeMap[node0] = con;
        }

        //		std::cout << "conectando dois n�s\n";
    }

    void disconnect(TNode *node0, TNode *node1)//Desaloca somente conex�es
    {
        typename TPNodeConnectionMap::iterator it; //::iterator it;
        it = node0->nodeMap.find(node1);

        if (it != node0->nodeMap.end()) {
            meshConnectionSet.erase((*it).second); //Remove a conex�o da lista de conex�es do mapa
            delete (*it).second;
            aloc_con--; //Destroi a conex�o que une node0 e node1
            node0->nodeMap.erase(node1);
            node1->nodeMap.erase(node0);
        }
        //		else
        //			std::cout << "Os n�s n�o est�o conectados\n";
        //		std::cout << "desconectando dois nois\n";
    }

    void disconnect(TConnection* con) //Desaloca somente conex�es
    {
        TNode *node0 = con->node[0];
        TNode *node1 = con->node[1];
        node0->nodeMap.erase(node1); //Remove o par <Node*,Connection*> do vizinho
        node1->nodeMap.erase(node0); //Remove o par <Node*,Connection*> do vizinho

        meshConnectionSet.erase(con); //Remove a conex�o da lista de conex�es do mapa
        delete con;
        aloc_con--; //Destroi a conex�o que une node0 e node1
        con = 0;
    }

    inline bool eraseNode(TNode* node)//Desaloca n�s e conex�es(via disconnect)
    {
        meshNodeSet.erase(node);

        typename TPNodeConnectionMap::iterator it;
        for (it = node->nodeMap.begin(); it != node->nodeMap.end(); it++) {
            disconnect(node, (*it).first);
        }

        delete node;
        aloc_node--; //Destroi a conex�o entre os dois n�s.
        node = 0;

        return true;
    }

    inline TNode* createNode(int id, const TVector &v, TPNodeSet *pnodeSet = 0) {
        TNode* node = new TNode(id, v);
        aloc_node++;
        meshNodeSet.insert(node);

        if (pnodeSet != 0) {
            typename TPNodeSet::iterator it;
            for (it = pnodeSet->begin(); it != pnodeSet->end(); it++)
                connect(node, (*it));
        }

        return node;
    }



    inline int size(void) {
        return meshNodeSet.size();
    }

    Mesh& resetNodes(int step = 0, int touched = 0) {
        typename TPNodeSet::iterator it;
        for (it = meshNodeSet.begin(); it != meshNodeSet.end(); it++) {
            (*it)->step = step;
            (*it)->touched = touched;
        }

        return *this;
    }

    Mesh& resetConnections(int step = 0, int touched = 0) {
        typename TPConnectionSet::iterator it;
        for (it = meshConnectionSet.begin(); it != meshConnectionSet.end(); it++) {
            (*it)->step = step;
            (*it)->touched = touched;
        }

        return *this;
    }

    inline bool isConnected(TNode *node0, TNode *node1) {
        if (node0->nodeMap.find(node1) != node0->nodeMap.end())
            return true;

        return false;
    }

    inline bool isConnected(TNode *node0, TConnection *con) {
        if ((con->node[0] == node0) || con->node[1] == node0)
            return true;

        return false;
    }

    void destroyMesh() {
        //std::cout << "Destrutor de Mesh sendo chamado\n";
        while (!meshNodeSet.empty()) {
            typename TPNodeSet::iterator it = meshNodeSet.begin();
            eraseNode((*it));
        }

        while (!meshConnectionSet.empty()) {
            typename TPConnectionSet::iterator it = meshConnectionSet.begin();
            disconnect((*it));
            errMsg("Esta linha não deveria ser executada em momento algum");
        }
    }

    template<class Number> Mesh& outputCenters(MatMatrix<Number> &m) {
        using namespace std;

        uint wSize = (*meshNodeSet.begin())->w.size();
        uint meshNodeSetSize = meshNodeSet.size();
        m.size(meshNodeSetSize, wSize);

        int i = 0;
        typename TPNodeSet::iterator it;
        for (it = meshNodeSet.begin(); it != meshNodeSet.end(); it++) {
            for (uint j = 0; j < wSize; j++)
                m[i][j] = (*it)->w[j];
            i++;
        }

        return *this;
    }

    Mesh& inputCenters(const MatMatrix<TNumber> &m) {

    }

    Mesh& outputCenterss(MatMatrix<TNumber> &m) {

    }

    Mesh& connectionMatrix(MatMatrix<int> &m) {

    }

    void enumerateNodes(void)
    {
        using namespace std;
        typename TPNodeSet::iterator it;
        int i=0;
        for (it = meshNodeSet.begin(); it != meshNodeSet.end(); it++)
        {
            (*it)->setId(i++);
            //cout << "id: " << i << endl;
        }
    }

    std::string nodesToString(const typename TPNodeSet::iterator &it) {
        using namespace std;
        ostringstream out;

        out << "id=" << (*it)->getId() << "; step=" << (*it)->step;
        out << "; touched=" << (*it)->touched << "; conexões=[";


        typename TPNodeConnectionMap::iterator itNode;
        for (itNode = (*it)->nodeMap.begin(); itNode != (*it)->nodeMap.end(); itNode++)
            out << " " << itNode->first->getId();

        out << " ]; w=" << (*it)->w << ";" << endl;

        return out.str();
    }

    std::string nodesToString() {
        using namespace std;
        typename TPNodeSet::iterator it;
        ostringstream out;
        for (it = meshNodeSet.begin(); it != meshNodeSet.end(); it++)
            out << nodesToString(it);

        return out.str();
    }


    std::string connectionsToString() {
        using namespace std;


    }

    Mesh& printNode(const typename TPNodeSet::iterator &it) {

        std::cout << nodesToString(it);

        return *this;
    }

    Mesh& printNodes(void) {

        std::cout << nodesToString();

        return *this;
    }

    Mesh& makeBinaryGrid(const TVector &v0, const TVector &v1)
    {
        /*
         * Posiciona os dois centros iniciais em v0 e v1
         */

        TNode *node0 = createNode(0,v0);
        TNode *node1 = createNode(1,v1);
        connect(node0,node1);
    }


    Mesh& makeSquareGrid(const TVector &v0, const TVector stepGridX, const TVector stepGridY,int rows, int cols) {
    /*
     *v0 = Vetor que define a posição do centro do Grid
     *stepGridX = vetor que dá espaçamento em x entre os nodos do grid
     *stepGridY = vetor que dá espaçamento em y entre os nodos do grid
     */

        int x, y;
        int dimw=v0.size();

        //int dimw = 2; //Esta declara��o esta aqui temporariamente
        //Vetor de referencia dos n�s do mapa para auxiliar na gera��o da malha.
        std::vector<TNode*> vectorNode;
        vectorNode.resize(rows * cols);
        
        TVector stepGrid=v0-stepGridX*(cols/2)-stepGridY*(rows/2);
        TVector stepGridL=stepGrid;

        //Cria os n�s do mapa
        for (y = 0; y < rows; y++) {
            for (x = 0; x < cols; x++) {
                
                TNode *node = createNode(x + cols*y, stepGrid);
                vectorNode[x + cols * y] = node;
                stepGrid+=stepGridX;
            }
            stepGridL+=stepGridY;
            stepGrid=stepGridL;

        }
        //Realiza as conex�es em uma malha quadrada.

        for (y = 0; y < (rows - 1); y++)
            for (x = 0; x < (cols - 1); x++) {
                connect(vectorNode[x + cols * y], vectorNode[(x + 1) + cols * y]);
                connect(vectorNode[x + cols * y], vectorNode[x + cols * (y + 1)]);
            }

        y = (rows - 1);
        for (x = 0; x < (cols - 1); x++) {
            connect(vectorNode[x + cols * y], vectorNode[(x + 1) + cols * y]);
        }

        x = (cols - 1);
        for (y = 0; y < (rows - 1); y++) {
            connect(vectorNode[x + cols * y], vectorNode[x + cols * (y + 1)]);
        }
        return *this;
    }

    Mesh& makeRandomGrid(const TVector &v0, const float &var,int rows, int cols) {
    /*
     *v0 = Vetor que define a posição do centro do Grid
     *var = desvio dos nós do grid em relação ao vetor v0
     */

        int x, y;
        int dimw=v0.size();

        //int dimw = 2; //Esta declara��o esta aqui temporariamente
        //Vetor de referencia dos n�s do mapa para auxiliar na gera��o da malha.
        std::vector<TNode*> vectorNode;
        vectorNode.resize(rows * cols);

        TVector v(dimw);


        //Cria os n�s do mapa
        for (y = 0; y < rows; y++) {
            for (x = 0; x < cols; x++) {
                v.randomn(v0,var);
                TNode *node = createNode(x + cols*y, v);
                vectorNode[x + cols * y] = node;
                
            }
        }
        //Realiza as conex�es em uma malha quadrada.

        for (y = 0; y < (rows - 1); y++)
            for (x = 0; x < (cols - 1); x++) {
                connect(vectorNode[x + cols * y], vectorNode[(x + 1) + cols * y]);
                connect(vectorNode[x + cols * y], vectorNode[x + cols * (y + 1)]);
            }

        y = (rows - 1);
        for (x = 0; x < (cols - 1); x++) {
            connect(vectorNode[x + cols * y], vectorNode[(x + 1) + cols * y]);
        }

        x = (cols - 1);
        for (y = 0; y < (rows - 1); y++) {
            connect(vectorNode[x + cols * y], vectorNode[x + cols * (y + 1)]);
        }
        return *this;
    }


    Mesh& makeCirculateGrid(MatVector<TNumber> wcenter, TNumber r, int numberOfNodes) {
        int x, y;
        int dimw = 2; //Esta declara��o esta aqui temporariamente
        //Vetor de referencia dos n�s do mapa para auxiliar na gera��o da malha.

        std::vector<TNode*> vectorNode;
        vectorNode.resize(numberOfNodes);
        TNumber tetaStep = MatUtils::TWO_PI / numberOfNodes;
        TNumber teta = 0;
        for (int i = 0; i < numberOfNodes; i++) {
            TVector w(dimw);
            w.x() = r * cos(teta);
            w.y() = r * sin(teta);
            w.add(wcenter);
            teta += tetaStep;
            TNode *node = createNode(i, w);
            vectorNode[i] = node;
        }

        for (int i = 1; i < numberOfNodes; i++) {
            connect(vectorNode[i - 1], vectorNode[i]);
        }
        connect(vectorNode[numberOfNodes - 1], vectorNode[0]);

    }

    Mesh() {
        aloc_node = 0;
        aloc_con = 0;
    }

    ~Mesh() {
        destroyMesh();
    }

};

template<class T> class MeshStatistics
{
public:
    const Mesh<T>& mesh;
    int numberOfNodes;
    int numberOfConnections;

    double meanNumberOfNeighbors;
    double varNumberOfNeighbors;
    int maxNumberOfNeighbors;
    int minNumberOfNeighbors;

    double meanConnectionLenght;
    double varConnectionLenght;
    double maxConnectionLenght;
    double minConnectionLenght;

    MeshStatistics(const Mesh<T>& meshIn):mesh(meshIn)
    {

    }

    MeshStatistics& makeStatistics()
    {
        numberOfNodes=mesh.meshNodeSet.size();
        numberOfConnections=mesh.meshConnectionSet.size();

        typename Mesh<T>::TPConnectionSet::iterator itCon=mesh.meshConnectionSet.begin();
        meanConnectionLenght=minConnectionLenght=maxConnectionLenght=
                (*itCon)->node[0]->w.dist((*itCon)->node[1]->w);
        itCon++;
        for(;itCon!=mesh.meshConnectionSet.end();itCon++)
        {
            double d=(*itCon)->node[0]->w.dist((*itCon)->node[1]->w);
            meanConnectionLenght+=d;

            if(d>maxConnectionLenght)
                maxConnectionLenght=d;

            if(d<minConnectionLenght)
                minConnectionLenght=d;

        }
        meanConnectionLenght=meanConnectionLenght/numberOfConnections;

        varConnectionLenght=0;
        for(itCon=mesh.meshConnectionSet.begin();itCon!=mesh.meshConnectionSet.end();itCon++)
        {
            double d=(*itCon)->node[0]->w.dist((*itCon)->node[1]->w);
            varConnectionLenght+=(d-meanConnectionLenght)*(d-meanConnectionLenght);
        }
        varConnectionLenght=varConnectionLenght/numberOfConnections;

        typename Mesh<T>::TPNodeSet::iterator itNode=mesh.meshNodeSet.begin();
        meanNumberOfNeighbors=maxNumberOfNeighbors=minNumberOfNeighbors=(*itNode)->nodeMap.size();

        for(;itNode!=mesh.meshNodeSet.end();itNode++)
        {
            double numberOfNeighbors=(*itNode)->nodeMap.size();
            meanNumberOfNeighbors+=numberOfNeighbors;

            if(numberOfNeighbors>maxNumberOfNeighbors)
                maxNumberOfNeighbors=numberOfNeighbors;

            if(numberOfNeighbors<minNumberOfNeighbors)
                minNumberOfNeighbors=numberOfNeighbors;

        }
        meanNumberOfNeighbors=meanNumberOfNeighbors/numberOfNodes;

        varNumberOfNeighbors=0;
        for(itNode=mesh.meshNodeSet.begin();itNode!=mesh.meshNodeSet.end();itNode++)
        {
            double neighbors=(*itNode)->nodeMap.size();
            varNumberOfNeighbors+=(neighbors-meanNumberOfNeighbors)*(neighbors-meanNumberOfNeighbors);
        }
        varNumberOfNeighbors=varNumberOfNeighbors/numberOfNodes;

        return *this;
    }


    std::string toString() const
    {
        using namespace std;
	register unsigned int i;
	ostringstream out;

        out << "numberOfNodes=" << numberOfNodes << ";" << endl;
        out << "numberOfConnections=" << numberOfConnections << ";" <<endl;
        out << "meanNumberOfNeighbors=" << meanNumberOfNeighbors << ";" <<endl;
        out << "varNumberOfNeighbors=" << varNumberOfNeighbors << ";" <<endl;
        out << "maxNumberOfNeighbors=" << maxNumberOfNeighbors << ";" <<endl;
        out << "minNumberOfNeighbors=" << minNumberOfNeighbors << ";" <<endl;

        out << "meanConnectionLenght=" << meanConnectionLenght << ";" <<endl;
        out << "varConnectionLenght=" << varConnectionLenght << ";" <<endl;
        out << "maxConnectionLenght=" << maxConnectionLenght << ";" <<endl;
        out << "minConnectionLenght=" << minConnectionLenght << ";" <<endl;



	return out.str();
    }



};

template<class T1,class T2> double sumOfSquaredError(MatMatrix<T1> &data, Mesh<T2> &clusters) {
    double erro = 0;

    MatVector<T1> vdata;
    typename Mesh<T2>::TPNodeSet::iterator it;
    
    for (int i = 0; i < data.rows(); i++) {
        data.row2Vector(i, vdata);
        
        //clusters.row2Vector(0, vcluster);
        it=clusters.meshNodeSet.begin();
        double dmin = vdata.dist2((*it)->w);

        for (; it!=clusters.meshNodeSet.end(); it++) {
            
            double dminTemp = vdata.dist2((*it)->w);

            if (dminTemp < dmin)
                dmin = dminTemp;
        }
        erro += dmin;
    }
    

    return erro;
}

template<class T> std::ostream& operator << (std::ostream& out,const MeshStatistics<T> &mstatistics)
{
	out << mstatistics.toString();
	return out;
}
//std::ostream& operator << (std::ostream& out,const MeshStatistics<NodeLb> &mstatistics);
#endif	/* _MESH_H */

