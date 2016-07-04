#ifndef _RAY_HPP_
#define _RAY_HPP_

#include <Eigen/Dense>

namespace Alectryon {

struct Ray {
	Eigen::Vector3f center;
	Eigen::Vector3f dir;
};

}

#endif /* _RAY_HPP_ */
