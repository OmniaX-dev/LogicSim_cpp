#include "Animation.hpp"
#include "Image.hpp"
#include "VertexArray.hpp"
#include <iostream>

namespace Omnia
{
	Animation::Animation(void) : Drawable()
	{
		create(AnimationData());
	}

	Animation::Animation(AnimationData ad) : Drawable()
	{
		create(ad);
	}

	void Animation::create(AnimationData ad)
	{
		m_current_time = 0;
		m_current_frame = 0;
		m_back = false;
		m_animData = ad;
		setSize(ad.frameWidth, ad.frameHeight);
		setID(Animation::s_nextID++);
	}

	void Animation::resetAnimation(void)
	{
		m_current_frame = 0;
		m_current_time = 0;
	}

	void Animation::render(GameData& gdata, VertexArray& layer, Color tintColor)
	{
		layer.addQuad(getBounds(), tintColor, m_texInfo);
	}

	void Animation::update(GameData& gdata)
	{
		if (m_current_time++ > m_animData.delay && !m_animData.isStill)
		{
			m_current_time = 0;
			if (m_animData.randomFrames)
			{
				m_current_frame = RANDOM(0, m_animData.frameCount);
			}
			else if (m_animData.playBackwards)
			{
				if (!m_back && m_current_frame >= m_animData.frameCount - 1)
				{
					m_back = true;
					m_current_frame--;
				}
				else if (!m_back)
					m_current_frame++;
				else if (m_back && m_current_frame <= 0)
				{
					m_back = false;
					m_current_frame++;
				}
				else if (m_back)
					m_current_frame--;
			}
			else
			{
				if (m_current_frame++ >= m_animData.frameCount - 1)
					m_current_frame = 0;
			}
		}
		else if (m_animData.isStill)
			m_current_frame = m_animData.stillFrame;
		int x = ((m_current_frame % m_animData.columns) + m_animData.columnOffset) * m_animData.frameWidth;
		int y = m_animData.rowOffset * m_animData.frameHeight;
		if (m_animData.rows > 1)
			y = ((m_current_frame / m_animData.columns) + m_animData.rowOffset) * m_animData.frameHeight;

		m_texInfo.texCoords = FPoint(x + m_animData.generalOffsetX, y + m_animData.generalOffsetY);
		m_texInfo.texSize = FPoint(m_animData.frameWidth, m_animData.frameHeight);
	}
}
