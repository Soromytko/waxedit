#pragma once
#include <iostream>
#include <map>
#include "IFontRaster.h"
#include "freetype.h"

class FontRaster : public IFontRaster
{
public:
	FontRaster(const std::filesystem::path& fontPath = "");
	~FontRaster();

	bool isInitialized() const override;
	const std::filesystem::path& getFontPath() const override;
	const glm::ivec2& getFontSize() const override;

	bool loadFont(const std::filesystem::path& fontPath) override;
	void setFontSize(const glm::ivec2 &size) override;

	bool rasterize(wchar_t from, wchar_t to, FontRasterizationResult& result) override;

private:
	bool init();
	void releaseFace();
	bool rasterizeChar(wchar_t character, FT_Glyph &result);
	FT_Glyph getGlyphStub();

	FT_Face _face{ nullptr };
	std::filesystem::path _fontPath{};
	glm::ivec2 _size{ 64, 64 };
};