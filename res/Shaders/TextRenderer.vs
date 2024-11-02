#version 430 core

#define NEW_LINE_CHARACTER 10

layout(location = 0) in vec2 a_VertexPosition;

uniform mat4 u_Matrix;
uniform vec2 u_FontSize;

buffer textBuffer { uint text[]; };
buffer glyphSizeBuffer { vec2 glyphSizes[]; };
buffer glyphBearingBuffer { vec2 glyphBearings[]; };
buffer glyphAdvanceBuffer { uint glyphAdvances[]; };

out vec2 v_UV;
flat out uint v_Character;

uint getCharacter(int characterIndex)
{
	return text[characterIndex];
}

uint getGlyphAdvanceInPixels(uint character)
{
	return glyphAdvances[character] >> 6;
}

vec2 getGlyphSize(uint character)
{
	return glyphSizes[character];
}

vec2 getLocalOffset(uint character)
{
	const vec2 bearing = glyphBearings[character];
	const vec2 size = getGlyphSize(character);
	return vec2(bearing.x, bearing.y - size.y);
}

vec2 getGlobalOffset(int characterIndex)
{
	vec2 result = vec2(0.0, 0.0);
	for (int i = 0; i < characterIndex; i++) {
		const uint character = getCharacter(i);
		if (character == NEW_LINE_CHARACTER) {
			result.x = 0.0;
			result.y -= u_FontSize.y;
		} else {
			result.x += getGlyphAdvanceInPixels(character);
		}
	}
	return result;
}

void main()
{
	const int characterIndex = gl_InstanceID;
	const uint character = getCharacter(characterIndex);

	const vec2 size = getGlyphSize(character);
	const vec2 localOffset = getLocalOffset(character);
	const vec2 globalOffset = getGlobalOffset(characterIndex);
	const vec2 pos = a_VertexPosition * size + localOffset + globalOffset;
	
	gl_Position = u_Matrix * vec4(pos, -1.0, 1.0);
	vec2 baseSize = vec2(64.0, 64.0);
	v_UV = vec2(a_VertexPosition.x, 1.0 - a_VertexPosition.y) * size / u_FontSize;
	v_Character = character;
}