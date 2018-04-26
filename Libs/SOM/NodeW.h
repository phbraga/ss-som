/* 
 * File:   NodeW.h
 * Author: hans
 *
 * Created on 29 de Março de 2012, 10:31
 */

#ifndef NODEW_H
#define	NODEW_H

#include "Mesh.h"
#include <map>

class NodeW;

class NodeWConnection : public Connection<NodeW> {
public:
    NodeWConnection(TNode *node0, TNode *node1) : Connection<NodeW>(node0, node1) {}
};

class NodeW : public NodeLb {
public:
    typedef NodeWConnection TConnection;
    typedef std::map<NodeW*, TConnection*> TPNodeConnectionMap; //Para mapeamento local dos n�s e conex�es ligadas a this
    TPNodeConnectionMap nodeMap;

    TVector w;

    NodeW(int idIn, const TVector &v) : NodeLb(idIn), w(v) {
    };

};
#endif	/* NODEW_H */

