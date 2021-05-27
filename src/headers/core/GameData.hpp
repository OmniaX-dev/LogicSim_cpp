#ifndef __GAMEDATA__HPP__
#define __GAMEDATA__HPP__

#include <SFML/Graphics.hpp>
#include "ErrorManager.hpp"
#include "DataTypes.hpp"
#include "GameStateManager.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "LayerRenderer.hpp"
#include "BitmapFont.hpp"
#include "Controls.hpp"
#include "DebugView.hpp"

namespace Omnia
{
	class Application;
	class GameData
	{
		private:
			static GameData* _instance;
		public:
			sf::RenderWindow window;
			ErrorManager errorQueue;
			RealTime gameClock;
			GameStateManager gameStateManager;
			Renderer renderer;
			Camera camera;
			CommandInterpreter command;
			ONSPool* onsp;

			UI8Point tileSize;
			UI8Point screenTiles;
			UIPoint screenSize;

			Application* application;

		#ifdef __DEBUG__
		#ifdef __TRACK_GAME_TIMING_INFO__
			GameTimes timing;
		#endif
		#ifdef __CREATE_DEFAULT_DBG_VIEW__
			DebugView debugView;
		#endif
			DebugConsole debugConsole;
		#endif

			inline void initONSP(void)
			{
				onsp = &(ONSPool::instance());
			#ifdef __DEBUG__
			#ifdef __CREATE_DEFAULT_DBG_VIEW__
				onsp->addObject(&debugView);
			#endif
				onsp->addObject(&debugConsole);
			#endif
				onsp->addObject(&camera);
			}
			inline static GameData& instance(void) { return *_instance; }
			inline GameState& getCurrentGameState(void) { return gameStateManager.getCurrentGameState(); }
			inline void setDefaultRenderTarget(void) { renderer.setRenderTarget(window); }
			inline void setRenderTarget(sf::RenderTarget& target) { renderer.setRenderTarget(target); }
			inline sf::RenderTarget& getRenderTarget(void) { return renderer.getRenderTarget(); }
			inline void clearTarget(Color color) { renderer.clear(color); }
			inline IPoint getScreenSize(void) { return IPoint(window.getSize().x, window.getSize().y); }
			inline UIPoint getScreenTiles(IPoint tileSize) { return UIPoint(getScreenSize().x / tileSize.x + 3, getScreenSize().y / tileSize.y + 3); } //INFO: +2 is the extra margin for visual purpose

		private:
			GameData(void) : onsp(nullptr), application(nullptr) { screenSize = UIPoint(1100, 680); tileSize = UI8Point(32, 32); screenTiles = UI8Point(34, 20); }
	};

	inline GameData* GameData::_instance = new GameData();
}

#endif
