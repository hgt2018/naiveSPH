#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstring>

#include "custom_vec3.h"


inline double ABS(double x) {
	return (x > 0 ? x : -x);
}

inline double Rand(double left, double right) { // left 到 right 的随机数
	return (left + (right - left) * (((double)rand()) / RAND_MAX));
}

class Octree_Node;

class Particle {
public:
	custom_vec3 position;		//Position
	custom_vec3 v;				//Velocity
	custom_vec3 a;				//Acceleration
	double rho;			//Density
	double p;			//Pressure

	Octree_Node* location;

	Particle() :rho(0), p(0), location(NULL) {}
	Particle(custom_vec3 vp, custom_vec3 vv, custom_vec3 va, double density = 0, double P = 0) :
		position(vp), v(vv), a(va), rho(density), p(P), location(NULL) {}

	void Print() {
		std::cout << "position: <x,y,z> is <" << position.x << ", " << position.y << ", " << position.z << ">\n";
		std::cout << "vel: <x,y,z> is <" << v.x << ", " << v.y << ", " << v.z << ">\n";
		std::cout << "acc: <x,y,z> is <" << a.x << ", " << a.y << ", " << a.z << ">\n";
		std::cout << "rho: " << rho << "    p:   " << p << std::endl;
	}

};

#if (PROGRAM_TYPE == OpenGL_Test_Octree)

#include "constant.h"

Particle particle[MAX_particle];
Particle Fake_one;


double Particle_Distance_sqrt(int i, int j) {
	double x = particle[i].position.x - particle[j].position.x;
	double y = particle[i].position.y - particle[j].position.y;
	double z = particle[i].position.z - particle[j].position.z;
	return sqrt(x * x + y * y + z * z);
}


double Particle_Distance_square(int i, int j) {
	double x = particle[i].position.x - particle[j].position.x;
	double y = particle[i].position.y - particle[j].position.y;
	double z = particle[i].position.z - particle[j].position.z;
	return (x * x + y * y + z * z);
}


custom_vec3 GradientSpiky(custom_vec3 & v, double d) {
	double P = (45 / (custom_PI * H6 * d)) * (custom_H - d) * (custom_H - d);
	return (v * -P);
}


double Ploy6(double r2) {
	double k = 315 / (64 * custom_PI * H9);
	return (k * pow((H2 - r2), 3));
}


double LaplaceVis(double d) {
	return (45 * (custom_H - d)) / (custom_PI * H6);
}


double L2(Particle & A, Particle & B) {
	double dx = A.position.x - B.position.x;
	double dy = A.position.y - B.position.y;
	double dz = A.position.z - B.position.z;
	return dx * dx + dy * dy + dz * dz;
}


#if MODEL_VERSION == 1
void Particle_Initialization(double *ptr) {
	//double xx = -0.5, yy = -0.5, zz = 8;
	int ccc = 0;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			for (int k = 0; k < 10; ++k) {
				particle[ccc].position.set(ptr[0] + i * custom_D, ptr[1] + j * custom_D, ptr[2] + k * custom_D);
				particle[ccc].v.set(0, 0, 0);
				++ccc;
			}
	/*
	for (int i = 0; i < MAX_particle; ++i) {
		particle[i].position.set(Rand(-1, 1), Rand(-1, 1), Rand(2, 3));
		particle[i].v.set(Rand(-5, 5), Rand(-5, 5), 0);
		//particle[i].Print();
	}
	/**/
	//std::cout << "************************\n";
	
}

#elif MODEL_VERSION == 2
void Particle_Initialization(double *ptr) {
	//double xx = -0.5, yy = -0.5, zz = 8;
	int ccc = 0;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			for (int k = 0; k < 10; ++k) {
				particle[ccc].position.set(ptr[0] + i * custom_D, ptr[1] + j * custom_D, ptr[2] + k * custom_D);
				particle[ccc].v.set(0, -5, -5);
				++ccc;
			}
	/**/
	//std::cout << "************************\n";

}

#elif MODEL_VERSION == 3
void Particle_Initialization(double *ptr) {
	//double xx = -0.5, yy = -0.5, zz = 8;
	int ccc = 0;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			for (int k = 0; k < 10; ++k) {
				particle[ccc].position.set(ptr[0] + i * custom_D, ptr[1] + j * custom_D, ptr[2] + k * custom_D);
				particle[ccc].v.set(0, -5, -5);
				++ccc;
			}
	/**/
	//std::cout << "************************\n";

}

#endif

#endif


void particle_write() {
	std::ofstream out;
	char z[100]={0};
	_itoa(frame,z,10);
	std::string filenameHead="particle_";
	std::string filenameTail=".txt";
	std::string fileNum=z;
	std::string filename=filenameHead+fileNum+filenameTail;
	out.open(filename, std::ios::out);
	out << "o model\n";
	for (int index=0; index<MAX_particle; ++index){
		out << "v ";
		out << particle[index].position.x;
		out << " ";
		out << particle[index].position.y;
		out << ' ';
		out << particle[index].position.z;
		out << "\n";
	}
	out.close();
}
