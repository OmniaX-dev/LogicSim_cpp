#include "GameEntities.hpp"
#include "GameData.hpp"

namespace Omnia
{
    void Player::create(Animation anim, FPoint size)
    {
        m_cc = ControllerComponent(*this);
        m_asc = AnimationSelectorComponent(*this, anim.getAnimationData());
        m_camc = CameraComponent(*this);
        m_colc = CollisionComponent(*this, eCollisionType::AABB);
        addComponent(m_cc);
        addComponent(m_asc);
        addComponent(m_camc);
        addComponent(m_colc);
        setSize(size);
        setBoundingBox(FRect(1, 1, size.x - 2, size.y - 2));
    #ifdef __DEBUG__
    #ifdef __CREATE_DEFAULT_DBG_VIEW__
        //GameData::instance().debugView.attachPtr(*this, "", Color::Pink);
    #endif
    #endif
    }

    String Player::toString(void)
    {
        String str = "Player position: (";
        str = str.addInt((int32)getx());
        str = str.add(", ");
        str = str.addInt((int32)gety());
        str = str.add(")");
        return str;
    }
}