#include "GameStateManager.hpp"
#include "GameData.hpp"

namespace Omnia
{
	void GameStateManager::create(GameData& gdata, Application& app)
	{
		m_currentGameState = nullptr;
		m_gameData = &gdata;
		m_parent = &app;
	}

	int GameStateManager::addGameState(GameState& state)
	{
		m_gameStates.push_back(&state);
		state.setParent(&getParent());
		state.init(*m_gameData);
		if (m_currentGameState == nullptr)
			setActiveGameState(state.getID());
		return m_currentGameState->getID();
	}

	bool GameStateManager::setActiveGameState(uint32 id)
	{
		GameState* ngs = getGameState(id);
		if (ngs == nullptr)
		{
			GameData::instance().errorQueue.push(Error::GAME_STATE_NOT_FOUND, ERROR_DATA());
			return false;
		}
		ngs->activate(*m_gameData);
		for (auto& state : m_gameStates)
			if (state->getID() != id)
				state->setInactive(*ngs);
		m_currentGameState = ngs;
		return true;
	}

	GameState& GameStateManager::getCurrentGameState(void)
	{
		if (m_currentGameState != nullptr)
			return *m_currentGameState;
		return INVALID_REF(GameState);
	}

	GameState* GameStateManager::getGameState(uint32 id)
	{
		for (auto& state : m_gameStates)
		{
			if (state->getID() == id)
				return state;
		}
		return nullptr;
	}

	void GameStateManager::update(GameData& gdata)
	{
		if (m_currentGameState != nullptr)
			m_currentGameState->update(gdata);
	}

	void GameStateManager::render(GameData& gdata)
	{
		if (m_currentGameState != nullptr)
			m_currentGameState->render(gdata);
	}

	void GameStateManager::renderGui(GameData& gdata)
	{
		if (m_currentGameState != nullptr)
			m_currentGameState->renderGui(gdata);
	}

	void GameStateManager::onEvent(sf::Event& evt, GameData& gdata)
	{
		if (m_currentGameState != nullptr)
			m_currentGameState->handleEvents(evt, gdata);
	}
}
