#include "Controls.hpp"
#include "GameData.hpp"

namespace Omnia
{
	void Control::setKey(sf::Keyboard::Key key)
	{
		this->key = key;
		isBound = true;
		type = eControlType::Keyboard;
	}

	void Control::setMouseButton(sf::Mouse::Button button)
	{
		mouseButton = button;
		isBound = true;
		type = eControlType::MouseButton;
	}

	void Control::setJoystickButton(uint32 button)
	{
		joystickButton = button;
		isBound = true;
		type = eControlType::Joystick;
	}

	bool Control::isPressed(sf::Event& evt) const
	{
		if (!isBound)
			return false;
		switch (type)
		{
			case eControlType::Keyboard:
				return evt.type == sf::Event::KeyPressed && evt.key.code == key;
			break;
			case eControlType::MouseButton:
				return evt.type == sf::Event::MouseButtonPressed && evt.mouseButton.button == mouseButton;
			break;
			case eControlType::Joystick:
				return evt.type == sf::Event::JoystickButtonPressed && evt.joystickButton.button == joystickButton;
			break;
			default:
				GameData::instance().errorQueue.push(Error::UNKNOWN_CONTROL_TYPE, ERROR_DATA());
				return false;
			break;
		}
	}

	UserControl& ControlGroup::addControl(UserControl control)
	{
		m_controls.push_back(control);
		return m_controls[getControlCount() - 1];
	}

	bool ControlGroup::isPressed(sf::Event& evt)
	{
		if (getControlCount() == 0)
			return false;
		if (getControlCount() == 1)
			return m_controls[0].get().isPressed(evt);
		for (auto& cc : m_controls)
		{
			if (cc.get().isPressed(evt))
				return true;
		}
		return false;
	}

	uint16 ControlGroup::getNextControlID(bool increment)
	{
		if (increment)
			return ControlGroup::s_nextID++;
		return ControlGroup::s_nextID;
	}

	UserControl& ControlGroup::bind(sf::Keyboard::Key key)
	{
		Control ctrl = Control(getNextControlID());
		ctrl.setKey(key);
		m_controls.push_back(UserControl(ctrl));
		return m_controls[getControlCount() - 1];
	}

	UserControl& ControlGroup::bind(uint32 joystickButton)
	{
		Control ctrl = Control(getNextControlID());
		ctrl.setJoystickButton(joystickButton);
		m_controls.push_back(UserControl(ctrl));
		return m_controls[getControlCount() - 1];
	}

	UserControl& ControlGroup::bind(sf::Mouse::Button mouseButton)
	{
		Control ctrl = Control(getNextControlID());
		ctrl.setMouseButton(mouseButton);
		m_controls.push_back(UserControl(ctrl));
		return m_controls[getControlCount() - 1];
	}


	bool ControlGroup::contains(sf::Keyboard::Key key)
	{
		for (auto& cc : m_controls)
		{
			if (cc.get().type == eControlType::Keyboard && cc.get().getKey() == key)
				return true;
		}
		return false;
	}

	bool ControlGroup::contains(uint32 joystickButton)
	{
		for (auto& cc : m_controls)
		{
			if (cc.get().type == eControlType::Joystick && cc.get().getJoystickButton() == joystickButton)
				return true;
		}
		return false;
	}

	bool ControlGroup::contains(sf::Mouse::Button mouseButton)
	{
		for (auto& cc : m_controls)
		{
			if (cc.get().type == eControlType::MouseButton && cc.get().getMouseButton() == mouseButton)
				return true;
		}
		return false;
	}
}
