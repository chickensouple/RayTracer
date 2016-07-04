#include "ColorCoordinates.hpp"
#include <cmath>
#include <stdexcept>

namespace Alectryon {

static const float PI_OVER_3 = M_PI / 3;	

std::array<float, 3> convertRGBtoHSV(std::array<uint8_t, 3> rgb) {
	std::array<float, 3> rgb_norm;
	for (int i = 0; i < 3; i++) {
		rgb_norm[i] = (float) rgb[i] / 255;
	}

	float cMax = *std::max_element(rgb_norm.begin(), rgb_norm.end());
	float cMin = *std::min_element(rgb_norm.begin(), rgb_norm.end());
	float delta = cMax - cMin;

	float hue;
	if (delta == 0) {
		hue = 0;
	} else if (cMax == rgb_norm[0]) {
		hue = (rgb_norm[1] - rgb_norm[2]) / delta;
	} else if (cMax == rgb_norm[1]) {
		hue = ((rgb_norm[2] - rgb_norm[0]) / delta) + 2;
	} else {
		hue = ((rgb_norm[0] - rgb_norm[1]) / delta) + 4;
	}
	hue *= PI_OVER_3;
	if (hue < 0) {
		hue += M_PI * 2;
	}

	float saturation;
	if (cMax == 0) {
		saturation = 0;
	} else {
		saturation = delta / cMax;
	}

	float value = cMax;

	return {hue, saturation, value};
}


std::array<uint8_t, 3> convertHSVtoRGB(std::array<float, 3> hsv) {
	if (hsv[1] == 0) {
		uint8_t v = (uint8_t) (hsv[2] * 255);
		return {v, v, v};
	}

	int i = floor(hsv[0] / PI_OVER_3);
	float f = hsv[0] - i;
	float v = hsv[2];
	float p = v * (1 - hsv[1]);
	float q = v *  (1 - hsv[1] * f);
	float t = v * (1 - hsv[1] * (1 - f));

	uint8_t v_int = (uint8_t) (v * 255);
	uint8_t p_int = (uint8_t) (p * 255);
	uint8_t q_int = (uint8_t) (q * 255);
	uint8_t t_int = (uint8_t) (t * 255);


	switch (i) {
		case 0:
			return {v_int, t_int, p_int};
		case 1:
			return {q_int, v_int, p_int};
		case 2:
			return {p_int, v_int, t_int};
		case 3:
			return {p_int, q_int, v_int};
		case 4:
			return {t_int, p_int, v_int};
		case 5:
			return {v_int, p_int, q_int};
		default:
			throw std::runtime_error("Invalid input");
	}
}

}
