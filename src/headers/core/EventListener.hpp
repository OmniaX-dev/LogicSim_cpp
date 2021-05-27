#ifndef __EVENTLISTENER__HPP__
#define __EVENTLISTENER__HPP__

#include <SFML/Graphics.hpp>
#include "Flags.hpp"
#include "Drawable.hpp"

namespace Omnia
{
	class GameData;
	class EventListener
	{
		public: struct EventLock : public Object
		{
		private: inline static int32 s_nextID = 0;
		private: inline static int32 s_lockedID = AUTO__EVENTS_UNLOCKED;

		public:
			EventLock(void);
			bool lock(void);
			bool unlock(void);
			bool isLocked(void);
			virtual inline String toString(void) { return String::intToStr(getID()); }

			friend class GuiComponent;
		};

		public:
			inline void handleEvents(sf::Event& evt, GameData& gdata)
			{
				if (evt.type == sf::Event::Closed)
					windowClosed(evt, gdata);
				else if (evt.type == sf::Event::Resized)
					windowResized(evt, gdata);
				else if (evt.type == sf::Event::LostFocus)
					windowLostFocus(evt, gdata);
				else if (evt.type == sf::Event::GainedFocus)
					windowGainedFocus(evt, gdata);
				else if (evt.type == sf::Event::KeyPressed)
					__key_pressed(evt, gdata);
				else if (evt.type == sf::Event::KeyReleased)
					__key_released(evt, gdata);
				else if (evt.type == sf::Event::TextEntered)
					textEntered(evt, gdata);
				else if (evt.type == sf::Event::MouseButtonPressed)
					__mouse_pressed(evt, gdata);
				else if (evt.type == sf::Event::MouseButtonReleased)
					__mouse_released(evt, gdata);
				else if (evt.type == sf::Event::MouseMoved)
					__mouse_moved(evt, gdata);
				else if (evt.type == sf::Event::MouseWheelScrolled)
					mouseScrolled(evt, gdata);
				else if (evt.type == sf::Event::MouseEntered)
					mouseEntered(evt, gdata);
				else if (evt.type == sf::Event::MouseLeft)
					mouseLeft(evt, gdata);
				else if (evt.type == sf::Event::JoystickButtonPressed)
					__joystick_pressed(evt, gdata);
				else if (evt.type == sf::Event::JoystickButtonReleased)
					__joystick_released(evt, gdata);
				else if (evt.type == sf::Event::JoystickMoved)
					joystickMoved(evt, gdata);
				else if (evt.type == sf::Event::JoystickConnected)
					joystickConnected(evt, gdata);
				else if (evt.type == sf::Event::JoystickDisconnected)
					joystickDisconnected(evt, gdata);
				onEvent(evt, gdata);
			}

			inline virtual void onEvent(sf::Event& evt, GameData& gdata) {}

			//Keyboard events
			inline virtual void keyPressed(sf::Event& evt, GameData& gdata) {}
			inline virtual void keyReleased(sf::Event& evt, GameData& gdata) {}
			inline virtual void textEntered(sf::Event& evt, GameData& gdata) {}

			//Mouse events
			inline virtual void mousePressed(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseReleased(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseMoved(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseDragged(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseScrolled(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseEntered(sf::Event& evt, GameData& gdata) {}
			inline virtual void mouseLeft(sf::Event& evt, GameData& gdata) {}

			//Window events
			inline virtual void windowClosed(sf::Event& evt, GameData& gdata) {}
			inline virtual void windowResized(sf::Event& evt, GameData& gdata) {}
			inline virtual void windowLostFocus(sf::Event& evt, GameData& gdata) {}
			inline virtual void windowGainedFocus(sf::Event& evt, GameData& gdata) {}

			//Joystick events
			inline virtual void joystickPressed(sf::Event& evt, GameData& gdata) {}
			inline virtual void joystickReleased(sf::Event& evt, GameData& gdata) {}
			inline virtual void joystickMoved(sf::Event& evt, GameData& gdata) {}
			inline virtual void joystickConnected(sf::Event& evt, GameData& gdata) {}
			inline virtual void joystickDisconnected(sf::Event& evt, GameData& gdata) {}

		protected:
			inline EventListener(void) : m_KeyPressed(false), m_mousePressed(false), m_joystickPressed(false) {  }
			inline bool isKeyPressed(void) { return m_KeyPressed; }
			inline bool isMousePressed(void) { return m_mousePressed; }
			inline bool isJoystickPressed(void) { return m_joystickPressed; }
			inline sf::Keyboard::Key getPressedKeyCode(void) { return m_keyPressedCode; }
			inline sf::Mouse::Button getMousePressedCode(void) { return m_mousePressedCode; }
			inline I16Point getMousePosition(void) { return m_mousePosition; }
			inline int16 getMouseX(void) { return m_mousePosition.x; }
			inline int16 getMouseY(void) { return m_mousePosition.y; }
			inline bool lockEvents(void) { return m_eventLock.lock(); }
			inline bool unlockEvents(void) { return m_eventLock.unlock(); }
			inline bool isLockedEvent(void) { return m_eventLock.isLocked(); }
			inline virtual bool allowEvents(void) { return Flags::isunset(FLG_SKIP_NEXT_EVENT_ROUND); }

		private:
			inline void __key_pressed(sf::Event& evt, GameData& gdata) { m_keyPressedCode = evt.key.code; m_KeyPressed = true; keyPressed(evt, gdata); }
			inline void __mouse_pressed(sf::Event& evt, GameData& gdata) { m_mousePressedCode = evt.mouseButton.button; m_mousePressed = true; mousePressed(evt, gdata); }
			inline void __joystick_pressed(sf::Event& evt, GameData& gdata) { m_joystickPressed = true; joystickPressed(evt, gdata); }
			inline void __mouse_moved(sf::Event& evt, GameData& gdata) { if (isMousePressed()) mouseDragged(evt, gdata); else mouseMoved(evt, gdata); m_mousePosition = I16Point((int16)evt.mouseMove.x, (int16)evt.mouseMove.y); }
			inline void __key_released(sf::Event& evt, GameData& gdata) { if (evt.key.code == m_keyPressedCode) m_KeyPressed = false; keyReleased(evt, gdata); }
			inline void __mouse_released(sf::Event& evt, GameData& gdata) { if (evt.mouseButton.button == m_mousePressedCode) m_mousePressed = false; mouseReleased(evt, gdata); }
			inline void __joystick_released(sf::Event& evt, GameData& gdata) { m_joystickPressed = false; joystickReleased(evt, gdata); }

		private://TODO: Add variable for joystick pressed
			bool m_KeyPressed;
			bool m_mousePressed;
			bool m_joystickPressed;
		protected:
			sf::Keyboard::Key m_keyPressedCode;
			sf::Mouse::Button m_mousePressedCode;
			I16Point m_mousePosition;
			EventLock m_eventLock;
	};

	class GuiFocusManager;
	class GuiComponent;
	class GuiEventListener : public EventListener, public Drawable
	{
		protected:
			inline GuiEventListener(void) : EventListener()
			{
				m_gfm = nullptr;
				m_focused = false;
				m_focusOnClick = true;
				m_tabIndex = 0;
				m_evtParent = nullptr;
			}
			inline void setEventParent(GuiComponent* parent) { m_evtParent = parent; }
			inline GuiComponent& getEventParent(void) { return (m_evtParent == nullptr ? INVALID_REF(GuiComponent) : *m_evtParent); }
			inline bool isFocusOnClick(void) { return m_focusOnClick; }
            inline void setFocusOnClick(bool foc) { m_focusOnClick = foc; }
            inline bool isFocused(void) { return m_focused; }

			//Keyboard events
			inline virtual void onKeyPressed(sf::Event& evt, GameData& gdata) {}
			inline virtual void onKeyReleased(sf::Event& evt, GameData& gdata) {}
			inline virtual void onTextEntered(sf::Event& evt, GameData& gdata) {}

			//Mouse events
			inline virtual void onMousePressed(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseReleased(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseMoved(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseDragged(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseScrolled(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseEntered(sf::Event& evt, GameData& gdata) {}
			inline virtual void onMouseLeft(sf::Event& evt, GameData& gdata) {}

			//Component focus
			inline virtual void componentFocusGained(GameData& gdata, GuiComponent& sender) {  }
			inline virtual void componentFocusLost(GameData& gdata, GuiComponent& sender) {  }

			bool isMouseInside(void);
			inline bool allowEvents(void) { return true; }

		private:
			//Keyboard events
			virtual void keyPressed(sf::Event& evt, GameData& gdata);
			virtual void keyReleased(sf::Event& evt, GameData& gdata);
			virtual void textEntered(sf::Event& evt, GameData& gdata);

			//Mouse events
			virtual void mousePressed(sf::Event& evt, GameData& gdata);
			virtual void mouseReleased(sf::Event& evt, GameData& gdata);
			virtual void mouseMoved(sf::Event& evt, GameData& gdata);
			virtual void mouseDragged(sf::Event& evt, GameData& gdata);
			virtual void mouseScrolled(sf::Event& evt, GameData& gdata);
			virtual void mouseEntered(sf::Event& evt, GameData& gdata);
			virtual void mouseLeft(sf::Event& evt, GameData& gdata);
		
		protected:
            bool m_focused;
            bool m_focusOnClick;
            uint8 m_tabIndex;
            GuiFocusManager* m_gfm;
			GuiComponent* m_evtParent;

			friend class GuiComponent;
			friend class GuiComponentHolder;
	};
}

#endif
