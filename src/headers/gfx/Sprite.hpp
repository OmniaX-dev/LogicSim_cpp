#ifndef __SPRITEUNIT__HPP__
#define __SPRITEUNIT__HPP__

#include "Object.hpp"
#include "DataTypes.hpp"
#include "Drawable.hpp"

namespace Omnia
{
	class SpriteUnit
	{
		public:
			SpriteUnit(void);
			void decode(void);
			uint64 encode(void);
			void set(uint64 c);
			void set(uint16 t, uint16 f, uint16 d, uint16 s);
			uint16 texValue(void);
			void update(void);

		public:
			uint16 type;
			uint16 frames;
			uint16 start;
			uint16 delay;

		private:
			uint64 m_code;
			uint16 m_currentFrame;
			uint16 m_currentTime;
			bool m_backwards;
	};

	class SpriteSharedData
	{
		public:
			inline static std::vector<SpriteUnit> data = std::vector<SpriteUnit>();
			inline static uint32 addData(SpriteUnit su) { data.push_back(su); return data.size() - 1; }
			inline static SpriteUnit& get(uint32 index) { return data[index]; }
			inline static void update(void) { for (auto& spu : data) spu.update(); }
	};

	class Camera;
	class TilesetFile;
	class VertexArray;
	class GameData;
	class Sprite : public Object, public Drawable
	{
		public:
			Sprite(void);
			Sprite(SpriteUnit tu, Color col, TexInfo tex, eSolidLevel s = eSolidLevel::NoSolid);
			void set(SpriteUnit tu, Color col, TexInfo tex, eSolidLevel s = eSolidLevel::NoSolid);
			void setTileUnit(uint16 t, uint16 f, uint16 s, uint16 d);
			bool isOnScreen(Camera& camera);
			void update(GameData& gdata, TilesetFile& tset);
			void render(GameData& gdata, VertexArray& layer);
			inline void setMapColor(Color color) { m_color = color; }
			inline void setMapColor(uint8 r, uint8 g, uint8 b, uint8 a = 255) { m_color.setColor(r, g, b, a); }
			inline uint16 getTexValue(void) { return m_unit.texValue(); }
			inline void setName(String name) { m_name = name; }
			inline String getName(void) { return m_name; }
			inline void setTexValue(int tv) { m_unit.set(tv); }
			inline void setSolidLevel(eSolidLevel sl) { m_solidLevel = sl; }
			inline eSolidLevel getSolidLevel(void) { return m_solidLevel; }
			inline void update(void) { m_unit.update(); }
			inline Color& getMapColor(void) { return m_color; }
			inline SpriteUnit& tileUnit(void) { return (hasSharedData() ? SpriteSharedData::get(m_sharedData) : m_unit); }
			inline void setLightEmitter(int8 lightEmitter) { m_lightEmitter = lightEmitter; }
			inline int8 getLightEmitter(void) { return m_lightEmitter; }
			inline void setTexInfo(TexInfo texInfo) { m_texInfo = texInfo; }
			inline TexInfo& getTexInfo(void) { return m_texInfo; }
			virtual String toString(void);
			inline void useSharedData(bool sd) { m_shareData = sd; }
			inline void setSharedData(uint32 index) { m_sharedData = index; }
			inline bool hasSharedData(void) { return m_shareData; }
			inline uint32 getSharedDataIndex(void) { return m_sharedData; }
			inline FRect getBoundingBox(void) { return m_boundingBox; }
			inline void setBoundingBox(FRect bounds) { m_boundingBox = bounds; }

		private:
			SpriteUnit m_unit;
			String m_name;
			Color m_color;
			eSolidLevel m_solidLevel;
			int8 m_lightEmitter;
			TexInfo m_texInfo;
			bool m_shareData;
			uint32 m_sharedData;
			FRect m_boundingBox;

		private:
			inline static int32 s_nextID = 0;
	};
}

#endif
