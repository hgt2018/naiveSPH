#pragma once

#include <cstdlib>
#include <ctime>
#include "glut.h"
#pragma comment(lib, "glut32.lib")

#include "particle.h"
#include "Octree_Fluid.h"
#include "Octree_Scene.h"

#define window_width 700
#define window_height 700
const double window_ratio = (double)window_width / (double)window_height;

float scale = 1.0;

float gl_light_position[] = { 10, 10, 10, 1 };
float gl_light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
float gl_light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float gl_light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
float gl_color_specular[] = { 0, 0, 0, 1 };
float gl_color_shininess[] = { 30 };
float gl_color_emission[] = { 0, 0, 0, 1 };
float gl_color_particle_ambient_diffuse[] = { 0, 0, 0, 1 };
float gl_color_wall_ambient_diffuse[] = { 0.5, 0.8, 0.5, 0.3 };

// glut 键鼠控制
class Key_Mouse_Control {
public:
	int Particle_Move_Start;
	int Particle_Init_Complete;

	Key_Mouse_Control():Particle_Move_Start(0), Particle_Init_Complete(0) {}

	void Print() {
		std::cout << Particle_Move_Start << ' ' << Particle_Init_Complete << std::endl;
	}
} Glut_Control;


// 相机
#if MODEL_VERSION == 1
double gluLookAt_eye[3] = { 9, 11, 10 };
double gluLookAt_at[3] = { -1, -1, -1 };
double gluLookAt_up[3] = { -1, -1, 2 };

double Init_coordinate[3] = { -0.05, -0.05, 0.5 };

void Draw_Scene() {
	// 墙壁绘制
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gl_color_wall_ambient_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_color_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_color_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_color_emission);
	static int list = 0;
	if (list == 0) {
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		double h = 10.0;
		glBegin(GL_QUADS);
		glVertex3d(gltest_box_size, gltest_box_size, 0);
		glVertex3d(gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, h);
		glVertex3d(gltest_box_size, -gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, -gltest_box_size, 0);
		glVertex3d(-gltest_box_size, -gltest_box_size, h);
		glVertex3d(-gltest_box_size, gltest_box_size, h);
		glVertex3d(-gltest_box_size, gltest_box_size, 0);
		glEnd();
		glBegin(GL_QUADS);
		glVertex3d(-gltest_box_size, gltest_box_size, 0);
		glVertex3d(-gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, gltest_box_size, h);
		glVertex3d(gltest_box_size, gltest_box_size, 0);
		glEnd();/*
				glBegin(GL_QUADS);
				glVertex3d(-gltest_box_size, gltest_box_size, 0);
				glVertex3d(-gltest_box_size,-gltest_box_size, 0);
				glVertex3d(gltest_box_size, -gltest_box_size, 0);
				glVertex3d(gltest_box_size, gltest_box_size, 0);
				glEnd();*/

		glEndList();
	}
	glCallList(list);
}

#elif MODEL_VERSION == 2
double gluLookAt_eye[3] = { 20, 20, 20 };	//视点
double gluLookAt_at[3] = { -50, -1, -3 };		//观测点
double gluLookAt_up[3] = { -1, 0, 5 };		//向上向量

double Init_coordinate[3] = { -2, 20, 16 };

void Draw_Scene() {
	for (int i = 0; i < Model_Num; ++i) {
		glPointSize(0.5f);
		glBegin(GL_POINTS);
		glVertex3f(model[i].position.x, model[i].position.y, model[i].position.z);
		glEnd();
	}
}

#endif



void OpenGL_Test_Diaplay() {

	/*if (frame%100==0)
		particle_write();*/
	++frame;

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

	Glut_Control.Print();

	if (Glut_Control.Particle_Move_Start == 1) {
		if (Glut_Control.Particle_Init_Complete == 1) {
			// 建立八叉树
			// Octree.h内有Fluid八叉树和Scene八叉树两课树已经建好

			//std::cout << "frame:  " << cnt << std::endl;
			//++cnt;


			// SPH插值
			SPH_Method();

			//int aaa;
			//std::cin >> aaa;

			// 粒子迭代演化以及修正
			Bounds_Checking();
			for (int i = 0; i < MAX_particle; ++i) {
				particle[i].position.change((particle[i].v + particle[i].a * 0.5 * time_step) * time_step);
				particle[i].v.change(particle[i].a * time_step);
			}

		}
		else {
			Particle_Initialization(Init_coordinate);
			Normalize_Mass();
			Glut_Control.Particle_Init_Complete = 1;
		}
	}
	else {
		if (Glut_Control.Particle_Init_Complete == 0) {
			Particle_Initialization(Init_coordinate);
			Glut_Control.Particle_Init_Complete = 1;
		}
	}

	
	// 粒子绘制
	glScalef(scale, scale, scale);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gl_color_particle_ambient_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gl_color_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gl_color_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gl_color_emission);
	for (int i = 0; i < MAX_particle; ++i) {
		glPointSize(6.0f);
		glBegin(GL_POINTS);
		glVertex3f(particle[i].position.x, particle[i].position.y, particle[i].position.z);
		glEnd();
	}

	/*particle[1].Print();
	std::cout << debug_cnt << std::endl;
	int ccc;
	std::cin >> ccc;*/


	// 场景
	Draw_Scene();

	glutSwapBuffers();
}


void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunction, 1);
}


void Recall_Keyboard(unsigned char key, int x, int y)
{
	/*if (debug_cnt == 10)
		for (int i = 0; i < 3; ++i)
			printf("%f   %f    %f \n\n\n", gluLookAt_eye[i], gluLookAt_at[i], gluLookAt_up[i]);*/
	switch (key) {
	case 'n':
	case 'N':
		if (MODEL_VERSION == 2) {
			gluLookAt_eye[0] = -1.5, gluLookAt_eye[1] = 24, gluLookAt_eye[2] = 30;
			gluLookAt_at[0] = -3.5, gluLookAt_at[1] = 0, gluLookAt_at[2] = -17.5;
			gluLookAt_up[0] = -1, gluLookAt_up[1] = -1, gluLookAt_up[2] = 1;
		}
		break;
	case 'm':
	case 'M':
		if (MODEL_VERSION == 2) {
			gluLookAt_eye[0] = gluLookAt_eye[1] = gluLookAt_eye[2] = 20;
			gluLookAt_at[0] = -50, gluLookAt_at[1] = -1, gluLookAt_at[2] = -3;
			gluLookAt_up[0] = -1, gluLookAt_up[1] = 0, gluLookAt_up[2] = 5;
		}
		break;
	case 'w':
	case 'W':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[0] -= 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
		break;
	case 's':
	case 'S':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[0] += 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
	break;
	case 'a':
	case 'A':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[1] -= 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
		break;
	case 'd':
	case 'D':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[1] += 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
		break;
	case 'e':
	case 'E':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[2] += 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
		break;
	case 'q':
	case 'Q':
		if (Glut_Control.Particle_Move_Start == 0) {
			Init_coordinate[2] -= 0.1;
			Glut_Control.Particle_Init_Complete = 0;
		}
		break;

	case ' ':
		Glut_Control.Particle_Move_Start = 1 - Glut_Control.Particle_Move_Start;
		Glut_Control.Particle_Init_Complete = 0;
		break;

	default:
		break;
	}
}


void Recall_Mouse_Click(int button, int state, int x, int y) {
	if (button == GLUT_WHEEL_UP) {
		scale += 0.05;
	}
	else if (button == GLUT_WHEEL_DOWN) {
		scale -= 0.05;
	}
}


void OpenGL_Test(int argc, char **argv) {
	srand(time(0));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("GL_test");

	glClearColor(1, 1, 1, 1);				//背景颜色
	glEnable(GL_DEPTH_TEST);				//启动深度测试

	glutKeyboardFunc(Recall_Keyboard);
	glutMouseFunc(Recall_Mouse_Click);

	glutDisplayFunc(OpenGL_Test_Diaplay);

	glutTimerFunc(20, TimerFunction, 1);

	glutMainLoop();
}