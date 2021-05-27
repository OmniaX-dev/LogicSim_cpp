#ifndef __GAMESTATE__HPP__
#define __GAMESTATE__HPP__

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include "EventListener.hpp"
#include "DataTypes.hpp"
#include "LayerRenderer.hpp"
#include "World.hpp"
#include "GuiComponents.hpp"

namespace Omnia
{
	class Application;
	class GameData;
	class GameState : public EventListener, public Object
	{
	private:
		void init(GameData& gdata);

		public:
			inline GameState(void) : m_active(false), m_parent(nullptr) {}
			inline bool isActive(void) { return m_active; }
			inline void activate(GameData& gdata) { m_active = true; onActivate(gdata); }
			bool setInactive(GameState& new_active);
			inline LayerRenderer& layerRenderer(void) { return m_layers; }
			inline GuiComponentHolder& guiHolder(void) { return m_gch; }

			inline virtual void onInit(GameData& gdata) {}
			inline virtual void onActivate(GameData& gdata) {}
			inline virtual void onAnyEvent(sf::Event& evt, GameData& gdata) {}
			inline virtual void onRender(GameData& gdata) {}
			inline virtual void onUpdate(GameData& gdata) {}

			void onEvent(sf::Event& evt, GameData& gdata);
			void render(GameData& gdata);
			void renderGui(GameData& gdata);
			void update(GameData& gdata);
			inline void setParent(Application* parent) { m_parent = parent; }
			inline Application& getParent(void) { return *m_parent; }
			inline bool isGuiEnabled(void) { return m_guiEnabled; }
			inline void enableGuiHolder(void) { setGuiHolderState(true); }
			inline void disableGuiHolder(void) { setGuiHolderState(false); }
			inline void setGuiHolderState(bool state) { m_guiEnabled = state; }
			World& world(void);

		protected:
			bool m_active;
			LayerRenderer m_layers;
			GuiComponentHolder m_gch;
			Application* m_parent;
			bool m_guiEnabled;

		private:
			inline static int NEXT_ID = 0;

			friend class GameStateManager;

	};

}
#endif
