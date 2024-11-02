#pragma once
#include <rendell/rendell.h>
#include <glm/glm.hpp>
#include "FontRasterizationResult.h"

class TextRenderer
{
public:
	TextRenderer(const FontRasterizationResult& rasterizedChars, uint32_t viewportWidth, uint32_t viewportHeight);
	~TextRenderer();

	bool isInitialized() const;

	void setMatrix(const glm::mat4& matrix);
	void setFontSize(const glm::vec2& fontSize);
	void setColor(const glm::vec4& color);

	const glm::vec4& getColor() const;

	void draw(const std::wstring& text);

private:
	bool init();

	void bindBuffers();
	void unbindBuffers();

	void renderBatch(const wchar_t* data, uint32_t count);
	const std::vector<uint32_t>& convertWCharToUint32(const wchar_t* data, uint32_t count);

	bool _initialized = false;
	FontRasterizationResult _fontRasterizationResult{};

	int _bufferLength{ 500 };
	static std::vector<uint32_t> _s_textConvertedBuffer;

	glm::vec2 _fontSize{};
	glm::vec4 _color{};
	
	uint32_t _fontSizeUniformIndex{};
	uint32_t _colorUniformIndex{};
	uint32_t _textureArrayUniformIndex{};

	std::unique_ptr<rendell::ShaderBuffer> _textBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _glyphSizeBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _glyphBearingBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _glyphAdvanceBuffer{};

};