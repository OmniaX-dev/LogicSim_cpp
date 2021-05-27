#include "Entity.hpp"
#include "GameData.hpp"

namespace Omnia
{
	void Entity::create(void)
	{
		m_direction = eDirection::None;
		m_collideCondition = new_sh(BasicCollideCondition)();
		setValid(true);
	#ifdef __DEBUG__
		//m_dbgLayer = GameData::instance().globalLayers.addLayer(AUTO__LAYER_RENDER_INDEX);
		//TODO: create a layer for collisions
	#endif
	}

	void Entity::update(GameData& gdata)
	{
		for (auto c : m_components)
			c->update(gdata);
		onUpdate(gdata);
	}

	void Entity::render(GameData& gdata, Layer& layer)
	{
		for (auto c : m_components)
			c->render(gdata, layer);
		onRender(gdata, layer);
	#ifdef __DEBUG__
		//if (Flags::isset(FLG_SHOW_COLLISION_RECTS))
		//	m_dbgLayer.get().addQuad(getBoundingBox(), Color(255, 0, 0, 150));
		//	m_dbgLayer.get().addQuad(getSprCollBox(), Color(255, 0, 0, 150));
		//TODO: Fix with new collision layer
	#endif
	}

	void Entity::onMove(GameData& gdata, eDirection dir)
	{
		for (auto c : m_components)
			c->onMove(gdata, dir);
	}

	void Entity::onEvent(sf::Event& evt, GameData& gdata)
	{
		for (auto c : m_components)
			c->handleEvents(evt, gdata);
	}

	void Entity::addComponent(EntityComponent& component)
	{
		if (component.isValid())
			m_components.push_back(&component);
	#ifdef __DEBUG__
		GameData::instance().debugConsole.system(StringBuilder("add_component: entity=").add(getID()).add("; component=").add(component.getID()).get());
	#endif
	}

	void Entity::handleCollision(eCollisionLevel ecl, FPoint margin)
	{
		if (m_collideCondition->collides(ecl, *this))
			addPos(-margin.x, -margin.y);
	}
}
