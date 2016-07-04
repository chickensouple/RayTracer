#ifndef _RAY_TRACER_HPP_
#define _RAY_TRACER_HPP_ 

#include "Camera.hpp"
#include "Scene.hpp"
#include <array>
#include <cstdint>

namespace Alectryon {

class RayTracer {
public:
	RayTracer(const Camera& camera, const Scene& scene);

	~RayTracer();

	const std::array<uint8_t, 3>* trace();

private:
	std::array<uint8_t, 3>* _screen;

	const Camera& _camera;
	const Scene& _scene;
};

}

#endif /* _RAY_TRACER_HPP_ */
