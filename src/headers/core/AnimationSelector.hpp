#ifndef __ANIMATIONSELECTOR__HPP__
#define __ANIMATIONSELECTOR__HPP__

#include <SFML/Graphics.hpp>
#include <vector>

namespace Omnia
{
	class Animation;
	class AnimationSelector
	{
	public:
		inline AnimationSelector(void) { create(); }
		void create(void);
		int add(Animation& r);
		Animation& get(int id);
		inline int getCount(void) { return m_animList.size(); }
		void setCurrent(int id);
		Animation& current(void);

	private:
		std::vector<Animation*> m_animList;
		int m_currentCount;
		int m_current;
	};
}
#endif
