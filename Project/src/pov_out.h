
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "constant.h"
#include "particle.h"
#include "Tag.h"

#if (PROGRAM_TYPE == POVRay_OUTPUT_Grid)

Particle particle[MAX_particle];

std::string pov_save_path = "E:\\computer graphics work\\pov1\\";
char pov_filename[11] = "00000.pov";

#define environment_line 13
const char environment[][100] = {
	"#include \"colors.inc\" #include \"textures.inc\" #include \"finish.inc\"\n",
	"#include \"Stones.inc\" #include \"Metals.inc\"\n",
	"camera{ location  <-20, 0, 18> sky <0,0,1> look_at <-1, 0, 3> }\n",
	"plane{ <0, 0, 1>, 0 texture{ pigment{ checker color rgb <0.1, 0.1, 0> \n",
	"color rgb <0, 0.1, 0.1> }\n",
	"finish{ diffuse 1 ambient 1 phong 1 phong_size 100 reflection 0 } } }\n",
	"light_source{ <-7, 0, 15> color White }\n",
	"cone{ <0,0,0.4>, 0.7, <0,0,4>, 0.1 texture{ T_Stone15 } }\n",
	"merge { box{ <-9,-9,0>,<9,9,0.5> } box{ <-9.1,-9.1,0>,<9.1,-9,3> }\n",
	"box{ <9,-9.1,0>,<9,9.1,3> } box{ <-9.1,-9.1,0>,<-9,9.1,3> }\n",
	"box{ <-9.1,9,0>,<9.1,9.1,3> } texture{ T_Stone6 }}\n",
	"#declare Water = texture{ pigment{ color White filter 0.99 }\n",
	"finish{ ambient 0.1 diffuse 0.1 reflection 0.4 specular 0.1 } }\n",
};

//std::vector<int> above_etag_particle;

bool POV_finish() {	
	int i = 4;
	while (i >= 0) {
		if (pov_filename[i] == '9') {
			pov_filename[i] = '0';
			--i;
		}
		else {
			++pov_filename[i];
			break;
		}
	}
	if (i == -1)
		return false;
	//std::cout << pov_filename << std::endl;
	return true;
}


void POV_write_environment(std::ofstream & file) {
	for (int i = 0; i < environment_line; ++i)
		file << environment[i];
}


void POV_write_cube(std::ofstream & file, int index, bool texture = false) {
	file << "box{ <";
	file << particle[index].position.x - 0.1 - draw_epsilon << ','
		 << particle[index].position.y - 0.1 - draw_epsilon << ','
		 << particle[index].position.z - 0.1 - draw_epsilon << ">, <";
	file << particle[index].position.x + 0.1 + draw_epsilon << ','
		 << particle[index].position.y + 0.1 + draw_epsilon << ','
		 << particle[index].position.z + 0.1 + draw_epsilon << "> ";
	if (texture)
		file << "texture {Water}";
	file << "}\n";
}


void POV_write_element(std::ofstream & file, custom_vec3 & v, int height = 1) { // error
	file << "box{ <";
	double a = v.x * element_size - half_box_size;
	double b = v.y * element_size - half_box_size;
	double c = v.z * element_size;
	file << a - draw_epsilon << ',' << b - draw_epsilon << ',' << c - draw_epsilon 
		 << ">, <" << a + element_size + draw_epsilon << ',' << b + element_size + draw_epsilon
		 << ',' << c + height * element_size + draw_epsilon << "> }\n";
}


void POV_space_handle() {
	E_Tag.clear();
	int a, b, c;
	//above_etag_particle.clear();

	for (int i = 0; i < N_particle; i++) {
		if (particle[i].position.z < Tag_height) {
			a = (int)((particle[i].position.x + offset) * e_1);
			b = (int)((particle[i].position.y + offset) * e_1);
			c = (int)((particle[i].position.z + offset - half_box_size) * e_1);
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					for (int l = 0; l < 4; ++l)
						if (a + j < Tag_Temp1 && b + k < Tag_Temp2 && c + l < Tag_Temp3)
							E_Tag.set(a + j, b + k, c + l);
		}
		else {
			//above_etag_particle.push_back(i);
		}
	}
}


void POV_fill_space(std::ofstream & file) {
	file << "merge {\n";
	file.precision(3);
	int k;
	// 对每个i，j从下往上得到box高度输出merge中的一个box，一共Tag_Temp1*Tag_Temp2次
	for (int i = 0; i < Tag_Temp1; ++i)
		for (int j = 0; j < Tag_Temp2; ++j) {
			int h = 0;
			int c = 0;
			for (k = 0; k < Tag_Temp3; ++k) {
				if (E_Tag.check(i, j, k))
					++h;
				else {
					if (h == 0) {
						c = k;
						continue;
					}
					POV_write_element(file, custom_vec3(i, j, c), h);
					c = k;
					h = 0;
				}
			}
			if (k == Tag_Temp3 && h > 0) {
				POV_write_element(file, custom_vec3(i, j, c), h);
			}
		}
	file << "texture {Water}}\n";


	/*
	// 对above_cone_particle中每个单独处理
	file << "merge {\n";
	int i = 0, j;
	j = above_etag_particle.size();
	while (i < j) {
		POV_write_cube(file, above_etag_particle[i]);
		++i;
	}
	file << "texture {Water}}\n";
	*/
}


bool POV_output() {
	// open 
	std::string name = pov_save_path + pov_filename;
	std::ofstream file(name.data());

	// write
	POV_write_environment(file);
	
	// 填充 E_Tag
	POV_space_handle();

	// 水体ele输出
	POV_fill_space(file);

	// close
	file.close();

	return POV_finish();
}


void test() {
	srand(time(0));
	// initiation
	for (int i = 0; i < particle_per_t; ++i) {
		particle[i].position.set(Rand(-0.1, 0.1), 0, 4);
		particle[i].v.set(Rand(-3, 3), Rand(-3, 3), Rand(7, 8));
		particle[i].a.set(0, 0, -9.8);
	}
	N_particle = particle_per_t;

	int cnt = 0;
	while (1) {
		std::cout << "Frame: " << cnt <<  "  START\n";

		// move
		for (int i = 0; i < N_particle; ++i) {
			particle[i].position.change((particle[i].v + particle[i].a * 0.5 * time_step) * time_step);
			particle[i].v.change(particle[i].a * time_step);
		}

		// add
		if (N_particle < MAX_particle) {
			for (int i = 0; i < particle_per_t; ++i) {
				int temp = N_particle + i;
				particle[temp].position.set(Rand(-0.1, 0.1), 0, 4);
				particle[temp].v.set(Rand(-3, 3), Rand(-3, 3), Rand(7, 9));
				particle[temp].a.set(0, 0, -9.8);
			}
			N_particle += particle_per_t;
		}

		// check bound
		for (int i = 0; i < N_particle; ++i) {
			if (particle[i].v.norm2() <= 1.0) {
				particle[i].position.set(Rand(-0.1, 0.1), 0, 4);
				particle[i].v.set(Rand(-3, 3), Rand(-3, 3), Rand(7, 9));
				particle[i].a.set(0, 0, -9.8);
			}
			if (ABS(particle[i].position.x) > half_box_size - 0.1) {
				particle[i].position.x = particle[i].position.x > 0 ? half_box_size - 0.1 : -half_box_size + 0.1;
				particle[i].v.x *= -0.8;
			}
			if (ABS(particle[i].position.y) > half_box_size - 0.1) {
				particle[i].position.y = particle[i].position.y > 0 ? half_box_size - 0.1 : -half_box_size + 0.1;
				particle[i].v.y *= -0.8;
			}
			if (particle[i].position.z < 0.1) {
				particle[i].position.z = 0.1;
				particle[i].v.z *= -0.8;
			}
		}

		if (!POV_output()) {
			std::cout << "Frame: " << cnt << "  END\n\n";
			break;
		}
		std::cout << "Frame: " << cnt << "  END\n\n";
		++cnt;
	}
}

#endif		// PROGRAM_TYPE


/*
// 该函数用不到，暂留
/**/