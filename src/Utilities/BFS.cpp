#include "src/Utilities/BFS.h"


BFS::BFS(int l, int w)
{
	L = l;
	W = w;
	pf = 0;
	map.resize(l);
	for (int i = 0; i < L; i++){
		map[i].resize(w);
		for (int j = 0; j < W; j++){
			map[i][j].parent.x = i;
			map[i][j].parent.y = j;
			map[i][j].dir = 0;
			map[i][j].blocked = false;
			map[i][j].checked = false;
		}
	}
}

void BFS::setStart(int x, int y){
	start.x = x;
	start.y = y;
	if(pf != 0){
		pf->setBFSStart(start.x, start.y);
	}
}

void BFS::solve(){
	//CCLOG("BFS SOLVING");
	for (int i = 0; i < L; i++){
		for (int j = 0; j < W; j++){
			map[i][j].parent.x = i;
			map[i][j].parent.y = j;
			map[i][j].dir = 0;
			map[i][j].checked = false;
		}
	}
	queueBFS->addBack(start);
	setChecked(start.x, start.y);
	//CCLOG("solve start: %d, %d", start.x, start.y);
	while (!queueBFS->empty()){
		mpair n;

		//need to add getfront
		if (!queueBFS->get(0)->empty) {
			n = queueBFS->get(0)->data;
		}
		else {
			n = queueBFS->get(1)->data;
		}
		//CCLOG("%d, %d", n.x, n.y);

		queueBFS->removeFront();
		if (n.x >= 0 && n.x < L && n.y >= 0 && n.y < W){

			/*   l
			 * d   u
			 *   r
			 */


			//check up
			checkNeighbour(-1, 0, n, "u");

			//check down
			checkNeighbour(1, 0, n, "d");

			//check left
			checkNeighbour(0, -1, n, "l");

			//check right
			checkNeighbour(0, 1, n, "r");

			//check diagonals
			if (n.x > 1 && n.x < W - 1 && n.y > 0 && n.y < L - 1){
				//check up left
				if (n == start ||
					(map[n.x][n.y].dir != map[n.x - 1][n.y].dir && map[n.x][n.y].dir != map[n.x][n.y - 1].dir) ||
					(map[n.x - 1][n.y].dir == 0 || map[n.x][n.y - 1].dir == 0) ){
					checkNeighbour(-1, -1, n, "ul");
				}

				//check up right
				if (n == start ||
					(map[n.x][n.y].dir != map[n.x - 1][n.y].dir && map[n.x][n.y].dir != map[n.x][n.y + 1].dir) ||
					(map[n.x - 1][n.y].dir == 0 || map[n.x][n.y + 1].dir == 0) ){
					checkNeighbour(-1, 1, n, "ur");
				}

				//check down left
				if (n == start ||
					(map[n.x][n.y].dir != map[n.x + 1][n.y].dir && map[n.x][n.y].dir != map[n.x][n.y - 1].dir) ||
					(map[n.x + 1][n.y].dir == 0 || map[n.x][n.y - 1].dir == 0) ){
					checkNeighbour(1, -1, n, "dl");
				}

				//check down right
				if (n == start ||
					(map[n.x][n.y].dir != map[n.x + 1][n.y].dir && map[n.x][n.y].dir != map[n.x][n.y + 1].dir) ||
					(map[n.x + 1][n.y].dir == 0 || map[n.x][n.y + 1].dir == 0) ){
					checkNeighbour(1, 1, n, "dr");
				}
			}

		}
	}
	//CCLOG("BFS SOLVED");
}

void BFS::checkNeighbour(int dx, int dy, mpair n, char* dir){
	if (n.x + dx >= 0 && n.x + dx < L && n.y + dy >= 0 && n.y + dy < W){
		if (!map[n.x + dx][n.y + dy].checked && !map[n.x + dx][n.y + dy].blocked){
			//cout << n.x << " " << n.y << "\t" << n.x + dx << " " << n.y + dy << endl;
			map[n.x + dx][n.y + dy].parent.x = n.x;
			map[n.x + dx][n.y + dy].parent.y = n.y;
			pf->setBFSParent(n.x, n.y, dx, dy);
			map[n.x + dx][n.y + dy].checked = true;
			map[n.x + dx][n.y + dy].dir = dir;
			pf->setBFSDir(n.x + dx, n.y + dy, dir);
			mpair addToQueue;
			addToQueue.x = n.x + dx;
			addToQueue.y = n.y + dy;
			queueBFS->addBack(addToQueue);

			//CCLOG("%s", dir);

			//print();
			//std::cin.ignore();
			//system("cls");
		}
	}
}

void BFS::print(){
	for (int i = 0; i < L; i++){
		for (int j = 0; j < W; j++){
			char c;
			if (map[i][j].blocked){
				c = 'x';
			}
			else if (map[i][j].dir == "u" || map[i][j].dir == "d"){
				c = '|';
			}
			else if (map[i][j].dir == "l" || map[i][j].dir == "r"){
				c = '-';
			}
			else if (map[i][j].dir == "ul" || map[i][j].dir == "dr"){
				c = 92;
			}
			else if (map[i][j].dir == "ur" || map[i][j].dir == "dl"){
				c = '/';
			}
			else{
				c = 'o';
			}
			cout << c << " ";
		}
		cout << endl;
	}
}


BFS::~BFS()
{
}
