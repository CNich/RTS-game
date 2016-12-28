#ifndef __HEAPPATHFINDER_H__
#define __HEAPPATHFINDER_H__

#include "src/Utilities/pathNode.h"
#include "cocos2d.h"
#include <vector>

class HeapPathFinder {
public:
	HeapPathFinder(int L);
	~HeapPathFinder();
	HeapPathFinder create();
	void insert(pathNode *a);
	void print();
	pathNode extract();
	bool check(int a); // prints the checking of the heap
	void sort();
	void change(int a, pathNode val);
	void change(int a);
	void change(pathNode * val);
	void setG(int a, int F);
	void checkAll();
	int getLength();
	//void swap(int a, int b);

private:
	void swap(int a, int b);
	std::vector<pathNode*> arr;
	//pathNode **arr;

	int length;
	void up(int a);
	void down(int a);
	int size;
};

#endif
