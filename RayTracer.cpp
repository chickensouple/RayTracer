#include "RayTracer.hpp"
#include <tuple>
#include <iostream>

namespace Alectryon {

static std::tuple<const Sphere*, Eigen::Vector3f> 
	calculateFirstIntersect(const Ray& ray, const Scene& scene);


static std::tuple<bool, Eigen::Vector3f, float> 
	calculateIntersect(const Ray& ray, const Sphere& sphere);

static float getPhongIlluminationIntensity(
	const Ray& ray,
	const Eigen::Vector3f& intersect,
	const Sphere* sphere,
	const Scene& scene);


RayTracer::RayTracer(const Camera& camera, const Scene& scene) :
	_camera(camera), _scene(scene) {
	_screen = new std::array<uint8_t, 3>[_camera.getRowPixels() * _camera.getColPixels()];
}

RayTracer::~RayTracer() {
	delete[] _screen;
}

const std::array<uint8_t, 3>* RayTracer::trace() {
	for (size_t i = 0; i < _camera.getRowPixels(); i++) {
		for (size_t j = 0; j < _camera.getColPixels(); j++) {
			size_t imageIndex = i * _camera.getColPixels() + j;

			Ray ray = _camera.getRay(i, j);

			std::tuple<const Sphere*, Eigen::Vector3f> intersect = calculateFirstIntersect(ray, _scene);

			const Sphere* intersectSphere = std::get<0>(intersect);
			Eigen::Vector3f& intersectPt = std::get<1>(intersect);

			if (intersectSphere == nullptr) {
				// ray doesn't hit anything
				_screen[imageIndex] = {0, 0, 0};
			} else {
				float intensity = getPhongIlluminationIntensity(ray, intersectPt, intersectSphere, _scene);
			
				_screen[imageIndex] = intersectSphere->color;
				for (auto& color : _screen[imageIndex]) {
					color *= intensity;
				}
			}
		}
	}
	return _screen;
}

std::tuple<const Sphere*, Eigen::Vector3f> 
calculateFirstIntersect(const Ray& ray, const Scene& scene) {

	const Sphere* intersectSphere = nullptr;
	float minDist = -1;
	Eigen::Vector3f minIntersectionPoint(0, 0, 0);

	for (auto& sphere : scene.spheres) {
		std::tuple<bool, Eigen::Vector3f, float> intersection = calculateIntersect(ray, sphere);
		if (not std::get<0>(intersection)) {
			continue;
		}

		float dist = std::get<2>(intersection);
		if (minDist == -1 or dist < minDist) {
			minDist = dist;
			minIntersectionPoint = std::get<1>(intersection);
			intersectSphere = &sphere;
		}
	}

	return std::make_tuple(intersectSphere, minIntersectionPoint);
}

std::tuple<bool, Eigen::Vector3f, float> calculateIntersect(const Ray& ray, const Sphere& sphere) {
	Eigen::Vector3f cVec = sphere.center - ray.center;
	float cDotD = cVec.dot(ray.dir);
	
	// if the ray is facing the wrong direction,
	// it can't intersect
	if (cDotD < 0) {
		return std::make_tuple(false, Eigen::Vector3f(), 0);
	}

	float cVecNorm = cVec.norm();

	// p measures closest distance from the center of the sphere to the ray
	float p = std::sqrt(cVecNorm*cVecNorm - cDotD*cDotD);

	// if p is greater than the radius, then it misses the sphere
	if (p > sphere.radius) {
		return std::make_tuple(false, Eigen::Vector3f(), 0);
	}

	// calculates how much of cVec's projection onto ray.dir is in the sphere
	float l = std::sqrt(sphere.radius*sphere.radius - p*p);

	float intersectionDist = cDotD - l;
	Eigen::Vector3f intersection = ray.center + (ray.dir * intersectionDist);
	return std::make_tuple(true, intersection, intersectionDist);
}

float getPhongIlluminationIntensity(
	const Ray& ray,
	const Eigen::Vector3f& intersect,
	const Sphere* sphere,
	const Scene& scene) {

	Eigen::Vector3f viewVec = ray.center - intersect;
	viewVec.normalize();

	// ambient light 
	float intensity = 0.1;

	for (auto& light : scene.lights) {
		Eigen::Vector3f lightDir = intersect - light.center;
		lightDir.normalize();


		Ray lightRay{light.center, lightDir};
		
		std::tuple<const Sphere*, Eigen::Vector3f> lightIntersect = 
			calculateFirstIntersect(lightRay, scene);

		const Sphere* lightIntersectSphere = std::get<0>(lightIntersect);
		Eigen::Vector3f& lightIntersectPt = std::get<1>(lightIntersect);

		// TODO: check why it doesn't intersect sometimes
		if (lightIntersectSphere != sphere or
			not lightIntersectPt.isApprox(intersect, 0.1)) {
			continue;
		}

		Eigen::Vector3f normalVec = intersect - sphere->center;
		normalVec.normalize();

		// adding diffuse component
		float normalDotLight = normalVec.dot(-lightDir);
		if (normalDotLight < 0) {
			normalDotLight = 0;
		}
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


}
