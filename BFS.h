#ifndef __BFS__
#define __BFS__

#include <vector>
#include <iostream>
#include "C:\Users\Sam\Desktop\New folder\LinkedList.h"

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
	void setStart(int x, int y) { start.x = x; start.y = y; };
	void setChecked(int x, int y) { map[x][y].checked = true; };
	bool getChecked(int x, int y) { return map[x][y].checked; };
	void solve();
	void print();
	BFSMapNode getNode(int x, int y) { return map[x][y]; };
	~BFS();

private:
	std::vector < std::vector <BFSMapNode> > map;
	LinkedList<mpair> *queueBFS = new LinkedList<mpair>;
	void checkNeighbour(int dx, int dy, mpair n, char* dir);
	mpair start;
	int L;
	int W;
};

#endif // __BFS__
