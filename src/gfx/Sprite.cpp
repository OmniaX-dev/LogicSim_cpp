#include "Sprite.hpp"
#include "Camera.hpp"
#include "DataFile.hpp"
#include "VertexArray.hpp"
#include "GameData.hpp"
#include <math.h>

namespace Omnia
{
	SpriteUnit::SpriteUnit(void)
	{
		m_code = 0;
		type = 0;
		frames = 0;
		start = 0;
		delay = __TUNIT_DEFAULT_DELAY;
		m_currentTime = 0;
		m_currentFrame = 0;
		m_backwards = false;
	}

	void SpriteUnit::decode(void)
	{
		type = m_code / __TUINT_MASK;
		frames = (int)(m_code / (int)(pow(__TUINT_PARAM_MASK, (__TUNIT_PARAM_COUNT - 2)))) % __TUINT_PARAM_MASK;
		delay =  (int)(m_code / (int)(pow(__TUINT_PARAM_MASK, (__TUNIT_PARAM_COUNT - 3)))) % __TUINT_PARAM_MASK;
		start = m_code % __TUINT_PARAM_MASK;
	}

	uint64 SpriteUnit::encode(void)
	{
		m_code = (type * __TUINT_MASK) + (frames * (int)(pow(__TUINT_PARAM_MASK, (__TUNIT_PARAM_COUNT - 2)))) + (delay * (int)(pow(__TUINT_PARAM_MASK, (__TUNIT_PARAM_COUNT - 3)))) + start;
		return m_code;
	}

	void SpriteUnit::set(uint64 c)
	{
		m_code = c;
		decode();
	}

	void SpriteUnit::set(uint16 t, uint16 f, uint16 d, uint16 s)
	{
		type = t;
		frames = f;
		delay = d;
		start = s;
		encode();
	}

	uint16 SpriteUnit::texValue(void)
	{
		return start + m_currentFrame;
	}

	void SpriteUnit::update(void)
	{
		if (type == (uint16)eTileUnitType::Animation)
		{
			if (m_currentTime++ > delay)
			{
				m_currentFrame += (m_backwards ? -1 : 1);
				m_currentTime = 0;
				if (m_backwards && m_currentFrame <= 0)
					m_backwards = false;
				else if (!m_backwards && m_currentFrame >= frames - 1)
					m_backwards = true;
			}
		}
		else if (type == (uint16)eTileUnitType::RandomAnimation)
		{
			if (m_currentTime++ > delay)
			{
				m_currentTime = RANDOM(0, frames - 1); //TODO: change default max value
				m_currentFrame = RANDOM(0, frames - 1);
			}
		}
	}

	Sprite::Sprite(void) : Drawable(0, 0, 0, 0)
	{
		m_lightEmitter = AUTO__NO_LGT_TMPLT;
		m_name = "Unknown tile";
		m_unit.set(0);
		setID(-1);
		m_solidLevel = eSolidLevel::NoSolid;
		m_shareData = false;
		m_sharedData = -1;
		invalidate();
	}

	Sprite::Sprite(SpriteUnit tu, Color col, TexInfo tex, eSolidLevel s) : Drawable(0, 0, 0, 0)
	{
		m_lightEmitter = AUTO__NO_LGT_TMPLT;
		m_name = "Unknown tile";
		m_shareData = false;
		m_sharedData = -1;
		set(tu, col, tex, s);
	}

	void Sprite::update(GameData& gdata, TilesetFile& tset)
	{
		if (isInvalid())
			return;
		if (!hasSharedData())
			m_unit.update();
		IPoint conv = CONVERT_1D_2D(tileUnit().texValue(), tset.getSpriteCount().x);
		m_texInfo.texCoords = FPoint(conv.x * tset.getSpriteSize().x, conv.y * tset.getSpriteSize().y);
	}

	void Sprite::render(GameData& gdata, VertexArray& layer)
	{
		layer.addQuad(getBounds(), m_texInfo.tintColor, m_texInfo);
	}

	void Sprite::set(SpriteUnit tu, Color col, TexInfo tex, eSolidLevel s)
	{
		setMapColor(col);
		setSolidLevel(s);
		m_unit = tu;
		m_texInfo = tex;
		setSize(m_texInfo.texSize);
		setBoundingBox(getBounds());
		setID(Sprite::s_nextID++);
		setValid(true);
	}

	void Sprite::setTileUnit(uint16 t, uint16 f, uint16 s, uint16 d)
	{
		m_unit.set(t, f, d, s);
	}

	bool Sprite::isOnScreen(Camera& camera)
	{
		if (isInvalid())
			return false;
		return true; //TODO: Implement a real method to return true if tile is on screen
		/*int ctx = camera.getTileHorizontal(), cty = camera.getTileVertical();
		int tx = m_tileX - ctx, ty = m_tileY - cty;
		return (tx >= 0 && tx < __SCREEN_TILES_X && ty >= 0 && ty < __SCREEN_TILES_Y);*/
	}

	String Sprite::toString(void)
	{
		std::ostringstream out;
		out << m_name.trim().cpp() << " - " << m_color.toString().trim().cpp();
		out << "(" << (int)(m_texInfo.texCoords.x) << "," << (int)(m_texInfo.texCoords.y) << ")";
		out << "(" << (int)(m_texInfo.texSize.x) << "," << (int)(m_texInfo.texSize.y) << ")";
		return String(out.str()).trim();
	}
}
