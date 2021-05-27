#ifndef __ENTITYCOMPONENTS__HPP__
#define __ENTITYCOMPONENTS__HPP__

#include "Entity.hpp"
#include "AnimationSelector.hpp"

namespace Omnia
{
	class ControllerComponent : public EntityComponent
	{
		public:
			inline ControllerComponent(void) { m_direction = eDirection::None; }
			ControllerComponent(Entity& parent);
			virtual void update(GameData& gdata);
			virtual void onEvent(sf::Event& evt, GameData& gdata);
		private:
			eDirection m_direction;
			ControlGroup m_cUp;
			ControlGroup m_cDown;
			ControlGroup m_cLeft;
			ControlGroup m_cRight;
	};
	class Animation;
	class SimpleAnimationComponent : public EntityComponent
	{
		public:
			inline SimpleAnimationComponent(void) { m_animation = nullptr; }
			SimpleAnimationComponent(Entity& parent, Animation& anim);
			virtual void render(GameData& gdata, Layer& layer);
			virtual void update(GameData& gdata);
		private:
			Animation* m_animation;
	};
	class AnimationSelectorComponent : public EntityComponent
	{
		public:
			inline AnimationSelectorComponent(void) :
				m_up(0), m_down(0), m_left(0), m_right(0)
			{  }
			AnimationSelectorComponent(Entity& parent, AnimationData ad);
			virtual void render(GameData& gdata, Layer& layer);
			virtual void update(GameData& gdata);
			virtual void keyPressed(sf::Event& evt, GameData& gdata);
			virtual void keyReleased(sf::Event& evt, GameData& gdata);
		private:
			uint16 m_up;
			uint16 m_down;
			uint16 m_left;
			uint16 m_right;
			
			ControlGroup m_cUp;
			ControlGroup m_cDown;
			ControlGroup m_cLeft;
			ControlGroup m_cRight;

			sh_ptr(Animation) m_upAnim;
			sh_ptr(Animation) m_downAnim;
			sh_ptr(Animation) m_leftAnim;
			sh_ptr(Animation) m_rightAnim;

			AnimationSelector m_animSelec;
	};
	class CameraComponent : public EntityComponent
	{
		public:
			inline CameraComponent(void) { m_screenMargin = 120; } //TODO: Add AUTO__
			inline CameraComponent(Entity& parent) : EntityComponent(parent) { m_screenMargin = 120; }
			virtual void onMove(GameData& gdata, eDirection dir);
		private:
			uint32 m_screenMargin;
	};
	class CollisionComponent : public EntityComponent
	{
		public:
			inline CollisionComponent(void) {  }
			inline CollisionComponent(Entity& parent, eCollisionType ect = eCollisionType::AABB) : EntityComponent(parent) { m_ect = ect; }
			virtual void onMove(GameData& gdata, eDirection dir);
			inline void setCollisionType(eCollisionType ect) { m_ect = ect; }
			inline eCollisionType getCollisionType(void) { return m_ect; }

		private:
			eCollisionType m_ect;
	};
}

#endif
