#pragma once
#define MODEL_VERSION 2


// ************
// For Calculating

#define custom_PI 3.14159
//统一单位制，所有长度单位全部是m,质量单位是kg，时间单位是s,每个粒子假设半径是0.04cm

//#define custom_mass 0.001				// mass
double Mass=1.0;

int frame = 0;

#define custom_K3 1000.0					// 流体压强相关常数 //K3如何选取尚未可知！
#define custom_MU 0.1				        // 粘度系数

#define custom_RHO 1000.0				// Stable Density
#define custom_G -9.8					// 重力常量

#define Gamma 7
#define Cs 1450			// 水中音速 1450m/s
const double custom_B = custom_RHO*Cs*Cs/Gamma;


#if (PROGRAM_TYPE == POVRay_OUTPUT_Grid)

#define time_step 0.05		// 时间步长     ???? 待定不急调
#define MAX_particle 5000		// 最大粒子数目
int N_particle = 0;				// 当前粒子数目
#define particle_per_t 200		// 每一步时间新增的粒子
#define particle_size 0.2		// 粒子（cube）边长

								// For POV-RAY Drawing
								// 池子尺寸
#define box_length 18
#define box_width 18
#define box_height 4
#define half_box_size 9
#define draw_epsilon 0.003

								// 水体模拟用
#define Tag_height 8
#define element_size 0.2
#define e_1 5
const double offset = half_box_size - particle_size / 2 + element_size / 2;
const int Tag_Temp1 = (box_length * e_1);		// x方向ele数
const int Tag_Temp2 = (box_width * e_1);		// y方向ele数
const int Tag_Temp3 = (Tag_height * e_1);		// z方向ele数
const int Tag_Temp4 = Tag_Temp2 * Tag_Temp3;
const int N_element = Tag_Temp1 * Tag_Temp4;

#endif


#if (PROGRAM_TYPE == OpenGL_Test_Octree)

int debug_cnt = 666;

#if MODEL_VERSION == 1

#define time_step 0.001		// 时间步长
#define MAX_particle 1000	// 最大粒子数目

#define custom_D 0.36
#define custom_H 0.5					// 光滑核半径
const double H2 = custom_H * custom_H;
const double H3 = custom_H * H2;
const double H6 = H3 * H3;
const double H9 = H6 * H3;

#define Scene_Xmin -10
#define Scene_Xmax 10
#define Scene_Ymin -10
#define Scene_Ymax 10
#define Scene_Zmin -1
#define Scene_Zmax 30

#define OCTREE_FLUID_THRESHOLD 0.1
#define OCTREE_SCENE_THRESHOLD 0.1

#elif MODEL_VERSION == 2

#define time_step 0.001		// 时间步长
#define MAX_particle 1000	// 最大粒子数目

#define custom_D 0.36
#define custom_H 0.5				// 光滑核半径
const double H2 = custom_H * custom_H;
const double H3 = custom_H * H2;
const double H6 = H3 * H3;
const double H9 = H6 * H3;

#define Scene_Xmin -10
#define Scene_Xmax 10
#define Scene_Ymin -10
#define Scene_Ymax 30
#define Scene_Zmin -5
#define Scene_Zmax 50

#define OCTREE_FLUID_THRESHOLD 0.4
#define OCTREE_SCENE_THRESHOLD 0.4

#elif MODEL_VERSION == 3

#define time_step 0.001		// 时间步长
#define MAX_particle 1000	// 最大粒子数目

#define custom_D 0.36
#define custom_H 0.5				// 光滑核半径
const double H2 = custom_H * custom_H;
const double H3 = custom_H * H2;
const double H6 = H3 * H3;
const double H9 = H6 * H3;

#define Scene_Xmin -10
#define Scene_Xmax 10
#define Scene_Ymin -10
#define Scene_Ymax 30
#define Scene_Zmin -10
#define Scene_Zmax 20

#define OCTREE_FLUID_THRESHOLD 0.4
#define OCTREE_SCENE_THRESHOLD 0.4

#endif
#endif

/*
// ************
// For Searching
#define __H 0.5
const int Grid_Temp1 = (box_length / __H);				// x方向网格数
const int Grid_Temp2 = (box_height / __H);				// y方向网格数
const int Grid_Temp3 = (box_width / __H);				// z方向网格数
const int Grid_Temp4 = Grid_Temp2 * Grid_Temp3;
const int N_grid = Grid_Temp1 * Grid_Temp3;


int grid_offset[26][3] = { { -1, -1, -1}, {-1, -1, 0}, {-1, -1, 1},
{ -1, 0, -1}, {-1, 0, 0}, {-1, 0, 1},
{ -1, 1, -1}, {-1, 1, 0}, {-1, 1, 1},
{ 0, -1, -1 }, {0, -1, 0 }, {0, -1, 1 },
{ 0, 0, -1 }, {0, 0, 1 }, { 0, 1, -1 },
{ 0, 1, 0 }, { 0, 1, 1 }, { 1, -1, -1 },
{ 1, -1, 0 }, { 1, -1, 1 }, { 1, 0, -1 },
{ 1, 0, 0 }, { 1, 0, 1 }, { 1, 1, -1 },
{ 1, 1, 0 }, { 1, 1, 1 }
};/**/

