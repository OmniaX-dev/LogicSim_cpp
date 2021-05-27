#include "BitmapFont.hpp"
#include "GameData.hpp"

namespace Omnia
{
	void BitmapFont::load(String path, UI8Point charSize, I8Point padding)
	{
		if (path.trim() == "")
		{
			GameData::instance().errorQueue.push(Error::INVALID_IMAGE_PATH, ERROR_DATA());
			invalidate();
			return;
		}
		m_texture.load(path);
		m_path = path;
		m_characterSize = charSize;
		m_textureSize = m_texture.getSize();
		setPadding(padding);
		setValid(true);
		setID(BitmapFont::s_nextID++);
		setName("Unnamed Font");
		m_characterCount = UI8Point(m_textureSize.x / m_characterSize.x, m_textureSize.y / m_characterSize.y);
	}

	void BitmapFont::setPadding(I8Point padding)
	{
		m_padding = padding;
	}

	TexInfo BitmapFont::getChar(char c, Color tintColor)
	{
		TexInfo texInfo;
		texInfo.tintColor = tintColor;
		IPoint coords = CONVERT_1D_2D(c, m_characterCount.x);
		coords.x *= m_characterSize.x;
		coords.y *= m_characterSize.y;
		texInfo.texCoords = FPoint(coords.x, coords.y);
		texInfo.texSize = FPoint(m_characterSize.x, m_characterSize.y);
		return texInfo;
	}

	UIPoint BitmapFont::getStringBounds(String str, uint16 charHeight)
	{
		if (str.trim().length() == 0)
			return UIPoint(0, 0);
		UI8Point chSize = getCharacterSize();
		if (charHeight != AUTO__LAYER_RENDER_STRING_FONT_HEIGHT)
			chSize = UI8Point(PROPORTION(chSize.x, charHeight, chSize.y), charHeight);
		I8Point pad = getPadding();
		UIPoint strSize((chSize.x * str.length() + pad.x * str.length()), chSize.y); //TODO: Hardcoded - (chSize.x) due to bug F0001
		return strSize;
	}
}
