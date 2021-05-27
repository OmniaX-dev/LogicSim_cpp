#include "GameState.hpp"
#include "GameData.hpp"
#include "Flags.hpp"
#include "Application.hpp"

namespace Omnia
{
	void GameState::init(GameData& gdata)
	{
		setID(GameState::NEXT_ID++);
		m_active = false;
		onInit(gdata);
	}

	World& GameState::world(void)
	{
		return getParent().world();
	}

	bool GameState::setInactive(GameState& new_active)
	{
		if (!new_active.isActive() && Flags::isunset(FLG_IGNORE_NEXT_ACTIVE_STATE_CONTROL))
		{
			GameData::instance().errorQueue.push(Error::DEACTIVATING_STATE_WITHOUT_SUBSTITUTE, ERROR_DATA());
			return false;
		}
		m_active = false;
		return true;
	}

	void GameState::render(GameData& gdata)
	{
		m_layers.clear();
		onRender(gdata);
		m_layers.render(gdata);
	}

	void GameState::renderGui(GameData& gdata)
	{
		if (isGuiEnabled())
			m_gch.render(gdata);
	}

	void GameState::update(GameData& gdata)
	{
		if (isGuiEnabled())
			m_gch.update(gdata);
		onUpdate(gdata);
	}

	void GameState::onEvent(sf::Event& evt, GameData& gdata)
	{
		if (isGuiEnabled())
			m_gch.handleEvents(evt, gdata);
		if (allowEvents())
			onAnyEvent(evt, gdata);
		if (evt.type == sf::Event::KeyPressed)
		{
		#ifdef __DEBUG__
			if (Flags::isset(FLG_DEBUG) && evt.key.code == SF_KEY(F9))
			{
				Flags::neg(FLG_SHOW_DEBUG_VIEW);
				#ifdef __CREATE_DEFAULT_DBG_VIEW__
					gdata.debugView.setVisible(!gdata.debugView.isVisible());
				#endif
			}
			else if (Flags::isset(FLG_DEBUG) && evt.key.code == SF_KEY(F8))
				gdata.debugConsole.setVisible(!gdata.debugConsole.isVisible());
		#endif
		}
	}
}
