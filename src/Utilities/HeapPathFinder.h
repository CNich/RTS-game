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

HeapPathFinder::HeapPathFinder(int L){
	arr.resize(L);
	//arr = new pathNode *[L];
	size = L;
	length = 0;
}

HeapPathFinder::~HeapPathFinder(){
	//delete arr;
}

int HeapPathFinder::getLength(){
	return length;
}

void HeapPathFinder::insert(pathNode *a){
	arr[length] = a;
	arr[length]->heapPos = length;
	//arr[length]->F = arr[length]->G + arr[length]->H;
	up(length);
	length++;
}

void HeapPathFinder::print(){
	for (int i = 0; i < length; i++){
		//int temp = *arr[i];
		////cout << arr[i]->F << " ";
	}
	////cout << endl;
	//////cout << "       : ";
	//for (int i = 0; i < length; i++){
	//	//int temp = *arr[i];
	//	////cout << arr[i]->heapPos << "  ";
	//}
	//////cout << endl;
}

void HeapPathFinder::setG(int a, int F){
	if (a < length) {
		arr[a]->F = F;
		change(a, *arr[a]);
	}
}

void HeapPathFinder::up(int a){
	int b = a + 1;
	//int bo2 = floor(b / 2.0);
	int bo2 = b / 2;
	////cout << b - 1 << "\t" << bo2 - 1;
	if (bo2 - 1 >= 0 && *arr[b - 1] < *arr[bo2 - 1]){
		swap(b - 1, bo2 - 1);
		up(bo2 - 1);
	}

}

void HeapPathFinder::down(int a){
	bool heapDebug = false;
	if(length > 2000) heapDebug = true;
	int b = a + 1;
	if(heapDebug) CCLOG("length: %d, b: %d, a: %d", length, b, a);
	if (length >= (b * 2 + 1)){
		if(heapDebug) CCLOG("first if");
		auto left = *arr[b * 2 - 1];
		auto right = *arr[b * 2 + 1 - 1];
		//////cout << "a: " << a << "\t" << "b: " << b << "\t" << "arr[" << b - 1 << "]" << arr[b - 1]->F
		//	<< "\t" << "arr[" << b * 2 - 1 << "]" << arr[b * 2 - 1]->F << "\t" << "arr[" << b * 2 + 1 - 1
		//	<< "]" << "\t" << arr[b * 2 + 1 - 1]->F << endl;
		if(heapDebug) CCLOG("left: %d, right: %d", left.F, right.F);
		if (left <= right){
			if(heapDebug) CCLOG("left <= right");
			if(heapDebug) CCLOG("arr[b - 1]: %d, arr[b * 2 - 1]: %d", arr[b - 1]->F, arr[b * 2 - 1]->F);
			if(heapDebug) CCLOG("%d", (*arr[b - 1] > *arr[b * 2 - 1]));
			if (*arr[b - 1] > *arr[b * 2 - 1]){
				if(heapDebug) CCLOG("left <= right inner");
				swap(b - 1, b * 2 - 1);
				down(b * 2 - 1);
			}
		}
		else{
			//	////cout << "a: " << a << "\t" << "b: " << b << "\t" << "arr[" << b - 1 << "]" << arr[b - 1]->F
			//		<< "\t" << "arr[" << b * 2 - 1 << "]" << arr[b * 2 - 1]->F << "\t" << "arr[" << b * 2 + 1 - 1
			//		<< "]" << "\t" << arr[b * 2 + 1 - 1]->F << endl;
			if(heapDebug) CCLOG("else:");
			if(heapDebug) CCLOG("arr[b - 1]: %d, arr[b * 2 + 1 - 1]: %d", arr[b - 1]->F, arr[b * 2 + 1 - 1]->F);
			if(heapDebug) CCLOG("%d", (*arr[b - 1] > *arr[b * 2 + 1 - 1]));
			if (*arr[b - 1] > *arr[b * 2 + 1 - 1]){
				if(heapDebug) CCLOG("left <= right else inner");
				swap(b - 1, b * 2 + 1 - 1);
				down(b * 2 + 1 - 1);
			}
		}
	}
	else if (length >= (b * 2)){
		if(heapDebug) CCLOG("length >= b*2");
		auto left = *arr[b * 2 - 1];
		if(heapDebug) CCLOG("%d", (*arr[b - 1] > *arr[b * 2 - 1]));
		if(heapDebug) CCLOG("*arr[b - 1]: %d, *arr[b * 2 - 1]: %d", arr[b - 1]->F, arr[b * 2 - 1]->F);
		if (*arr[b - 1] > *arr[b * 2 - 1]){
			if(heapDebug) CCLOG("else else inner");
			swap(b - 1, b * 2 - 1);
			down(b * 2 - 1);
		}
	}
}

pathNode HeapPathFinder::extract(){
	auto temp = *arr[0];
	length--;
	if (length >= 1){
		swap(0, length);
		down(0);
	}
	//////cout << "temp:\t" << temp
	return temp;
}

void HeapPathFinder::sort(){
	////cout << "Sorting" << endl;
	while (length > 0){
		pathNode temp = extract();
		////cout << length << "\t" << temp.F << endl;
	}
}

bool HeapPathFinder::check(int a){
	int b = a + 1;
	int bo2 = b / 2.0;
	//////cout << "a: " << a << endl;
	//////cout << b / 2 - 1 << endl;
	//////cout << arr[0]->F << endl;
	//////cout << arr[b - 1]->F << " " << arr[b / 2 - 1]->F << endl;
	bool par = true;
	bool left = true;
	bool right = true;
	//pathNode t = *arr[b - 1];

	if (b > 1 && *arr[b - 1] < *arr[bo2 - 1]){
		par = false;
	}
	if (length > (b * 2) - 1 && *arr[b - 1] > *arr[(b * 2) - 1]){
		left = false;
	}
	if (length > (b * 2) && *arr[b - 1] > *arr[(b * 2) - 1 + 1]){
		right = false;
	}

	if (a == 0 && length > (b * 2) - 1 + 1){
		////cout << par << " " << left << " " << right << "\t";
		////cout << "< " << " " << arr[a]->F << " " << arr[(b * 2) - 1]->F << " " << arr[(b * 2) - 1 + 1]->F << endl;
	}
	else if (a == 0 && length > (b * 2) - 1){
		////cout << par << " " << left << " " << right << "\t";
		////cout << "< " << " " << arr[a]->F << " " << arr[(b * 2) - 1]->F << " >" << endl;
	}
	else if (a == 0){
		////cout << par << " " << left << " " << right << "\t";
		////cout << "< " << " " << arr[a]->F << " >  >" << endl;
	}
	else if (length > (b * 2) && a > 0){
		////cout << par << " " << left << " " << right << "\t";
		////cout << arr[bo2 - 1]->F << " " << arr[a]->F << " " << arr[(b * 2) - 1]->F << " " << arr[(b * 2) - 1 + 1]->F << endl;
	}
	else if (length > (b * 2) - 1 && a > 0){
		////cout << par << " " << left << " " << right << "\t";
		////cout << arr[bo2 - 1]->F << " " << arr[a]->F << " " << arr[(b * 2) - 1]->F << " " << " >" << endl;
	}
	else{
		////cout << par << " " << left << " " << right << "\t";
		////cout << arr[bo2 - 1]->F << " " << arr[a]->F << endl;
	}

	if (par && left && right){
		return true;
	}
	else{
		return false;
	}
}

void HeapPathFinder::checkAll(){
	////cout << "checking all..." << endl;
	for (int i = 0; i < length; i++){
		this->check(i);
	}
}

void HeapPathFinder::change(int a, pathNode val){
	*arr[a] = val;
	int b = a + 1;
	//int bo2 = floor(b / 2.0);
	int bo2 = b / 2;
	if (arr[b - 1] < arr[bo2 - 1]){
		up(b - 1);
	}
	else if (length > 2 * b - 1 + 1 && ((arr[b - 1] > arr[b * 2 - 1]) || (arr[b - 1] > arr[b * 2 - 1 + 1]))){
		down(b - 1);
	}
	else if (length > 2 * b - 1 && (arr[b - 1] > arr[b * 2 - 1])){
		down(b - 1);
	}
}

void HeapPathFinder::change(int a){
	//////cout << "changing... " << endl << endl;
	int b = a + 1;
	//int bo2 = floor(b / 2.0);
	int bo2 = b / 2;
	if (a >= 0 && a < length){
		if (b > 1 && *arr[b - 1] < *arr[bo2 - 1]){
			////cout << "case 2" << endl;
			////cout << b - 1 << ": " << arr[b - 1]->F << "\t" << bo2 - 1 << ": " << arr[bo2 - 1]->F << endl;
			up(b - 1);
			//////cout << "up" << endl;
		}
		else if (length > 2 * b - 1 + 1 && ((*arr[b - 1] > *arr[b * 2 - 1]) || (*arr[b - 1] > *arr[b * 2 - 1 + 1]))){
			////cout << "case 3" << endl;
			down(b - 1);
			//////cout << "down" << endl;
		}
		else if (length > 2 * b - 1 && (*arr[b - 1] > *arr[b * 2 - 1])){
			////cout << "case 4" << endl;
			down(b - 1);
			//////cout << "down" << endl;
		}
	}
	////cout << endl << endl;
}

void HeapPathFinder::change(pathNode * val){
	if (val->G > 0 && val->H > 0) val->F = val->G + val->H;
	//cout << "change\t" << val->F << "\t" << val->G << "\t" << val->H << endl;
	change(val->heapPos);
}

void HeapPathFinder::swap(int a, int b){
	//pathNode temp = *arr[a];
	//*arr[a] = *arr[b];
	//*arr[b] = temp;
	//
	//arr[a]->heapPos = a;
	//arr[b]->heapPos = b;

	//Remember what this does
	auto temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;

	arr[a]->heapPos = a;
	arr[b]->heapPos = b;

	//////cout << arr[a] << " " << arr[a]->F << "\t" << arr[b] << " " << arr[b]->F << endl;
}
/*
void HeapPathFinder::print(){
for (int i = 0; i < length; i++){
////cout << arr[i]->F << " ";
}
////cout << endl;
}
*/
#endif
