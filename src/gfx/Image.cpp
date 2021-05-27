#include "Image.hpp"
#include "GameData.hpp"

namespace Omnia
{
	bool Image::load(String path)
	{
		if (path.trim() == "")
			return false;
		if (!m_tex.loadFromFile(path.cpp()))
		{
			GameData::instance().errorQueue.push(Error::FAILED_TO_LOAD_IMAGE, ERROR_DATA());
			return false;
		}
		m_path = path;
		m_tex.setRepeated(true);
		m_img = m_tex.copyToImage();
		return true;
	}

	sf::Sprite Image::sfSprite(void)
	{
		sf::Sprite spr;
		spr.setTexture(m_tex);
		return spr;
	}

	const uint8* Image::getPixels(void)
	{
		return m_img.getPixelsPtr();
	}
}
