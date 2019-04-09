#version 120

uniform int selected;
uniform float time;
uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos;
attribute vec3 aNor;

varying vec3 vCol;

void main()
{
	float offset  = aPos.y + aPos.x + aPos.z + time;
	//vec3 newPos = vec3(cos(offset * 1.1) + 1, sin(offset * 1.2) + 1, sin(offset * 1.3) + 1) * 0.03 + vec3(aPos.x,aPos.y,aPos.z) * 1.2;
	//gl_Position = P * MV * (vec4(newPos, 1));

	gl_Position = P * MV * aPos;

	vec3 baseColor = vec3(.75, 0.25, 0.25);

	vec3 lightPosition = vec3(0,0, 500);
	float lightDistance = length((MV * aPos).xyz - lightPosition) / 512;
	vec3 normalDirection = normalize((MV * vec4(normalize(aNor), 0)).xyz);
	vec3 lightDirection = normalize(vec3(1, -1, 1));
	float dotP = clamp(dot(normalDirection, lightDirection), 0, 1);

	vCol = baseColor * (dotP / (lightDistance*lightDistance) * 0.8 + 0.2);
}
