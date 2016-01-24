#ifndef __PATHNODE__
#define __PATHNODE__

struct pairC{
	int x;
	int y;
};

class BasicUnit;

struct pathNode{
	//tentative_g_score := g_score[current] + dist_between(current,neighbor)
	int H; // distance to goal
	int G; // cost to get to current nodes through other nodes
	int F; // G + H
	pairC parent;
	int heapPos;
	bool taken;
	bool blocked;
	bool permaBlocked;
	bool closed;
	bool success;
	bool checked;
	int x;
	int y;
	BasicUnit *unit;

	inline bool operator==(pathNode a){ return (F == a.F); }
	inline bool operator<=(pathNode a){ return (F <= a.F ? true : false); }
	inline bool operator<(pathNode a){ return  (F <  a.F ? true : false); }
	inline bool operator>(pathNode a){ return  (F >  a.F); }
	inline bool operator>=(pathNode a){ return (F >= a.F); }
};

#endif
