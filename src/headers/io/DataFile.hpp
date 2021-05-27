#ifndef __DATAFILE__HPP__
#define __DATAFILE__HPP__

#include "Object.hpp"
#include "String.hpp"
#include "DataTypes.hpp"
#include "Sprite.hpp"
#include "Image.hpp"
#include "Animation.hpp"
#include "Fonts.hpp"
#include "World.hpp"
#include <vector>

namespace Omnia
{
	class Application;
	class GameData;
	class DataFile : public Object
	{
		public:
			inline DataFile(void) { invalidate(); }
			inline DataFile(String filePath, GameData& gdata, Application& app) { load(filePath, gdata, app); }
			inline virtual ~DataFile(void) = default;
			inline virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount) {  }
			inline virtual void onCustomAction(String action, DFD_LIST params) {  }
			void load(String filePath, GameData& gdata, Application& app);
			void addLocal(String name, int value);
			void addLocal(String name, String value, bool literal = false);
			const DataBlock getLocal(String name);
			static void addGlobal(String name, int value);
			static void addGlobal(String name, String value, bool literal = false);
			static DataBlock getGlobal(String name);

		protected:
			void loadDefines(String data);
			String replaceDefines(String data);
			DataBlock parseParameter(String param);

		protected:
			String m_dataFilePath;
			DFD_LIST m_localDefs;
			GameData* m_gdata;
			String m_rawContent;
			bool m_hasAutoLoadInfo;
			Application* m_app;

		public:
			inline static DFD_LIST m_globalDefs;
	};

	class AnimationSheetFile : public DataFile
	{
		public:
			inline AnimationSheetFile(void) { invalidate(); }
			inline AnimationSheetFile(String path, GameData& gdata, Application& app) { load(path, gdata, app); }
			virtual ~AnimationSheetFile(void);
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);

			Animation animation(int32 id);
			Animation animation(String name);
			inline IPoint getSpriteCount(void) { return IPoint(m_animCount.x, m_animCount.y); }
			inline IPoint getSpriteSize(void) { return IPoint(m_animSize.x, m_animSize.y); }
			inline Image& getTexture(void) { return m_texture; }
			inline String getName(void) { return m_name; }

		private:
			std::vector<Animation> m_animList;
			Image m_texture;
			String m_name;
			UI16Point m_animCount;
			UI8Point m_animSize;
	};

	class FontFile : public DataFile
	{
		public:
		#ifdef __DEBUG__
			inline FontFile(void) : m_defTTF(nullptr), m_dbgFont(nullptr) { invalidate(); }
			inline FontFile(String path, GameData& gdata, Application& app) : m_defTTF(nullptr), m_dbgFont(nullptr) { load(path, gdata, app); }
		#else
			inline FontFile(void) : m_defTTF(nullptr) { invalidate(); }
			inline FontFile(String path, GameData& gdata, Application& app) : m_defTTF(nullptr) { load(path, gdata, app); }
		#endif
			virtual ~FontFile(void);
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);
			virtual void onCustomAction(String action, DFD_LIST params);

			BitmapFont& font(String name);
			BitmapFont& font(int32 id);
			ttf::Font& ttf(String name);
			ttf::Font& ttf(int32 id);

			inline std::vector<BitmapFont>& list(void) { return m_fontList; }
		#ifdef __DEBUG__
			BitmapFont& getDebugFont(void);;
		#endif
			ttf::Font& getDefaultTTF(void);
		#ifdef __DEBUG__
			bool hasDebugFont(void);
		#endif
			bool hasDefaultTTF(void);

		private:
			std::vector<BitmapFont> m_fontList;
		#ifdef __DEBUG__
			BitmapFont* m_dbgFont;
		#endif
			ttf::Font* m_defTTF;
	};

	class TilesetFile : public DataFile
	{
		public:
			inline TilesetFile(void) { invalidate(); }
			inline TilesetFile(String path, GameData& gdata, Application& app) { load(path, gdata, app); }
			virtual ~TilesetFile(void);
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);

			Sprite sprite(int32 tid);
			Sprite sprite(String name);
			Sprite sprite(Color color);
			inline Sprite sprite(uint8 r, uint8 g, uint8 b, uint8 a = 255) { return sprite(Color(r, g, b, a)); }
			inline IPoint getSpriteCount(void) { return IPoint(m_spriteCount.x, m_spriteCount.y); }
			inline IPoint getSpriteSize(void) { return IPoint(m_spriteSize.x, m_spriteSize.y); }
			inline Image& getTexture(void) { return m_texture; }
			inline String getName(void) { return m_name; }

			inline std::vector<Sprite>& list(void) { return m_spriteList; }

		private:
			std::vector<Sprite> m_spriteList;
			Image m_texture;
			String m_name;
			UI16Point m_spriteCount;
			UI8Point m_spriteSize;
	};

	class WorldFile : public DataFile
	{
		public:
			inline WorldFile(void) { invalidate(); }
			WorldFile(String path, GameData& gdata, Application& app);
			virtual ~WorldFile(void);
			virtual void onCustomInstruction(String instr, DFD_LIST data, int paramCount);
			virtual void onCustomAction(String action, DFD_LIST params);

			Map& map(String name);
			Map& map(int32 id);

			static bool isTilesetInLibrary(String name);
			static TilesetFile& getTileset(String name);
			static TilesetFile& addTileset(String tsetDataPath, Application& app);
			static TilesetFile& addTileset(TilesetFile tset);

			inline std::vector<Map>& list(void) { return m_mapList; }

			inline bool hasFirstMap(void) { return m_firstMap != nullptr; }
			inline Map& getFirstMap(void) { if (!hasFirstMap()) return INVALID_REF(Map); return *m_firstMap; }

		private:
			std::vector<Map> m_mapList;
			Map* m_firstMap;
			DataFunctionHeader m_enterMap;

			inline static std::vector<TilesetFile> tilesetLib = std::vector<TilesetFile>();
	};

	class Application;
	class LoadScriptFile : public DataFile
	{
		public:
			inline LoadScriptFile(void) { invalidate(); }
			LoadScriptFile(String path, GameData& gdata, Application& app);
			virtual ~LoadScriptFile(void);
			virtual void onCustomAction(String action, DFD_LIST params);

		private:
			Application* m_app;
			DataFunctionHeader m_loadTileData;
			DataFunctionHeader m_loadWorldFile;
			DataFunctionHeader m_loadFontFile;
			DataFunctionHeader m_setupSplashScreen;
	};
}

#endif
