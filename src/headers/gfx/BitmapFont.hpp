#ifndef __BITMAPFONT__HPP__
#define __BITMAPFONT__HPP__

#include "Image.hpp"
#include "DataTypes.hpp"

namespace Omnia
{
	class BitmapFont : public Object
	{
		public:
			inline BitmapFont(void) { load(); }
			inline BitmapFont(String path) { load(path); }
			inline BitmapFont(String path, UI8Point charSize, I8Point padding = I8Point(0, 0)) { load(path, charSize, padding); }
			void load(String path = "", UI8Point charSize = UI8Point(AUTO__BITMAP_FONT_CHAR_WIDTH, AUTO__BITMAP_FONT_CHAR_HEIGHT), I8Point padding = I8Point(0, 0));
			TexInfo getChar(char c, Color tintColor = Color::White);
			inline Image& getTexture(void) { return m_texture; }
			inline UI8Point getCharacterSize(void) { return m_characterSize; }
			inline I8Point getPadding(void) { return m_padding; }
			void setPadding(I8Point padding);
			inline String getName(void) { return m_name; }
			inline void setName(String name) { m_name = name; }
			UIPoint getStringBounds(String str, uint16 charHeight = AUTO__BITMAP_FONT_CHAR_HEIGHT);

		private:
			Image m_texture;
			String m_path;
			String m_name;
			UI8Point m_characterSize;
			UIPoint m_textureSize;
			UI8Point m_characterCount;
			I8Point m_padding;

		private:
			inline static int32 s_nextID = 0;
	};
}

#endif
