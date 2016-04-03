#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include "src/Utilities/pathNode.h"
#include "src/Utilities/LinkedList.h"
#include "src/Utilities/HeapPathFinder.h"
#include "cocos2d.h"
#include <vector>

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
	int getTileX(){return tileX;};
	int getTileY(){return tileY;};
	int getOffX(){ return offX;};
	int getOffY(){ return offY;};

	//~PathFinder();
	PathFinder create();
	void set(int x, int y, pathNode a);
	pathNode defaultPathNode();
	void print();
	void permaBlock(int x, int y);
	bool checkPermaBlock(int x, int y) { return map[x][y].permaBlocked; };
	void setBFSParent(int x, int y, int dx, int dy);
	void setBFSDir(int x, int y, char *dir) { map[x][y].dir = dir; };
	char* getBFSDir(int x, int y) { return map[x][y].dir; };
	void setBFSStart(int x, int y) { BFSStart.x = x; BFSStart.y = y; };
	cocos2d::Point getBFSStart() { return BFSStart; };
	cocos2d::Point getBFSParent(int x, int y);
	void block(int x, int y);
	void unblock(int x, int y);
	bool checkBlock(int x, int y);
	void taken(int x, int y);
	void untaken(int x, int y);
	bool checkTaken(int x, int y);
	LinkedList<cocos2d::Point> * solve();
	int getRows();
	int getCols();

	void setUnit(int x, int y, T *ty) { map[x][y].unit = ty; };
	void setUnitZero(int x, int y) { map[x][y].unit = 0; };
	T* getUnit(int x, int y) { return map[x][y].unit; };

private:
	//pathNode **map;
	std::vector<std::vector<pathNode> > map;
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
	cocos2d::Point BFSStart;

};

/*
 * Initialize the pathfinder's map
 */
template<class T>
PathFinder<T>::PathFinder(int a, int b, pairC s, pairC e){
	map.resize(a);
	//map = new pathNode *[a];
	for (int i = 0; i < a; i++){
		map[i].resize(b);
		//map[i] = new pathNode[b];
		for (int j = 0; j < b; j++){
			//set map coordinate to default values
			map[i][j] = defaultPathNode();

			//calculate distance to travel from current node to the end
			map[i][j].H = 10 * (abs(i - e.x) + abs(j - e.y));

			//set map coordinate's pathNode's coordinate
			map[i][j].x = i;
			map[i][j].y = j;
		}
	}
	//initialize start node and end coordinate
	start = s;
	end = e;
	map[s.x][s.y].parent = s;
	map[s.x][s.y].G = 0;
	map[s.x][s.y].F = map[s.x][s.y].H;

	//save size of map
	rows = a;
	cols = b;

	//initialize heap and queue
	openL = new HeapPathFinder(a*b);
	queueP = new LinkedList<cocos2d::Point>();
}

/*
 * Initialize the pathfinder's map WITHOUT a start and end
 */
template<class T>
PathFinder<T>::PathFinder(int a, int b){
	map.resize(a);
	//map = new pathNode *[a];
	for (int i = 0; i < a; i++){
		map[i].resize(b);
		//map[i] = new pathNode[b];
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

/*
 * Set all map nodes to defaultPathNode
 */
template<class T>
void PathFinder<T>::resetMap(){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			map[i][j] = defaultPathNode();
			map[i][j].H = 10 * (abs(i - end.x) + abs(j - end.y));//distance to end * 10

			//set pathNode's location
			map[i][j].x = i;
			map[i][j].y = j;
		}
	}
	setStart(start.x, start.y);
}

/*
 * Initialize start node
 */
template<class T>
void PathFinder<T>::setStart(int a, int b){
	start.x = a;
	start.y = b;

	//set parent to self at the start
	map[start.x][start.y].parent = start;

	//cost to get to the start node = 0
	map[start.x][start.y].G = 0;

	//since F(start) = H(start) + ( G(start) = 0)
	map[start.x][start.y].F = map[start.x][start.y].H;
}

/*
 * Whenever we set a new end, the entire map has to be recalculated as
 * all of the H values are now different
 */
template<class T>
void PathFinder<T>::setEnd(int a, int b){
	end.x = a;
	end.y = b;
	resetMap();
}

/*
 * Set size of X tile
 */
template<class T>
void PathFinder<T>::setTileX(int x){
	tileX = x;
}

/*
 * Set size of Y tile
 */
template<class T>
void PathFinder<T>::setTileY(int y){
	tileY = y;
}

/*
 * Set size of X tile offset
 */
template<class T>
void PathFinder<T>::setOffX(int x){
	offX = x;
}

/*
 * Set size of Y tile offset
 */
template<class T>
void PathFinder<T>::setOffY(int y){
	offY = y;
}

/*
 * Set map size
 */
template<class T>
int PathFinder<T>::getRows(){
	return rows;
}

/*
 * Set map size
 */
template<class T>
int PathFinder<T>::getCols(){
	return cols;
}

/*
 * Get next coordinate of BFS path
 */
template<class T>
cocos2d::Point PathFinder<T>::getBFSParent(int x, int y){
	cocos2d::Point temp;

	//convert BFS parent from map to game coordinates
	temp.y = offY + map[x][y].bfsParent.y * tileY;
	temp.x = offX + map[x][y].bfsParent.x * tileX;

	//CCLOG("offY: %d, tileY: %d, offX: %d, tileX: %d", offY, tileY, offX, tileX);

	//temp.y = 16 + map[x][y].bfsParent.y * 32;
	//temp.x = 16 + map[x][y].bfsParent.x * 32;

	return temp;
};

/*
 * Set BFS node's parent
 */
template<class T>
void PathFinder<T>::setBFSParent(int x, int y, int dx, int dy) {
	map[x + dx][y + dy].bfsParent.x = x;
	map[x + dx][y + dy].bfsParent.y = y;
};

/*
 * Initialize path node
 * Sets the blocked status to false
 * Sets the pathfinder F G H to "infinity"
 */
template<class T>
pathNode PathFinder<T>::defaultPathNode(){
	pathNode p;
	p.blocked = false;
	p.permaBlocked = false;
	p.closed = false;

	//F(n) = G(n) + H(n)
	p.F = 10000;

	//G(n) = total cost to get to the current node
	//eg. G(start) = 0
	p.G = 10000;

	//H(n) = distance from node n to the end node
	p.H = 10000;

	//position in the heap
	p.heapPos = -1;

	//node n's parent
	//if parent = -1, this node hasn't been visited yet
	pairC pt; pt.x = -1; pt.y = -1;
	p.parent = pt;

	p.taken = false;
	p.success = false;
	p.checked = false;

	//who occupies this current node (in this case it is a BasicUnit
	p.unit = 0;

	return p;
}

/*
 * Set map coordinate's pathNode
 */
template<class T>
void PathFinder<T>::set(int x, int y, pathNode a){
	map[x][y] = a;
}

/*
 * Dynamically block a map point (not a valid point on any path finder)
 * Generally done when a unit is moving
 */
template<class T>
void PathFinder<T>::block(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].blocked = true;
	}
}

/*
 * Block map point (not a valid point on any path finder)
 * Permablock means that it CANNOT be unblocked
 * This is only called at the init() of a Scene by it's setMap() call
 * Only blocked points on the tilemap are permablocked
 */
template<class T>
void PathFinder<T>::permaBlock(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].permaBlocked = true;
	}
}

/*
 * Unblock a map coordinate
 * Called during unit's movement
 */
template<class T>
void PathFinder<T>::unblock(int x, int y){
	if (!map[x][y].permaBlocked && !(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].blocked = false;
	}
}

/*
 * Check if a map coordinate is blocked
 */
template<class T>
bool PathFinder<T>::checkBlock(int x, int y){
	return map[x][y].blocked;
}

/*
 * Set a map coordinate to taken (not yet implemented)
 */
template<class T>
void PathFinder<T>::taken(int x, int y){
	if (!(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].taken = true;
	}
}

/*
 * Set a map coordinate to untaken (not yet implemented)
 */
template<class T>
void PathFinder<T>::untaken(int x, int y){
	if (!map[x][y].permaBlocked && !(x == end.x && y == end.y) && !(x == start.x && y == start.y) && x >= 0 && x < rows && y >= 0 && y < cols){
		map[x][y].taken = false;
	}
}

/*
 * Check if map coordinate is taken (not yet implemented)
 */
template<class T>
bool PathFinder<T>::checkTaken(int x, int y){
	return map[x][y].taken;
}

/*
 * Be careful about the new change function!!!
 * Find the shorted path from the start node to the end node
 * This is the actual implementation of the A* Search algorithm
 */
template<class T>
LinkedList<cocos2d::Point> * PathFinder<T>::solve(){
	//reset the queue
	queueP->reset();

	//insert start into the heap
	openL->insert(&map[start.x][start.y]);

	//main A* loop
	//check the first item on the heap/mark it as visited
	//add it's unchecked neighbors to the heap
	//keep doing this until the heap is empty or we get to the finish (end) node
	while (openL->getLength() > 0){

		//extract the first item in the heap
		pathNode curr = openL->extract();

		//if we have reached the end node, then no need to check the rest of the heap
		if (curr.x == end.x && curr.y == end.y){
			map[curr.x][curr.y].closed = true;
			break;
		}
		//mark current node as visited
		map[curr.x][curr.y].closed = true;
		
		//check all 8 adjacent nodes to see if they can be added to the heap
		//check left
		checkAdj(curr.x - 1, curr.y, curr.x, curr.y);
		
		//check right
		checkAdj(curr.x + 1, curr.y, curr.x, curr.y);

		//check up
		checkAdj(curr.x, curr.y - 1, curr.x, curr.y);

		//check down
		checkAdj(curr.x, curr.y + 1, curr.x, curr.y);

		//check upper left
		checkAdj(curr.x - 1, curr.y - 1, curr.x, curr.y);

		//check upper right
		checkAdj(curr.x + 1, curr.y - 1, curr.x, curr.y);

		//check lower left
		checkAdj(curr.x - 1, curr.y + 1, curr.x, curr.y);

		//check lower right
		checkAdj(curr.x + 1, curr.y + 1, curr.x, curr.y);
		
	}
	//either the end node was reached or the the heap ran out
	pathNode curr = map[end.x][end.y];

	//travel parent to parent starting from the end node until the start node
	//break if current node is already a success (should never happen)
	//break if parent = -1 (should only happen if the end node was never reached)
	//Results are recorded as pathNodes in the queue (queueP)
	while (!map[curr.x][curr.y].success && map[curr.x][curr.y].parent.x != -1
			&& (curr.x != start.x || curr.y != start.y)){
		cocos2d::Point temp;

		//need to convert from the map coordinates (0, 1, 2...tileX) to the game coordinates (tilemap)
		temp.x = curr.x * tileX + offX;
		temp.y = curr.y * tileY + offY;

		queueP->addFront(temp);

		//set current node to a success node (it is in the final solution)
		map[curr.x][curr.y].success = true;

		//set current node to it's parent
		curr = map[curr.parent.x][curr.parent.y];
	}

	//for printing purposes
	if (curr.x == start.x && curr.y == start.y){
		//cout << "SUCCESS!!!" << endl;
	}
	else{
		//cout << "Failed :(" << endl;
	}
	////cout << "done" << endl;
	return queueP;
}

/*
 * Check node's neighbors
 * (x1,y1) = neighbor
 * (x2,y2) = current node
 */
template<class T>
void PathFinder<T>::checkAdj(int x1, int y1, int x2, int y2){
	//cout << openL->getLength() << endl;

	//diagonal neighbors cost are 14
	//straight neighbors cost are 10
	int cost = 14;
	if (x1 == x2 || y1 == y2){
		cost = 10;
	}

	//check if node is within the bounds, already visited, or blocked
	if (x1 >= 0 && y1 >= 0 && x1 <= rows - 1 && y1 <= cols - 1 && !map[x1][y1].closed && !map[x1][y1].blocked){

		//if the cost to get to the neighbor's node is currently greater than getting there
		//through the current node, switch it's G value and update it's position in the heap
		if (map[x1][y1].G > map[x2][y2].G + cost){

			//set new cost of neighbor to current node's cost + 10 or 14
			map[x1][y1].G = map[x2][y2].G + cost;

			//set parent of neighbor to current node
			map[x1][y1].parent.x = x2;
			map[x1][y1].parent.y = y2;

			//calculate new F value
			map[x1][y1].F = map[x1][y1].G + map[x1][y1].H;
			
			//for debugging
			map[x1][y1].checked = true;
			
			//check to see if neighbor is in heap. If not, add it
			//items not in the heap have heapPos = -1
			if (map[x1][y1].heapPos == -1){
				openL->insert(&map[x1][y1]);
			}
			//if the neighbor is already in the heap, update it's position
			else{
				openL->change(&map[x1][y1]);
			}
		}
	}
}

/*
 * Return positive value of a number
 */
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
