#ifndef __FONTS__H__
#define __FONTS__H__

#include <SFML/Graphics.hpp>
#include "DataTypes.hpp"
#include "String.hpp"
#include "Object.hpp"
#include <vector>

namespace Omnia
{
	namespace ttf
	{
		class Font : public Object
		{
			public:
				inline Font(void) { setID(-1); name = ""; }
				sf::Font sf;
				String name;
				friend class Fonts;
		};
		class Fonts
		{
			public:
				static int32 addFont(String path, String name);
				static int32 addFont(Font font);
				static Font& getFont(int32 id);
				static Font& getFont(String name);
				inline static int getFontCount(void) { return m_fontList.size(); }

			private:
				inline static std::vector<Font> m_fontList;
				inline static int32 s_nextID = 0;
		};
	}
}

#endif
