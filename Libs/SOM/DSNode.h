/* 
 * File:   DSNode.h
 * Author: hans
 *
 * Created on 28 de Março de 2012, 13:18
 */

#ifndef DSNODE_H
#define	DSNODE_H

#include "Mesh.h"
#include "NodeW.h"
#include "DebugOut.h"
#include <map>

class DSNode;

class DSNodeConnection : public Connection<DSNode> {
public:
    DSNodeConnection(TNode *node0, TNode *node1) : Connection<DSNode>(node0, node1) {}
};

class DSNode : public NodeW {
public:
    typedef DSNodeConnection TConnection;
    typedef std::map<DSNode*, TConnection*> TPNodeConnectionMap; //Para mapeamento local dos n�s e conex�es ligadas a this
    TPNodeConnectionMap nodeMap;

    TVector a;
    TVector ds;

    DSNode(int idIn, const TVector &v) : NodeW(idIn, v) {
        ds.size(v.size());
        ds.fill(1);

        a.size(v.size());
        a.fill(0);
    };    
    
    void write(std::ofstream &file) {
        for (int i=0; i<w.size(); i++) {
            file << w[i];
            if (i<w.size()-1) 
                file << "\t";
        }
        file << std::endl;
        
        for (int i=0; i<a.size(); i++) {
            file << a[i];
            if (i<a.size()-1) 
                file << "\t";
        }
        file << std::endl;
        
        for (int i=0; i<ds.size(); i++) {
            file << ds[i];
            if (i<ds.size()-1) 
                file << "\t";
        }
        file << std::endl;
    }
    
    void read(std::istream &file) {
        
        std::string line;
        getline(file, line);
        std::stringstream parserW(line);
        float value;
        int i=0;
        while (!parserW.eof()) {
            parserW >> value;
            if (i<w.size())
                w[i] = value;
            else
                w.append(value);
            i++;
        }
        
        getline(file, line);
        
        std::stringstream parserA(line);
        i=0;
        while (!parserA.eof()) {
            parserA >> value;
            if (i<a.size())
                a[i] = value;
            else
                a.append(value);
            i++;
        }
        
        getline(file, line);
        std::stringstream parserDS(line);
        i=0;
        while (!parserDS.eof()) {
            parserDS >> value;
            if (i<ds.size())
                ds[i] = value;
            else
                ds.append(value);
            i++;
        }
    }
};

#endif	/* DSNODE_H */

