#include "Color.hpp"
#include "DataTypes.hpp"
#include "StringTokens.hpp"
#include <sstream>

namespace Omnia
{
	const Color Color::Red = Color(255, 0, 0);
	const Color Color::DarkRed = Color(128, 0, 0);
	const Color Color::Green = Color(0, 255, 0);
	const Color Color::DarkGreen = Color(0, 128, 0);
	const Color Color::Blue = Color(0, 0, 255);
	const Color Color::DarkBlue = Color(0, 0, 128);
	const Color Color::White = Color(255, 255, 255);
	const Color Color::Black = Color(0, 0, 0);
	const Color Color::Gray = Color(192, 192, 192);
	const Color Color::DarkGray = Color(128, 128, 128);
	const Color Color::Yellow = Color(255, 255, 0);
	const Color Color::Olive = Color(128, 128, 0);
	const Color Color::Gold = Color(215, 165, 30);
	const Color Color::Aqua = Color(0, 255, 255);
	const Color Color::DarkAqua = Color(0, 128, 128);
	const Color Color::Pink = Color(255, 0, 255);
	const Color Color::Purple = Color(128, 0, 128);
	const Color Color::Orange = Color(255, 128, 0);
	const Color Color::Transparent = Color(0, 0, 0, 0);

	RgbColor Color::hsvToRgb(HsvColor hsv)
	{
		RgbColor rgb;
		unsigned char region, remainder, p, q, t;

		if (hsv.s == 0)
		{
			rgb.r = hsv.v;
			rgb.g = hsv.v;
			rgb.b = hsv.v;
			rgb.a = hsv.a;
			return rgb;
		}

		region = hsv.h / 43;
		remainder = (hsv.h - (region * 43)) * 6;

		p = (hsv.v * (255 - hsv.s)) >> 8;
		q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
		t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

		switch (region)
		{
			case 0:
				rgb.r = hsv.v; rgb.g = t; rgb.b = p;
				break;
			case 1:
				rgb.r = q; rgb.g = hsv.v; rgb.b = p;
				break;
			case 2:
				rgb.r = p; rgb.g = hsv.v; rgb.b = t;
				break;
			case 3:
				rgb.r = p; rgb.g = q; rgb.b = hsv.v;
				break;
			case 4:
				rgb.r = t; rgb.g = p; rgb.b = hsv.v;
				break;
			default:
				rgb.r = hsv.v; rgb.g = p; rgb.b = q;
				break;
		}
		rgb.a = hsv.a;
		return rgb;
	}

	HsvColor Color::rgbToHsv(RgbColor rgb)
	{
		HsvColor hsv;
		unsigned char rgbMin, rgbMax;

		rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
		rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

		hsv.v = rgbMax;
		if (hsv.v == 0)
		{
			hsv.h = 0;
			hsv.s = 0;
			hsv.a = rgb.a;
			return hsv;
		}

		hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
		if (hsv.s == 0)
		{
			hsv.h = 0;
			hsv.a = rgb.a;
			return hsv;
		}

		if (rgbMax == rgb.r)
			hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
		else if (rgbMax == rgb.g)
			hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
		else
			hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

		hsv.a = rgb.a;
		return hsv;
	}

	void Color::setColor(unsigned char rgb)
	{
		_r = _g = _b = rgb;
		_a = 255;
		converted = false;
	}

	void Color::setColor(unsigned char rgb, unsigned char a)
	{
		_r = _g = _b = rgb;
		_a = a;
		converted = false;
	}

	void Color::setColor(unsigned char r, unsigned char g, unsigned char b)
	{
		_r = r;
		_g = g;
		_b = b;
		_a = 255;
		converted = false;
	}

	void Color::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		_r = r;
		_g = g;
		_b = b;
		_a = a;
		converted = false;
	}

	void Color::setColor(HsvColor col)
	{
		RgbColor c = hsvToRgb(col);
		setColor(c.r, c.g, c.b, c.a);
	}

	unsigned char Color::h(void)
	{
		convertToHsv();
		return hsv.h;
	}

	unsigned char Color::s(void)
	{
		convertToHsv();
		return hsv.s;
	}

	unsigned char Color::v(void)
	{
		convertToHsv();
		return hsv.v;
	}

	bool Color::operator==(Color& col2)
	{
		return r() == col2.r() && g() == col2.g() && b() == col2.b() && a() == col2.a();
	}

	RgbColor Color::asRgb(void)
	{
		RgbColor col;
		col.r = r();
		col.g = g();
		col.b = b();
		col.a = a();
		return col;
	}

	HsvColor Color::asHsv(void)
	{
		convertToHsv();
		return hsv;
	}

	void Color::convertToHsv(void)
	{
		if (converted)
			return;
		RgbColor rgb;
		rgb.r = _r;
		rgb.g = _g;
		rgb.b = _b;
		rgb.a = _a;
		hsv = rgbToHsv(rgb);
		converted = true;
	}

	String Color::toString(void)
	{
		std::ostringstream out;
		out  << (int)_r << "," << (int)_g << "," << (int)_b << "," << (int)_a;
		return String(out.str()).trim();
	}

	String Color::getMember(String memberName, eNamespaceMemberType& outType)
	{
		if (!hasMember(memberName))
			return NS_MEMBER_NOT_FOUND;
		outType = eNamespaceMemberType::Integer;
		if (memberName.trim() == "red")   return StringBuilder(_r).get();
		if (memberName.trim() == "green") return StringBuilder(_g).get();
		if (memberName.trim() == "blue")  return StringBuilder(_b).get();
		if (memberName.trim() == "alpha") return StringBuilder(_a).get();
		if (memberName.trim() == "full")
		{
			outType = eNamespaceMemberType::Color;
			return toString();
		}
		outType = eNamespaceMemberType::Null;
		return NS_MEMBER_NOT_FOUND;
	}

	bool Color::setMember(String memberName, String value)
	{
		if (!hasMember(memberName)) return false;
		if (memberName.trim() == "red")
		{
			if (!value.isNumeric()) return false;
			_r = value.toInt();
			return true;
		}
		if (memberName.trim() == "green")
		{
			if (!value.isNumeric()) return false;
			_g = value.toInt();
			return true;
		}
		if (memberName.trim() == "blue")
		{
			if (!value.isNumeric()) return false;
			_b = value.toInt();
			return true;
		}
		if (memberName.trim() == "alpha")
		{
			if (!value.isNumeric()) return false;
			_a = value.toInt();
			return true;
		}
		if (memberName.trim() == "full")
		{
			StringTokens st = value.tokenize(",", true);
			if (st.count() != 4) return false;

			for (uint8 i = 0; i < st.count(); i++)
				if (!st.next().isNumeric()) return false;
			st.cycle();
			
			_r = st.next().toInt();
			_g = st.next().toInt();
			_b = st.next().toInt();
			_a = st.next().toInt();
			return true;
		}
		return false;
	}
}
