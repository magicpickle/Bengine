#include "SpriteBatch.h"

#include <algorithm>

namespace Bengine
{
	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType)
	{
		_sortType = sortType;
		_renderBatches.clear();
		for (int i = 0; i < _glyphs.size(); i++)
		{
			delete _glyphs[i];
		}
		_glyphs.clear();
	}
	void SpriteBatch::end()
	{
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.SetPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.SetUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.SetPosition(destRect.x + +destRect.z, destRect.y);
		newGlyph->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);
		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);

	}

	void SpriteBatch::createRenderBatches()
	{
		std::vector <Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);
		if (_glyphs.empty())
			return;
		 
		int cv = 0; //current vertex
		int offset = 0;

		_renderBatches.emplace_back(0, 6, _glyphs[0]->texture);
		vertices[cv++] = _glyphs[0]->topLeft;
		vertices[cv++] = _glyphs[0]->bottomLeft;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->bottomRight;
		vertices[cv++] = _glyphs[0]->topRight;
		vertices[cv++] = _glyphs[0]->topLeft;
		offset += 6;

		for (int cg = 1; cg < _glyphs.size(); cg++)
		{
			if (_glyphs[cg]->texture != _glyphs[cg - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphs[cg]->texture);
			}
			else
			{
				_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = _glyphs[cg]->topLeft;
			vertices[cv++] = _glyphs[cg]->bottomLeft;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->bottomRight;
			vertices[cv++] = _glyphs[cg]->topRight;
			vertices[cv++] = _glyphs[cg]->topLeft;
			offset += 6;
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//Orphan the Buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
		//Upload the buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}


	void SpriteBatch::createVertexArray()
	{
		if (_vao == 0)
		{
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);
		if (_vbo == 0)
		{
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the position attrib pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//This is the color attrib pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attrib pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}
	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType)
		{
		case Bengine::GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
			break;
		case Bengine::GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
			break;
		case Bengine::GlyphSortType::TEXTURE:
			std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
			break;
		default:
			break;
		}
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);

	}


}