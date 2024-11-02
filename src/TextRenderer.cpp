#include "TextRenderer.h"
#include <fstream>

#define TEXTURE_ARRAY_BLOCK 0

#define UNIFORM_BUFFER_BINDING 0
#define TEXT_BUFFER_BINDING 1
#define GLYPH_SIZE_BUFFER_BINDING 2
#define GLYPH_BEARING_BUFFER_BINDING 3
#define GLYPH_ADVANCE_BUFFER_BINDING 4

static const char* s_vertShaderFilePath = "../res/Shaders/TextRenderer.vs";
static const char* s_fragShaderFilePath = "../res/Shaders/TextRenderer.fs";

static std::unique_ptr<rendell::VertexArray> s_vertexArray;
static std::unique_ptr<rendell::ShaderProgram> s_shaderProgram;
static bool s_initialized = false;

static rendell::VertexArray* createVertexArray()
{
	static std::vector<float> vertexPos{
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	rendell::VertexBuffer* vertexBuffer = rendell::createVertexBuffer(vertexPos);
	vertexBuffer->setLayouts({ { rendell::ShaderDataType::float2, false, 0 } });

	rendell::VertexArray* vertexArray = rendell::createVertexArray();
	vertexArray->addVertexBuffer(vertexBuffer);

	return vertexArray;
}

static rendell::ShaderProgram* createShaderProgram(std::string&& vertexSrc, std::string&& fragmentSrc)
{
	rendell::ShaderProgram* program = rendell::createShaderProgram(std::move(vertexSrc), std::move(fragmentSrc));

	if (std::string vertInfoLog, fragInfoLog; !program->compile(&vertInfoLog, &fragInfoLog))
	{
		std::cout << "ERROR:TextRenderer: Shader compilation failure:\n" << vertInfoLog << fragInfoLog << std::endl;
		return nullptr;
	}
	if (std::string infoLog; !program->link(&infoLog))
	{
		std::cout << "ERROR:TextRenderer: Shader linking failure:\n" << infoLog << std::endl;
		return nullptr;
	}
	return program;
}

static bool loadShaders(std::string& vertSrcResult, std::string& fragSrcResult)
{
	std::fstream vertStream(s_vertShaderFilePath);
	if (!vertStream.is_open())
	{
		std::cout << "ERROR:TextRenderer: Failed to open file " << s_vertShaderFilePath << std::endl;
		return false;
	}

	std::fstream fragStream(s_fragShaderFilePath);
	if (!fragStream.is_open())
	{
		std::cout << "ERROR:TextRenderer: Failed to open file " << s_fragShaderFilePath << std::endl;
		return false;
	}

	std::string vertexSrc((std::istreambuf_iterator<char>(vertStream)), std::istreambuf_iterator<char>());
	std::string fragmentSrc((std::istreambuf_iterator<char>(fragStream)), std::istreambuf_iterator<char>());

	vertSrcResult = std::move(vertexSrc);
	fragSrcResult = std::move(fragmentSrc);

	return true;
}

static bool initStaticRendererStuff()
{
	s_vertexArray.reset(createVertexArray());

	std::string vertexSrc, fragmentSrc;
	if (!loadShaders(vertexSrc, fragmentSrc))
	{
		std::cout << "ERROR:TextRenderer: Shader loading failed" << std::endl;
		return false;
	}

	s_shaderProgram.reset(createShaderProgram(std::move(vertexSrc), std::move(fragmentSrc)));

	if (s_vertexArray != nullptr && s_shaderProgram != nullptr)
	{
		s_shaderProgram->setUniformBufferBinding("uniforms", UNIFORM_BUFFER_BINDING);
		s_shaderProgram->setShaderBufferBinding("textBuffer", TEXT_BUFFER_BINDING);
		s_shaderProgram->setShaderBufferBinding("glyphSizeBuffer", GLYPH_SIZE_BUFFER_BINDING);
		s_shaderProgram->setShaderBufferBinding("glyphBearingBuffer", GLYPH_BEARING_BUFFER_BINDING);
		s_shaderProgram->setShaderBufferBinding("glyphAdvanceBuffer", GLYPH_ADVANCE_BUFFER_BINDING);
		return true;
	}

	return false;
}

TextRenderer::TextRenderer(const FontRasterizationResult& fontRasterizationResult,
	uint32_t viewportWidth, uint32_t viewportHeight)
{
	_fontRasterizationResult = fontRasterizationResult;
	init();
}

TextRenderer::~TextRenderer()
{
	delete _fontRasterizationResult.texture2DArray;
	delete _fontRasterizationResult.rasterizedChars;
}

bool TextRenderer::isInitialized() const
{
	return _initialized;
}

void TextRenderer::setMatrix(const glm::mat4& matrix)
{
	const uint32_t index = s_shaderProgram->getUniformIndex("u_Matrix");
	s_shaderProgram->setUniformMat4(index, reinterpret_cast<const float*>(&matrix));
}

void TextRenderer::setFontSize(const glm::vec2& fontSize)
{
	_fontSize = fontSize;
	s_shaderProgram->setUniformFloat2(_fontSizeUniformIndex, _fontSize.x, _fontSize.y);
}

void TextRenderer::setColor(const glm::vec4& color)
{
	_color = color;
	s_shaderProgram->setUniformFloat4(_colorUniformIndex, _color.r, _color.g, _color.b, color.a);
}

const glm::vec4& TextRenderer::getColor() const
{
	return _color;
}

void TextRenderer::draw(const std::wstring& text)
{
	const size_t textLength = text.length();
	if (textLength == 0)
	{
		return;
	}

	bindBuffers();

	const wchar_t* textData = text.data();
	const uint32_t batchLength = textLength / _bufferLength;

	if (batchLength == 0)
	{
		renderBatch(textData, textLength);
	}
	else
	{
		for (uint32_t i = 0; i < batchLength; i++)
		{
			renderBatch(textData + i * _bufferLength, _bufferLength);
		}
		const uint32_t remainingLength = textLength - _bufferLength * batchLength;
		renderBatch(textData + batchLength * _bufferLength, remainingLength);
	}

	unbindBuffers();
}

bool TextRenderer::init()
{
	if (!_initialized)
	{
		_initialized = initStaticRendererStuff();
	}

	if (!_initialized)
	{
		return false;
	}

	_fontSizeUniformIndex = s_shaderProgram->getUniformIndex("u_FontSize");
	_colorUniformIndex = s_shaderProgram->getUniformIndex("u_TextColor");
	_textureArrayUniformIndex = s_shaderProgram->getUniformIndex("u_Textures");

	s_shaderProgram->setUniformInt1(_textureArrayUniformIndex, TEXTURE_ARRAY_BLOCK);

	const size_t charCount = _fontRasterizationResult.rasterizedChars->size();

	std::vector<glm::vec2> glyphSizeData;
	std::vector<glm::vec2> glyphBearingData;
	std::vector<uint32_t> glyphAdvanceData;

	glyphSizeData.reserve(charCount);
	glyphBearingData.reserve(charCount);
	glyphAdvanceData.reserve(charCount);

	for (const RasterizedChar& asterizedChar : *_fontRasterizationResult.rasterizedChars)
	{
		glyphSizeData.push_back(asterizedChar.glyphSize);
		glyphBearingData.push_back(asterizedChar.glyphBearing);
		glyphAdvanceData.push_back(asterizedChar.glyphAdvance);
	}

	// This buffer changes during runtime.
	_textBuffer.reset(rendell::createShaderBuffer(
		std::vector<wchar_t>(_bufferLength, -1).data(),
		_bufferLength
	));

	// These buffers don't change during runtime.
	_glyphSizeBuffer.reset(rendell::createShaderBuffer(
		static_cast<const void*>(glyphSizeData.data()),
		glyphSizeData.size() * sizeof(glm::vec2)
	));
	_glyphBearingBuffer.reset(rendell::createShaderBuffer(
		static_cast<const void*>(glyphBearingData.data()),
		glyphBearingData.size() * sizeof(glm::vec2)
	));
	_glyphAdvanceBuffer.reset(rendell::createShaderBuffer(
		static_cast<const void*>(glyphAdvanceData.data()),
		glyphAdvanceData.size() * sizeof(uint32_t)
	));

	return _initialized;
}

void TextRenderer::bindBuffers()
{
	s_shaderProgram->bind();
	s_vertexArray->bind();
	_textBuffer->bind(TEXT_BUFFER_BINDING);
	_glyphSizeBuffer->bind(GLYPH_SIZE_BUFFER_BINDING);
	_glyphBearingBuffer->bind(GLYPH_BEARING_BUFFER_BINDING);
	_glyphAdvanceBuffer->bind(GLYPH_ADVANCE_BUFFER_BINDING);
	_fontRasterizationResult.texture2DArray->bind(TEXTURE_ARRAY_BLOCK);
}

void TextRenderer::unbindBuffers()
{
	s_shaderProgram->unbind();
	s_vertexArray->unbind();
	_textBuffer->unbind();
	_glyphSizeBuffer->unbind();
	_glyphBearingBuffer->unbind();
	_glyphAdvanceBuffer->unbind();
	_fontRasterizationResult.texture2DArray->unbind();
}

void TextRenderer::renderBatch(const wchar_t* data, uint32_t count)
{
	if (count > 0)
	{
		const std::vector<uint32_t>& convertedData = convertWCharToUint32(data, count);

		_textBuffer->setSubData(
			static_cast<const void*>(convertedData.data()),
			count * sizeof(uint32_t)
		);
		rendell::drawTriangleStripArraysInstanced(0, 4, count);
	}
}

const std::vector<uint32_t>& TextRenderer::convertWCharToUint32(const wchar_t* data, uint32_t count)
{
	if (_s_textConvertedBuffer.size() < count)
	{
		_s_textConvertedBuffer.resize(count);
	}
	for (int i = 0; i < count; i++) {
		_s_textConvertedBuffer[i] = data[i];
	}
	return _s_textConvertedBuffer;
}

std::vector<uint32_t> TextRenderer::_s_textConvertedBuffer{};
