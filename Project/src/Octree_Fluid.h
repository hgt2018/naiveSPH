#pragma once
#include "Octree.h"

class Octree_Fluid {
public:
	Octree_Node* Root;

	int SPH_STEP;

	double Octree_Threshold;

	void Insert(int);
	void Act_With_Neighbour(int);

	Octree_Fluid() {
		Root = new Octree_Node(0, NULL, 'O', Scene_Xmin, Scene_Xmax, Scene_Ymin, Scene_Ymax, Scene_Zmin, Scene_Zmax);
		Octree_Threshold = OCTREE_FLUID_THRESHOLD;
		for (int i = 0; i < MAX_particle; ++i) {
			Insert(i);
		}
		//std::cout << "fluid complete\n";
	}

	~Octree_Fluid() { delete Root; }
	void clear() { Root->clear(); }
} Fluid;


// 给定序号为index的粒子，如果temp指向的流体结点内的粒子与之距离小于distance
// 则作SPH插值运算
void Octree_Calculate_SPH(int index, Octree_Node* ptr) {
	Node_Data* temp = &(ptr->data);
	if (temp->index != -1) {
		do {
			double d = Particle_Distance_sqrt(index, temp->index);
			if (d <= custom_H) {
				// Do something
				if (Fluid.SPH_STEP == 1) {
					//++debug_cnt;
					//if (index != temp->index)
					//std::cout << "distance: " << d << "\n";
					particle[index].rho += Ploy6(d * d) * Mass;
					//if (index == 1)
					//Neighbour_Temp_Save.push_back(temp->index);
				}
				else if (Fluid.SPH_STEP == 2 && d > 0) {
					custom_vec3 dr = particle[index].position - particle[temp->index].position;
					//dr.Print();
					custom_vec3 temp_v = GradientSpiky(dr, d);
					double x1 = (-1 * Mass * (particle[index].p + particle[temp->index].p)) / ((2 * particle[temp->index].rho * particle[index].rho));
					particle[index].a.change(temp_v * x1);

					/*if (index == 1) {
					std::cout << "11....p[1]::     ";
					particle[index].a.Print();
					}*/

					x1 = (custom_MU * Mass * LaplaceVis(d)) / ((particle[temp->index].rho * particle[index].rho));
					particle[index].a.change((particle[temp->index].v - particle[index].v) * x1);

					/*if (index == 1) {
					std::cout << "22....p[1]::     ";
					particle[index].a.Print();
					}*/
				}
			}
			temp = temp->next;
		} while (temp);
	}
}


void Octree_Fluid::Insert(int index) {
	Octree_Node *temp = Root;
	while (temp->diameter > Octree_Threshold) {
		double x = particle[index].position.x, y = particle[index].position.y, z = particle[index].position.z;
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
				else {
					particle[index].location = NULL;
					return;
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
				else {
					particle[index].location = NULL;
					return;
				}
			}
			else {
				particle[index].location = NULL;
				return;
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
				else {
					particle[index].location = NULL;
					return;
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
				else {
					particle[index].location = NULL;
					return;
				}
			}
			else {
				particle[index].location = NULL;
				return;
			}
		}
		else {
			particle[index].location = NULL;
			return;
		}
	}
	(temp->data).append(index);
	particle[index].location = temp;
}


void Octree_Fluid::Act_With_Neighbour(int index) {
	if (particle[index].location == NULL)
		return;

	if (debug_cnt == 4) {
		if (particle[index].position.x < Scene_Xmin) {
			std::cout << "particle " << index << " is out of scene!!!  x1\n";
			particle[index].Print();
			system("pause");
			exit(1);
		}
		if (particle[index].position.x > Scene_Xmax) {
			std::cout << "particle " << index << " is out of scene!!!  x2\n";
			particle[index].Print();
			system("pause");
			exit(1);
		}
		if (particle[index].position.y < Scene_Ymin) {
			std::cout << "particle " << index << " is out of scene!!!  y1\n";
			particle[index].Print();
			system("pause");
			exit(1);
		}
		if (particle[index].position.y > Scene_Ymax) {
			std::cout << "particle " << index << " is out of scene!!!  y2\n";
			particle[index].Print();
			system("pause");
			exit(1);
		}
		if (particle[index].position.z < Scene_Zmin) {
			std::cout << "particle " << index << " is out of scene!!!  z1\n";
			particle[index].Print();
			system("pause");
			exit(1);
		}
		if (particle[index].position.z > Scene_Zmax) {
			std::cout << "particle " << index << " is out of scene!!!  z2\n";
			particle[index].Print();
			exit(1);
		}
	}

	double a[6] = { particle[index].position.x - custom_H,
		particle[index].position.x + custom_H,
		particle[index].position.y - custom_H,
		particle[index].position.y + custom_H,
		particle[index].position.z - custom_H,
		particle[index].position.z + custom_H };

	//debug_cnt = 0;
	Root->Find_Octree_Leaf(index, a[0], a[1], a[2], a[3], a[4], a[5], Octree_Calculate_SPH);
	//std::cout << debug_cnt << std::endl;
	/*if (index == 1)
	std::cout << "rho: " << particle[index].rho << std::endl;*/
	if (SPH_STEP == 1)
		particle[index].p = custom_K3 * (particle[index].rho - custom_RHO);
		//particle[index].p = custom_B * (pow(particle[index].rho / custom_RHO, Gamma) - 1.0);

	/*if (index % 100 == 0) {
	std::cout << "************\n";
	particle[index].Print();
	}*/
}


void Normalize_Mass() {
	Mass = 1.0;
	// 构建八叉树并且计算密度
	Fluid.clear();
	for (int i = 0; i < MAX_particle; i++) {
		Fluid.Insert(i);
	}
	Fluid.SPH_STEP = 1;
	for (int i = 0; i < MAX_particle; ++i)
		Fluid.Act_With_Neighbour(i);
	// 正则化质量
	double rho2s = 0.0;
	double rhos = 0.0;
	for (int i = 0; i<MAX_particle; ++i) {
		rho2s += particle[i].rho * particle[i].rho;
		rhos += particle[i].rho;
	}
	Mass = custom_RHO * rhos / rho2s;
	//printf("%f,%f,%f", Mass, rho2s,rhos);
}


void SPH_Method() {
	Fluid.clear();

	for (int i = 0; i < MAX_particle; i++) {
		//std::cout << i << std::endl;
		particle[i].rho = 0;
		particle[i].a.x = 0;
		particle[i].a.y = 0;
		particle[i].a.z = custom_G;
		particle[i].p = 0;
		Fluid.Insert(i);
	}

	/*if (debug_cnt == 1)
	particle[1].Print();*/
	Fluid.SPH_STEP = 1;
	for (int i = 0; i < MAX_particle; ++i)
		Fluid.Act_With_Neighbour(i);
	//std::cout << "2333333333333333333333\n";
	/*if (debug_cnt == 1)
	particle[1].Print();*/


	Fluid.SPH_STEP = 2;
	for (int i = 0; i < MAX_particle; ++i)
		Fluid.Act_With_Neighbour(i);
	if (debug_cnt == 1) {
		particle[1].Print();
		std::cout << "\n\n\n\n" << std::endl;
	}
}