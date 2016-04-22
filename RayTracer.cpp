#include "RayTracer.hpp"
#include <iostream>

using namespace std;

RayTracer::RayTracer(float screenColsLength, 
	float screenRowsLength, 
	int numColPixels, 
	int numRowPixels, 
	float focalLength) :
	_screenColsLength(screenColsLength), 
	_screenRowsLength(screenRowsLength),
	_numColPixels(numColPixels),
	_numRowPixels(numRowPixels),
	_focalLength(focalLength),
	_focalPoint(0, 0, focalLength) {

	_screen = new std::array<uint8_t, 3>[numColPixels * numRowPixels];
}

RayTracer::~RayTracer() {
	delete[] _screen;
}

const std::array<uint8_t, 3>* RayTracer::trace(const Scene& scene) {
	for (int i = 0; i < _numRowPixels; i++) {
		float pixelYLoc = (float(i) / _numRowPixels) * _screenRowsLength - (_screenRowsLength/2);

		for (int j = 0; j < _numColPixels; j++) {
			float pixelXLoc = (float(j) / _numColPixels) * _screenColsLength - (_screenColsLength/2);

			int imageIndex = i * _numColPixels + j;

			// std::cout << "image Index: " << imageIndex << "\ti: " << i << "\tj: " << j << '\n';

			Eigen::Vector3f pixelLoc(pixelXLoc, pixelYLoc, 0);
			Eigen::Vector3f dir = pixelLoc - _focalPoint;
			dir.normalize();

			Ray ray{_focalPoint, dir};

			std::tuple<bool, Eigen::Vector3f, const Sphere*> intersect = calculateClosestIntersect(ray, scene.spheres);

			Eigen::Vector3f& intersectPt = std::get<1>(intersect);
			const Sphere* intersectSphere = std::get<2>(intersect);

			// cout << "Ray Center:\n" << ray.center << "\nRay Dir:\n" << ray.dir << '\n';

			if (not std::get<0>(intersect)) {
				// ray doesn't hit anything
				_screen[imageIndex] = {0, 0, 0};
			} else {
				float intensity = getPhongIlluminationIntensity(intersectPt, intersectSphere, scene);

				_screen[imageIndex] = intersectSphere->color;
				for (auto& color : _screen[imageIndex]) {
					color *= intensity;
				}
			}
		}
	}

	return _screen;
}

std::tuple<bool, Eigen::Vector3f, const Sphere*>  
RayTracer::calculateClosestIntersect(const Ray& ray, const std::vector<Sphere>& spheres) {

	bool foundIntersection = false;
	float minDist = -1;
	const Sphere* closestSphere = nullptr;
	Eigen::Vector3f minIntersectionPoint(0, 0, 0);

	for (auto& sphere : spheres) {
		std::pair<bool, Eigen::Vector3f> intersection = calculateIntersect(ray, sphere);
		if (not intersection.first) {
			continue;
		}

		foundIntersection = true;
		float dist = (intersection.second - ray.center).norm();
		if (minDist == -1 or dist < minDist) {
			minDist = dist;
			minIntersectionPoint = intersection.second;
			closestSphere = &sphere;
		}
	}

	return std::make_tuple(foundIntersection, minIntersectionPoint, closestSphere);
}

std::pair<bool, Eigen::Vector3f> RayTracer::calculateIntersect(const Ray& ray, const Sphere& sphere) {

	Eigen::Vector3f cVec = sphere.center - ray.center;
	float cDotD = cVec.dot(ray.dir);
	float cVecNorm = cVec.norm();
	float p = std::sqrt(cVecNorm*cVecNorm - cDotD*cDotD);

	if (cDotD < 0 or p > sphere.radius) {
		return std::make_pair(false, Eigen::Vector3f());
	}

	float l = std::sqrt(sphere.radius*sphere.radius - p*p);

	float intersectionDist = cDotD - l;
	Eigen::Vector3f intersection = ray.center + (ray.dir * intersectionDist);
	return std::make_pair(true, intersection);
}


float RayTracer::getPhongIlluminationIntensity(const Eigen::Vector3f& intersect,
	const Sphere* sphere,
	const Scene& scene) {

	Eigen::Vector3f viewVec = _focalPoint - intersect;
	viewVec.normalize();


	// ambient light 
	float intensity = 0.1;

	for (auto& light : scene.lights) {
		Eigen::Vector3f lightDir = intersect - light.center;
		lightDir.normalize();


		Ray lightRay{light.center, lightDir};
		auto lightIntersect = calculateClosestIntersect(lightRay, scene.spheres);

		// TODO: check why it doesn't intersect sometimes
		if (std::get<0>(lightIntersect) and 
			(std::get<2>(lightIntersect) != sphere or
			not std::get<1>(lightIntersect).isApprox(intersect, 0.1))) {
			
			continue;
		}

		Eigen::Vector3f normalVec = intersect - sphere->center;
		normalVec.normalize();

		// adding diffuse component
		float normalDotLight = normalVec.dot(-lightDir);
		intensity += normalDotLight * light.intensity * 0.8;

		// adding specular component

		// reflection vector
		Eigen::Vector3f lightReflection = lightDir - 2*(normalVec.dot(lightDir))*normalVec;
		float specular = std::pow(lightReflection.dot(viewVec), 16);
		intensity += specular * light.intensity;
	}

	if (intensity > 1) {
		intensity = 1;
	}

	return intensity;
}

