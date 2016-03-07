#ifndef __BFS__
#define __BFS__

#include <vector>
#include <iostream>
#include "src/Utilities/LinkedList.h"
#include "src/Utilities/PathFinder.h"
#include "src/Units/BasicUnit.h"

struct mpair{
	int x;
	int y;
};

struct BFSMapNode{
	mpair parent;
	char* dir = 0;
	bool blocked = false;
	bool checked = false;
};

class BFS {
public:
	BFS(int l, int w);
	void setBlocked(int x, int y){ map[x][y].blocked = true; };
	void setStart(int x, int y) { start.x = x; start.y = y; CCLOG("setStart: %f, %f", start.x, start.y); CCLOG("setStart x y: %f, %f", x, y); };
	void setChecked(int x, int y) { map[x][y].checked = true; };
	bool getChecked(int x, int y) { return map[x][y].checked; };

	void setPathFinder(PathFinder<BasicUnit> *tpf) { pf = tpf; };

	void solve();
	void print();
	BFSMapNode getNode(int x, int y) { return map[x][y]; };
	~BFS();
	mpair start;

private:
	std::vector < std::vector <BFSMapNode> > map;
	LinkedList<mpair> *queueBFS = new LinkedList<mpair>;
	void checkNeighbour(int dx, int dy, mpair n, char* dir);
	int L;
	int W;

	PathFinder<BasicUnit> *pf;
};

#endif // __BFS__