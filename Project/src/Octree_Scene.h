#pragma once
#include "Octree.h"

class Octree_Scene {
public:
	Octree_Node* Root;

	double Octree_Threshold;

	int Save_min_index;
	double Save_min_dis;

	void Insert(int);
	void Act_With_Neighbour(int);

	Octree_Scene() {
		Root = new Octree_Node(0, NULL, 'O', Scene_Xmin, Scene_Xmax, Scene_Ymin, Scene_Ymax, Scene_Zmin, Scene_Zmax);
		Octree_Threshold = OCTREE_SCENE_THRESHOLD;
		Model_Import();
		for (int i = 0; i < Model_Num; ++i) {
			if (debug_cnt == 3) {
				if (i == 2144)
					model[i].position.Print();
				std::cout << i << std::endl;
			}
			Insert(i);
		}
		//std::cout << "scene complete\n";
	}

	~Octree_Scene() { delete Root; }
	void clear() { Root->clear(); }
} Scene;


void Octree_Find_Min(int index, Octree_Node* ptr) {
	Node_Data* temp = &(ptr->data);
	/*if (index == 1) {
	ptr->Print_M();
	particle[index].Print();
	}*/
	if (temp->index != -1) {
		do {
			/*if (index == 1)
			std::cout << "*********:  " << temp->index << std::endl;*/
			double d = ((particle[index].position - model[temp->index].position).norm2());
			if (d < Scene.Save_min_dis) {
				Scene.Save_min_index = temp->index;
				Scene.Save_min_dis = d;
			}
			temp = temp->next;
		} while (temp);
	}
}


void Octree_Scene::Insert(int index) {
	Octree_Node *temp = Root;
	while (temp->diameter > Octree_Threshold) {
		double x = model[index].position.x, y = model[index].position.y, z = model[index].position.z;
		/*std::cout << x << ' ' << y << ' ' << z << std::endl;
		std::cout << temp->X_min << ' ' << temp->X_max << std::endl;
		std::cout << temp->Y_min << ' ' << temp->Y_max << std::endl;
		std::cout << temp->Z_min << ' ' << temp->Z_max << std::endl;*/
		if (x >= temp->X_min && x < temp->X_mid) {
			if (y >= temp->Y_min && y < temp->Y_mid) {
				if (z >= temp->Z_min && z < temp->Z_mid) {
					if (temp->dlb == NULL)
						temp->dlb = new Octree_Node((temp->depth) + 1, temp, 'F', temp->X_min, temp->X_mid, temp->Y_min, temp->Y_mid, temp->Z_min, temp->Z_mid);
					temp = temp->dlb;
				}
				else if (z >= temp->Z_mid && z <= temp->Z_max) {
					if (temp->ulb == NULL)
						temp->ulb = new Octree_Node((temp->depth) + 1, temp, 'B', temp->X_min, temp->X_mid, temp->Y_min, temp->Y_mid, temp->Z_mid, temp->Z_max);
					temp = temp->ulb;
				}
			}
			else if (y >= temp->Y_mid && y <= temp->Y_max) {
				if (z >= temp->Z_min && z < temp->Z_mid) {
					if (temp->dlf == NULL)
						temp->dlf = new Octree_Node((temp->depth) + 1, temp, 'E', temp->X_min, temp->X_mid, temp->Y_mid, temp->Y_max, temp->Z_min, temp->Z_mid);
					temp = temp->dlf;
				}
				else if (z >= temp->Z_mid && z <= temp->Z_max) {
					if (temp->ulf == NULL)
						temp->ulf = new Octree_Node((temp->depth) + 1, temp, 'A', temp->X_min, temp->X_mid, temp->Y_mid, temp->Y_max, temp->Z_mid, temp->Z_max);
					temp = temp->ulf;
				}
			}
		}
		else if (x >= temp->X_mid && x <= temp->X_max) {
			if (y >= temp->Y_min && y < temp->Y_mid) {
				if (z >= temp->Z_min && z < temp->Z_mid) {
					if (temp->drb == NULL)
						temp->drb = new Octree_Node((temp->depth) + 1, temp, 'H', temp->X_mid, temp->X_max, temp->Y_min, temp->Y_mid, temp->Z_min, temp->Z_mid);
					temp = temp->drb;
				}
				else if (z >= temp->Z_mid && z <= temp->Z_max) {
					if (temp->urb == NULL)
						temp->urb = new Octree_Node((temp->depth) + 1, temp, 'D', temp->X_mid, temp->X_max, temp->Y_min, temp->Y_mid, temp->Z_mid, temp->Z_max);
					temp = temp->urb;
				}
			}
			else if (y >= temp->Y_mid && y <= temp->Y_max) {
				if (z >= temp->Z_min && z < temp->Z_mid) {
					if (temp->drf == NULL)
						temp->drf = new Octree_Node((temp->depth) + 1, temp, 'G', temp->X_mid, temp->X_max, temp->Y_mid, temp->Y_max, temp->Z_min, temp->Z_mid);
					temp = temp->drf;
				}
				else if (z >= temp->Z_mid && z <= temp->Z_max) {
					if (temp->urf == NULL)
						temp->urf = new Octree_Node((temp->depth) + 1, temp, 'C', temp->X_mid, temp->X_max, temp->Y_mid, temp->Y_max, temp->Z_mid, temp->Z_max);
					temp = temp->urf;
				}
			}
		}
	}
	(temp->data).append(index);
	//temp->Print_Path();
}


// 这个index是particle的index，而非模型的index
void  Octree_Scene::Act_With_Neighbour(int index) {
	/*if ()
	return;*/

	Save_min_index = -1;
	Save_min_dis = 666;
	double a[6] = { particle[index].position.x - custom_H,
					particle[index].position.x + custom_H,
					particle[index].position.y - custom_H,
					particle[index].position.y + custom_H,
					particle[index].position.z - custom_H,
					particle[index].position.z + custom_H };

	Root->Find_Octree_Leaf(index, a[0], a[1], a[2], a[3], a[4], a[5], Octree_Find_Min);

	if (Save_min_dis < 1 && Save_min_index != -1) {
		if (debug_cnt == 2)
			std::cout << "233333333\n";

		double C = -10000.0;		// 线性系数
		double x_projection = vec3_dot(model[Save_min_index].normal_vec, particle[index].position - model[Save_min_index].position);
		double difference = x_projection - custom_D / 2;
		double v_projection = vec3_dot(model[Save_min_index].normal_vec, particle[index].v);
		if (difference < 0) {
			double F = C * difference;
			//particle[index].a.change(model[Save_min_index].normal_vec * F);
			particle[index].v.change(model[Save_min_index].normal_vec * 1.75 * abs(v_projection));
		}
	}
}


#if MODEL_VERSION == 1

#define gltest_box_size 5

void Bounds_Checking() { // 为真则表示超出边界或者近乎静止，已初始化
	for (int i = 0; i < MAX_particle; ++i) {
		if (particle[i].position.x > gltest_box_size) {
			particle[i].position.x = gltest_box_size;
			particle[i].v.x = -ABS(particle[i].v.x) * 0.9;
		}
		else if (particle[i].position.x < -gltest_box_size) {
			particle[i].position.x = -gltest_box_size;
			particle[i].v.x = ABS(particle[i].v.x) * 0.9;
		}
		if (particle[i].position.y > gltest_box_size) {
			particle[i].position.y = gltest_box_size;
			particle[i].v.y = -ABS(particle[i].v.y) * 0.9;
		}
		else if (particle[i].position.y < -gltest_box_size) {
			particle[i].position.y = -gltest_box_size;
			particle[i].v.y = ABS(particle[i].v.y) * 0.9;
		}
		if (particle[i].position.z < 0) {
			particle[i].position.z = 0;
			particle[i].v.z = ABS(particle[i].v.z) * 0.7;
		}
		/*if (particle[index].position.z > 10) {
		particle[index].position.z = 10;
		particle[index].v.z = ABS(particle[index].v.z) * -0.9;
		flag = true;
		}*/
	}
}
#elif MODEL_VERSION == 2
void Bounds_Checking() {
	for (int i = 0; i < MAX_particle; ++i) {
		if (particle[i].a.z > 8) {
			particle[i].v.z = -ABS(particle[i].v.z);
		}
		Scene.Act_With_Neighbour(i);
	}
}

#endif