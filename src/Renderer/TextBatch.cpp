#include "TextBatch.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

TextBatch::TextBatch(wchar_t from, wchar_t to, IFontRaster *fontRaster)
{
#ifdef _DEBIG
	assert(from < to);
#endif
	_range = { from, to };

	const size_t length = static_cast<size_t>(to - from);
	_textBufferData.resize(length);
	_transformBufferData.resize(length);

	if (!fontRaster->rasterize(from, to, _fontRasterizationResult))
	{
		std::cout << "ERROR:TextBatch: Rasterization failure, {" << from << ", " << to << "}" << std::endl;
	}

	_textures.reset(_fontRasterizationResult.texture2DArray);
	_textBuffer.reset(rendell::createShaderBuffer(nullptr, length * sizeof(uint32_t)));
	_transformBuffer.reset(rendell::createShaderBuffer(nullptr, length * sizeof(glm::mat4)));
}

void TextBatch::beginUpdating()
{
	_counter = 0;
}

void TextBatch::addCharacter(wchar_t character, glm::vec2 offset)
{
#ifdef _DEBUG
	assert(character >= _range.first && character < _range.second);
#endif
	const RasterizedChar& rasterizedChar = _fontRasterizationResult.rasterizedChars[character - _range.first];

	glm::vec4 glyphTransform(offset, rasterizedChar.glyphSize.x, rasterizedChar.glyphSize.y);

	_textBufferData[_counter] = static_cast<uint32_t>(character);
	_transformBufferData[_counter] = std::move(glyphTransform);
	_counter++;
}

void TextBatch::endUpdating()
{
	_textBuffer->setSubData(
		static_cast<const void*>(_textBufferData.data()),
		_counter * sizeof(uint32_t)
	);
	_transformBuffer->setSubData(
		static_cast<const void*>(_transformBufferData.data()),
		_counter * sizeof(glm::mat4)
	);
}

void TextBatch::bind(uint32_t textureBlock, uint32_t textBufferBinding, uint32_t transformBufferBinding) const
{
	_textures->bind(textureBlock);
	_textBuffer->bind(textBufferBinding);
	_transformBuffer->bind(transformBufferBinding);
}

void TextBatch::unbind() const
{
	_textures->unbind();
	_textBuffer->unbind();
	_transformBuffer->unbind();
}

const RasterizedChar& TextBatch::getRasterizedChar(wchar_t character) const
{
	const size_t index = static_cast<size_t>(character - _range.first);
	return _fontRasterizationResult.rasterizedChars[index];
}

const std::pair<wchar_t, wchar_t>& TextBatch::getRange() const
{
	return _range;
}
