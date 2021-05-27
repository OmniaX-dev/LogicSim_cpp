#include "Fonts.hpp"
#include "GameData.hpp"

namespace Omnia
{
	namespace ttf
	{
		int32 Fonts::addFont(String path, String name)
		{
			if (getFontCount() < AUTO__MAX_FONTS)
			{
				Font font;
				font.sf.loadFromFile(path.cpp());
				font.name = name;
				font.setID(s_nextID++);
				m_fontList.push_back(font);
				return font.getID();
			}
			GameData::instance().errorQueue.push(Error::MAX_FONTS_REACHED, ERROR_DATA());
			return -1;
		}

		int32 Fonts::addFont(Font font)
		{
			if (getFontCount() < AUTO__MAX_FONTS)
			{
				m_fontList.push_back(font);
				return font.getID();
			}
			GameData::instance().errorQueue.push(Error::MAX_FONTS_REACHED, ERROR_DATA());
			return -1;
		}

		Font& Fonts::getFont(int32 id)
		{
			for (auto& f : m_fontList)
			{
				if (f.getID() == id)
					return f;
			}
			GameData::instance().errorQueue.push(Error::FONT_NOT_FOUND, ERROR_DATA());
			return INVALID_REF(Font);
		}

		Font& Fonts::getFont(String name)
		{
			for (auto& f : m_fontList)
			{
				if (f.name.trim() == name.trim())
					return f;
			}
			GameData::instance().errorQueue.push(Error::FONT_NOT_FOUND, ERROR_DATA());
			return INVALID_REF(Font);
		}
	}
}
