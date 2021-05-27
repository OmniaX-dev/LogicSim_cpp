#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__

#include "DataTypes.hpp"

namespace Omnia
{
	class Control : public Identifiable
	{
		public:
			inline Control(void) { setID(-1); isBound = false; type = eControlType::Unknown; }
			inline Control(int id) { setID(id); isBound = false; type = eControlType::Unknown; }
			void setKey(sf::Keyboard::Key key);
			void setMouseButton(sf::Mouse::Button button);
			void setJoystickButton(uint32 button);
			bool isPressed(sf::Event& evt) const;
			sf::Keyboard::Key getKey(void) const { return key; }
			sf::Mouse::Button getMouseButton(void) const { return mouseButton; }
			uint32 getJoystickButton(void) const { return joystickButton; }

		public:
			sf::Keyboard::Key key;
			sf::Mouse::Button mouseButton;
			uint32 joystickButton;
			bool isBound;

			eControlType type;
	};

	class UserControl
	{
		public:
			inline UserControl(void) {  }
			inline UserControl(Control control) { set(control); }
			inline void set(Control control) { m_control = control; }
			inline const Control& get(void) { return std::as_const(m_control); }

		private:
			Control m_control;
			friend class ControlGroup;
	};

	class ControlGroup
	{
		public:
			inline ControlGroup(void) {  }
			UserControl& addControl(UserControl control);
			bool isPressed(sf::Event& evt);
			inline uint16 getControlCount(void) { return m_controls.size(); }
			UserControl& bind(sf::Keyboard::Key key);
			UserControl& bind(uint32 joystickButton);
			UserControl& bind(sf::Mouse::Button mouseButton);
			static uint16 getNextControlID(bool increment = false);
			bool contains(sf::Keyboard::Key key);
			bool contains(uint32 joystickButton);
			bool contains(sf::Mouse::Button mouseButton);

		private:
			std::vector<UserControl> m_controls;
		private:
			inline static uint16 s_nextID = 0;
	};
}

#endif
