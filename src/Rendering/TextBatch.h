#pragma once
#include "GlyphBuffer.h"

struct TextBatch
{
	GlyphBuffer* glyphBuffer;
	std::vector<TextBuffer*> textBuffers;
};	
