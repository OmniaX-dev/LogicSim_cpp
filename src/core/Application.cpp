#include "Application.hpp"
#include <iostream>
#include "Renderer.hpp"
#include "Image.hpp"
#include <SFML/System.hpp>
#include "Fonts.hpp"
#include "String.hpp"
#include "DataFile.hpp"
#include "World.hpp"
#include <cstdlib>
#include "../../frontend/headers/Runtime.hpp"
#include "../../frontend/headers/LSColors.hpp"

namespace Omnia
{
	Application::~Application(void)
	{
	}

	void Application::handleEvents(void)
	{
		sf::Event evt;
		while (m_gameData.window.pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
				m_gameData.window.close();
			m_gameData.gameStateManager.handleEvents(evt, m_gameData);
		#ifdef __DEBUG__
			m_gameData.debugView.handleEvents(evt, m_gameData);
		#endif
		}
		Flags::unset(FLG_SKIP_NEXT_EVENT_ROUND);
	}

#ifdef __DEBUG__
	void Application::setDebugFont(BitmapFont& font)
	{
		//m_gameData.debugView.m_textLayer.get().setBitmapFont(font);
		//GameData::instance().debugConsole.system(StringBuilder("set_dbg_font: ").add(font.getName()).get());
	}
#endif

	void Application::loadWorld(String worldFilePath)
	{
		m_world = new_un(World)(worldFilePath, *this);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("load_world_file: ").add(worldFilePath).get());
	#endif
	}

	void Application::loadFonts(String fontFilePath)
	{
		m_fonts = new_un(FontFile)(fontFilePath, m_gameData, *this);

	#ifdef __DEBUG__
		if (Flags::isset(FLG_DEBUG) && m_fonts->hasDebugFont())
		{
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
			ls_cmd.setBitmapFont(m_fonts->getDebugFont());
			ls_statusBar.setBitmapFont(m_fonts->getDebugFont());
			ls_menuBar.setBitmapFont(m_fonts->getDebugFont());
			ls_pinMenu.setBitmapFont(m_fonts->getDebugFont());
			ls_toolBar.setBitmapFont(m_fonts->getDebugFont());
			ls_guiBase.setBitmapFont(m_fonts->getDebugFont());
			ls_statusBar.attachPtr(m_fpsPtr, "FPS:", ls::LSColors::StatusBarText, 18);
			ls_statusBar.attachPtr(m_upsPtr, "UPS:", ls::LSColors::StatusBarText, 18);
			ls_statusBar.attachPtr(m_drawCallsPtr, "Draw Calls:", ls::LSColors::StatusBarText, 18);
		#endif
			GameData::instance().debugConsole.system(StringBuilder("load_font_file: ").add(fontFilePath).get());
		}
	#endif
	}

	void Application::openLoadScript(String path)
	{
		m_load = new_un(LoadScriptFile)(path, m_gameData, *this);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("open_load_script: ").add(path).get());
	#endif
	}

	void Application::init(void)
	{
		Flags::init();
		m_buildNr = 0;
		m_buildNrPtr = IntPtr(m_buildNr);
		m_gameData.application = this;
		m_gameData.gameStateManager.create(m_gameData, *this);
		m_gameData.camera.setMax(UI16Point(AUTO__CAM_MAX_X, AUTO__CAM_MAX_Y));
	#if defined(__DEBUG__)
		Flags::set(FLG_DEBUG);
	#endif
	#if !defined(__HIDE_SPL_SCR__) 
		Flags::set(FLG_SHOW_SPLASH_SCREEN);
	#endif
		String str = "NO_TITLE", str2 = "";
		int bn = 0;
	#if defined(__APP_NAME__) 
		str = __APP_NAME__;
		#if defined(__BUILD_NUMBER__)
			bn = __BUILD_NUMBER__;
			m_buildNr = bn;
			str = str.add(" - Build ").add(String::intToStr(bn));
		#endif
	#endif
		m_title = str;
	#ifdef __DEBUG__
		Flags::set(FLG_AUTO_DUMP_EVERY_ERROR);
		Flags::set(FLG_LONG_MESSAGE_ON_ERROR_DUMP);
	#endif 

		ls_cmd.create();

		DataFile::addGlobal("TRUE", DATABLOCK_TRUE);
		DataFile::addGlobal("FALSE", DATABLOCK_FALSE);
		DataFile::addGlobal("TL_NORMAL", (int32)eTileUnitType::Normal);
		DataFile::addGlobal("TL_ANIMATED", (int32)eTileUnitType::Animation);
		DataFile::addGlobal("TL_RND_ANIMATED", (int32)eTileUnitType::RandomAnimation);
		DataFile::addGlobal("CL_NO_TINT", "(255, 255, 255, 255)", true);
		DataFile::addGlobal("DEFAULT_SPRITE_FORMATTER", "(3, 0, 1, 2)", true);
	#ifdef __DEBUG__
	#ifdef __CREATE_DEFAULT_DBG_VIEW__
		m_fpsPtr = IntPtr(m_gameData.timing.fps);
		m_upsPtr = IntPtr(m_gameData.timing.ups);
		m_drawCallsPtr = IntPtr(m_drawCalls);
	#endif
	#endif

	#ifdef __DEBUG__
	#ifdef __CREATE_DEFAULT_DBG_VIEW__
		m_currentTime = m_debugDelay = 20;
	#endif
	#endif
	#ifdef __GOD_MODE__
		for (uint8 i = 0; i < 4; i++)
			camMove[i] = false;
	#endif
		setClearColor(Color::Black);
		srand(time(NULL));
		onInit();
	}

	void Application::createWindow(void)
	{
		m_gameData.window.create(sf::VideoMode(m_gameData.screenSize.x, m_gameData.screenSize.y), m_title.cpp(), sf::Style::Titlebar | sf::Style::Default);
		m_gameData.window.setKeyRepeatEnabled(false);
		m_gameData.setDefaultRenderTarget();
	}

	void Application::setTime(uint8 hh, uint8 mm)
	{
		m_gameData.gameClock.hours = hh;
		m_gameData.gameClock.minutes = mm;
		m_gameData.gameClock.start();
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("set_time: ").add(m_gameData.gameClock.asString()).get());
	#endif
	}

	void Application::setIcon(String iconPath)
	{
		sf::Image icon;
		icon.loadFromFile(iconPath.cpp());
		m_gameData.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	LoadScriptFile& Application::loadScript(void)
	{
		if (m_load == nullptr)
			return INVALID_REF(LoadScriptFile);
		return *m_load;
	}

	void Application::drawSplashScreen(void)
	{
		if (Flags::isunset(FLG_SHOW_SPLASH_SCREEN))
			return;
		Image splash(m_splashScreenPath);
		sf::RenderWindow spl;
		spl.create(sf::VideoMode(splash.getSize().x * m_splashScale, splash.getSize().y * m_splashScale), "", sf::Style::None);
		m_gameData.setRenderTarget(spl);
		sf::Sprite spr = splash.sfSprite();
		spr.setScale(m_splashScale, m_splashScale);
		spr.setPosition(0, 0);
		spl.draw(spr);
		String ver = String("Omnia Engine v0.").add(String::intToStr(__BUILD_NUMBER__));
		if (m_fonts != nullptr && m_fonts->hasDefaultTTF())
			m_gameData.renderer.drawText(ver, FPoint(10, (splash.getSize().y - 20) * m_splashScale), Color(0, 128, 255), 12, m_fonts->getDefaultTTF().sf);
		spl.setPosition(sf::Vector2i(1366 / 2 - splash.getSize().x / 2, 768 / 2 - splash.getSize().y / 2));
		spl.display();
		sf::sleep(sf::seconds(m_splashSeconds));
	}

	void Application::startGameLoop(void)
	{
		sf::Clock clock;
		sf::Time accumulator = sf::Time::Zero;
		sf::Time ups = sf::seconds(1.f / AUTO__MAX_UPS);
		m_gameData.gameClock.start();

	#ifdef __DEBUG__
	#ifdef __TRACK_GAME_TIMING_INFO__
		sf::Clock info_clock;
		int _ups = 0, _fps = 0;
	#endif
		m_gameData.debugView.setPosition(m_gameData.getScreenSize().x - 500, 20);
		m_gameData.debugConsole.print("Press <F11>    to toggle debug console.");
		m_gameData.debugConsole.print("Use /help to see a list of all available commands.");
	#endif
		while (m_gameData.window.isOpen())
		{
			handleEvents();
			while (accumulator > ups)
			{
				accumulator -= ups;
				update();
			#ifdef __DEBUG__
			#ifdef __TRACK_GAME_TIMING_INFO__
				_ups++;
			#endif
			#endif
			}

			render();
		#ifdef __DEBUG__
		#ifdef __TRACK_GAME_TIMING_INFO__
			m_drawCalls = m_gameData.timing.drawCalls;
			m_gameData.timing.drawCalls = 0;
			_fps++;

			if (info_clock.getElapsedTime().asMilliseconds() >= 1000)
			{
				m_gameData.timing.fps = _fps;
				m_gameData.timing.ups = _ups;
				_fps = _ups = 0;
				info_clock.restart();
			}
		#endif
		#endif

			accumulator += clock.restart();
		}
	}

	void Application::render(void)
	{
		m_gameData.clearTarget(m_clearColor);
		m_gameData.gameStateManager.render(m_gameData);
		m_gameData.gameStateManager.renderGui(m_gameData);
		m_gameData.window.display();
	}

	void Application::update(void)
	{
	#ifdef __DEBUG__
		m_gameData.debugView.update(m_gameData);
	#endif
		SpriteSharedData::update();
		//m_gameData.gameClock.update();
		m_gameData.gameStateManager.update(m_gameData);
	}
}
