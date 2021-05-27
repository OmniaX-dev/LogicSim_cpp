#include "Object.hpp"
#include "Defines.hpp"
#include "DataTypes.hpp"

namespace Omnia
{
	Object Object::Invalid = Object(__INVALID_OBJ);
    
    String Object::getObjectHeader(void)
    {
		StringBuilder sb(getTypeName());
		sb.add(String(": oid="));
		sb.add(m_oid);
		sb.add(String(", valid="));
		sb.add(String(isValid() ? "true" : "false"));
		sb.add(String(", uid="));
		sb.add(getID());
		sb.add(String(", ons="));
		sb.add(getMemberNamespace());
		return sb.get();
    }

	std::ostream& operator<<(std::ostream& os, Object& obj)
	{
		os << obj.toString().cpp();
		return os;
	}

	bool Interrogable::hasMember(Member member)
	{
		for (auto& m : m_members)
			if (m.equals(member)) return true;
		return false;
	}

	bool Interrogable::hasMember(String member)
	{
		for (auto& m : m_members)
			if (m.name == member) return true;
		return false;
	}

	bool Interrogable::removeMember(String name)
	{
		for (uint16 i = 0; i < m_members.size(); i++)
		{
			Member& m = m_members[i];
			if (m.name == name)
			{
				STDVEC_REMOVE(m_members, i);
				return true;
			}
		}
		return false;
	}

	void ONSPool::addObject(Object* ptr)
	{
		m_pool.push_back(ptr);
	}

	std::vector<Object*>& ONSPool::list(void)
	{
		return m_pool;
	}
	
	Object& ONSPool::getByNamespae(String ns)
	{
		for (auto& o : m_pool)
		{
			if (o->getMemberNamespace() == ns)
				return *o;
		}
		return Object::Invalid;
	}

	bool ONSPool::removeObject(Object* ptr)
	{
		for (uint16 i = 0; i < m_pool.size(); i++)
		{
			if (ptr->getObjectID() == m_pool[i]->getObjectID())
			{
				STDVEC_REMOVE(m_pool, i);
				return true;
			}
		}
		return false;
	}
	
	bool ONSPool::hasNamespace(String ns)
	{
		for (auto& o : m_pool)
		{
			if (o->getMemberNamespace() == ns)
				return true;
		}
		return false;
	}
}
