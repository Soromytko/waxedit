#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <rendell/rendell.h>
#include "../IFontRaster.h"

class TextBuffer
{
public:
	TextBuffer(size_t length);

	void beginUpdating();
	bool appendCharacter(const RasterizedChar& rasterizedChar, glm::vec2 offset);
	void insertCharacter(const RasterizedChar &rasterizedChar, glm::vec2 offset, size_t index);
	void endUpdating();
	void updateBufferSubData(size_t from, size_t to);

	void bind(uint32_t textBufferBinding, uint32_t transformBufferBinding) const;
	void unbind() const;

	size_t getLength() const;
	size_t getCurrentLength() const;

private:
	struct Buffer
	{
		std::vector<uint32_t> _textBufferData{};
		std::vector<glm::vec4> _transformBufferData{};
		std::unique_ptr<rendell::ShaderBuffer> _textBuffer{};
		std::unique_ptr<rendell::ShaderBuffer> _transformBuffer{};
	};

	size_t _length{};
	size_t _counter{};

	std::vector<Buffer> _buffers;

	std::vector<uint32_t> _textBufferData{};
	std::vector<glm::vec4> _transformBufferData{};

	std::unique_ptr<rendell::ShaderBuffer> _textBuffer{};
	std::unique_ptr<rendell::ShaderBuffer> _transformBuffer{};
};
