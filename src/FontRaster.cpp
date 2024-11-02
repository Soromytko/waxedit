#include "FontRaster.h"
#include "freetype.h"
#include <glm/glm.hpp>

#define CHAR_COUNT 300

static uint32_t s_instanceCount = 0;
static bool s_freeTypeInitialized = false;
static FT_Library s_ft;

FontRaster::FontRaster(const std::filesystem::path& fontPath) :
	_fontPath(fontPath)
{
	init();
	s_instanceCount++;
}

FontRaster::~FontRaster()
{
	s_instanceCount--;
	if (s_instanceCount == 0 && s_freeTypeInitialized)
	{
		FT_Done_FreeType(s_ft);
		s_freeTypeInitialized = false;
	}
}

bool FontRaster::isInitialized() const
{
	return s_freeTypeInitialized;
}

const std::filesystem::path& FontRaster::getFontPath() const
{
	return _fontPath;
}

static bool initFreeType()
{
	return !FT_Init_FreeType(&s_ft);
}

void FontRaster::setFontPath(const std::filesystem::path& fontPath)
{
	_fontPath = fontPath;
}

void FontRaster::setFontSize(uint32_t width, uint32_t height)
{
	_width = width;
	_height = height;
}

bool FontRaster::rasterize(FontRasterizationResult& result)
{
	const std::string& fontPath = _fontPath.string();
	if (fontPath.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(s_ft, fontPath.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, _width, _height);

	auto texture2DArray = rendell::createTexture2DArray(_width, _height, CHAR_COUNT, rendell::TextureFormat::R);
	auto rasterizedChars = new std::vector<RasterizedChar>();

	for (wchar_t currentChar = 0; currentChar < CHAR_COUNT; currentChar++)
	{
		if (FT_Load_Char(face, currentChar, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << currentChar << std::endl;
			continue;
		}

		texture2DArray->setSubTextureData(
			static_cast<uint32_t>(currentChar),
			static_cast<uint32_t>(face->glyph->bitmap.width),
			static_cast<uint32_t>(face->glyph->bitmap.rows),
			static_cast<const uint8_t*>(face->glyph->bitmap.buffer)
		);

		RasterizedChar rasterizedChar{
			currentChar,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<uint32_t>(face->glyph->advance.x)
		};
		rasterizedChars->push_back(rasterizedChar);
	}

	FT_Done_Face(face);

	result = { texture2DArray, rasterizedChars };
	return true;
}

bool FontRaster::init()
{
	if (!s_freeTypeInitialized)
	{
		s_freeTypeInitialized = initFreeType();
		if (!s_freeTypeInitialized)
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			return false;
		}
	}

	return s_freeTypeInitialized;
}
