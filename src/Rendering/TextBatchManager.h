#pragma once
#include "../FontRaster.h"
#include "TextBuffer.h"
#include "GlyphBuffer.h"
#include "TextBatch.h"

class TextBatchManager
{
public:
	TextBatchManager(size_t batchSize = 100);
	~TextBatchManager() = default;

	void setText(const std::wstring& text);

	const std::vector<TextBatch*>& getTextBatches() const;

private:
	size_t _batchSize{};

	std::vector<std::unique_ptr<TextBuffer>> _textBuffers{};
	std::vector<std::unique_ptr<GlyphBuffer>> _glyphBuffers{};
	std::vector<std::vector<TextBuffer>> _textBuffers{};
};