#ifndef _RAY_TRACER_HPP_
#define _RAY_TRACER_HPP_


#include "Ray.hpp"
#include "Scene.hpp"
#include <utility>
#include <tuple>
#include <array>
#include <cstdint>

class RayTracer {
public:
	RayTracer(float screenWidth, 
		float screenLength, 
		int numWidthPixels, 
		int numLengthPixels, 
		float focalLength);

	~RayTracer();

	const std::array<uint8_t, 3>* trace(const Scene& scene);

private:
	float _screenWidth;
	float _screenLength;
	int _numWidthPixels;
	int _numLengthPixels;
	float _focalLength;
	Eigen::Vector3f _focalPoint;

	std::array<uint8_t, 3>* _screen;

private:
	static std::tuple<bool, Eigen::Vector3f, const Sphere*> 
	calculateClosestIntersect(const Ray& ray, const std::vector<Sphere>& spheres);

	// finds the first intersection of a ray with a sphere, if at least one exists
	static std::pair<bool, Eigen::Vector3f> calculateIntersect(const Ray& ray, const Sphere& sphere);


	float getPhongIlluminationIntensity(const Eigen::Vector3f& intersect,
		const Sphere* sphere,
		const Scene& scene);

};

#endif /* _RAY_TRACER_HPP_ */
