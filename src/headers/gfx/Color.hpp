#ifndef __COLOR__H__
#define __COLOR__H__

#include <SFML/Graphics/Color.hpp>
#include "String.hpp"
#include "Object.hpp"

namespace Omnia
{
	typedef struct RgbColor
	{
		inline RgbColor(void) : r(0), g(0), b(0), a(0) {}
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} RgbColor;

	typedef struct HsvColor
	{
		inline HsvColor(void) : h(0), s(0), v(0), a(0) {}
		unsigned char h;
		unsigned char s;
		unsigned char v;
		unsigned char a;
	} HsvColor;

	class Color : public Interrogable
	{
		private:
			RgbColor hsvToRgb(HsvColor hsv);
			HsvColor rgbToHsv(RgbColor rgb);
			void convertToHsv(void);

		public:
			inline Color(void) : _r(0), _g(0), _b(0), _a(255), converted(false), m_alphaPercent(1.f) {}
			inline Color(unsigned char rgb) : _r(rgb), _g(rgb), _b(rgb), _a(255), converted(false), m_alphaPercent(1.f)  {}
			inline Color(unsigned char rgb, unsigned char a) : _r(rgb), _g(rgb), _b(rgb), _a(a), converted(false), m_alphaPercent(1.f) {}
			inline Color(unsigned char r, unsigned char g, unsigned char b) : _r(r), _g(g), _b(b), _a(255), converted(false), m_alphaPercent(1.f) {}
			inline Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : _r(r), _g(g), _b(b), _a(a), converted(false), m_alphaPercent(1.f)  {}

			inline void set_r(unsigned char r) { _r = r; converted = false; }
			inline void set_g(unsigned char g) { _g = g; converted = false; }
			inline void set_b(unsigned char b) { _b = b; converted = false; }
			inline void set_a(unsigned char a) { _a = a; converted = false; }

			inline unsigned char r(void) { return _r; }
			inline unsigned char g(void) { return _g; }
			inline unsigned char b(void) { return _b; }
			inline unsigned char a(void) { return _a; }

			inline void setAlphaPercent(float percent) { m_alphaPercent = CAPB(percent, 0.0f, 1.0f); }
			inline float getAlphaPercent(void) { return m_alphaPercent; }
			inline Color getShaded(void) { return Color(_r, _g, _b, (unsigned char)(_a * m_alphaPercent)); }
			inline Color getShaded(float shade) { return Color(_r, _g, _b, (unsigned char)(_a * CAPB(shade, 0.0f, 1.0f))); }

			inline sf::Color sf(void) { return sf::Color(_r, _g, _b, _a); }

			bool operator==(Color& col2);

			void setColor(unsigned char rgb);
			void setColor(unsigned char rgb, unsigned char a);
			void setColor(unsigned char r, unsigned char g, unsigned char b);
			void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
			void setColor(HsvColor col);

			unsigned char h(void);
			unsigned char s(void);
			unsigned char v(void);

			RgbColor asRgb(void);
			HsvColor asHsv(void);

			String toString(void);

			String getMember(String memberName, eNamespaceMemberType& outType);
			bool setMember(String memberName, String value);

		private:
			unsigned char _r;
			unsigned char _g;
			unsigned char _b;
			unsigned char _a;
			bool converted;
			HsvColor hsv;
			float m_alphaPercent;

		public:
			static const Color Red;
			static const Color DarkRed;
			static const Color Green;
			static const Color DarkGreen;
			static const Color Blue;
			static const Color DarkBlue;
			static const Color White;
			static const Color Black;
			static const Color Gray;
			static const Color DarkGray;
			static const Color Yellow;
			static const Color Olive;
			static const Color Gold;
			static const Color Aqua;
			static const Color DarkAqua;
			static const Color Pink;
			static const Color Purple;
			static const Color Orange;
			static const Color Transparent;
	};
}
#endif
