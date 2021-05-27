#include "EntityComponents.hpp"
#include "LayerRenderer.hpp"
#include "Animation.hpp"
#include "GameData.hpp"
#include "Application.hpp"
#include "World.hpp"

namespace Omnia
{
	ControllerComponent::ControllerComponent(Entity& parent) : EntityComponent(parent)
	{
		m_direction = eDirection::None;
		m_cUp.bind(SF_KEY(W));
		m_cDown.bind(SF_KEY(S));
		m_cLeft.bind(SF_KEY(A));
		m_cRight.bind(SF_KEY(D));
	}
	void ControllerComponent::update(GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (isKeyPressed())
		{
			switch (m_direction)
			{
				case eDirection::Up:
					if (m_cUp.contains(getPressedKeyCode()))
					{
						getParent().suby(getParent().getStats().speed);
						getParent().onMove(gdata, eDirection::Up);
					}
					break;
				case eDirection::Down:
					if (m_cDown.contains(getPressedKeyCode()))
					{
						getParent().addy(getParent().getStats().speed);
						getParent().onMove(gdata, eDirection::Down);
					}
					break;
				case eDirection::Right:
					if (m_cRight.contains(getPressedKeyCode()))
					{
						getParent().addx(getParent().getStats().speed);
						getParent().onMove(gdata, eDirection::Right);
					}
					break;
				case eDirection::Left:
					if (m_cLeft.contains(getPressedKeyCode()))
					{
						getParent().subx(getParent().getStats().speed);
						getParent().onMove(gdata, eDirection::Left);
					}
					break;
				default:
					break;
			}
		}
	}

	void ControllerComponent::onEvent(sf::Event& evt, GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (m_cUp.isPressed(evt))
			m_direction = eDirection::Up;
		else if (m_cLeft.isPressed(evt))
			m_direction = eDirection::Left;
		else if (m_cDown.isPressed(evt))
			m_direction = eDirection::Down;
		else if (m_cRight.isPressed(evt))
			m_direction = eDirection::Right;
	}


	SimpleAnimationComponent::SimpleAnimationComponent(Entity& parent, Animation& anim) : EntityComponent(parent)
	{
		m_animation = &anim;
		parent.setSize(m_animation->getSize());
	}

	void SimpleAnimationComponent::render(GameData& gdata, Layer& layer)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (m_animation == nullptr)
		{
			gdata.errorQueue.push(Error::NULL_ANIMATION_POINTER, ERROR_DATA());
			return;
		}
		if (layer.isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_LAYER, ERROR_DATA());
			return;
		}
		layer.renderAnimation(gdata, *m_animation, getParent().getPosition(), m_animation->getTintColor());
	}

	void SimpleAnimationComponent::update(GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (m_animation == nullptr)
		{
			gdata.errorQueue.push(Error::NULL_ANIMATION_POINTER, ERROR_DATA());
			return;
		}
		m_animation->setSize(getParent().getSize());
		m_animation->update(gdata);
	}



	AnimationSelectorComponent::AnimationSelectorComponent(Entity& parent, AnimationData ad) : EntityComponent(parent)
	{
		SpriteFormatter& sf = ad.sf;
		int32 ro = ad.rowOffset;
		ad.rowOffset = ro + sf.up;
		m_upAnim = new_sh(Animation)(ad);
		ad.rowOffset = ro + sf.down;
		m_downAnim = new_sh(Animation)(ad);
		ad.rowOffset = ro + sf.left;
		m_leftAnim = new_sh(Animation)(ad);
		ad.rowOffset = ro + sf.right;
		m_rightAnim = new_sh(Animation)(ad);
		m_rightAnim->setTintColor(Color::Green);

		m_up = m_animSelec.add(*m_upAnim);
		m_down = m_animSelec.add(*m_downAnim);
		m_left = m_animSelec.add(*m_leftAnim);
		m_right = m_animSelec.add(*m_rightAnim);

		m_downAnim->setStill(true);
		m_animSelec.setCurrent(m_down);

		m_cUp.bind(SF_KEY(W));
		m_cDown.bind(SF_KEY(S));
		m_cLeft.bind(SF_KEY(A));
		m_cRight.bind(SF_KEY(D));
	}

	void AnimationSelectorComponent::render(GameData& gdata, Layer& layer)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		layer.renderAnimation(gdata, m_animSelec.current(), getParent().getPosition());
	}

	void AnimationSelectorComponent::update(GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		m_animSelec.current().update(gdata);
	}

	void AnimationSelectorComponent::keyPressed(sf::Event& evt, GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (m_cUp.isPressed(evt))
		{
			m_animSelec.setCurrent(m_up);
			getParent().setDirection(eDirection::Up);
			m_animSelec.current().setStill(false);
		}
		else if (m_cLeft.isPressed(evt))
		{
			m_animSelec.setCurrent(m_left);
			getParent().setDirection(eDirection::Left);
			m_animSelec.current().setStill(false);
		}
		else if (m_cDown.isPressed(evt))
		{
			m_animSelec.setCurrent(m_down);
			getParent().setDirection(eDirection::Down);
			m_animSelec.current().setStill(false);
		}
		else if (m_cRight.isPressed(evt))
		{
			m_animSelec.setCurrent(m_right);
			getParent().setDirection(eDirection::Right);
			m_animSelec.current().setStill(false);
		}
	}

	void AnimationSelectorComponent::keyReleased(sf::Event& evt, GameData& gdata)
	{
		if (isInvalid())
		{
			gdata.errorQueue.push(Error::INVALID_ENTITY_COMPONENT, ERROR_DATA());
			return;
		}
		if (evt.key.code == m_keyPressedCode)
			m_animSelec.current().setStill(true);
	}

	void CameraComponent::onMove(GameData& gdata, eDirection dir)
	{
		FPoint localPos = gdata.camera.convert(getParent().getx(), getParent().gety());
		IPoint ss = gdata.getScreenSize();
		switch (dir)
		{
			case eDirection::Left:
				if (localPos.x < m_screenMargin)
					gdata.camera.addHorizontal(-getParent().getStats().speed);
			break;
			case eDirection::Right:
				if (localPos.x > ss.x - m_screenMargin - getParent().getw())
					gdata.camera.addHorizontal(getParent().getStats().speed);
			break;
			case eDirection::Up:
				if (localPos.y < m_screenMargin)
					gdata.camera.addVertical(-getParent().getStats().speed);
			break;
			case eDirection::Down:
				if (localPos.y > ss.y - m_screenMargin - getParent().geth())
					gdata.camera.addVertical(getParent().getStats().speed);
			break;
			default:
			break;
		}
	}

	void CollisionComponent::onMove(GameData& gdata, eDirection dir)
	{
		World& world = gdata.application->world();
		if (world.isInvalid())
			return;
		FPoint margin;
		eCollisionLevel ect = world.entityCollides(getParent(), dir, m_ect, &margin);
		getParent().handleCollision(ect, margin);
	}

}
