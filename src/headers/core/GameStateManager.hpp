#ifndef __GAMESTATEMANAGER__HPP__
#define __GAMESTATEMANAGER__HPP__

#include "GameState.hpp"
#include <vector>
#include "EventListener.hpp"

namespace Omnia
{
	class Application;
	class GameData;
	class GameStateManager : public EventListener
	{
		public:
			inline GameStateManager(void) { m_currentGameState = nullptr; m_gameData = nullptr; m_parent = nullptr; }
			void create(GameData& gdata, Application& app);
			int addGameState(GameState& state);
			bool setActiveGameState(uint32 id);
			GameState* getGameState(uint32 id);
			void update(GameData& gdata);
			void render(GameData& gdata);
			void renderGui(GameData& gdata);
			virtual void onEvent(sf::Event& evt, GameData& gdata);
			GameState& getCurrentGameState(void);
			inline Application& getParent(void) { return *m_parent; }

		private:
			std::vector<GameState*> m_gameStates;
			GameState* m_currentGameState;
			GameData* m_gameData;
			Application* m_parent;
	};
}

#endif
