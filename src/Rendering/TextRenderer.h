#pragma once
#include <map>
#include <functional>
#include <unordered_set>
#include <rendell/rendell.h>
#include <glm/glm.hpp>
#include "IFontRaster.h"
#include "GlyphBuffer.h"
#include "TextBatch.h"
#include "TextBuffer.h"

class TextRenderer
{
public:
	TextRenderer(IFontRaster* fontRaster);
	~TextRenderer();

	bool isInitialized() const;

	void setText(const std::wstring& value);
	void setText(std::wstring&& value);
	void setMatrix(const glm::mat4& matrix);
	void setFontSize(const glm::vec2& fontSize);
	void setColor(const glm::vec4& color);

	const std::wstring& getText() const;
	const glm::vec4& getColor() const;

	void draw();

private:
	bool init();

	void updateShaderBuffers();

	void beginDrawing();
	void endDrawing();

	TextBatch* createTextBatch(wchar_t character);
	GlyphBuffer* createGlyphBuffer(uint16_t rangeIndex);

	bool _initialized = false;
	glm::mat4 _matrix{};
	glm::vec2 _fontSize{};
	glm::vec4 _color{};

	std::shared_ptr<IFontRaster> _fontRaster{ nullptr };

	std::map<uint32_t, std::unique_ptr<TextBatch>> _textBatches{};
	std::unordered_set<TextBatch*> _textBatchesForRendering{};

	std::wstring _text{};

};