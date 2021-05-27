#include "Renderer.hpp"
#include <math.h>
#include "GameData.hpp"

namespace Omnia
{
	bool Renderer::renderTargetCheck(bool error)
	{
		bool isNull = m_renderTarget == nullptr;
		if (error && isNull)
			GameData::instance().errorQueue.push(Error::NULL_RENDER_TARGET, ERROR_DATA());
		return !isNull;
	}

	void Renderer::drawVertexArray(VertexArray& varr)
	{
		if (!renderTargetCheck())
			return;
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(varr);
	}

	void Renderer::fillRect(FRect bounds, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::RectangleShape rect(sf::Vector2f(bounds.w, bounds.h));
		rect.setPosition(sf::Vector2f(bounds.x, bounds.y));
		rect.setFillColor(color.sf());

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(rect);
	}

	void Renderer::drawRect(FRect bounds, float thickness, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::RectangleShape rect(sf::Vector2f(bounds.w, bounds.h));
		rect.setPosition(sf::Vector2f(bounds.x, bounds.y));
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(color.sf());
		rect.setOutlineThickness(thickness);

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(rect);
	}

	void Renderer::outlineRect(FRect bounds, float outline_thickness, Color outline_color, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::RectangleShape rect(sf::Vector2f(bounds.w, bounds.h));
		rect.setPosition(sf::Vector2f(bounds.x, bounds.y));
		rect.setFillColor(color.sf());
		rect.setOutlineColor(outline_color.sf());
		rect.setOutlineThickness(outline_thickness);

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(rect);
	}

	void Renderer::drawLine(FRect bounds, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(bounds.x, bounds.y)),
			sf::Vertex(sf::Vector2f(bounds.w, bounds.h))
		};
		line[0].color = color.sf();
		line[1].color = color.sf();
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(line, 2, sf::Lines);
	}

	void Renderer::fillLine(FRect bounds, float thickness, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::Vertex vertices[4];

		sf::Vector2f point1 = sf::Vector2f(bounds.x, bounds.y);
		sf::Vector2f point2 = sf::Vector2f(bounds.w, bounds.h);

		sf::Vector2f direction = point1 - point2;
		sf::Vector2f unitDirection = direction / (float)sqrt(direction.x * direction.x + direction.y * direction.y);
		sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

		sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

		vertices[0].position = point1 + offset;
		vertices[1].position = point2 + offset;
		vertices[2].position = point2 - offset;
		vertices[3].position = point1 - offset;

		for (int i=0; i<4; ++i)
			vertices[i].color = color.sf();

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(vertices, 4, sf::Quads);
	}

	void Renderer::fillCircle(FRect bounds, Color color)
	{
		fillCircle(FPoint(bounds.x, bounds.y), bounds.w / 2.0f, color);
	}

	void Renderer::fillCircle(FPoint position, float radius, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::CircleShape circle(radius);
		circle.setPointCount(100);
		circle.setPosition(sf::Vector2f(position.x, position.y));
		circle.setFillColor(color.sf());
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(circle);
	}

	void Renderer::drawCircle(FRect bounds, float thickness, Color color)
	{
		drawCircle(FPoint(bounds.x, bounds.y), bounds.w / 2.0f, thickness, color);
	}

	void Renderer::drawCircle(FPoint position, float radius, float thickness, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::CircleShape circle(radius);
		circle.setPointCount(100);
		circle.setPosition(sf::Vector2f(position.x, position.y));
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(color.sf());
		circle.setOutlineThickness(thickness);
		#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(circle);
	}

	void Renderer::fillTriangle(Triangle vertices, Color color)
	{
		if (!renderTargetCheck())
			return;

		sf::VertexArray triangle{ sf::PrimitiveType::Triangles, 3 };
		triangle[0].position = { vertices.A.x, vertices.A.y };
		triangle[0].color = color.sf();
		triangle[1].position = { vertices.B.x, vertices.B.y };
		triangle[1].color = color.sf();
		triangle[2].position = { vertices.C.x, vertices.C.y };
		triangle[2].color = color.sf();
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(triangle);
	}

	void Renderer::drawTriangle(Triangle vertices, float thickness, Color color)
	{
		fillLine(FRect(vertices.A.x, vertices.A.y, vertices.B.x, vertices.B.y), thickness, color);
		fillLine(FRect(vertices.B.x, vertices.B.y, vertices.C.x, vertices.C.y), thickness, color);
		fillLine(FRect(vertices.C.x, vertices.C.y, vertices.A.x, vertices.A.y), thickness, color);
	}

	void Renderer::fillEllipse(FPoint position, FPoint radius, Color color)
	{
		if (!renderTargetCheck())
			return;

		float pc = 100;

		sf::ConvexShape ellipse;
		ellipse.setPointCount(pc);
		ellipse.setPosition(sf::Vector2f(position.x, position.y));
		ellipse.setFillColor(color.sf());

		for(unsigned short i = 0; i < pc; ++i)
		{
		    float rad = (360 / pc * i) / (360 / PI / 2);
		    float x = cos(rad) * radius.x;
		    float y = sin(rad) * radius.y;
		    ellipse.setPoint(i, sf::Vector2f(x, y));
		}
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(ellipse);
	}

	void Renderer::drawText(String text, FPoint position, Color color, int font_size, sf::Font& font)
	{
		if (!renderTargetCheck())
			return;

		sf::Text txt;
		txt.setFont(font);
		txt.setCharacterSize(font_size);
		txt.setFillColor(color.sf());
		txt.setString(text.cpp());
		txt.setPosition(position.x, position.y);

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(txt);
	}

	void Renderer::drawCenteredText(String text, FRect bounds, Color color, int font_size, sf::Font& font)
	{
		if (!renderTargetCheck())
			return;

		sf::Text txt;
		txt.setFont(font);
		txt.setCharacterSize(font_size);
		txt.setFillColor(color.sf());
		txt.setString(text.cpp());
		int xx = bounds.x + ((bounds.w / 2) - (txt.getGlobalBounds().width / 2));
		txt.setPosition(xx, bounds.y);

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(txt);
	}

	void Renderer::drawImage(Image& img, FPoint position, TexInfo texInfo)
	{
		if (!renderTargetCheck())
			return;

		sf::Sprite spr = img.sfSprite();
		if (texInfo.texCoords.x != -1)
			spr.setTextureRect(sf::IntRect(texInfo.texCoords.x, texInfo.texCoords.y, texInfo.texSize.x, texInfo.texSize.y));
		spr.setColor(texInfo.tintColor.sf());
		spr.setPosition(position.x, position.y);
	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		GameData::instance().timing.drawCalls++;
	#endif
	#endif
		m_renderTarget->draw(spr);
	}
}
