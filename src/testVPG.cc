
#include "vpg.h"

int NO = 2;
int main(){
	// generate
	VPGenerator *vpg = new VPGenerator(6);
	// print to file
	//vpg->print_mcl_fmt("mclInput");
	//vpg->print("normalCFG.dot");
	// analysis
	//vpg->build_Preds_Succs();
	//vpg->computeDom();
	// print dom tree
	//vpg->printDomTree("FwdDomTree.dot");

	// reverse the graph
	//vpg->reverseGraph();
	//vpg->print("reverseCFG.dot");
	//vpg->computeDom();
	//vpg->printDomTree("PstDomTree.dot");

	// print snap format
	//vpg->print_snap_fmt("/home/llvm/MyFiles/snap-cfg-master/examples/concomp/snap_cfg.txt");
	vpg->print("org.dot");
	vpg->print_snap_fmt("snap_cfg.txt");
	vpg->build_Preds_Succs();
	vpg->computeDom();
	vpg->printDomTree("td.dot");

	VPGenerator *vpg1 = new VPGenerator("snap_cfg.txt");
	vpg1->print("test.dot");

	vpg1->build_Preds_Succs();
	vpg1->computeDom();
	vpg1->printDomTree("dt.dot");
	return 1;
}
