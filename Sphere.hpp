#ifndef _SPHERE_HPP_
#define _SPHERE_HPP_

#include <Eigen/Dense>
#include <array>
#include <cstdint>

struct Sphere {
	Eigen::Vector3f center;
	float radius;
	std::array<uint8_t, 3> color;
};

#endif /* _SPHERE_HPP_ */
