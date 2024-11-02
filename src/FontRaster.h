#pragma once
#include <iostream>
#include <map>
#include <filesystem>
#include <rendell/rendell.h>
#include "FontRasterizationResult.h"

class FontRaster
{
public:
	FontRaster(const std::filesystem::path& fontPath = "");
	~FontRaster();

	bool isInitialized() const;

	void setFontPath(const std::filesystem::path& fontPath);
	void setFontSize(uint32_t width, uint32_t height);
	bool rasterize(FontRasterizationResult &result);

	const std::filesystem::path& getFontPath() const;

private:
	bool init();

	std::filesystem::path _fontPath;
	uint32_t _width = 64, _height = 64;
};