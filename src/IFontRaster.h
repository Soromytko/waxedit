#pragma once
#include <filesystem>
#include <glm/glm.hpp>

struct RasterizedChar
{
	wchar_t character{};
	glm::ivec2 glyphSize{};
	glm::ivec2 glyphBearing{};
	uint32_t glyphAdvance{};
};

class IFontRaster
{
protected:
	IFontRaster() = default;

public:
	virtual ~IFontRaster() = default;

public:
	virtual bool isInitialized() const = 0;
	virtual const std::filesystem::path& getFontPath() const = 0;
	virtual const glm::ivec2& getFontSize() const = 0;

	virtual bool loadFont(const std::filesystem::path& fontPath) = 0;
	virtual void setFontSize(const glm::ivec2 &size) = 0;

	virtual bool rasterize(wchar_t character, RasterizedChar& result) = 0;
};