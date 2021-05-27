#include "VertexArray.hpp"
#include "BitmapFont.hpp"
#include "GameData.hpp"

namespace Omnia
{
	void VertexArray::create(sf::PrimitiveType pt)
	{
		setID(VertexArray::s_nextID++);
		m_primitiveType = pt;
		m_renderStates = sf::RenderStates::Default;
		m_autoClear = true;
		m_adjToCam = true;
		m_drawIndex = AUTO__LAYER_RENDER_INDEX;
		m_texture = nullptr;
	}

	void VertexArray::addVertex(FPoint position, Color color, FPoint texCoords)
	{
		if (isInvalid())
			return;
		sf::Vertex vertex;
		if (m_adjToCam)
		{
			Camera& camera = GameData::instance().camera;
			vertex.position.x = camera.convertX(position.x);
			vertex.position.y = camera.convertY(position.y);
		}
		else
		{
			vertex.position.x = position.x;
			vertex.position.y = position.y;
		}
		vertex.color = color.sf();
		if (texCoords.x != -1)
			vertex.texCoords = sf::Vector2f(texCoords.x, texCoords.y);
		m_vertices.push_back(vertex);
	}

	void VertexArray::addQuad(FRect bounds, Color color, TexInfo texInfo)
	{
		if (isInvalid())
			return;
		addVertex(bounds.pair1(), color, texInfo.texCoords);
		addVertex(FPoint(bounds.x + bounds.w, bounds.y), color, FPoint(texInfo.texCoords.x + texInfo.texSize.x, texInfo.texCoords.y));
		addVertex(FPoint(bounds.x + bounds.w, bounds.y + bounds.h), color, FPoint(texInfo.texCoords.x + texInfo.texSize.x, texInfo.texCoords.y + texInfo.texSize.y));
		addVertex(FPoint(bounds.x, bounds.y + bounds.h), color, FPoint(texInfo.texCoords.x, texInfo.texCoords.y + texInfo.texSize.y));
	}

	void VertexArray::addQuad(FRect bounds, TexInfo texInfo)
	{
		if (isInvalid())
			return;
		addQuad(bounds, texInfo.tintColor, texInfo);
	}

	void VertexArray::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (isInvalid())
			return;
		target.draw(&m_vertices[0], m_vertices.size(), m_primitiveType, m_renderStates);
	}

	BitmapFont& VertexArray::getBitmapFont(void)
	{
		if (m_bitmapFont != nullptr && m_bitmapFont->isValid())
			return *m_bitmapFont;
		return INVALID_REF(BitmapFont);
	}

	void VertexArray::setBitmapFont(BitmapFont& font)
	{
		setTexture(font.getTexture());
		m_bitmapFont = &font;
	}
}
