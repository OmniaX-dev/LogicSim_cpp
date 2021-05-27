#include "AnimationSelector.hpp"
#include "Defines.hpp"
#include "Object.hpp"
#include "GameData.hpp"
#include <iostream>

namespace Omnia
{
	void AnimationSelector::create(void)
	{
		m_currentCount = 0;
		m_current = 0;
	}

	int AnimationSelector::add(Animation& anim)
	{
		m_animList.push_back(&anim);
		return m_currentCount++;
	}

	Animation& AnimationSelector::get(int id)
	{
		if (id < m_currentCount)
			return *(m_animList[id]);
		GameData::instance().errorQueue.push(Error::ANIMATION_NOT_FOUND, ERROR_DATA());
		return INVALID_REF(Animation);
	}

	void AnimationSelector::setCurrent(int id)
	{
		if (id < m_currentCount)
			m_current = id;
	}

	Animation& AnimationSelector::current(void)
	{
		if (m_currentCount > 0)
			return *(m_animList[m_current]);
		return INVALID_REF(Animation);
	}
}
