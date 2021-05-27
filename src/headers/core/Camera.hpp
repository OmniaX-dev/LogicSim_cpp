#ifndef __CAMERA__H__
#define __CAMERA__H__

#include "DataTypes.hpp"

namespace Omnia
{
	class Camera : public Object
	{
		public:
			inline Camera(void)
			{
				m_viewX = 0;
				m_viewY = 0;
				setTypeName("Camera");
				setMemberNamespace(StringBuilder("camera").add(getObjectID()).get());
				addMember("view.x", eNamespaceMemberType::Integer);
				addMember("view.y", eNamespaceMemberType::Integer);
				addMember("max.x", eNamespaceMemberType::Integer);
				addMember("max.y", eNamespaceMemberType::Integer);
			}
			inline void set(uint32 mvx, uint32 mvy) { m_viewX = (signed)(ZERO(mvx)); m_viewY = (signed)(ZERO(mvy)); normalize(); }
			inline void setHorizontal(uint32 mvx) { m_viewX = (signed)(ZERO(mvx)); normalize(); }
			inline void setVertical(uint32 mvy) { m_viewY = (signed)(ZERO(mvy)); normalize(); }
			inline void add(int32 dmvx, int32 dmvy) { m_viewX += dmvx; m_viewY += dmvy; normalize(); }
			inline void addHorizontal(int32 dmvx) { m_viewX += dmvx; normalize(); }
			inline void addVertical(int32 dmvy) { m_viewY += dmvy; normalize(); }
			inline uint32 getHorizontal(void) { return (unsigned)m_viewX; }
			inline uint32 getVertical(void) { return (unsigned)m_viewY; }
			inline float convertX(float x) { return x - m_viewX; }
			inline float convertY(float y) { return y - m_viewY; }
			inline FPoint convert(float x, float y) { return FPoint(x - m_viewX, y - m_viewY); }
			inline IPoint getTileOffset(IPoint tileSize) { return IPoint(getHorizontal() / tileSize.x, getVertical() / tileSize.y); }
			inline void normalize(void)
			{
				m_viewX = ZERO(m_viewX);
				m_viewY = ZERO(m_viewY);
				if (m_viewX > m_max.x)
					m_viewX = m_max.x;
				if (m_viewY > m_max.y)
					m_viewY = m_max.y;
			}
			inline void setMax(UI16Point max) { m_max = max; }
			inline UI16Point getMax(void) { return m_max; }
			inline virtual String toString(void)
			{
				String str = "Camera: (";
				str = str.addInt(m_viewX);
				str = str.add(", ");
				str = str.addInt(m_viewY);
				str = str.add(")");
				return str;
			}
			inline String getMember(String name, eNamespaceMemberType& outType)
			{
				if (!hasMember(name)) return NS_MEMBER_NOT_FOUND;
				outType = eNamespaceMemberType::Integer;
				if (name == "view.x")
					return StringBuilder(m_viewX).get();
				if (name == "view.y")
					return StringBuilder(m_viewY).get();
				if (name == "max.x")
					return StringBuilder(m_max.x).get();
				if (name == "max.y")
					return StringBuilder(m_max.y).get();
				return NS_MEMBER_NOT_FOUND;
			}

			inline bool setMember(String name, String value)
			{
				if (!hasMember(name) || !value.isNumeric()) return false;
				if (name == "view.x")
				{
					m_viewX = value.toInt();
					return true;
				}
				if (name == "view.y")
				{
					m_viewY = value.toInt();
					return true;
				}
				if (name == "max.x")
				{
					m_max.x = value.toInt();
					return true;
				}
				if (name == "max.y")
				{
					m_max.y = value.toInt();
					return true;
				}
				return false;
			}

		private:
			int32 m_viewX;
			int32 m_viewY;
			UI16Point m_max;
	};
}
#endif
