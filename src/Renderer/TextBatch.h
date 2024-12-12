#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <rendell/rendell.h>
#include "../IFontRaster.h"

struct TextBatch
{
public:
	TextBatch(wchar_t from, wchar_t to, IFontRaster *fontRaster);

	void beginUpdating();
	void addCharacter(wchar_t character, glm::vec2 offset);
	void endUpdating();

	void bind(uint32_t textureBlock, uint32_t textBufferBinding, uint32_t transformBufferBinding) const;
	void unbind() const;

	const RasterizedChar& getRasterizedChar(wchar_t character) const;
	const std::pair<wchar_t, wchar_t>& getRange() const;

private:
	size_t _counter{};

	FontRasterizationResult _fontRasterizationResult{};

	std::vector<uint32_t> _textBufferData{};
	std::vector<glm::vec4> _transformBufferData{};

	std::pair<wchar_t, wchar_t> _range{};
	std::unique_ptr<rendell::Texture2DArray> _textures{};
	std::unique_ptr<rendell::ShaderBuffer> _textBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _transformBuffer{};
};
