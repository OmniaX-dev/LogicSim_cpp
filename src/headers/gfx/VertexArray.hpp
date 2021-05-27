#ifndef __VERTEXARRAY__HPP__
#define __VERTEXARRAY__HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "DataTypes.hpp"
#include "Image.hpp"

namespace Omnia
{
	class BitmapFont;
	class VertexArray : public sf::Drawable, public sf::Transformable, public Object
	{
		public:
			inline VertexArray(void) { create(); }
			void create(sf::PrimitiveType pt = sf::Quads);
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			inline void clear(void) { m_vertices.clear(); }

			inline void setPrimitiveType(sf::PrimitiveType pt) { m_primitiveType = pt; }
			inline void setTexture(Image& tex) { m_renderStates.texture = &tex.sfTexture(); m_texture = &tex; }

			inline void addVertex(sf::Vertex vertex) { m_vertices.push_back(vertex); }
			void addVertex(FPoint position, Color color = Color::White, FPoint texCoords = FPoint(-1, -1));
			void addQuad(FRect bounds, Color color = Color::White, TexInfo texInfo = TexInfo());
			void addQuad(FRect bounds, TexInfo texInfo = TexInfo());

			BitmapFont& getBitmapFont(void);
			inline Image& getTexture(void) { return (m_texture != nullptr ? *m_texture : INVALID_REF(Image)); }
			void setBitmapFont(BitmapFont& font);
			inline bool isTextLayer(void) { return m_bitmapFont != nullptr; }
			inline bool isAdjustedToCamera(void) { return m_adjToCam; }
			inline void setAdjustedToCamera(bool adj) { m_adjToCam = adj; }
			inline int32 getDrawIndex(void) { return m_drawIndex; }
			inline void setDrawIndex(int32 drawIndex) { m_drawIndex = drawIndex; }
			inline void swapDrawOrder(VertexArray& va) { int32 tmp = va.m_drawIndex; va.m_drawIndex = m_drawIndex; m_drawIndex = tmp; }
			inline bool isAutoClearEnabled(void) { return m_autoClear; }
			inline void setAutoClear(bool ac) { m_autoClear = ac; }

		private:
			sf::RenderStates m_renderStates;
			sf::PrimitiveType m_primitiveType;
			std::vector<sf::Vertex> m_vertices;
			bool m_autoClear;
			BitmapFont* m_bitmapFont;
			bool m_adjToCam;
			int32 m_drawIndex;
			Image* m_texture;

			inline static int32 s_nextID = 0;

			friend class LayerRenderer;
			friend class GuiComponent;
	};
}

#endif
