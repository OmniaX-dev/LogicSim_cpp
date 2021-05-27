#ifndef __GAME_ENTITIES__HPP__
#define __GAME_ENTITIES__HPP__

#include "EntityComponents.hpp"
#include "Animation.hpp"

namespace Omnia
{
	class Player : public Entity
	{
		public:
			inline Player(void) { invalidate(); }
            inline Player(Animation anim, FPoint size) { create(anim, size); }
			void create(Animation anim, FPoint size);
			virtual String toString(void);

		private:
			AnimationSelectorComponent m_asc;
			ControllerComponent m_cc;
			CameraComponent m_camc;
			CollisionComponent m_colc;
	};
}

#endif