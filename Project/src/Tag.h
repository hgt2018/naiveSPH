#pragma once
#include "constant.h"
#include <cstring>

#if (PROGRAM_TYPE == POVRay_OUTPUT_Grid)


class Element_Tag {
public:
	char *tag;
	
	Element_Tag() {
		tag = new char[N_element / 8];
	}

	~Element_Tag() {
		delete [] tag;
	}

	int check(int x, int y, int z) {
		int temp = x * Tag_Temp4 + y * Tag_Temp3 + z;
		int k = temp / 8;
		int b = temp - 8 * k;
		int a = (int)tag[k];
		return ((a >> (7 - b)) & 1);
	}

	void set(int x, int y, int z) {
		int temp = x * Tag_Temp4 + y * Tag_Temp3 + z;
		int k = temp / 8;
		int b = temp - 8 * k;
		int a = (int)tag[k];
		a |= (1 << (7 - b));
		tag[k] = (char)a;
	}

	void clear() {
		if (tag)
			memset(tag, 0, N_element / 8);
	}
} E_Tag;

#endif


/*
inline int Grid_return_index(int x, int y, int z) {
return x * Grid_Temp4 + y * Grid_Temp2 + z;
}

inline bool Grid_exist(int x, int y, int z) {
return (x >= 0 && y >= 0 && z >= 0 && x < Grid_Temp1 && y < Grid_Temp3 && z < Grid_Temp2);
}


class List {
public:
int index;
List *next;

List():index(-1), next(NULL) {}
List(int n):index(n), next(NULL) {}
~List() { if (next) delete next; }

void append(int x) {
if (index == -1) {
index = x;
}
else if (next == NULL) {
next = new List(x);
}
else {
next->append(x);
}
}

void append(List *ptr) {
if (index == -1) {
index = ptr->index;
next = ptr->next;
}
else if (next == NULL) {
next = ptr;
}
else {
next->append(ptr);
}
}

void clear() {
if (next)
delete next;
index = -1;
}

bool exist() {
return (index != -1);
}
};


class Particle_Grid {
public:
List *tag;

Particle_Grid() {
tag = new List[N_grid];
}

~Particle_Grid() {
delete [] tag;
}
} Grid;

List* check(int x, int y, int z) {
List * ptr = new List;
// 待定，看计算要用的形式
int own = Grid_return_index(x, y, z);
for (int i = 0; i < 26; ++i) {
int a = x + grid_offset[i][0];
int b = y + grid_offset[i][1];
int c = z + grid_offset[i][2];
if (Grid_exist(a, b, c)) {
int temp = Grid_return_index(a, b, c);
ptr->append(Grid.tag + temp);
}
}
return ptr;
}
*/
