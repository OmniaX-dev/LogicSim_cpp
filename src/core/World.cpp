#include "World.hpp"
#include "Sprite.hpp"
#include "DataFile.hpp"
#include "GameData.hpp"
#include "ErrorManager.hpp"
#include "Camera.hpp"
#include "Entity.hpp"

namespace Omnia
{
	void Map::mouseMoved(sf::Event& evt, GameData& gdata)
	{
	#ifdef __DEBUG__
	#ifdef __CREATE_DEFAULT_DBG_VIEW__
		Camera& camera = gdata.camera;
		IPoint mc(getMouseX() + camera.getHorizontal(), getMouseY() + camera.getVertical());
		IPoint st(mc.x / m_tileSize.x, mc.y / m_tileSize.y);
		uint32 i = CONVERT_2D_1D(st.x, st.y, m_size.x);
		String c = "[";
		c = c.addInt(st.x); 
		c = c.add(",");
		c = c.addInt(st.y);
		c = c.add("]");
		if (m_dbgIndex != -1)
			gdata.debugView.removeTooltp(m_dbgIndex);
		
		m_dbgIndex = gdata.debugView.attachPtr(m_map[i], c, Color::Gold, 20, true, true);
	#endif
	#endif
	}

	void Map::loadFromFile(String filePath, GameData& gdata, UI16Point size, String name, TilesetFile& tset)
	{
		if (filePath.trim() == "")
		{
			gdata.errorQueue.push(Error::INVALID_MAP_FILE_PATH, ERROR_DATA());
			return;
		}
		m_gdata = &gdata;
		m_filePath = filePath;
		m_img.load(m_filePath);
		m_size = size;	
		m_name = name;
		m_tileSize = tset.getSpriteSize();
		m_tsetFile = &tset;

		IPoint xy;
		uint64 pi = 0;
		Sprite tmp;
		const uint8* pix = m_img.getPixels();
		for (uint64 i = 0; i < m_size.x * m_size.y; i++)
		{
			xy = CONVERT_1D_2D(i, m_size.x);
			pi = i * 4;
			tmp = tset.sprite(Color(pix[pi], pix[pi + 1], pix[pi + 2], pix[pi + 3]));
			tmp.setPosition(xy.x * m_tileSize.x, xy.y * m_tileSize.y);
			tmp.setSize(m_tileSize.x, m_tileSize.y);
			m_map.push_back(tmp);
		}
		setID(Map::s_nextID++);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("load_map: ").add(name).add(" (").add(filePath).add(")").get());
	#endif
		setValid(true);
	}

	PixelRect Map::getPixelRect(UIRect rect)
	{
		PixelRect pr;
		pr.pixels = m_img.getPixels();
		pr.rect = rect;
		pr.pixelSize = m_img.getSize();
		pr.arraySize = pr.pixelSize.x * pr.pixelSize.y * 4;
		return pr;
	}

	bool Map::setTile(uint16 x, uint16 y, int32 id)
	{
		if (x >= m_size.x || y >= m_size.y) return false;
		Sprite s = m_tsetFile->sprite(id);
		if (s.isValid())
		{
			s.setBounds(x * m_tileSize.x, y * m_tileSize.y, m_tileSize.x, m_tileSize.y);
			m_map[CONVERT_2D_1D(x, y, m_size.x)] = s;
		}
		return s.isValid();
	}

	void Map::render(GameData& gdata, Layer& layer)
	{
		if (isInvalid() || layer.isInvalid())
			return;
		Camera& camera = gdata.camera;
		UIPoint st = gdata.getScreenTiles(m_tileSize);
		IPoint ot = camera.getTileOffset(m_tileSize);
		uint32 i = 0;
		for (uint32 x = ZERO(ot.x - 1); x < st.x + ZERO(ot.x - 1); x++)
		{
			for (uint32 y = ZERO(ot.y - 1); y < st.y + ZERO(ot.y - 1); y++)
			{
				i = CONVERT_2D_1D(x, y, m_size.x);
				m_map[i].render(gdata, layer.get());
			}
		}
	}

	void Map::update(GameData& gdata)
	{
		if (isInvalid() || m_tsetFile->isInvalid())
			return;
		Camera& camera = gdata.camera;
		UIPoint st = gdata.getScreenTiles(m_tileSize);
		IPoint ot = camera.getTileOffset(m_tileSize);
		uint32 i = 0;
		for (uint32 x = ZERO(ot.x - 1); x < st.x + ZERO(ot.x - 1); x++)
		{
			for (uint32 y = ZERO(ot.y - 1); y < st.y + ZERO(ot.y - 1); y++)
			{
				i = CONVERT_2D_1D(x, y, m_size.x);
				m_map[i].update(gdata, *m_tsetFile);
			}
		}
	}

	Sprite& Map::at(uint16 x, uint16 y)
	{
		if (x < 0 || y < 0 || x >= m_size.x || y >= m_size.y)
			return INVALID_REF(Sprite);
		uint32 i = CONVERT_2D_1D(x, y, m_size.x);
		return m_map[i];
	}

	World::~World(void)
	{
		delete m_worldFile;
		m_worldFile = NULL;
	}

	//TODO: Set camera max values when setting current map
	void World::create(String wfPath, Application& parent)
	{
		if (wfPath.trim() == "")
			return;
		m_parent = &parent;
		m_currentMap = NULL;
		m_worldFile = new WorldFile(wfPath, GameData::instance(), parent);
		if (m_worldFile->isValid() && m_worldFile->hasFirstMap())
		{
			m_currentMap = &m_worldFile->getFirstMap();
			UI16Point ms = m_currentMap->getSize();
			UI16Point cm(ms.x * m_currentMap->getTileSize().x, ms.y * m_currentMap->getTileSize().y);
			cm.x -= m_currentMap->getTileSize().x * GameData::instance().screenTiles.x;
			cm.y -= m_currentMap->getTileSize().y * GameData::instance().screenTiles.y;
			GameData::instance().camera.setMax(cm);
		}
		setValid(true);
	}

	Map& World::getMap(String name)
	{
		return m_worldFile->map(name);
	}

	Map& World::getMap(int32 id)
	{
		return m_worldFile->map(id);
	}

	bool World::setCurrentMap(Map& map)
	{
		if (map.isInvalid())
			return false;
		m_currentMap = &map;
		return true;
	}
	
	void World::render(GameData& gdata, Layer& layer)
	{
		if (isInvalid() || m_currentMap == NULL || m_currentMap->isInvalid()) return;
		m_currentMap->render(gdata, layer);
	}

	void World::update(GameData& gdata)
	{
		if (isInvalid() || m_currentMap == NULL || m_currentMap->isInvalid()) return;
		m_currentMap->update(gdata);
	}

	void World::onEvent(sf::Event& evt, GameData& gdata)
	{
		if (isInvalid() || m_currentMap == NULL || m_currentMap->isInvalid()) return;
		m_currentMap->handleEvents(evt, gdata);
	}

	eCollisionLevel World::entityWorldBoundCollision(Entity& entity, FPoint* margin)
	{
		if (isInvalid() || m_currentMap == NULL || m_currentMap->isInvalid()) return eCollisionLevel::NoCollision;
		UI16Point cmSize = m_currentMap->getSize();
		UI8Point ts = m_currentMap->m_gdata->tileSize;
		FPoint ep = entity.getPosition();
		FPoint tmp;
		if (margin == nullptr)
			margin = &tmp;
		if (ep.x <= 0)
		{
			margin->x = ep.x;
			margin->y = 0;
			return eCollisionLevel::HardCollision;
		}
		else if (ep.y <= 0)
		{
			margin->x = 0;
			margin->y = ep.y;
			return eCollisionLevel::HardCollision;
		}
		else if (ep.x >= (cmSize.x * ts.x) - entity.getw())
		{
			margin->x = ep.x - ((cmSize.x * ts.x) - entity.getw());
			margin->y = 0;
			return eCollisionLevel::HardCollision;
		}
		else if (ep.y >= (cmSize.y * ts.y) - entity.geth())
		{
			margin->x = 0;
			margin->y = ep.y - ((cmSize.y * ts.y) - entity.geth());
			return eCollisionLevel::HardCollision;
		}
		margin->x = 0;
		margin->y = 0;
		return eCollisionLevel::NoCollision;
	}

	eCollisionLevel World::entityCollides(Entity& entity, eDirection direction, eCollisionType ect, FPoint* margin)
	{
		if (isInvalid() || m_currentMap == nullptr || m_currentMap->isInvalid()) return eCollisionLevel::NoCollision;
		FPoint tmp;
		if (margin == nullptr)
			margin = &tmp;
		if (entityWorldBoundCollision(entity, margin) == eCollisionLevel::HardCollision)
			return eCollisionLevel::HardCollision;
		Map& curMap = *m_currentMap;
		UI16Point mapSize = m_currentMap->getSize();
		UI8Point ts = m_currentMap->m_gdata->tileSize;
		FRect eBounds = entity.getBoundingBox();
		FPoint center = eBounds.getCenter();
		UI16Point eTile(center.x / ts.x, center.y / ts.y);
		Sprite* sprCollide = nullptr;
		std::vector<Sprite*> check;
		if (ect == eCollisionType::AABB)
		{
			switch (direction)
			{
				case eDirection::Left:
					check.push_back(&curMap.at(eTile.x - 1, eTile.y - 1));
					check.push_back(&curMap.at(eTile.x - 1, eTile.y));
					check.push_back(&curMap.at(eTile.x - 1, eTile.y + 1));
				break;
				case eDirection::Right:
					check.push_back(&curMap.at(eTile.x + 1, eTile.y - 1));
					check.push_back(&curMap.at(eTile.x + 1, eTile.y));
					check.push_back(&curMap.at(eTile.x + 1, eTile.y + 1));
				break;
				case eDirection::Up:
					check.push_back(&curMap.at(eTile.x - 1, eTile.y - 1));
					check.push_back(&curMap.at(eTile.x, eTile.y - 1));
					check.push_back(&curMap.at(eTile.x + 1, eTile.y - 1));
				break;
				case eDirection::Down:
					check.push_back(&curMap.at(eTile.x - 1, eTile.y + 1));
					check.push_back(&curMap.at(eTile.x, eTile.y + 1));
					check.push_back(&curMap.at(eTile.x + 1, eTile.y + 1));
				break;
				default:
					margin->x = 0;
					margin->y = 0;
					return eCollisionLevel::NoCollision;
				break;
			}
			for (auto& spr : check)
			{
				if (spr->isInvalid())
					continue;
				if (spr->getBounds().intersects(eBounds) && (spr->getSolidLevel() == eSolidLevel::FullLiquid || spr->getSolidLevel() == eSolidLevel::FullSolid))
				{
					switch (direction)
					{
						case eDirection::Left:
							margin->x = eBounds.x - (spr->getx() + ts.x);
							margin->y = 0;
						break;
						case eDirection::Right:
							margin->x = (eBounds.x + eBounds.w) - spr->getx();
							margin->y = 0;
						break;
						case eDirection::Up:
							margin->x = 0;
							margin->y = eBounds.y - (spr->gety() + ts.y);
						break;
						case eDirection::Down:
							margin->x = 0;
							margin->y = (eBounds.y + eBounds.h) - spr->gety();
						break;
						default:
							margin->x = 0;
							margin->y = 0;
						break;
					}
					sprCollide = spr;
					break;
				}
			}
			if (sprCollide == nullptr)
			{
				margin->x = 0;
				margin->y = 0;
				entity.setSprCollBox(FRect(-1, -1, -1, -1));
				return eCollisionLevel::NoCollision;
			}
		#ifdef __DEBUG__
			if (Flags::isset(FLG_SHOW_COLLISION_RECTS))
				entity.setSprCollBox(sprCollide->getBounds());
		#endif
			switch (sprCollide->getSolidLevel())
			{
				case eSolidLevel::NoSolid:
					return eCollisionLevel::NoCollision;
				case eSolidLevel::MediumSolid:
					return eCollisionLevel::MediumCollision;
				case eSolidLevel::FullSolid:
					return eCollisionLevel::HardCollision;
				case eSolidLevel::MediumLiquid:
					return eCollisionLevel::MediumLiquidCollision;
				case eSolidLevel::FullLiquid:
					return eCollisionLevel::HardLiquidCollision;
				default:
					return eCollisionLevel::NoCollision;
			}
		}
		return eCollisionLevel::NoCollision;
	}
}
