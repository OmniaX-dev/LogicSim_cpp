#ifndef __RENDERER__H__
#define __RENDERER__H__

#include <SFML/Graphics.hpp>
#include "Color.hpp"
#include "Fonts.hpp"
#include "VertexArray.hpp"

namespace Omnia
{
	class GameData;
	class Animation;
	class Renderer
	{
		public:
			inline Renderer(void) : m_renderTarget(nullptr) {}
			inline void setRenderTarget(sf::RenderTarget& renderTarget) { m_renderTarget = &renderTarget; }
			inline sf::RenderTarget& getRenderTarget(void) { return *m_renderTarget; }
			void clear(Color color) { m_renderTarget->clear(sf::Color(color.r(), color.g(), color.b(), color.a())); }
			void drawText(String text, FPoint position, Color color, int font_size, sf::Font& font);
			void drawCenteredText(String text, FRect bounds, Color color, int font_size, sf::Font& font);
			void drawImage(Image& img, FPoint position, TexInfo texInfo = TexInfo());
			void drawVertexArray(VertexArray& varr);

			//Primitives
			void fillRect(FRect bounds, Color color);
			void drawRect(FRect bounds, float thickness, Color color);
			void outlineRect(FRect bounds, float outline_thickness, Color outline_color, Color color);
			void drawLine(FRect bounds, Color color);
			void fillLine(FRect bounds, float thickness, Color color);
			void fillCircle(FRect bounds, Color color);
			void fillCircle(FPoint position, float radius, Color color);
			void drawCircle(FRect bounds, float thickness, Color color);
			void drawCircle(FPoint position, float radius, float thickness, Color color);
			void fillEllipse(FPoint position, FPoint radius, Color color);
			void fillTriangle(Triangle vertices, Color color);
			void drawTriangle(Triangle vertices, float thickness, Color color);

		private:
			bool renderTargetCheck(bool error = true);

		private:
			sf::RenderTarget* m_renderTarget;
	};
}
#endif
