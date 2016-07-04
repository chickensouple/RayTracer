#include "Camera.hpp"
#include <stdexcept>

namespace Alectryon {

static const float PERPENDICULAR_THRESHOLD = 0.02f;

Camera::Camera(size_t rowPixels, 
	size_t colPixels, 
	float rowLength,
	float colLength,
	float focalLength) :
	_rowPixels(rowPixels),
	_colPixels(colPixels),
	_rowLength(rowLength),
	_colLength(colLength),
	_focalLength(focalLength) {

	if (rowLength <= 0) {
		throw std::runtime_error("Width must be greater than 0");
	}

	if (colLength <= 0) {
		throw std::runtime_error("Length must be greater than 0");
	}

	if (focalLength <= 0) {
		throw std::runtime_error("Focal Length must be greater than 0");
	}
}

Camera::~Camera() { }

void Camera::setCenter(const Eigen::Vector3f& center) {
	_center = center;
}

void Camera::setViewVectors(const Eigen::Vector3f& normal,
	const Eigen::Vector3f& up) {

	if (normal.dot(up) > PERPENDICULAR_THRESHOLD) {
		throw std::runtime_error("Normal vector must be perpendicular to Up vector");
	}

	_axes[2] = -normal;
	_axes[1] = up;
	_axes[0] = _axes[1].cross(_axes[2]);
}

size_t Camera::getRowPixels() const {
	return _rowPixels;
}

size_t Camera::getColPixels() const {
	return _colPixels;
}

Ray Camera::getRay(size_t row, size_t col) const {
	float pixelYLoc = (float(row) / _rowPixels) * _rowLength - (_rowLength / 2);
	float pixelXLoc = (float(col) / _colPixels) * _colLength - (_colLength / 2);

	Eigen::Vector3f pixelLoc = _center - _focalLength * _axes[2] +
		pixelXLoc * _axes[0] +
		pixelYLoc * _axes[1];

	Eigen::Vector3f dir = pixelLoc - _center;
	dir.normalize();

	return {_center, dir};
}

}


