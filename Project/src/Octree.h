#pragma once
#include <iostream>
#include <cmath>
//#include <vector>

#include "constant.h"
#include "particle.h"
#include "model.h"

/*
#define OCTREE_XY_MAX_DEPTH 6
#define OCTREE_Z_MAX_DEPTH 4
*/
#define kernel_distance 0.75


/*
const int grid_offset[26][3] = { { -1, -1, -1 },{ -1, -1, 0 },{ -1, -1, 1 },
{ -1, 0, -1 },{ -1, 0, 0 },{ -1, 0, 1 },
{ -1, 1, -1 },{ -1, 1, 0 },{ -1, 1, 1 },
{ 0, -1, -1 },{ 0, -1, 0 },{ 0, -1, 1 },
{ 0, 0, -1 },{ 0, 0, 1 },{ 0, 1, -1 },
{ 0, 1, 0 },{ 0, 1, 1 },{ 1, -1, -1 },
{ 1, -1, 0 },{ 1, -1, 1 },{ 1, 0, -1 },
{ 1, 0, 0 },{ 1, 0, 1 },{ 1, 1, -1 },
{ 1, 1, 0 },{ 1, 1, 1 }
};
*/

inline double Min(double x, double y) {
	return (x < y ? x : y);
}


inline double Max(double x, double y) {
	return (x > y ? x : y);
}


inline double Min(double x, double y, double z) {
	double a = x < y ? x : y;
	return (a < z ? a : z);
}


class Node_Data {
public:
	int index;
	Node_Data* next;

	Node_Data() :index(-1), next(NULL) {}		// 初始条件没有粒子
	Node_Data(int n) :index(n), next(NULL) {}
	~Node_Data() { if (next) delete next; }

	void append(int x) {
		if (index == -1)
			index = x;
		else if (next)
			next->append(x);
		else

			next = new Node_Data(x);
	}

	void clear() {
		if (next)
			delete next;
		next = NULL;
		index = -1;
	}

	bool exist() {
		return (index != -1);
	}
};


// up-u,down-d		right-r,left-l		front-f,back-b
/*
/|\ up
|  (z)        left <------> right(x)		    ".":front (y)  "X":back
|down

+: right,front,up
*/


class Octree_Node {
public:
	Node_Data data;
	int depth;
	int Is_Leaf;
	double X_min, X_max, Y_min, Y_max, Z_min, Z_max;
	double X_mid, Y_mid, Z_mid;
	double diameter;		// 最小半径   

							// 子结点
	Octree_Node *ulf, *ulb, *urf, *urb, *dlf, *dlb, *drf, *drb;

	// 父节点指针以及在父节点的方位
	Octree_Node *parent;
	/*
	ROOT-O, ULF-A, ULB-B, URF-C, URB-D, DLF-E, DLB-F, DRF-G, DRB-H
	*/
	char orientation;

	Octree_Node(int D, Octree_Node* father, char c,
				double xmin, double xmax, double ymin,
				double ymax, double zmin, double zmax) :depth(D), parent(father), orientation(c),
				X_min(xmin), X_max(xmax), Y_min(ymin),
				Y_max(ymax), Z_min(zmin), Z_max(zmax),
				ulf(NULL), ulb(NULL), urf(NULL), urb(NULL),
				dlf(NULL), dlb(NULL), drf(NULL), drb(NULL) {
		X_mid = (X_min + X_max) / 2;
		Y_mid = (Y_min + Y_max) / 2;
		Z_mid = (Z_min + Z_max) / 2;
		diameter = Min(X_max - X_min, Y_max - Y_min, Z_max - Z_min);
		if (father)
			father->Is_Leaf = 0;
		Is_Leaf = 1;
	}


	~Octree_Node() {
		if (ulf) delete ulf;
		if (ulb) delete ulb;
		if (urf) delete urf;
		if (urb) delete urb;
		if (dlf) delete dlf;
		if (dlb) delete dlb;
		if (drf) delete drf;
		if (drb) delete drb;
	}


	void clear() {
		if (ulf) delete ulf;
		if (ulb) delete ulb;
		if (urf) delete urf;
		if (urb) delete urb;
		if (dlf) delete dlf;
		if (dlb) delete dlb;
		if (drf) delete drf;
		if (drb) delete drb;
		ulf = ulb = urf = urb = dlf = dlb = drf = drb = NULL;
	}

	Octree_Node* Get_Child(char);

	void Find_Octree_Leaf(int, double, double, double, double, double, double, void(*f)(int, Octree_Node*));

	void Print_Path() {
		if (parent)
			parent->Print_Path();
		std::cout << depth << orientation << "->";
	}
	void Print_M() {
		std::cout << "min: " << X_min << ' ' << Y_min << ' ' << Z_min << std::endl;
		std::cout << "max: " << X_max << ' ' << Y_max << ' ' << Z_max << std::endl;
	}
};


Octree_Node* Octree_Node::Get_Child(char c) {
	switch (c) {
	case 'O':
	default:
	return NULL;
	case 'A':
	return ulf;
	case 'B':
	return ulb;
	case 'C':
	return urf;
	case 'D':
	return urb;
	case 'E':
	return dlf;
	case 'F':
	return dlb;
	case 'G':
	return drf;
	case 'H':
	return drb;
	}
}


void Octree_Find_Min(int, Octree_Node*);
void Octree_Calculate_SPH(int, Octree_Node*);

// 这里要找到包含给定区间的叶节点
// 初次调用时保证给定区间在Root里，省去边界检查
void Octree_Node::Find_Octree_Leaf(int index, double xmin, double xmax, double ymin,
								   double ymax, double zmin, double zmax, void(*func)(int, Octree_Node*)) {
	if (Is_Leaf == 0) {
		if (xmin >= X_mid) { // xmin-xmax
			if (ymin >= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (urf)
						urf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (drf)
						drf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (urf)
						urf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, Z_mid, zmax, func);
					if (drf)
						drf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else if (ymax <= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (urb)
						urb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (drb)
						drb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (urb)
						urb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, Z_mid, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else { // ymin-Y_mid-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (urb)
						urb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, zmax, func);
					if (urf)
						urf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (drb)
						drb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, zmax, func);
					if (drf)
						drf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (urb)
						urb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, Z_mid, zmax, func);
					if (urf)
						urf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, Z_mid, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, Z_mid, func);
					if (drf)
						drf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, Z_mid, func);
				}
			}
		}
		else if (xmax <= X_mid) { // xmin-xmax
			if (ymin >= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, Z_mid, zmax, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else if (ymax <= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, Z_mid, zmax, func);
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else { // ymin-Y_mid-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, zmax, func);
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, zmax, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, Z_mid, zmax, func);
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, Z_mid, zmax, func);
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, xmax, ymin, Y_mid, zmin, Z_mid, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, xmax, Y_mid, ymax, zmin, Z_mid, func);
				}
			}
		}
		else { // xmin-X_mid-xmax
			if (ymin >= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, zmax, func);
					if (urf)
						urf->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, zmax, func);
					if (drf)
						drf->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, Z_mid, zmax, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, Z_mid, func);
					if (urf)
						urf->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, Z_mid, zmax, func);
					if (drf)
						drf->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else if (ymax <= Y_mid) { // ymin-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, zmax, func);
					if (urb)
						urb->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, Z_mid, zmax, func);
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, X_mid, ymin, ymax, zmin, Z_mid, func);
					if (urb)
						urb->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, Z_mid, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, X_mid, xmax, ymin, ymax, zmin, Z_mid, func);
				}
			}
			else { // ymin-Y_mid-ymax
				if (zmin >= Z_mid) { // zmin-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, X_mid, ymin, Y_mid, zmin, zmax, func);
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, X_mid, Y_mid, ymax, zmin, zmax, func);
					if (urb)
						urb->Find_Octree_Leaf(index, X_mid, xmax, ymin, Y_mid, zmin, zmax, func);
					if (urf)
						urf->Find_Octree_Leaf(index, X_mid, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else if (zmax <= Z_mid) { // zmin-zmax
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, X_mid, ymin, Y_mid, zmin, zmax, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, X_mid, Y_mid, ymax, zmin, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, X_mid, xmax, ymin, Y_mid, zmin, zmax, func);
					if (drf)
						drf->Find_Octree_Leaf(index, X_mid, xmax, Y_mid, ymax, zmin, zmax, func);
				}
				else { // zmin-Z_mid-zmax
					if (ulb)
						ulb->Find_Octree_Leaf(index, xmin, X_mid, ymin, Y_mid, Z_mid, zmax, func);
					if (ulf)
						ulf->Find_Octree_Leaf(index, xmin, X_mid, Y_mid, ymax, Z_mid, zmax, func);
					if (dlb)
						dlb->Find_Octree_Leaf(index, xmin, X_mid, ymin, Y_mid, zmin, Z_mid, func);
					if (dlf)
						dlf->Find_Octree_Leaf(index, xmin, X_mid, Y_mid, ymax, zmin, Z_mid, func);
					if (urb)
						urb->Find_Octree_Leaf(index, X_mid, xmax, ymin, Y_mid, Z_mid, zmax, func);
					if (urf)
						urf->Find_Octree_Leaf(index, X_mid, xmax, Y_mid, ymax, Z_mid, zmax, func);
					if (drb)
						drb->Find_Octree_Leaf(index, X_mid, xmax, ymin, Y_mid, zmin, Z_mid, func);
					if (drf)
						drf->Find_Octree_Leaf(index, X_mid, xmax, Y_mid, ymax, zmin, Z_mid, func);
				}
			}
		}
	}
	else {
		func(index, this);
	}
}


