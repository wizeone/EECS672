#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

out vec4 fragmentColor;

// Phong material properties:
uniform vec3 kd = // "kd" - diffuse reflectivity; basic object color
	vec3(0.8, 0.0, 0.0); // default: darkish red

// Light sources
const int MAX_NUM_LIGHTS = 3;
uniform vec4 p_ecLightSource[MAX_NUM_LIGHTS] = vec4[](
	vec4(0.7, 0.2, 1.0, 0.0),
	vec4(0.0, 0.0, 1.0, 0.0),
	vec4(0.0, 0.0, 1.0, 0.0));

vec4 evaluateLightingModel()
{
	// THIS IS JUST A PLACEHOLDER FOR A LIGHTING MODEL.
	// It only currently implements simple Lambert shading.

	// NOTE: The use of "abs" here is a temporary hack. As we study the
	//       Phong lighting model more carefully, we will REMOVE "abs"
	//       while incorporating more sophisticated processing.

	float factor = abs(dot(normalize(p_ecLightSource[0].xyz), pvaIn.ecUnitNormal));

	return vec4(factor * kd, 1.0);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
