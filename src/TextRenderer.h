#pragma once
#include <map>
#include <rendell/rendell.h>
#include <glm/glm.hpp>
#include "IFontRaster.h"

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
	//bool initBuffers();

	void updateShaderBuffers();
	void addCharToBuffer(const RasterizedChar& rasterizedChar, const glm::vec3& offset, size_t index);

	void bindBuffers();
	void unbindBuffers();

	const RasterizedChar& getRasterizedChar(wchar_t character);

	void renderBatch(const wchar_t* data, uint32_t count);

	bool _initialized = false;
	glm::vec2 _fontSize{};
	glm::vec4 _color{};

	std::shared_ptr<IFontRaster> _fontRaster{ nullptr };
	std::map<wchar_t, RasterizedChar> _rasterizedCharCache{};

	std::wstring _text{};
	std::vector<uint32_t> _textBufferData{};
	std::vector<glm::mat4> _glyphMatrixBufferData{};

	uint32_t _fontSizeUniformIndex{};
	uint32_t _colorUniformIndex{};
	uint32_t _textureArrayUniformIndex{};

	std::unique_ptr<rendell::Texture2DArray> _textures{};
	std::unique_ptr<rendell::ShaderBuffer> _textBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _glyphMatrixBuffer{};

};