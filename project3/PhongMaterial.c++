// PhongMaterial.c++ - Utility struct with Phong model properties

#include "PhongMaterial.h"

PhongMaterial::PhongMaterial(float r, float g, float b, float af, float df, float sf,
		float shine, float a) :
		shininess(shine), alpha(a)
{
	float rgb[] = { r, g, b};
	copyColors(rgb, af, df, sf);
}

void PhongMaterial::copyColors(const float* rgb, float af, float df, float sf)
{
	if (rgb == NULL)
		return;
	for (int i=0 ; i<3 ; i++)
	{
		ka[i] = af * rgb[i];
		kd[i] = df * rgb[i];
		ks[i] = sf * rgb[i];
	}
}
