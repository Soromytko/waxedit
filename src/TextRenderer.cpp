#include "TextRenderer.h"
#include <fstream>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#define TEXTURE_ARRAY_BLOCK 0

#define UNIFORM_BUFFER_BINDING 0
#define TEXT_BUFFER_BINDING 1
#define GLYPH_MATRIX_BUFFER_BINDING 2

static const char* s_vertShaderFilePath = "../res/Shaders/TextRenderer.vs";
static const char* s_fragShaderFilePath = "../res/Shaders/TextRenderer.fs";

static const uint32_t sss = 500;

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
		s_shaderProgram->setShaderBufferBinding("glyphMatrixBuffer", GLYPH_MATRIX_BUFFER_BINDING);
		return true;
	}

	return false;
}

TextRenderer::TextRenderer(IFontRaster* fontRaster)
{
	_fontRaster.reset(fontRaster);
	init();
}

TextRenderer::~TextRenderer()
{

}

bool TextRenderer::isInitialized() const
{
	return _initialized;
}

void TextRenderer::setText(const std::wstring& value)
{
	std::wstring text = value;
	setText(std::move(text));
}

void TextRenderer::setText(std::wstring&& value)
{
	_text = std::move(value);
	updateShaderBuffers();
}

void TextRenderer::setMatrix(const glm::mat4& matrix)
{
	const uint32_t index = s_shaderProgram->getUniformIndex("u_Matrix");
	s_shaderProgram->setUniformMat4(index, reinterpret_cast<const float*>(&matrix));
}

void TextRenderer::setFontSize(const glm::vec2& fontSize)
{
	_fontSize = fontSize;
	_textures.reset(rendell::createTexture2DArray(_fontSize.x, _fontSize.y, sss, rendell::TextureFormat::R));
	s_shaderProgram->setUniformFloat2(_fontSizeUniformIndex, _fontSize.x, _fontSize.y);
}

void TextRenderer::setColor(const glm::vec4& color)
{
	_color = color;
	s_shaderProgram->setUniformFloat4(_colorUniformIndex, _color.r, _color.g, _color.b, color.a);
}

const std::wstring& TextRenderer::getText() const
{
	return _text;
}

const glm::vec4& TextRenderer::getColor() const
{
	return _color;
}

void TextRenderer::draw()
{
	if (_text.length() == 0) {
		return;
	}

	bindBuffers();

	rendell::drawTriangleStripArraysInstanced(0, 4, _text.length());

	unbindBuffers();



	/*const size_t textLength = text.length();
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

	unbindBuffers();*/
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

	//initBuffers();

	_fontSizeUniformIndex = s_shaderProgram->getUniformIndex("u_FontSize");
	_colorUniformIndex = s_shaderProgram->getUniformIndex("u_TextColor");
	_textureArrayUniformIndex = s_shaderProgram->getUniformIndex("u_Textures");

	s_shaderProgram->setUniformInt1(_textureArrayUniformIndex, TEXTURE_ARRAY_BLOCK);

	_textBuffer.reset(rendell::createShaderBuffer(nullptr, sss * sizeof(uint32_t)));
	_glyphMatrixBuffer.reset(rendell::createShaderBuffer(nullptr, sss * sizeof(glm::mat4)));

	return _initialized;
}

static glm::vec3 getInstanceLocalOffset(const RasterizedChar& rasterizedChar)
{
	const glm::vec2 bearing = rasterizedChar.glyphBearing;
	const glm::vec2 size = rasterizedChar.glyphSize;
	return glm::vec3(bearing.x, bearing.y - size.y, 0.0f);
}

void TextRenderer::updateShaderBuffers()
{
	const size_t length = _text.length();
	_textBufferData.resize(length);
	_glyphMatrixBufferData.resize(length);

	glm::vec3 currentOffset(0.0f, 0.0f, 0.0f);

	for (size_t i = 0; i < length; i++)
	{
		const wchar_t currectCharacter = _text[i];

		if (currectCharacter == '\n')
		{
			currentOffset.x = 0.0f;
			currentOffset.y += _fontSize.y;
			continue;
		}

		const RasterizedChar& rasterizedChar = getRasterizedChar(currectCharacter);

		if (currectCharacter != ' ')
		{
			addCharToBuffer(rasterizedChar, currentOffset, i);
		}

		currentOffset.x += (rasterizedChar.glyphAdvance >> 6);
	}

	_textBuffer->setSubData(
		static_cast<const void*>(_textBufferData.data()),
		_text.length() * sizeof(uint32_t)
	);
	_glyphMatrixBuffer->setSubData(
		static_cast<const void*>(_glyphMatrixBufferData.data()),
		_text.length() * sizeof(glm::mat4)
	);
}

void TextRenderer::addCharToBuffer(const RasterizedChar& rasterizedChar, const glm::vec3& offset, size_t index)
{
	const glm::vec3 glyphOffset = offset + getInstanceLocalOffset(rasterizedChar);
	const glm::vec3 glyphSize = glm::vec3(rasterizedChar.glyphSize, 0.0f);

	glm::mat4 glyphMatrix(1.0f);
	glyphMatrix = glm::translate(glyphMatrix, glyphOffset);
	glyphMatrix = glm::scale(glyphMatrix, glyphSize);

	_textBufferData[index] = static_cast<uint32_t>(rasterizedChar.character);
	_glyphMatrixBufferData[index] = std::move(glyphMatrix);
}

void TextRenderer::bindBuffers()
{
	s_shaderProgram->bind();
	s_vertexArray->bind();
	_textBuffer->bind(TEXT_BUFFER_BINDING);
	_glyphMatrixBuffer->bind(GLYPH_MATRIX_BUFFER_BINDING);
	_textures->bind(TEXTURE_ARRAY_BLOCK);
}

void TextRenderer::unbindBuffers()
{
	s_shaderProgram->unbind();
	s_vertexArray->unbind();
	_textBuffer->unbind();
	_glyphMatrixBuffer->unbind();
	_textures->unbind();
}

const RasterizedChar& TextRenderer::getRasterizedChar(wchar_t character)
{
	auto it = _rasterizedCharCache.find(character);
	if (it != _rasterizedCharCache.end())
	{
		return it->second;
	}

	RasterizedChar result;
	if (!_fontRaster->rasterize(character, result))
	{
		std::cout << "WARNING:TextRenderer: Rasterization failure " << character << std::endl;
	}

	_rasterizedCharCache[character] = std::move(result);

	return _rasterizedCharCache.find(character)->second;
}

void TextRenderer::renderBatch(const wchar_t* data, uint32_t count)
{
	/*if (count > 0)
	{
		const std::vector<uint32_t>& convertedData = convertWCharToUint32(data, count);

		_textBuffer->setSubData(
			static_cast<const void*>(convertedData.data()),
			count * sizeof(uint32_t)
		);
		rendell::drawTriangleStripArraysInstanced(0, 4, count);
	}*/
}
