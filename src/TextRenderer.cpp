#include "TextRenderer.h"
#include <fstream>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

#define CHAR_RANGE_SIZE 200

#define TEXTURE_ARRAY_BLOCK 0

#define UNIFORM_BUFFER_BINDING 0
#define TEXT_BUFFER_BINDING 1
#define GLYPH_TRANSFORM_BUFFER_BINDING 2

static const char* s_vertShaderFilePath = "../res/Shaders/TextRenderer.vs";
static const char* s_fragShaderFilePath = "../res/Shaders/TextRenderer.fs";

static std::unique_ptr<rendell::VertexArray> s_vertexArray;
static std::unique_ptr<rendell::ShaderProgram> s_shaderProgram;
static uint32_t s_matrixUniformIndex{};
static uint32_t s_fontSizeUniformIndex{};
static uint32_t s_colorUniformIndex{};
static uint32_t s_charFromUniformIndex{};
static uint32_t s_textureArrayUniformIndex{};
static uint32_t s_instanceCount{};
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
		s_shaderProgram->setShaderBufferBinding("textBuffer", TEXT_BUFFER_BINDING);
		s_shaderProgram->setShaderBufferBinding("glyphTransformBuffer", GLYPH_TRANSFORM_BUFFER_BINDING);

		s_matrixUniformIndex = s_shaderProgram->getUniformIndex("u_Matrix");
		s_fontSizeUniformIndex = s_shaderProgram->getUniformIndex("u_FontSize");
		s_colorUniformIndex = s_shaderProgram->getUniformIndex("u_TextColor");
		s_charFromUniformIndex = s_shaderProgram->getUniformIndex("u_CharFrom");
		s_textureArrayUniformIndex = s_shaderProgram->getUniformIndex("u_Textures");

		s_shaderProgram->setUniformInt1(s_textureArrayUniformIndex, TEXTURE_ARRAY_BLOCK);

		return true;
	}

	return false;
}

static void releaseStaticRendererStuff()
{
	s_vertexArray.reset(nullptr);
	s_shaderProgram.reset(nullptr);
	s_initialized = false;
}

TextRenderer::TextRenderer(IFontRaster* fontRaster)
{
	s_instanceCount++;
	_fontRaster.reset(fontRaster);
	init();
}

TextRenderer::~TextRenderer()
{
	s_instanceCount--;
	if (s_instanceCount == 0)
	{
		releaseStaticRendererStuff();
	}
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
	_matrix = matrix;
}

void TextRenderer::setFontSize(const glm::vec2& fontSize)
{
	_fontSize = fontSize;
}

void TextRenderer::setColor(const glm::vec4& color)
{
	_color = color;
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

	beginDrawing();

	for (const TextBatch* textBatch : _textBatchesForRendering)
	{
		textBatch->glyphBuffer->bind(TEXTURE_ARRAY_BLOCK);
		s_shaderProgram->setUniformInt1(s_charFromUniformIndex, textBatch->glyphBuffer->getRange().first);
		for (const TextBuffer* textBuffer : textBatch->textBuffers)
		{
			textBuffer->bind(TEXT_BUFFER_BINDING, GLYPH_TRANSFORM_BUFFER_BINDING);
			rendell::drawTriangleStripArraysInstanced(0, 4, _text.length());
			textBuffer->unbind();
		}
		textBatch->glyphBuffer->unbind();
	}

	endDrawing();
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

	return _initialized;
}

static glm::vec2 getInstanceLocalOffset(const RasterizedChar& rasterizedChar)
{
	const glm::vec2 bearing = rasterizedChar.glyphBearing;
	const glm::vec2 size = rasterizedChar.glyphSize;
	return glm::vec2(bearing.x, bearing.y - size.y);
}

void TextRenderer::updateShaderBuffers()
{
	_textBatchesForRendering.clear();

	glm::vec2 currentOffset(0.0f, 0.0f);
	const size_t length = _text.length();
	for (size_t i = 0; i < length; i++)
	{
		const wchar_t currentCharacter = _text[i];

		if (currentCharacter == '\n')
		{
			currentOffset.x = 0.0f;
			currentOffset.y += _fontSize.y;
			continue;
		}

		TextBatch* textBatch = createTextBatch(currentCharacter);
#ifdef _DEBUG
		assert(textBatch);
#endif
		if (_textBatchesForRendering.find(textBatch) == _textBatchesForRendering.end())
		{
			textBatch->beginUpdating();
			_textBatchesForRendering.insert(textBatch);
		}

		const RasterizedChar& rasterizedChar = textBatch->getRasterizedChar(currentCharacter);

		if (currentCharacter != ' ')
		{
			const glm::vec2 glyphOffset = currentOffset + getInstanceLocalOffset(rasterizedChar);
			textBatch->appendCharacter(currentCharacter, glyphOffset);
		}

		currentOffset.x += (rasterizedChar.glyphAdvance >> 6);
	}

	for (TextBatch* textBatch : _textBatchesForRendering)
	{
		textBatch->endUpdating();
	}
}

void TextRenderer::beginDrawing()
{
	s_shaderProgram->bind();
	s_vertexArray->bind();

	s_shaderProgram->setUniformMat4(s_matrixUniformIndex, reinterpret_cast<const float*>(&_matrix));
	s_shaderProgram->setUniformFloat2(s_fontSizeUniformIndex, _fontSize.x, _fontSize.y);
	s_shaderProgram->setUniformFloat4(s_colorUniformIndex, _color.r, _color.g, _color.b, _color.a);
}

void TextRenderer::endDrawing()
{
	s_shaderProgram->unbind();
	s_vertexArray->unbind();
}

TextBatch* TextRenderer::createTextBatch(wchar_t character)
{
	const uint16_t rangeIndex = static_cast<uint16_t>(character) / CHAR_RANGE_SIZE;
	if (auto it = _textBatches.find(rangeIndex); it != _textBatches.end())
	{
		return it->second.get();
	}

	//TextBatch* textBatch = new TextBatch(
	//	static_cast<wchar_t>(rangeIndex * CHAR_RANGE_SIZE),
	//	static_cast<wchar_t>((rangeIndex + 1) * CHAR_RANGE_SIZE), _fontRaster.get());

	TextBatch* textBatch = new TextBatch;
	_textBatches[rangeIndex] = std::make_unique<TextBatch>(textBatch);

	return textBatch;
}

void TextRenderer::addCharacterToTextBatch(const RasterizedChar& rasterizedChar)
{

}
