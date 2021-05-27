#ifndef __IMAGE__HPP__
#define __IMAGE__HPP__

#include <SFML/Graphics.hpp>
#include "DataTypes.hpp"
#include "String.hpp"

namespace Omnia
{
	class Image : public Object
	{
		public:
			inline Image(void) { load(); }
			inline Image(String path) { load(path); }
			bool load(String path = "");
			sf::Sprite sfSprite(void);
			inline sf::Texture& sfTexture(void) { return m_tex; }
			inline UIPoint getSize(void) { return UIPoint(m_tex.getSize().x, m_tex.getSize().y); }
			inline String getFilePath(void) { return m_path; }
			const uint8* getPixels(void);

		private:
			sf::Texture m_tex;
			sf::Image m_img;
			String m_path;
	};
}

#endif
