#ifndef _MATERIAL_PROPERTY_HPP_
#define _MATERIAL_PROPERTY_HPP_

namespace Alectryon {

namespace Material {

struct MaterialProperty {
	float ambientConstant; // [0, 1]
	float diffuseConstant; // [0, 1]
	float specularConstant; // [0, 1]
	int specularFocus; // [1, inf]
};

static const MaterialProperty DefaultShiny {0.1, 0.8, 1, 16};

}

}

#endif /* _MATERIAL_PROPERTY_HPP_ */
