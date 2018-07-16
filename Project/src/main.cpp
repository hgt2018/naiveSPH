#define _CRT_SECURE_NO_WARNINGS
// 程序运行版本
#define PROGRAM_TYPE 3

#define OpenGL_Test_Oblique_Movement 1
#define POVRay_OUTPUT_Grid 2
#define OpenGL_Test_Octree 3

#if (PROGRAM_TYPE == OpenGL_Test_Oblique_Movement)

#include "gltest_oblique.h"

void main(int argc, char **argv) {
	OpenGL_Test(argc, argv);
}


#elif (PROGRAM_TYPE == POVRay_OUTPUT_Grid)

#include "pov_out.h"
#include "Tag.h"

int main()  {
	test();
	return 0;
}

#elif (PROGRAM_TYPE == OpenGL_Test_Octree)

#include "gltest_octree.h"
//#include "model.h"

int main(int argc, char **argv) {
	/*Model_Import();
	system("pause");*/
	OpenGL_Test(argc, argv);
	return 0;
}

#endif // PROGRAM_TYPE