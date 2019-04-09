#version 330

uniform mat4 P;
uniform mat4 MV;
uniform float time;

in vec4 aPos;
in vec3 aNor;
in vec2 aTex;

out vec2 texCoord;
out float Time;
out float lightFactor;

void main()
{
	//gl_Position = (P * MV * aPos) + vec4(400*sin(time),400*sin(time * 1.5),0,0);
	gl_Position = (P * MV * aPos);
	texCoord = aTex;
	Time = time;

	vec3 lightPosition = vec3(0,0, 500);
	float lightDistance = length((MV * aPos).xyz - lightPosition) / 512;
	vec3 normalDirection = normalize((MV * vec4(normalize(aNor), 0)).xyz);
	vec3 lightDirection = normalize(vec3(1, -1, 1));
	float dotP = clamp(dot(normalDirection, lightDirection), 0, 1);

	lightFactor = dotP / (lightDistance*lightDistance);
}
