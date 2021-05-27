#ifndef __WORLD__HPP__
#define __WORLD__HPP__

#include "LayerRenderer.hpp"
#include "EventListener.hpp"
#include "Sprite.hpp"

namespace Omnia
{
	class MapDataFrame
	{
		public:
			IPoint camPos;
			int32 mapID;
			inline MapDataFrame(void) { mapID = -1; }
	};

	class Tooltip;
	class TilesetFile;
	class Map : public Object, public EventListener
	{
		public:
			inline Map(void) : m_gdata(nullptr)
			{
			#ifdef __DEBUG__
				m_dbgIndex = -1;
			#endif
				invalidate();
			}
			inline virtual ~Map(void) {  }
			void loadFromFile(String filePath, GameData& gdata, UI16Point size, String name, TilesetFile& tset);
			Sprite& at(uint16 x, uint16 y);
			inline Sprite& at(UI16Point coords) { return at(coords.x, coords.y); }
			inline String getName(void) { return m_name; }
			PixelRect getPixelRect(UIRect rect);
			bool setTile(uint16 x, uint16 y, int32 id);
			void render(GameData& gdata, Layer& layer);
			void update(GameData& gdata);
			virtual void mouseMoved(sf::Event& evt, GameData& gdata);
			inline UI16Point getSize(void) { return m_size; }
			inline IPoint getTileSize(void) { return m_tileSize; }

		private:
			String m_filePath;
			String m_name;
			std::vector<Sprite> m_map;
			Image m_img;
			GameData* m_gdata;
			UI16Point m_size;
			IPoint m_tileSize;
			TilesetFile* m_tsetFile;
		#ifdef __DEBUG__
			uint16 m_dbgIndex;
		#endif
		private:
			inline static int32 s_nextID = 0;

			friend class World;
	};

	class Application;
	class WorldFile;
	class Entity;
	class World : public Object, public EventListener
	{
		public:
			inline World(void) { invalidate(); }
			inline World(String wfPath, Application& parent) { create(wfPath, parent); }
			virtual ~World(void);
			void create(String wfPath, Application& parent);
			eCollisionLevel entityCollides(Entity& entity, eDirection direction, eCollisionType ect = eCollisionType::AABB, FPoint* margin = nullptr);
			eCollisionLevel entityWorldBoundCollision(Entity& entity, FPoint* margin = nullptr);
			inline Map& getCurrentMap(void) { return (m_currentMap != nullptr ? *m_currentMap : INVALID_REF(Map)); }
			Map& getMap(String name);
			Map& getMap(int32 id);
			inline bool setCurrentMap(int32 id) { return setCurrentMap(getMap(id)); }
			inline bool setCurrentMap(String name) { return setCurrentMap(getMap(name)); }
			bool setCurrentMap(Map& map);
			void render(GameData& gdata, Layer& layer);
			void update(GameData& gdata);
			virtual void onEvent(sf::Event& evt, GameData& gdata);
			TilesetFile& getCurrentTileset(void) { return (m_currentMap == nullptr ? INVALID_REF(TilesetFile) : *(m_currentMap->m_tsetFile)); }
			WorldFile& getWorldFile(void) { return (m_worldFile == nullptr ? INVALID_REF(WorldFile) : * m_worldFile); }

		private:
			WorldFile* m_worldFile;
			Map* m_currentMap;
			Application* m_parent;
	};
}

#endif
