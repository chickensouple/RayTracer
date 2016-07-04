#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_ 

#include <Eigen/Dense>
#include "Ray.hpp"

namespace Alectryon {

class Camera {
public:
	Camera(size_t rowPixels, 
		size_t colPixels, 
		float rowLength,
		float colLength,
		float focalLength);

	~Camera();

	void setCenter(const Eigen::Vector3f& center);

	void setViewVectors(const Eigen::Vector3f& normal,
		const Eigen::Vector3f& up);

	size_t getRowPixels() const;

	size_t getColPixels() const;

	Ray getRay(size_t row, size_t col) const;

private:
	size_t _rowPixels, _colPixels;
	float _rowLength, _colLength; // meters
	float _focalLength;
	Eigen::Vector3f _center;
	Eigen::Vector3f _axes[3];
};

}

#endif /* _CAMERA_HPP_ */

