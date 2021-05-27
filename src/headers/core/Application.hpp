#ifndef __APPLICATION__HPP__
#define __APPLICATION__HPP__

#include "GameState.hpp"
#include <memory>
#include <list>
#include <iostream>
#include "GameData.hpp"
#include "BitmapFont.hpp"
#include "World.hpp"
#include "Defines.hpp"
#include "DataFile.hpp"
#include "Color.hpp"

namespace Omnia
{
	class DFAutoLoadInfoInterface;
	class FontFile;
	class World;
	class LoadScriptFile;
	class Application : public Object
	{
		public:
		#ifdef __DEBUG__
			inline Application(void) : m_drawCalls(0), m_currentTime(0), m_debugDelay(0), m_gameData(GameData::instance()), m_splashScale(0), m_splashSeconds(0), m_splashScreenPath("") {}
		#else
			inline Application(void) : m_gameData(GameData::instance()), m_splashScale(0), m_splashSeconds(0), m_splashScreenPath("") {}
		#endif
			virtual ~Application(void);
			void handleEvents(void);
			void init(void);
			void startGameLoop(void);
			void setIcon(String iconPath);
			void setTime(uint8 hh, uint8 mm);
		#ifdef __DEBUG__
			void setDebugFont(BitmapFont& font);
		#endif
			inline void splashScreenSetup(String imgPath, int seconds = 5, float scale = 1.0f) { m_splashScreenPath = imgPath; m_splashScale = scale; m_splashSeconds = seconds; }
			inline virtual void onInit(void) {}
			void createWindow(void);
			LoadScriptFile& loadScript(void);

			inline void setClearColor(Color col) { m_clearColor = col; }
			inline Color& getClearColor(void) { return m_clearColor; }
			inline void setWindowTitle(String title) { m_gameData.window.setTitle(title.cpp()); }
			inline void setWindowSize(int width, int height) { m_gameData.window.setSize(sf::Vector2u(width, height)); }
			inline void setWindowPosition(int x, int y) { m_gameData.window.setPosition(sf::Vector2i(x, y)); }
			inline void setVSync(bool vsync) { m_gameData.window.setVerticalSyncEnabled(vsync); }
			inline UIPoint getWindowSize(void) { return UIPoint(m_gameData.window.getSize().x, m_gameData.window.getSize().y); }
			inline IPoint getWindowPosition(void) { return IPoint(m_gameData.window.getPosition().x, m_gameData.window.getPosition().y); }
			inline sf::RenderWindow& getWindowInstance(void) { return m_gameData.window; }
			inline World& world(void) { return (m_world == nullptr ? INVALID_REF(World) :  *m_world); }
			inline int32 getBuildNumber(void) { return m_buildNr; }
			inline IntPtr& getBuildNumberPtr(void) { return m_buildNrPtr; }
		#ifdef __DEBUG__
		#ifdef __TRACK_GAME_TIMING_INFO__
			inline GameTimes& getTimingInfo(void) { return m_gameData.timing; }
		#endif
		#endif
			inline ErrorManager& getErrorMManagerInstance(void) { return m_gameData.errorQueue; }
			inline GameData& gdata(void) { return m_gameData; }

			inline const RealTime& getGameClock(void) { return m_gameData.gameClock; }
			inline void addHoursToGameTime(uint8 hours) { m_gameData.gameClock.hours += hours; m_gameData.gameClock.start(); }
			inline FontFile& fonts(void) { return *m_fonts; }
			void loadFonts(String fontFilePath);
			void loadWorld(String worldFilePath);
			void openLoadScript(String path);
			void drawSplashScreen(void);

		private:
			void render(void);
			void update(void);

		protected:
			GameData& m_gameData;
			String m_splashScreenPath;
			float m_splashScale;
			int m_splashSeconds;
			un_ptr(FontFile) m_fonts;
			un_ptr(World) m_world;
			un_ptr(LoadScriptFile) m_load;

			Color m_clearColor;

		private:
		#ifdef __DEBUG__
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
			int32 m_currentTime;
			int32 m_debugDelay;
		#endif
		#endif
			String m_title;
			int32 m_buildNr;
			IntPtr m_buildNrPtr;

		#ifdef __DEBUG__
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
			IntPtr m_fpsPtr;
			IntPtr m_upsPtr;
			IntPtr m_drawCallsPtr;
			OString m_dbgTime;
		#endif
		#ifdef __TRACK_GAME_TIMING_INFO__
			int32 m_drawCalls;
		#endif	
		#endif

		friend class LoadScriptFile;
	};
}
#endif
