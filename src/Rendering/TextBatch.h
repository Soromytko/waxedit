#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "GlyphBuffer.h"
#include "TextBuffer.h"
#include "../IFontRaster.h"

struct TextBatch
{
public:
	TextBatch(GlyphBuffer *glyphBuffer, size_t textBufferSize);
	~TextBatch() = default;

	void beginUpdating();
	void appendCharacter(wchar_t character, glm::vec2 offset);
	void endUpdating();

	const GlyphBuffer* getGlyphBuffer() const;
	const std::vector<std::unique_ptr<TextBuffer>>& GetTextBuffers() const;

private:
	void trimTextBuffers();

	size_t _counter{};
	size_t _textBufferSize{};

	std::unique_ptr<GlyphBuffer> _glyphBuffer{};
	std::vector<std::unique_ptr<TextBuffer>> _textBuffers{};

};
