#include "FontRaster.h"

static uint32_t s_instanceCount = 0;
static bool s_freeTypeInitialized = false;
static FT_Library s_freetype;

FontRaster::FontRaster(const std::filesystem::path& fontPath)
{
	if (!init())
	{
		std::cout << "ERROR:FontRaster: Initialization failure" << std::endl;
		return;
	}

	s_instanceCount++;

	loadFont(fontPath);
}

FontRaster::~FontRaster()
{
	releaseFace();

	s_instanceCount--;
	if (s_instanceCount == 0 && s_freeTypeInitialized)
	{
		FT_Done_FreeType(s_freetype);
		s_freeTypeInitialized = false;
	}
}

bool FontRaster::isInitialized() const
{
	return s_freeTypeInitialized && _isFontLoaded;
}

const std::filesystem::path& FontRaster::getFontPath() const
{
	return _fontPath;
}

const glm::ivec2& FontRaster::getFontSize() const
{
	return _size;
}

bool FontRaster::loadFont(const std::filesystem::path& fontPath)
{
	assert(s_freeTypeInitialized);

	releaseFace();

	_isFontLoaded = false;
	_fontPath = fontPath;

	const std::string& path = _fontPath.string();
	if (fontPath.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return false;
	}

	if (FT_New_Face(s_freetype, path.c_str(), 0, &_face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	_isFontLoaded = true;

	return true;
}

void FontRaster::setFontSize(const glm::ivec2& size)
{
	assert(s_freeTypeInitialized);

	_size = size;
	FT_Set_Pixel_Sizes(_face, size.x, size.y);
}

bool FontRaster::rasterize(wchar_t character, RasterizedChar& result)
{
	assert(s_freeTypeInitialized);

	if (FT_Load_Char(_face, character, FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << character << std::endl;
		return false;
	}

	result = {
		character,
		glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
		glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
		static_cast<uint32_t>(_face->glyph->advance.x)
	};

	return true;
}

static bool initFreeType()
{
	return !FT_Init_FreeType(&s_freetype);
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

void FontRaster::releaseFace()
{
	if (_face)
	{
		FT_Done_Face(_face);
	}
}
