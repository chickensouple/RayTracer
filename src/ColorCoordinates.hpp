#ifndef _COLOR_COORDINATES_HPP_
#define _COLOR_COORDINATES_HPP_

#include <array>
#include <algorithm>
#include <cstdint>

namespace Alectryon {

std::array<float, 3> convertRGBtoHSV(std::array<uint8_t, 3> rgb);

std::array<uint8_t, 3> convertHSVtoRGB(std::array<float, 3> hsv);

}

#endif /* _COLOR_COORDINATES_HPP_ */
