#version 430 core

in vec2 v_UV;
flat in uint v_Character;
out vec4 o_Color;

uniform sampler2D u_Texture;
uniform sampler2DArray u_Textures;
uniform vec4 u_TextColor;

void main()
{
	const float sampled = texture2DArray(u_Textures, vec3(v_UV, v_Character)).r;
	o_Color = vec4(u_TextColor.xyz * sampled, u_TextColor.a);
}