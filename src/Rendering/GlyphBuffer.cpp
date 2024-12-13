#include "GlyphBuffer.h"

GlyphBuffer::GlyphBuffer(wchar_t from, wchar_t to, IFontRaster* fontRaster)
{
#ifdef _DEBIG
	assert(from > 0);
	assert(from < to);
#endif
	_range = { from, to };

	if (!fontRaster->rasterize(from, to, _fontRasterizationResult))
	{
		std::cout << "ERROR:TextBatch: Rasterization failure, {" << from << ", " << to << "}" << std::endl;
	}

	_textures.reset(_fontRasterizationResult.texture2DArray);
}

const std::vector<RasterizedChar>& GlyphBuffer::getRasterizedChars() const
{
	return FontRasterizationResult.rasterizedChars;
}

const std::pair<wchar_t, wchar_t>& GlyphBuffer::getRange() const
{
	return _range;
}
