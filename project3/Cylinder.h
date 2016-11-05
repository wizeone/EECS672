// Cylinder.h

#ifndef CYLINDER_H
#define CYLINDER_H

#include "SceneElement.h"
#include "ShaderIF.h"

class Cylinder : public SceneElement
{
public:
	Cylinder(ShaderIF* sIF, PhongMaterial& matl);
	virtual ~Cylinder();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	// The ShaderIF and kd (and other) material properties will be
	// stored with the SceneElement piece of this object instance.
	// You only need add instance variables here that are unique
	// to the new subclass you are creating.
};

#endif
