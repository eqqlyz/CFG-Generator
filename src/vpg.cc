
#include "vpg.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- Nd VPGenerator::mknode(int no)
// function  -- make a node with label no
//////////////////////////////////////////////////////////////////////////////////////////////
Nd VPGenerator::mknode(int no){
	Nd nd=new node;
	nd->num=no;
	nd->succ1=NULL;
	nd->succ2=NULL;
	if(no!=CFG){
		nd->preds = new list<node*>();
		nd->succs = new list<node*>();
	}
   	nd->idom=NULL;
	
	if(no!=CFG)
		all.push_front(nd);
	return nd;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::P_cfg(Nd &Prog,Nd &cfg)
// function  -- make a structure corresponding to grammar 1
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::P_cfg(Nd &Prog,Nd &cfg){
	Prog=mknode(CFG);
	Prog->succ1=mknode(BEGIN);	//succ1 represents begin
	Prog->succ2=mknode(END);	//succ2 represents end
	cfg=mknode(CFG);
	cfg->succ1=mknode(NO++);	//succ1 represents begin
	Prog->succ1->succ1=cfg->succ1;	//succ1->begin;succ1->succ1
	cfg->succ2=Prog->succ2;		//succ2->end;succ2->end
	cfg->succ1->succ1=cfg->succ2;	//succ1->succ1;succ2->end
	Nd temp=Prog;
	Prog=Prog->succ1;
	delete temp;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2,Nd &cfg3)
// function  -- make a structure corresponding to grammar 2
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2,Nd &cfg3){
	cfg1=mknode(CFG);
	cfg2=mknode(CFG);
	cfg1->succ1=mknode(NO++);					
	cfg2->succ1=mknode(NO++);
	cfg1->succ2=mknode(NO++);
	cfg2->succ2=cfg1->succ2;
	cfg3=mknode(CFG);
	cfg3->succ1=mknode(NO++);
	cfg->succ1->succ1=cfg1->succ1;
	cfg->succ1->succ2=cfg2->succ1;
	cfg1->succ1->succ2=cfg1->succ2;	
	cfg2->succ1->succ2=cfg2->succ2;
	cfg1->succ2->succ1=cfg3->succ1;
	cfg3->succ1->succ1=cfg->succ2;	
	cfg3->succ2=cfg->succ2;	
	
	delete cfg;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2)
// function  -- make a structure corresponding to grammar 3
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::if_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2){
	cfg1=mknode(CFG);
	cfg1->succ1=mknode(NO++);
	cfg2=mknode(CFG);
	cfg2->succ1=mknode(NO++);

	cfg->succ1->succ1=cfg1->succ1;
	cfg1->succ1->succ1=cfg2->succ1;
	cfg->succ1->succ2=cfg2->succ1;	
	cfg1->succ2=cfg2->succ1;
	cfg2->succ1->succ1=cfg->succ2;
	cfg2->succ2=cfg->succ2;
	delete cfg;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::for_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2)
// function  -- make a structure corresponding to grammar 4
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::for_cfg(Nd &cfg,Nd &cfg1,Nd &cfg2){
	cfg1=mknode(CFG);
	cfg1->succ1=mknode(NO++);
	cfg1->succ2=mknode(NO++);
	cfg2=mknode(CFG);
	cfg2->succ1=mknode(NO++);
					
	cfg->succ1->succ1=cfg1->succ1;
	cfg1->succ1->succ1=cfg1->succ2;	
	cfg1->succ2->succ2=cfg1->succ1;
	cfg1->succ2->succ1=cfg2->succ1;
	cfg2->succ1->succ1=cfg->succ2;
	cfg2->succ2=cfg->succ2;
	delete cfg;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::block_cfg(Nd &cfg,Nd &cfg1)
// function  -- make a structure corresponding to grammar 5
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::block_cfg(Nd &cfg,Nd &cfg1){
	cfg1=mknode(CFG);
	cfg1->succ1=mknode(NO++);
	cfg1->succ2=cfg1->succ1;
	cfg->succ1->succ1=cfg1->succ1;
	cfg1->succ2->succ1=cfg->succ2;
	cfg1->succ2=cfg->succ2;
	delete cfg;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::block_cfg(Nd &cfg,Nd &cfg1)
// function  -- make a structure corresponding to grammar 6
/////////////////////////////////////////////////////////void print_snap_fmt(const char* filename);/////////////////////////////////////
void VPGenerator::e_cfg(Nd &cfg){
	delete cfg;
}

void VPGenerator::generateGraph(Nd &cfg,int level){
	/*随机调度函数*/
	/*三个局部变量*/
	Nd temp1;
	Nd temp2;
	Nd temp3;
	if(level>=depthLevel){
		e_cfg(cfg);
		return;
	}
	int scale=5;

	int n=rand()%scale+1;	/*随机生成[1,2,3,4,5]中的整数*/
	switch(n){
		case 1:{

			if_cfg(cfg,temp1,temp2,temp3);
			generateGraph(temp1,level+1);
			generateGraph(temp2,level+1);
			generateGraph(temp3,level+1);
			break;
		}
		case 2:{
			if_cfg(cfg,temp1,temp2);
			generateGraph(temp1,level+1);
			generateGraph(temp2,level+1);
			break;
		}
		case 3:{
			for_cfg(cfg,temp1,temp2);
			generateGraph(temp1,level+1);
			generateGraph(temp2,level+1);
			break;
		}
		case 4:{
			block_cfg(cfg,temp1);
			generateGraph(temp1,level+1);
			break;
		}
		case 5:{
			e_cfg(cfg);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::loadGraph(const char* filename)
// function  -- load graph from file
// note: the input file format should be like below:
// src	dst
// And the first number should be the begin node of the cfg.
// Also note: currently, the VPGenerator can not deal with the N-WAY structure
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::loadGraph(const char* filename){
	// if all is null, release it
	proc = NULL;
	NO = 0;
	// open file
	FILE *f = fopen(filename,"r");
	fseek(f, 0, SEEK_END);
	long m_lImageSize = ftell(f);
	char* graph = new char[m_lImageSize+1];
	fseek(f, 0, SEEK_SET);
	size_t size = fread(graph, 1, m_lImageSize, f);
	if (size != (size_t) m_lImageSize){
		fprintf(stderr, "WARNING! Only read %ud of %ld bytes of graph file!\n", size, m_lImageSize);
	}
	// parse file
	int srcIdx, dstIdx;
	bool isRoot = true;
	long i=0,sIdx,dIdx;
	char src[20];
	char dst[20];
	while(i < m_lImageSize){
		// get srcIdx and dstIdx
		sIdx=dIdx=0;
		while('0'<=graph[i] && graph[i]<='9'){
			src[sIdx++] = graph[i++];
		}
		src[sIdx]='\0';
		srcIdx = atoi(src);
		while(!('0'<=graph[i] && graph[i]<='9'))
			i++;
		while('0'<=graph[i] && graph[i]<='9'){
			dst[dIdx++] = graph[i++];
		}
		dst[dIdx]='\0';
		dstIdx = atoi(dst);
		while(!('0'<=graph[i] && graph[i]<='9'))
			i++;
		//cout << srcIdx << " -- " << dstIdx<<endl;
		// make nodes
		if(isRoot){
			Nd temp = mknode(srcIdx);
			proc = temp;
			all.push_back(temp);
			temp = mknode(dstIdx);
			proc->succ1 = temp;
			all.push_back(temp);
			isRoot = false;
		}else{
			// make node
			Nd temp1 = find_node(srcIdx);
			if(temp1 == NULL){
				temp1 = mknode(srcIdx);
			}
			Nd temp2 = find_node(dstIdx);
			if(temp2 == NULL){
				temp2 = mknode(dstIdx);
			}

			// add connection
			if(temp1->succ1 == NULL){
				temp1->succ1 = temp2;
			}else{
				temp1->succ2 = temp2;
			}
		}
	}

	// close file
	fclose(f);
	free(graph);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::reverseGraph()
// function  -- reverse the direction of each edge
// It will be very hard to reverse the graph using current graph representation
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::reverseGraph(){

}
//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::build_Preds_Succs()
// function  -- build the preds and succs of each node in proc
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::build_Preds_Succs(){
	list<Nd> visited;
	list<Nd> unvisited;
	list<Pair> draw;
	unvisited.push_front(proc);

	while(!unvisited.empty()){
		Nd temp=unvisited.front();
		// initialize the space for this node to all ones
		temp->in = new bit_set(0,NO,false);
		temp->out = new bit_set(0,NO,false);
		temp->in->clear();
		temp->out->clear();

		unvisited.pop_front();
		visited.push_front(temp);

		if(temp->succ1){/*第一个后继*/
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ1->num;
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ1);
				draw.push_front(p);
				// set the succs and preds
				temp->succs->push_back(temp->succ1);
				temp->succ1->preds->push_back(temp);
			}
		}
		if(temp->succ2){/*第二个后继*/
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ2->num;
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ2);
				draw.push_front(p);
				// set the succs and preds
				temp->succs->push_back(temp->succ2);
				temp->succ2->preds->push_back(temp);
			}
		}
	}

	//test preds and succs
	/*list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++){
		cout << (*it)->num<<":";
		// preds
		list<Nd>::iterator pit;
		for(pit=(*it)->preds->begin();pit!=(*it)->preds->end();pit++)
			cout<<(*pit)->num <<",";
		cout<<" | ";
		for(pit=(*it)->succs->begin();pit!=(*it)->succs->end();pit++)
			cout<<(*pit)->num <<",";
		cout<<endl;
	}*/
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::computeDom()
// function  -- compute the dominators of each node; only use the out set
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::computeDom(){
	//Step 1: Initialize
	proc->out->clear();
	proc->out->add(BEGIN);
	list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++){
		if((*it)!=proc){
			(*it)->out->universal(); //all ones
			(*it)->out->remove((*it)->num);
		}
	}
	bool change = false;
	int iterTimes = 0;
	//Step 2: Iterate
	do{
		change = false;
		list<Nd>::reverse_iterator it;
		for(it=all.rbegin();it!=all.rend();it++){
			if((*it)!=proc){
				// dom[n] = {n} U ( intersection of out of all n's preds)
				bit_set temp;
				list<Nd>::iterator pIt=(*it)->preds->begin();
				// Intersection of the preds
				temp.copy((*pIt)->out);
				for(;pIt!=(*it)->preds->end();pIt++){
					temp *= *((*pIt)->out);
				}
				// union with itself
				temp.add((*it)->num);
				if(temp != *((*it)->out)){
					change = true;
					*((*it)->out) = temp;
				}
			}
		}
	}while(change);
	//test the algorithm
//	for(it=all.begin();it!=all.end();it++){
//		cout << (*it)->num<<":";
//		(*it)->out->print();
//		cout << endl;
//	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::setImmDom()
// function  -- set the imm Dom of each node in the graph
// algorithm -- according to the characteristics of dominate.
//      IF (a1 dom b) AND (a2 dom b) AND (a3 dom b) AND ... AND (an dom b) THEN
//        There exists a sequence like <ai1,ai2,ai3,...,ain>,
//        sat. <i1, i2, i3, ..., in> is a permutation of <1, 2, 3, ..., n>.
//        Such that ai dom ai+1, then ain will be the immediate dominator of b.
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::setImmDom(){
	list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++){
		bit_set dom ;
		int domNum = -1;
		dom.copy((*it)->out);
		int i = (*it)->out->ub();
		int domSize = (*it)->out->count();
		// find the immediate dominator
		while(i>=0){
			if(dom.contains(i)){
				if(i == (*it)->num){
					i--;
					continue;
				}
				int domTime = 0;
				// get the dominate list of i
				bit_set zdom;
				list<Nd>::iterator rit;
				for(rit=all.begin();rit!=all.end();rit++){
					if((*rit)->num == i){
						zdom.copy((*rit)->out);
						break;
					}
				}
				int j = (*it)->out->ub();
				while(j>=0){
					// if j dom i, then j is not the immediate dominator of *it
					if(dom.contains(j)&&zdom.contains(j)){
						domTime++;
					}
					j--;
				}
				// NOTE :: Got it!
				if(domTime == domSize - 1){
					domNum = i;
					break;
				}
			}
			i--;
		}

		// Set the immediate dominator
		list<Nd>::iterator rit;
		for(rit=all.begin();rit!=all.end();rit++){
			if((*rit)->num == domNum){
				(*it)->idom = *rit;
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::print(const char* filename)
// function  -- print the file to dot file
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::print(const char* filename){
	/*以png的形式输出生成的CFG*/
	list<Nd> visited;
	list<Nd> unvisited;
	list<Pair> draw;
	
	FILE *f=fopen(filename,"w");
	fprintf(f,"digraph G{\n");
	/*1.初始化*/
	unvisited.push_front(proc);
			
	while(!unvisited.empty()){	/*因为排除了entry和end结点*/
		Nd temp=unvisited.front();
		unvisited.pop_front();
		/*--------生成基本块--------*/
		fprintf(f,"\tNode%d[label=\"B%d\"];\n",temp->num,temp->num);

		visited.push_front(temp);
    
		/*--------生成边--------*/
		if(temp->succ1){/*第一个后继*/
		  //Pair记录边，如果该边已经被绘制，则不输出此边
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ1->num;			
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ1);
				draw.push_front(p);
				fprintf(f,"\tNode%d->{Node%d}\n",temp->num,temp->succ1->num);
			}
		}
		if(temp->succ2){/*第二个后继*/
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ2->num;	/*不可简单的复制*/
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ2);
				draw.push_front(p);
				fprintf(f,"\tNode%d->{Node%d}\n",temp->num,temp->succ2->num);
			}
		}
	}
	fprintf(f,"}");
	fclose(f);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::printDomTree(const char* filename)
// function  -- print the dominate tree
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::printDomTree(const char* filename){
	// First, calculate the immediate dominator
	setImmDom();
	// Then, output the dot file
	FILE *f=fopen(filename,"w");
	fprintf(f,"digraph G{\n");
	// output the node
	list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++){
		fprintf(f,"\tNode%d[label=\"B%d\"];\n",(*it)->num,(*it)->num);
	}
	// output the edge
	for(it=all.begin();it!=all.end();it++){
		if((*it)->idom!=NULL)
			fprintf(f,"\tNode%d->{Node%d}\n",(*it)->idom->num,(*it)->num);;
	}
	fprintf(f,"}");
	fclose(f);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::print_mcl_fmt(const char* fileName)
// function  -- print the file to mcl format file
// mcl link : http://micans.org/mcl/ 
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::print_mcl_fmt(const char* fileName){
  	/*打印出可以供mcl软件输入的图文件*/
	list<Nd> visited;
	list<Nd> unvisited;
	
	FILE *f=fopen(fileName,"w");
	/*1.初始化*/
	unvisited.push_front(proc);
			
	while(!unvisited.empty()){	/*因为排除了entry和end结点*/
		Nd temp=unvisited.front();
		unvisited.pop_front();

    if(!is_visited(visited,temp)){
		  visited.push_front(temp);
      /*生成头节点*/
      fprintf(f,"%d ",temp->num);
    }else{
      continue;
    }
            
    /*--------生成边--------*/
		if(temp->succ1){/*第一个后继*/
		  //Pair记录边，如果该边已经被绘制，则不输出此边		
		  if(!is_visited(visited,temp->succ1))
			  unvisited.push_front(temp->succ1);
			fprintf(f,"  %d",temp->succ1->num);
		}
		if(temp->succ2){/*第二个后继*/
		  if(!is_visited(visited,temp->succ2))
			  unvisited.push_front(temp->succ2);
			fprintf(f," %d",temp->succ2->num);
		}
		/*结束一行*/
		fprintf(f,"\n");
	}
	fclose(f);
} 

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::print_snap_fmt(const char* filename)
// function  -- print the snap format
// snap link : http://snap.stanford.edu/index.html
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::print_snap_fmt(const char* filename){
	list<Nd> visited;
	list<Nd> unvisited;
	list<Pair> draw;

	FILE *f=fopen(filename,"w");
	/*1.初始化*/
	unvisited.push_front(proc);

	while(!unvisited.empty()){	/*因为排除了entry和end结点*/
		Nd temp=unvisited.front();
		unvisited.pop_front();

		visited.push_front(temp);

		/*--------生成边--------*/
		if(temp->succ1){/*第一个后继*/
		  //Pair记录边，如果该边已经被绘制，则不输出此边
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ1->num;
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ1);
				draw.push_front(p);
				fprintf(f,"%d\t%d\n",temp->num,temp->succ1->num);
			}
		}
		if(temp->succ2){/*第二个后继*/
			Pair p=new pairs;
			p->from=temp->num;
			p->to=temp->succ2->num;	/*不可简单的复制*/
			if(!is_in(draw,p)){
				unvisited.push_front(temp->succ2);
				draw.push_front(p);
				fprintf(f,"%d\t%d\n",temp->num,temp->succ2->num);
			}
		}
	}
	fclose(f);
}
//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- bool VPGenerator::is_in(list<Pair> vec,Pair n)
// function  -- test if Pair n is in list<Pair> vec
//////////////////////////////////////////////////////////////////////////////////////////////
bool VPGenerator::is_in(list<Pair> vec,Pair n){
	/*判断元素n是否在vec中*/
	list<Pair>::iterator it;
	for(it=vec.begin();it!=vec.end();it++)
		if(((*it)->from==n->from)&&((*it)->to==n->to))
			return true;
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- bool VPGenerator::is_visited(list<Nd> vec, Nd n)
// function  -- test if Nd n is in list<Nd> vec
//////////////////////////////////////////////////////////////////////////////////////////////
bool VPGenerator::is_visited(list<Nd> vec, Nd n){
  /*判断元素n是否在vec中*/
  list<Nd>::iterator it;
	for(it=vec.begin();it!=vec.end();it++)
		if((*it)->num==n->num)
			return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- Nd VPGenerator::find_node(int n)
// function  -- find the node with number n in list all
//////////////////////////////////////////////////////////////////////////////////////////////
Nd VPGenerator::find_node(int n){
	list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++)
		if((*it)->num==n)
			return (*it);
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////////////////
// prototype -- void VPGenerator::release()
// function  -- release the memory allocated by the graph
//////////////////////////////////////////////////////////////////////////////////////////////
void VPGenerator::release(){
	/*释放内存的函数*/
	list<Nd>::iterator it;
	for(it=all.begin();it!=all.end();it++)
		delete (*it);
}

