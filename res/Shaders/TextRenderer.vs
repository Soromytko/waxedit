#version 430 core

#define NEW_LINE_CHARACTER 10

layout(location = 0) in vec2 a_VertexPosition;

uniform mat4 u_Matrix;
uniform vec2 u_FontSize;
uniform int u_CharFrom;

buffer textBuffer { uint text[]; };
buffer glyphMatrixBuffer { mat4 glyphMatrices[]; };

out vec2 v_UV;
flat out uint v_TextureIndex;

vec3 getScaleFromMat4(mat4 matrix) {
    vec3 scale;
    scale.x = length(matrix[0].xyz);
    scale.y = length(matrix[1].xyz);
    scale.z = length(matrix[2].xyz);
    return scale;
}

void main()
{
	const uint characterIndex = gl_InstanceID;
	const uint character = text[characterIndex];
	const mat4 glyphMatrix = glyphMatrices[characterIndex];

	gl_Position = u_Matrix * glyphMatrix * vec4(a_VertexPosition, -1.0, 1.0);
	vec2 size = getScaleFromMat4(glyphMatrix).xy;
	v_UV = vec2(a_VertexPosition.x, 1.0 - a_VertexPosition.y) * size / u_FontSize;
	v_TextureIndex = character - u_CharFrom;
}