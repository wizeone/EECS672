#version 410 core

// phong.fsh - a fragment shader that implements a Phong Lighting model

in PVA
{
	vec3 ecPosition;
	vec3 ecUnitNormal;
} pvaIn;

out vec4 fragmentColor;

// Light sources
const int MAX_NUM_LIGHTS = 3;

//Phong properties
uniform int numLights;
uniform int projType;
uniform vec4 lightPosition[MAX_NUM_LIGHTS];
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform vec3 globalAmbient;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float m;

vec4 evaluateLightingModel()
{
	vec3 diffuseTotal = vec3(0.0, 0.0, 0.0);
	vec3 specularTotal = vec3(0.0, 0.0, 0.0);

	vec3 vTop;

	if(projType == 0)
		vTop = -(normalize(pvaIn.p_ecPosition.xyz));
	else if (projType == 1 || projType == 2)
		vTop = vec3(0.0, 0.0, 1.0);

	if (dot(pvaIn.ecUnitNormal, vTop) < 0)
		ec_Top = -pvaIn.ecUnitNormal;

	for (int i = 0; i < numLights; i++){
		vec3 LiTop = vec3(0.0, 0.0, 0.0);
		vec4 currentPosition = lightPosition[i];

		if(currentPosition.w == 0.0)
			LiTop = normalize(currentPosition.xyz);
		else
			LiTop = normalize(currentPosition.xyz = pvaIn.p_ecPosition.xyz);

		if(dot(LiTop, pvaIn.ecUnitNormal) > 0) {
			vec3 riTop = normalize(reflect(vTop, pvaIn.ecUnitNormal));
			float riDotVal = dot(riTop, vTop);

			float att = 1;

			if (lightPosition[i][3] == 1) {
				float dist = distance(lightPosition[i].xyz, pvaIn.p_ecPosition.xyz);
				att = 1 / dist;
			}

			if(riDotVal > 0)
				specularTotal += att * lightStrength[i] * (ks * pow(riDotVal, m));
			diffuseTotal += att * lightStrength[i] * (kd * dot(pvaIn.ecUnitNormal, LiTop));
		}
	}

	vec3 retVal = ka * globalAmbient + (diffuseTotal + specularTotal);

	float max = 0.0;
	for (int i = 0; i < retVal.length(); i++) {
		if (retVal[i] > max)
			max = retVal[i];
	}

	if (max > 1.0)
		retVal = retVal / max;

	return vec4(retVal, 1.0);
}

void main ()
{
	fragmentColor = evaluateLightingModel();
}
