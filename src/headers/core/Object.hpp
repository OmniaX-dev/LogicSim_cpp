#ifndef __OBJECT__HPP__
#define __OBJECT__HPP__

#include <utility>
#include "String.hpp"
#include <sstream>
#include "Enums.hpp"
#include "Defines.hpp"

namespace Omnia
{
	class Identifiable
	{
		public:
			inline Identifiable(void) { m_uid = -1; }
			inline int getID(void) const { return m_uid; }
		protected:
			inline void setID(int id) { m_uid = id; }
		private:
			int m_uid;
			
			friend class ONSPool;
	};

	class Validable
	{
		public:
			inline Validable(void) { m_valid = true; }
			inline bool isValid(void) const { return m_valid; }
			inline bool isInvalid(void) const { return !isValid(); }
			inline void invalidate(void) { m_valid = false; }
			inline void setValid(bool valid) { m_valid = valid; }
		private:
			bool m_valid;

			friend class ONSPool;
	};

	class Comparable
	{
		public:
			inline Comparable(void) { m_oid = -1; }
			virtual inline bool operator==(Comparable& obj) {
				return m_oid == obj.m_oid;
			}
			virtual inline bool operator!=(Comparable& obj) {
				return m_oid != obj.m_oid;
			}
			inline const long int getObjectID(void) { return std::as_const(m_oid); }

		private:
			long int m_oid;
			inline static long int s_curr_oid = 0;

			friend class Object;
			friend class ONSPool;
	};

	class Printable
	{
		public:
			virtual inline String toString(void) { return "Printable::toString()"; }
			virtual inline void print(bool newLine = true) { std::cout << toString().cpp() << (newLine ? "\n" : ""); }
			
			friend class ONSPool;
	};

	class ObjectTypeInfo
	{
		public:
			inline ObjectTypeInfo(void) { m_typeName = "UNKNOWN"; }
			inline void setTypeName(String tn) { m_typeName = tn; }
			inline String getTypeName(void) { return m_typeName; }

		private:
			String m_typeName;
	};

	class Interrogable : public ObjectTypeInfo
	{
		public: struct Member
		{
			public:
				String name;
				eNamespaceMemberType type;

				inline Member(void) : name(""), type(eNamespaceMemberType::Null) {  }
				inline Member(String n, eNamespaceMemberType t) : name(n), type(t) {  }
				inline bool equals(Member member) { return member.name == name && member.type == type; }
		};
		public:
			virtual inline String getMember(String memberName, eNamespaceMemberType& outType) { outType = eNamespaceMemberType::Null; return NS_GET_METHOD_NOT_OVERLOADED; }
			virtual inline bool setMember(String memberName, String value) { return false; }
			inline unsigned int memberCount(void) { return m_members.size(); }
			bool hasMember(String memberName);
			bool hasMember(Member member);
			inline String getMemberNamespace(void) { return m_memberNamespace; }
			inline void setMemberNamespace(String mns) { m_memberNamespace = mns; }
			inline void addMember(Member member) { m_members.push_back(member); }
			inline void addMember(String name, eNamespaceMemberType type) { m_members.push_back(Member(name, type)); }
			bool removeMember(String name);
			inline std::vector<Member>& getMemberList(void) { return m_members; }

		private:
			std::vector<Member> m_members;
			String m_memberNamespace;

			friend class ONSPool;
	};

	class Object : public Identifiable, public Validable, public Comparable, public Printable, public Interrogable
	{
	    public:
	        inline Object(void) { m_oid = Comparable::s_curr_oid++; }
	        virtual inline bool equals(Object& obj) { return *this == obj; }
	        friend std::ostream& operator<<(std::ostream& os, Object& obj);
			virtual String getObjectHeader(void);
			virtual inline String toString(void) { return getObjectHeader(); }
			inline virtual ~Object(void) = default;

	    private:
	        inline Object(bool invalid) { if (invalid) invalidate(); }

	    public:
	        static Object Invalid;
			friend class ONSPool;
	};

	class ONSPool
	{
		public:
			void addObject(Object* ptr);
			Object& getByNamespae(String ns);
			std::vector<Object*>& list(void);
			bool removeObject(Object* ptr);
			bool hasNamespace(String ns);

			inline static ONSPool& instance(void) { return *(s_instance); }

		private:
			inline ONSPool(void) { }

		private:
			std::vector<Object*> m_pool;
			static ONSPool* s_instance;

			friend class Object;
	};

	inline ONSPool* ONSPool::s_instance = new ONSPool();

	template<
    	typename T,
    	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	>
	class TypedObject : public Object
	{
		public:
			inline TypedObject(void) { val = 0; precision = 2; }
			inline TypedObject(T v) { val = v; precision = 2; }
			virtual inline String toString(void) const 
			{
				std::ostringstream out;
				out.precision(precision);
				out << std::fixed << val;
				return String(out.str());
			}
			virtual inline TypedObject<T>& operator=(T v) { val = v; return *this; }
			virtual inline TypedObject<T>& operator=(TypedObject<T>& v) { val = v.val; return *this; }

			virtual inline bool operator==(const TypedObject<T>& o) { return o.val == val; }
			virtual inline bool operator!=(const TypedObject<T>& o) { return o.val != val; }
			virtual inline bool operator<=(const TypedObject<T>& o) { return val <= o.val; }
			virtual inline bool operator>=(const TypedObject<T>& o) { return val >= o.val; }
			virtual inline bool operator<(const TypedObject<T>& o) { return val < o.val; }
			virtual inline bool operator>(const TypedObject<T>& o) { return val > o.val; }

			virtual inline bool operator==(const T& o) { return val == o; }
			virtual inline bool operator!=(const T& o) { return val != o; }
			virtual inline bool operator<=(const T& o) { return val <= o; }
			virtual inline bool operator>=(const T& o) { return val >= o; }
			virtual inline bool operator<(const T& o) { return val < o; }
			virtual inline bool operator>(const T& o) { return val > o; }

			virtual inline TypedObject<T> operator+(const TypedObject<T>& o) { return TypedObject<T>(val + o.val); }
			virtual inline TypedObject<T> operator-(const TypedObject<T>& o) { return TypedObject<T>(val - o.val); }
			virtual inline TypedObject<T> operator*(const TypedObject<T>& o) { return TypedObject<T>(val * o.val); }
			virtual inline TypedObject<T> operator/(const TypedObject<T>& o) { return TypedObject<T>(val / o.val); }

			virtual inline TypedObject<T> operator+(const T& o) { return TypedObject<T>(val + o); }
			virtual inline TypedObject<T> operator-(const T& o) { return TypedObject<T>(val - o); }
			virtual inline TypedObject<T> operator*(const T& o) { return TypedObject<T>(val * o); }
			virtual inline TypedObject<T> operator/(const T& o) { return TypedObject<T>(val / o); }

			T val;
			int precision;
	};

	typedef TypedObject<int> Integer;
	typedef TypedObject<float> Float;
	typedef TypedObject<double> Double;

	class OString : public String, public Object
	{
		public:
			inline OString(void) { string = ""; }
			inline OString(_string str) { string = str; }
			inline OString(String str) { string = str.cpp(); }
			inline virtual String toString(void) { return String(cpp()); }
	};

	template<
    	typename T,
    	typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
	>
	class TypedPtr : public Object
	{
		public:
			T* ptr;
			TypedObject<T>* object;
			inline TypedPtr(void) { ptr = nullptr; object = nullptr; setValid(false); }
			inline TypedPtr(T& value) { ptr = &value; object = nullptr; setValid(true); }
			inline TypedObject<T>& asObject(void) { if (object != nullptr) delete object; object = new TypedObject<T>(*ptr); return *object; }
			virtual inline String toString(void) { if (isInvalid()) return "Invalid"; return asObject().toString(); }
	};

	typedef TypedPtr<int> IntPtr;
	typedef TypedPtr<float> FloatPtr;
	typedef TypedPtr<double> DoublePtr;
}

#endif
