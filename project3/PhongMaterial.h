// PhongMaterial.h - Utility struct with Phong model properties

#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include <stdlib.h>

struct PhongMaterial
{
	float ka[3], kd[3], ks[3];
	float shininess, alpha;

	PhongMaterial(float r, float g, float b,
		float af=0.2, float df=0.2, float sf=0.8,
		float shine=25.0, float a=1.0);

	void copyColors(const float* rgb, float af, float df, float sf);
};

#endif
