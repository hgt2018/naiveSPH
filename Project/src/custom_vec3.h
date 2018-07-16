#pragma once
#include <iostream>
#include <cmath>

class custom_vec3 {
public:
	double x;
	double y;
	double z;

	custom_vec3() { x = y = z = 0; }
	custom_vec3(double xx, double yy, double zz) :x(xx), y(yy), z(zz) {}

	custom_vec3(const custom_vec3 & v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline void set(double xx, double yy, double zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	inline void set(custom_vec3 & v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	inline void change(double delta_x, double delta_y, double delta_z) {
		x += delta_x;
		y += delta_y;
		z += delta_z;
	}

	inline void change(custom_vec3 & v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}

	custom_vec3 operator + (custom_vec3 & v) {
		return custom_vec3(x + v.x, y + v.y, z + v.z);
	}

	custom_vec3 operator - (custom_vec3 & v) {
		return custom_vec3(x - v.x, y - v.y, z - v.z);
	}

	custom_vec3 operator * (double t) {
		return custom_vec3(x * t, y * t, z * t);
	}

	inline double norm2() {
		return x * x + y * y + z * z;
	}

	inline void Print() {
		std::cout << "vec3 is <" << x << ", " << y << ", " << z << ">\n" << std::endl;
	}
};


// 求向量v2在v1上的投影长度,v1模长为1
inline double vec3_dot(custom_vec3 & v1, custom_vec3 & v2) {
	return ((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z));
}
