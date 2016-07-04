#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <vector>
#include <Eigen/Dense>
#include "Sphere.hpp"
#include "Light.hpp"

namespace Alectryon {

struct Scene {
	std::vector<Sphere> spheres;
	std::vector<Light> lights;
};

}

#endif /* _SCENE_HPP_ */
