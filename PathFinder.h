#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "pathNode.h"
#include "LinkedList.h"
#include "HeapPathFinder.h"
#include "cocos2d.h"

template<class T>
class PathFinder {
public:
	PathFinder(int x, int y, pairC start, pairC end);
	PathFinder(int x, int y);
	void resetMap();
	void setStart(int x, int y);
	void setEnd(int x, int y);
	void setTileX(int x);
	void setTileY(int y);
	void setOffX(int x);
	void setOffY(int y);
	//~PathFinder();
	PathFinder create();
	void set(int x, int y, pathNode a);
	pathNode defaultPathNode();
	void print();
	void permaBlock(int x, int y);
	void block(int x, int y);
	void unblock(int x, int y);
	bool checkBlock(int x, int y);
	void taken(int x, int y);
	void untaken(int x, int y);
	bool checkTaken(int x, int y);
	LinkedList<cocos2d::Point> * solve();
	int getRows();
	int getCols();

private:
	pathNode **map;
	//cocos2d::Vector< cocos2d::Vector < pathNode > > map;
	pairC start;
	pairC end;
	int rows;
	int cols;
	HeapPathFinder *openL;
	LinkedList<cocos2d::Point> *queueP;
	void checkAdj(int x1, int y1, int x2, int y2);
	int abs(int a);
	int tileX = 1;
	int tileY = 1;
	int offX = 0;
	int offY = 0;

};

template<class T>
PathFinder<T>::PathFinder(int a, int b, pairC s, pairC e){
	//map.resize(a);
	map = new pathNode *[a];
	for (int i = 0; i < a; i++){
		//map[i].resize(b);
		map[i] = new pathNode[b];
		for (int j = 0; j < b; j++){
			map[i][j] = defaultPathNode();
			map[i][j].H = 10 * (abs(i - e.x) + abs(j - e.y));
			map[i][j].x = i;
			map[i][j].y = j;
		}
	}
	start = s;
	end = e;
	map[s.x][s.y].parent = s;
	map[s.x][s.y].G = 0;
	map[s.x][s.y].F = map[s.x][s.y].H;
	rows = a;
	cols = b;

	openL = new HeapPathFinder(a*b);
	queueP = new LinkedList<cocos2d::Point>();
}

template<class T>
PathFinder<T>::PathFinder(int a, int b){
	//map.resize(a);
	map = new pathNode *[a];
	for (int i = 0; i < a; i++){
		//map[i].resize(b);
		map[i] = new pathNode[b];
		for (int j = 0; j < b; j++){
			map[i][j] = defaultPathNode();
			map[i][j].x = i;
			map[i][j].y = j;
		}
	}

	rows = a;
	cols = b;

	openL = new HeapPathFinder(a*b);
	queueP = new LinkedList<cocos2d::Point>();
}

template<class T>
void PathFinder<T>::resetMap(){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			map[i][j] = defaultPathNode();
			map[i][j].H = 10 * (abs(i - end.x) + abs(j - end.y));
			map[i][j].x = i;
			map[i][j].y = j;
		}
	}
	setStart(start.x, start.y);
}

template<class T>
void PathFinder<T>::setStart(int a, int b){
	start.x = a;
	start.y = b;
	map[start.x][start.y].parent = start;
	map[start.x][start.y].G = 0;
	map[start.x][start.y].F = map[start.x][start.y].H;
}

template<class T>
void PathFinder<T>::setEnd(int a, int b){
	end.x = a;
	end.y = b;
	resetMap();
}

template<class T>
void PathFinder<T>::setTileX(int x){
	tileX = x;
}

template<class T>
void PathFinder<T>::setTileY(int y){
	tileY = y;
}

template<class T>
void PathFinder<T>::setOffX(int x){
	offX = x;
}

template<class T>
void PathFinder<T>::setOffY(int y){
	offY = y;
}

template<class T>
int PathFinder<T>::getRows(){
	return rows;
}

template<class T>
int PathFinder<T>::getCols(){
	return cols;
}

template<class T>
pathNode PathFinder<T>::defaultPathNode(){
	pathNode p;
	p.blocked = false;
	p.permaBlocked = false;
	p.closed = false;
	p.F = 10000;
	p.G = 10000;
	p.H = 10000;
	p.heapPos = -1;
	pairC pt; pt.x = -1; pt.y = -1;
	p.parent = pt;
	p.taken = false;
	p.success = false;
	p.checked = false;

	return p;
}

template<class T>
void PathFinder<T>::set(int x, int y, pathNode a){
	map[x][y] = a;
}

template<class T>
void PathFinder<T>::block(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].blocked = true;
	}
}

template<class T>
void PathFinder<T>::permaBlock(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].permaBlocked = true;
	}
}

template<class T>
void PathFinder<T>::unblock(int x, int y){
	if (!map[x][y].permaBlocked && !(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].blocked = false;
	}
}

template<class T>
bool PathFinder<T>::checkBlock(int x, int y){
	return map[x][y].blocked;
}

template<class T>
void PathFinder<T>::taken(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].taken = true;
	}
}

template<class T>
void PathFinder<T>::untaken(int x, int y){
	if (!map[x][y].permaBlocked && !(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].taken = false;
	}
}

template<class T>
bool PathFinder<T>::checkTaken(int x, int y){
	return map[x][y].taken;
}

//Be careful about the new change function!!!
template<class T>
LinkedList<cocos2d::Point> * PathFinder<T>::solve(){
	queueP->reset();
	openL->insert(&map[start.x][start.y]);
	while (openL->getLength() > 0){
		pathNode curr = openL->extract();
		if (curr.x == end.x && curr.y == end.y){
			map[curr.x][curr.y].closed = true;
			//map[end.x][end.y].parent.x = curr.x;
			//map[end.x][end.y].parent.y = curr.y;
			////cout << "breaking" << endl;
			break;
		}
		map[curr.x][curr.y].closed = true;
		
		//check left
		checkAdj(curr.x - 1, curr.y, curr.x, curr.y);
		
		//check right
		checkAdj(curr.x + 1, curr.y, curr.x, curr.y);

		//check up
		checkAdj(curr.x, curr.y - 1, curr.x, curr.y);

		//check down
		checkAdj(curr.x, curr.y + 1, curr.x, curr.y);

		//check upper left
		//checkAdj(curr.x - 1, curr.y - 1, curr.x, curr.y);

		//check upper right
		//checkAdj(curr.x + 1, curr.y - 1, curr.x, curr.y);

		//check lower left
		//checkAdj(curr.x - 1, curr.y + 1, curr.x, curr.y);

		//check lower right
		//checkAdj(curr.x + 1, curr.y + 1, curr.x, curr.y);
		
	}
	pathNode curr = map[end.x][end.y];
	while (!map[curr.x][curr.y].success && map[curr.x][curr.y].parent.x != -1
			&& (curr.x != start.x || curr.y != start.y)){
		cocos2d::Point temp;
		temp.x = curr.x * tileX + offX;
		temp.y = curr.y * tileY + offY;
		/*GOING
		 * TO
		 * HAVE
		 * TO
		 * FIX
		 * THIS*/
		temp.y = -1 * (temp.y - 750) + 750;

		temp.y = 16 + curr.y * 32;
		temp.x = 16 + curr.x * 32;

		queueP->addFront(temp);
		map[curr.x][curr.y].success = true;
		curr = map[curr.parent.x][curr.parent.y];
		////cout << curr.x << " " << curr.y << "\t" << map[curr.x][curr.y].x << " " << map[curr.x][curr.y].y << endl

	}
	if (curr.x == start.x && curr.y == start.y){
		//cout << "SUCCESS!!!" << endl;
	}
	else{
		//cout << "Failed :(" << endl;
	}
	////cout << "done" << endl;
	return queueP;
}

template<class T>
void PathFinder<T>::checkAdj(int x1, int y1, int x2, int y2){
	//cout << openL->getLength() << endl;
	int cost = 14;
	if (x1 == x2 || y1 == y2){
		cost = 10;
	}
	if (x1 >= 0 && y1 >= 0 && x1 <= rows - 1 && y1 <= cols - 1 && !map[x1][y1].closed && !map[x1][y1].blocked){
		////cout <<cost << "  " <<  x1 << "," << y1 << "\t" << x2 << "," << y2 << "\t" << map[x1][y1].G << " " << map[x2][y2].G + cost << "\t";
		if (map[x1][y1].G > map[x2][y2].G + cost){
			////cout << "true ";
			map[x1][y1].G = map[x2][y2].G + cost;
			map[x1][y1].parent.x = x2;
			map[x1][y1].parent.y = y2;
			map[x1][y1].F = map[x1][y1].G + map[x1][y1].H;
			
			//for debugging
			map[x1][y1].checked = true;
			
			//check to see if left is in heap. If not, add it
			if (map[x1][y1].heapPos == -1){
				////cout << "insert " << map[x1][y1].parent.x << " " << map[x1][y1].parent.y;
				openL->insert(&map[x1][y1]);
			}
			else{
				////cout << "change";
				openL->change(&map[x1][y1]);
			}
		}
		////cout << endl;
	}
}

template<class T>
int PathFinder<T>::abs(int a){
	if(a > 0){
		return a;
	} else{
		return a * -1;
	}
}

template<class T>
void PathFinder<T>::print(){
	////cout << endl << "map" << endl << endl;
	////cout << start.x << " " << start.y << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			//if ( map[i][j].success && (i != start.x && j != start.y) && (i != end.x && j != end.y) ){
			if (map[i][j].success && !( (i == start.x && j == start.y) || (i == end.x && j == end.y) ) ){
				//cout << "+";
			}
			else if (map[i][j].checked == true && !map[i][j].blocked && !((i == start.x && j == start.y) || (i == end.x && j == end.y))){
				//cout << "*";
			}
			else if (!map[i][j].blocked && !((i == start.x && j == start.y) || (i == end.x && j == end.y))){
				//cout << " ";
			}
			else if (map[i][j].blocked){
				//cout << "#";
			}
			else if ((i == start.x && j == start.y) || (i == end.x && j == end.y)){
				//cout << "O";
			}
			else{
				//cout << "X";
			}
		}
		//cout << endl;
	}

	/*
	////cout << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			if (map[i][j].parent.x == -1){
				////cout << "x,x  ";
			}
			else{
				////cout << map[i][j].parent.x << "," << map[i][j].parent.y << "  ";
			}
		}
		////cout << endl;
	}

	//cout << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%5d ", map[i][j].F);
		}
		//cout << endl;
	}

	//cout << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%5d ", map[i][j].G);
		}
		//cout << endl;
	}

	//cout << endl;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			printf("%5d ", map[i][j].H);
		}
		//cout << endl;
	}
	*/

}

#endif
