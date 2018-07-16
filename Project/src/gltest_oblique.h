#pragma once

#define Need_Preheat

#include <cstdlib>
#include <ctime>
#include "glut.h"
#include "particle.h"
#pragma comment(lib, "glut32.lib")

#define window_width 700
#define window_height 700
const double window_ratio = (double)window_width / (double)window_height;

float scale = 1.0;
int gltest_N_particle = 0;

#ifdef Need_Preheat
#define gltest_preheat true
#else
#define gltest_preheat false
#endif // Need_Preheat

int preheat_times = 100;

float gl_light_position[] = { 10, 10, 10, 1 };
float gl_light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
float gl_light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float gl_light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
float gl_color_specular[] = { 0, 0, 0, 1 };
float gl_color_shininess[] = { 30 };
float gl_color_emission[] = { 0, 0, 0, 1 };
float gl_color_particle_ambient_diffuse[] = { 0, 0, 0, 1 };
float gl_color_wall_ambient_diffuse[] = { 0.5, 0.8, 0.5, 0.3 };

// Type 1: 斜抛运动
// Type 2: 池子模拟（仍是斜抛）
#define Type 2
#if Type == 1

// 这个用来检查一下表现效果和preheat有效性，可忽略
#define Continuous_particle_flow
#ifdef Continuous_particle_flow
#define gltest_MAX_particle 10000
#define gltest_particle_per_t 100
#else
#define gltest_MAX_particle 10000
#define gltest_particle_per_t 2000
#endif // Continuous_particle_flow

// 初始化粒子
#define init_pos custom_vec3(0, 0, 20)
#define gltest_p_init_horizontal_v 4
#define gltest_p_init_vertical_v 11
#define rand_v1 Rand(-gltest_p_init_horizontal_v, gltest_p_init_horizontal_v)
#define rand_v2 Rand(gltest_p_init_vertical_v-2,gltest_p_init_vertical_v+2)
#define init_v custom_vec3(rand_v1,rand_v1,rand_v2)
#define init_a custom_vec3(0, 0, -9.8)

#define gltest_time_step 0.05
#define gltest_box_size 25

// 相机
double gluLookAt_eye[3] = { 30, 30, 30 };	//视点
double gluLookAt_at[3] = { -1, -1, 2 };		//观测点
double gluLookAt_up[3] = { -1, -1, 1 };		//向上向量

void Draw_Other_Things() {
	// 墙壁绘制
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gl_color_wall_ambient_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_color_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_color_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_color_emission);
	static int list = 0;
	if (list == 0) {
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		double h = 6.0;
		glBegin(GL_QUADS);
		glVertex3d(gltest_box_size, gltest_box_size, -h);
		glVertex3d(gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, -h);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, -h);
		glVertex3d(-gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, -h);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, -h);
		glVertex3d(-gltest_box_size, -gltest_box_size, h);
		glVertex3d(-gltest_box_size, gltest_box_size, h);
		glVertex3d(-gltest_box_size, gltest_box_size, -h);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, gltest_box_size, -h);
		glVertex3d(-gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, gltest_box_size, -h);
		glEnd();

		glEndList();
	}
	glCallList(list);
}


#elif Type == 2

#define gltest_MAX_particle 10000
#define gltest_particle_per_t 200

// 初始化粒子
#define init_pos custom_vec3(0, 0, 5)
#define gltest_p_init_horizontal_v 2.5
#define gltest_p_init_vertical_v 8
#define rand_v1 Rand(-gltest_p_init_horizontal_v, gltest_p_init_horizontal_v)
#define rand_v2 Rand(gltest_p_init_vertical_v-1,gltest_p_init_vertical_v+1)
#define init_v custom_vec3(rand_v1,rand_v1,rand_v2)
#define init_a custom_vec3(0, 0, -9.8)

#define gltest_time_step 0.01
#define gltest_box_size 9
#define gltest_box_height 5

// 相机
double gluLookAt_eye[3] = { 0, 15, 15 };	//视点
double gluLookAt_at[3] = { 0, 0, 2 };		//观测点
double gluLookAt_up[3] = { 0, -1, 1 };		//向上向量

void Draw_Other_Things() {
	// 墙壁绘制
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gl_color_wall_ambient_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_color_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_color_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_color_emission);
	static int list = 0;
	if (list == 0) {
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		glBegin(GL_QUADS);
		glVertex3d(gltest_box_size, gltest_box_size, 0);
		glVertex3d(gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(gltest_box_size, gltest_box_size, 0);
		glVertex3d(gltest_box_size, gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, -gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, -gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, -gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, -gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, -gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, -gltest_box_size, gltest_box_height);
		glVertex3d(-gltest_box_size, gltest_box_size, gltest_box_height);
		glVertex3d(-gltest_box_size, gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, gltest_box_size, 0);
		glVertex3d(-gltest_box_size, gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, gltest_box_size, gltest_box_height);
		glVertex3d(gltest_box_size, gltest_box_size, 0);
		glEnd();

		glEndList();
	}
	glCallList(list);
}

#endif

Particle particle[gltest_MAX_particle];

#if Type == 1
// 完成边界检查等，超出边界的粒子还需要修改
bool Check_Particle(int index) { // 为真则表示超出边界，已初始化
	if (particle[index].position.z <= -25) {
		particle[index].position.set(init_pos);
		particle[index].v.set(init_v);
		particle[index].a.set(init_a);
		return true;
	}
	return false;
}
#elif Type == 2

// 完成边界检查等，超出边界的粒子还需要修改
bool Check_Particle(int index) { // 为真则表示超出边界或者近乎静止，已初始化
	if (particle[index].v.norm2() <= 2.0) {
		particle[index].position.set(init_pos);
		particle[index].v.set(init_v);
		particle[index].a.set(init_a);
		return true;
	}
	bool flag = false;
	if (ABS(particle[index].position.x) > gltest_box_size) {
		particle[index].position.x = particle[index].position.x > 0 ? gltest_box_size : -gltest_box_size;
		particle[index].v.x *= -0.7;
		flag = true;
	}
	if (ABS(particle[index].position.y) > gltest_box_size) {
		particle[index].position.y = particle[index].position.y > 0 ? gltest_box_size : -gltest_box_size;
		particle[index].v.y *= -0.7;
		flag = true;
	}
	if (particle[index].position.z < 0) {
		particle[index].position.z = 0;
		particle[index].v.z *= -0.7;
		flag = true;
	}
	return flag;
}

#endif


void OpenGL_Test_Particle_Init() {
	gltest_N_particle = 0;
	if (!gltest_preheat) {
		for (int i = 0; i < gltest_particle_per_t; ++i) {
			int temp = gltest_N_particle + i;
			particle[temp].position.set(init_pos);
			particle[temp].v.set(init_v);
			particle[temp].a.set(init_a);
		}
		gltest_N_particle = gltest_particle_per_t;
	}
	else {
		for (int z = 0; z < preheat_times; ++z) { // 预先迭代（一百次），除去不稳定的前期
			// 粒子迭代
			for (int i = 0; i < gltest_N_particle; ++i) {
				particle[i].position.change((particle[i].v + particle[i].a * 0.5 * gltest_time_step) * gltest_time_step);
				particle[i].v.change(particle[i].a * gltest_time_step);
			}

			// 前期  生成新的粒子
			if (gltest_N_particle < gltest_MAX_particle) {
				for (int i = 0; i < gltest_particle_per_t; ++i) {
					int temp = gltest_N_particle + i;
					particle[temp].position.set(init_pos);
					particle[temp].v.set(init_v);
					particle[temp].a.set(init_a);
				}
				gltest_N_particle += gltest_particle_per_t;
			}

			// 边界检查等
			for (int i = 0; i < gltest_N_particle; ++i)
				Check_Particle(i);
		}
	}
}


void OpenGL_Test_Diaplay() {
	// 环境初始化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(75.0, window_ratio, 0.1, 200.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	gluLookAt(gluLookAt_eye[0], gluLookAt_eye[1], gluLookAt_eye[2], gluLookAt_at[0], gluLookAt_at[1],
			  gluLookAt_at[2], gluLookAt_up[0], gluLookAt_up[1], gluLookAt_up[2]);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 光
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gl_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, gl_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, gl_light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, gl_light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// 粒子迭代
	for (int i = 0; i < gltest_N_particle; ++i) {
		particle[i].position.change((particle[i].v + particle[i].a * 0.5 * gltest_time_step) * gltest_time_step);
		particle[i].v.change(particle[i].a * gltest_time_step);
	}

	// 新粒子生成
	if (gltest_N_particle < gltest_MAX_particle) {
		for (int i = 0; i < gltest_particle_per_t; ++i) {
			int temp = gltest_N_particle + i;
			particle[temp].position.set(init_pos);
			particle[temp].v.set(init_v);
			particle[temp].a.set(init_a);
		}
		gltest_N_particle += gltest_particle_per_t;
	}

	// 粒子绘制
	glScalef(scale, scale, scale);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gl_color_particle_ambient_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_color_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_color_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_color_emission);

	for (int i = 0; i < gltest_N_particle; ++i) {
		if (Check_Particle(i))		// 边界检查等
			continue;

		glPointSize(2.0f);
		glBegin(GL_POINTS);
		glVertex3f(particle[i].position.x, particle[i].position.y, particle[i].position.z);
		glEnd();
	}
	
	Draw_Other_Things();

	glutSwapBuffers();

}


void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunction, 1);
}


void Recall_Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'w':
		case 'W':
			gluLookAt_eye[1] -= 0.5;
			break;
		case 's':
		case 'S':
			gluLookAt_eye[1] += 0.5;
			break;
		case 'a':
		case 'A':
			gluLookAt_eye[0] += 0.5;
			break;
		case 'd':
		case 'D':
			gluLookAt_eye[0] -= 0.5;
			break;
		case 'q':
		case 'Q':
			gluLookAt_eye[2] += 0.5;
			break;
		case 'e':
		case 'E':
			gluLookAt_eye[2] -= 0.5;
			break;
		case 'j':
		case 'J':
			gluLookAt_at[0] -= 0.5;
			break;
		case 'l':
		case 'L':
			gluLookAt_at[0] += 0.5;
			break;
		case 'k':
		case 'K':
			gluLookAt_at[2] += 0.5;
			break;
		case 'i':
		case 'I':
			gluLookAt_at[2] -= 0.5;
			break;
	
		case 'o':
		case 'O':
			scale += 0.05;
			break;
		case 'p':
		case 'P':
			scale -= 0.05;
			break;
		case ' ':
			gluLookAt_eye[0] = gluLookAt_eye[1] = gluLookAt_eye[2] = 30;
			gluLookAt_at[0] = gluLookAt_at[1] = gluLookAt_up[0] = gluLookAt_up [1] = -1;
			gluLookAt_at[2] = 2;
			gluLookAt_up[2] = 1;
			break;
	}
}


void OpenGL_Test(int argc, char **argv) {
	srand(time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("GL_test");

	glClearColor(1, 1, 1, 1);			//背景颜色
	glEnable(GL_DEPTH_TEST);				//启动深度测试

	OpenGL_Test_Particle_Init();

	glutDisplayFunc(OpenGL_Test_Diaplay);
	glutKeyboardFunc(Recall_Keyboard);

	glutTimerFunc(20, TimerFunction, 1);
	glutMainLoop();
}