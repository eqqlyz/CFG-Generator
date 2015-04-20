#ifndef VPG_H
#define VPG_H

#include<iostream>
#include<list>
#include<stdio.h>
#include<stdlib.h>
#include"bitset.h"

using namespace std;

/*文法规则如下：
 *	P->cfg
 * 	cfg->if cfg1 else cfg2;cfg3
 *	cfg->if cfg1;cfg2
 *	cfg->for cfg1;cfg2
 *	cfg->block
 *	cfg->e
 *
 */

#define BEGIN 	0
#define END 	1
#define CFG	-1


extern int NO ;	//global variable to label each node


typedef struct node{
	int num;	    	//the ID of each node
	node* succ1;	    //successor 1, the normal successor
	node* succ2;	    //successor 2, represent the loop or branch successor
    list<node*> *preds; //all the preds of the current node
    list<node*> *succs; //all the succcs of the current node
	bit_set *in;		//in bitset of dominators
	bit_set *out;		//out bitset of dominators
	node* idom;	    	//the immediate predominater of current node
}*Nd,Node;

typedef struct pair{
	int from;
	int to;
}*Pair,pairs;

class VPGenerator{
  // private data
  private:
    Nd proc;	        //the whole CFG
    list<Nd> all;	//remember all the nodes in CFG, to help release memory
    int depthLevel; 	//control the recurse depth
  public:
    VPGenerator(int level){
    	depthLevel = level;
    	Nd temp;
	    //use time as the random seed
	    srand((unsigned)time(0));
	    P_cfg(proc,temp);
	    generateGraph(temp,1);	//initial depth is one
    }
    VPGenerator(const char* filename){
    	loadGraph(filename);
    }
    ~VPGenerator(){
      release();
    }
  public:
    //-------generate function and release function---------
    Nd mknode(int no);			//generate a node with label no
    void release();				//release memory

    //-------procedure for each function-------
    void P_cfg(Nd &Prog,Nd &cfg);	                      //grammar 1
    void if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2,Nd &cfg3);	  //grammar 2
    void if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2);		          //grammar 3
    void for_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2);	          //grammar 4
    void block_cfg(Nd &cfg,Nd &cfg1);	                  //grammar 5
    void e_cfg(Nd &cfg);	                              //grammar 6
    
    //-------generate function---------
    void generateGraph(Nd &cfg,int level);
    void loadGraph(const char* filename);				  //load graph from file
    void reverseGraph();
    
    //-------analysis the graph---------
    void build_Preds_Succs();
    void computeDom();
    void setImmDom();

    //-------print the cfg---------
    void print(const char* filename);
    void printDomTree(const char* filename);
    void print_mcl_fmt(const char* filename);
    void print_snap_fmt(const char* filename);
    
    //-------helper function---------
    bool is_in(list<Pair> vec,Pair n);
    bool is_visited(list<Nd> vec, Nd n);
    Nd find_node(int n);
};

#endif //VPG_H
