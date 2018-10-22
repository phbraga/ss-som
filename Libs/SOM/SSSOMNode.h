/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SSSOMNode.h
 * Author: pedromagalhaes
 *
 * Created on February 10, 2018, 1:53 PM
 */

#ifndef SSSOMNODE_H
#define	SSSOMNODE_H

#include "Mesh.h"
#include "NodeW.h"
#include "DebugOut.h"
#include <map>

class SSSOMNode;

class SSSOMNodeConnection : public Connection<SSSOMNode> {
public:
    SSSOMNodeConnection(TNode *node0, TNode *node1) : Connection<SSSOMNode>(node0, node1) {}
};

class SSSOMNode : public NodeW {
public:
    typedef SSSOMNodeConnection TConnection;
    typedef std::map<SSSOMNode*, TConnection*> TPNodeConnectionMap; //Para mapeamento local dos n�s e conex�es ligadas a this
    TPNodeConnectionMap nodeMap;

    TVector a;
    TVector ds;
    int cls;
    
    int wins;
    TNumber act;
    
    SSSOMNode(int idIn, const TVector &v) : NodeW(idIn, v) {
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
        
        file << cls;
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
        
        int nodeClass;
        getline(file, line);
        std::stringstream parserCLS(line);
        parserCLS >> nodeClass;
        cls = nodeClass;
    }
};

#endif	/* SSSOMNODE_H */



