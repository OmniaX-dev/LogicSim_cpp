#ifndef __ENTITY__HPP__
#define __ENTITY__HPP__

#include "DataTypes.hpp"
#include "Drawable.hpp"
#include "EventListener.hpp"
#include "Controls.hpp"
#include "LayerRenderer.hpp"

namespace Omnia
{
	class EntityComponent;
	class GameData;
	class Entity : public Drawable, public Object, public EventListener
	{
		public: class BasicCollideCondition : public ICollideCondition
		{
			public:
				inline virtual bool collides(eCollisionLevel ecl, Entity& parent) { return ecl == eCollisionLevel::HardCollision || ecl == eCollisionLevel::HardLiquidCollision; } 
		};
		public:
			inline Entity(void) : Drawable() { create(); }
			void create(void);
			void update(GameData& gdata);
			void render(GameData& gdata, Layer& layer);
			virtual inline void onUpdate(GameData& gdata) {  }
			virtual inline void onRender(GameData& gdata, Layer& layer) {  }
			virtual void handleCollision(eCollisionLevel ecl, FPoint margin);
			void onMove(GameData& gdata, eDirection dir);

			virtual void onEvent(sf::Event& evt, GameData& gdata);
			void addComponent(EntityComponent& component);

			inline const Stats& getStats(void) { return std::as_const(m_stats); }
			inline eDirection getDirection(void) { return m_direction; }
			inline void setDirection(eDirection dir) { m_direction = dir; }
			inline FRect getBoundingBox(void) { return FRect(m_boundingBox.x + getx(), m_boundingBox.y + gety(), m_boundingBox.w, m_boundingBox.h); }
			inline void setBoundingBox(FRect bounds) { m_boundingBox = bounds; }
			inline void setCollideCondition(ICollideCondition cc) { *m_collideCondition = cc; }
			inline ICollideCondition& getCollideCondition(void) { return *m_collideCondition; }
			inline void setSprCollBox(FRect colBox) { m_collSprBox = colBox; }
			inline FRect getSprCollBox(void) { return m_collSprBox; }
		#ifdef __DEBUG__
			inline Layer& getDebugLayer(void) { return m_dbgLayer; }
		#endif

		protected:
			Stats m_stats;
		#ifdef __DEBUG__
			Layer m_dbgLayer;
		#endif

		private:
			std::vector<EntityComponent*> m_components;
			eDirection m_direction;
			FRect m_boundingBox;
			sh_ptr(ICollideCondition) m_collideCondition;
			FRect m_collSprBox;
	};
	class EntityComponent : public Object, public EventListener
	{
		public:
			inline EntityComponent(void) : m_parent(nullptr) { invalidate(); }
			inline EntityComponent(Entity& parent) : m_parent(&parent) { setValid(true); }
			inline bool hasParent(void) { return m_parent != nullptr; }

			inline Entity& getParent(void) { return *m_parent; }
			inline virtual void update(GameData& gdata) {  }
			inline virtual void render(GameData& gdata, Layer& layer) {  }
			inline virtual void onMove(GameData& gdata, eDirection dir) {  }

		private:
			Entity* m_parent;
	};
}

#endif
