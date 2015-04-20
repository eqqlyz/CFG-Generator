all:	bitset.o	vpg.o	testVPG.o
	g++ -o vpg bitset.o vpg.o testVPG.o 
	rm *.o
 
testVPG.o:	testVPG.cc
	g++ -c testVPG.cc

vpg.o:	vpg.h	vpg.cc
	g++ -c vpg.cc

bitset.o:	bitset.h	bitset.cc
	g++ -c bitset.cc

clean:
	rm *.o vpg
