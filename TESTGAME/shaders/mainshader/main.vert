#version 330 core
layout (location = 0) in vec3 aVertexPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 atexCoords;
layout (location = 3) in vec3 aKa;
layout (location = 4) in vec3 aKd;
layout (location = 5) in vec3 aKs;
layout (location = 6) in vec3 aMaterial_others;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out highp vec3 vFragPos;
out highp vec3 vNormal;
out highp vec2 vtexCoords;
out highp vec3 vKa;
out highp vec3 vKd;
out highp vec3 vKs;
out highp vec3 vMaterial_others;

void main(void)
{
	vFragPos = vec3(uModel * vec4(aVertexPos, 1.0f));
	vNormal = mat3(transpose(inverse(uModel))) * aNormal;
	vtexCoords = atexCoords;
	vKa = aKa;
	vKd = aKd;
	vKs = aKs;
	vMaterial_others = aMaterial_others;

	gl_Position = uProjection * uView * uModel * vec4(aVertexPos, 1.0f);
}