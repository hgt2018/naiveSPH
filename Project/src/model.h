#pragma once



#include <iostream>
#include <fstream>

#include "custom_vec3.h"

class Model {
public:
	custom_vec3 position;
	custom_vec3 normal_vec;

	Model() {}

	inline void set(double x1, double x2, double x3, double x4, double x5, double x6) {
		position.set(x1, x2, x3);
		normal_vec.set(x4, x5, x6);
	}

	inline void set(double *ptr) {
		position.set(ptr[0], ptr[1], ptr[2]);
		normal_vec.set(ptr[3], ptr[4], ptr[5]);
	}

	inline void set(custom_vec3 & v1, custom_vec3 & v2) {
		position.set(v1);
		normal_vec.set(v2);
	}
};

#if (PROGRAM_TYPE == OpenGL_Test_Octree)

#include "constant.h"

#if MODEL_VERSION == 1

#define Model_Num 2560
Model model[Model_Num];

void Model_Import() {
	std::ifstream inFile;
	inFile.open("model_data\\cube.txt", std::ios::in);
	if (!inFile) {
		std::cout << "Error in opening model.txt!!\n";
		return;
	}

	int line = 0;
	double a[6];
	/*double b[6] = { 666,-666, 666,-666, 666,-666};
	int c[6] = {-1,-1,-1,-1,-1,-1};*/
	while (inFile >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]) {
		//++line;
		//std::cout << a[0] << ' ' << a[3] << std::endl;
		model[line].set(a[0], a[1], (a[2] + 2.40), -a[3], -a[4], -a[5]);
		/*if (a[0] < b[0]){
			c[0]=line;
			b[0] = a[0];}
		else if (a[0] > b[1]){
			c[1] = line;
			b[1] = a[0];}
		if (a[1] < b[2]){
			c[2] = line;
			b[2] = a[1];}
		else if (a[1] > b[3]){
			c[3] = line;
			b[3] = a[1];}
		if (a[2] < b[4]){
			c[4] = line;
			b[4] = a[2];}
		else if (a[2] > b[4]){
			c[5] = line;
			b[5] = a[2];}*/
		++line;
	}
	/*std::cout << line << std::endl;
	for (int i = 0; i < 6; ++i)
		std::cout << b[i] << ' ' << c[i] << std::endl;*/
	inFile.close();
}

#elif MODEL_VERSION == 2

#define Model_Num 12032
Model model[Model_Num];

/*
x:-1 ~ 1
y:-1 ~ 7
z:1 ~ 5
*/

void Model_Import() {
	std::ifstream inFile;
	inFile.open("model_data\\stairs.txt", std::ios::in);
	if (!inFile) {
		std::cout << "Error in opening model.txt!!\n";
		return;
	}

	int line = 0;
	double a[6];
	/*double b[6] = { 666,-666, 666,-666, 666,-666};
	int c[6] = {-1,-1,-1,-1,-1,-1};*/
	while (inFile >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]) {
		//++line;
		//std::cout << a[0] << ' ' << a[3] << std::endl;
		model[line].set(3.6*a[0],3.6*a[1],3.6*a[2],-a[3],-a[4],-a[5]);
		/*if (a[0] < b[0]){
		c[0]=line;
		b[0] = a[0];}
		else if (a[0] > b[1]){
		c[1] = line;
		b[1] = a[0];}
		if (a[1] < b[2]){
		c[2] = line;
		b[2] = a[1];}
		else if (a[1] > b[3]){
		c[3] = line;
		b[3] = a[1];}
		if (a[2] < b[4]){
		c[4] = line;
		b[4] = a[2];}
		else if (a[2] > b[4]){
		c[5] = line;
		b[5] = a[2];}*/
		++line;
	}
	/*std::cout << line << std::endl;
	for (int i = 0; i < 6; ++i)
	std::cout << b[i] << ' ' << c[i] << std::endl;*/
	inFile.close();
}

#elif MODEL_VERSION == 3

#define Model_Num 26624
Model model[Model_Num];

/*
x:-1 ~ 1
y:-1 ~ 7
z:1 ~ 5
*/

void Model_Import() {
	std::ifstream inFile;
	inFile.open("model_data\\octcup4.txt", std::ios::in);
	if (!inFile) {
		std::cout << "Error in opening model.txt!!\n";
		return;
	}

	int line = 0;
	double a[6];
	/*double b[6] = { 666,-666, 666,-666, 666,-666};
	int c[6] = {-1,-1,-1,-1,-1,-1};*/
	while (inFile >> a[0] >> a[1] >> a[2] >> a[3] >> a[4] >> a[5]) {
		++line;
		//std::cout << a[0] << ' ' << a[3] << std::endl;
		model[line].set(3.6*a[0], 3.6*a[1], 3.6*a[2], a[3], a[4], a[5]);
		/*if (a[0] < b[0]){
		c[0]=line;
		b[0] = a[0];}
		else if (a[0] > b[1]){
		c[1] = line;
		b[1] = a[0];}
		if (a[1] < b[2]){
		c[2] = line;
		b[2] = a[1];}
		else if (a[1] > b[3]){
		c[3] = line;
		b[3] = a[1];}
		if (a[2] < b[4]){
		c[4] = line;
		b[4] = a[2];}
		else if (a[2] > b[4]){
		c[5] = line;
		b[5] = a[2];}
		*/
		++line;
	}
	/*std::cout << line << std::endl;
	for (int i = 0; i < 6; ++i)
	std::cout << b[i] << ' ' << c[i] << std::endl;*/
	inFile.close();
}

#endif 
#endif