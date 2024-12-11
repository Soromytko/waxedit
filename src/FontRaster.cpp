#include "FontRaster.h"

static uint32_t s_instanceCount = 0;
static bool s_freeTypeInitialized = false;
static FT_Library s_freetype;

FontRaster::FontRaster(const std::filesystem::path& fontPath)
{
	s_instanceCount++;

	if (!init())
	{
		std::cout << "ERROR:FontRaster: Initialization failure" << std::endl;
		return;
	}

	setFontPath(fontPath);
}

FontRaster::~FontRaster()
{
	s_instanceCount--;
	if (s_instanceCount == 0 && s_freeTypeInitialized)
	{
		FT_Done_FreeType(s_freetype);
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

const glm::ivec2& FontRaster::getFontSize() const
{
	return _size;
}

void FontRaster::setFontPath(const std::filesystem::path& fontPath)
{
	_fontPath = fontPath;
}

void FontRaster::setFontSize(const glm::ivec2& size)
{
	_size = size;
}

bool FontRaster::rasterize(wchar_t from, wchar_t to, FontRasterizationResult& result)
{
#ifdef _DEBUG
	assert(s_freeTypeInitialized);
	assert(from < to);
#endif

	const std::string& fontPath = _fontPath.string();
	if (fontPath.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return false;
	}

	FT_Face face;
	if (FT_New_Face(s_freetype, fontPath.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return false;
	}

	FT_Set_Pixel_Sizes(face, _size.x, _size.y);

	const size_t charCount = static_cast<size_t>(to - from);
	auto texture2DArray = rendell::createTexture2DArray(_size.x, _size.y, charCount, rendell::TextureFormat::R);
	std::vector<RasterizedChar> rasterizedChars{};
	rasterizedChars.reserve(charCount);

	for (wchar_t currentChar = from; currentChar < to; currentChar++)
	{
		if (FT_Load_Char(face, currentChar, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph " << currentChar << std::endl;
			continue;
		}

		texture2DArray->setSubTextureData(
			static_cast<uint32_t>(currentChar - from),
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
		rasterizedChars.push_back(std::move(rasterizedChar));
	}

	FT_Done_Face(face);

	result = { texture2DArray, std::move(rasterizedChars)};
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
