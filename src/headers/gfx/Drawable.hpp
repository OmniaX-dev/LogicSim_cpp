#ifndef __DRAWABLE__H__
#define __DRAWABLE__H__

#include <math.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "DataTypes.hpp"

namespace Omnia
{
	class Drawable
	{
		public:
			inline Drawable(void) : x(0), y(0), w(0), h(0) {}
			inline Drawable(float xx, float yy, float ww, float hh) : x(xx), y(yy), w(ww), h(hh) {}

			inline float& getx(void) { return x; }
			inline float& gety(void) { return y; }
			inline float& getw(void) { return w; }
			inline float& geth(void) { return h; }
			inline float getCenterX(void) { return getx() + getw() / 2; }
			inline float getCenterY(void) { return gety() + geth() / 2; }

			inline void setx(float xx) { x = xx; }
			inline void sety(float yy) { y = yy; }
			inline void setw(float ww) { w = ww; }
			inline void seth(float hh) { h = hh; }

			inline FPoint getPosition(void) { return FPoint(getx(), gety()); }
			inline FPoint getSize(void) { return FPoint(getw(), geth()); }
			inline FRect getBounds(void) { return FRect(getx(), gety(), getw(), geth()); }
			inline FPoint getCenter(void) { return FPoint(getx() + getw() / 2, gety() + geth() / 2); }

			inline void setPosition(FPoint pos) { setx(pos.x); sety(pos.y); }
			inline void setPosition(float xx, float yy) { setx(xx); sety(yy); }
			inline void setSize(FPoint size) { setw(size.x); seth(size.y); }
			inline void setSize(float ww, float hh) { setw(ww); seth(hh); }
			inline void setBounds(FRect bounds) { setx(bounds.x); sety(bounds.y); setw(bounds.w); seth(bounds.h); }
			inline void setBounds(float xx, float yy, float ww, float hh) { setx(xx); sety(yy); setw(ww); seth(hh); }

			inline float addx(float xx) { setx(getx() + xx); return getx(); }
			inline float addy(float yy) { sety(gety() + yy); return gety(); }
			inline FPoint addPos(float xx, float yy) { return FPoint(addx(xx), addy(yy)); }
			inline FPoint addPos(FPoint pos) { return addPos(pos.x, pos.y); }
			inline float addw(float ww) { setw(getw() + ww); return getw(); }
			inline float addh(float hh) { seth(geth() + hh); return geth(); }
			inline FPoint addSize(float ww, float hh) { return FPoint(addw(ww), addh(hh)); }
			inline FPoint addSize(FPoint size) { return addPos(size.x, size.y); }

			inline float subx(float xx) { setx(getx() - xx); return getx(); }
			inline float suby(float yy) { sety(gety() - yy); return gety(); }
			inline FPoint subPos(float xx, float yy) { return FPoint(subx(xx), suby(yy)); }
			inline FPoint subPos(FPoint pos) { return subPos(pos.x, pos.y); }
			inline float subw(float ww) { setw(getw() - ww); return getw(); }
			inline float subh(float hh) { seth(geth() - hh); return geth(); }
			inline FPoint subSize(float ww, float hh) { return FPoint(subw(ww), subh(hh)); }
			inline FPoint subSize(FPoint size) { return subPos(size.x, size.y); }

			inline float mulx(float xx) { setx(getx() * xx); return getx(); }
			inline float muly(float yy) { sety(gety() * yy); return gety(); }
			inline FPoint mulPos(float xx, float yy) { return FPoint(mulx(xx), muly(yy)); }
			inline FPoint mulPos(FPoint pos) { return mulPos(pos.x, pos.y); }
			inline float mulw(float ww) { setw(getw() * ww); return getw(); }
			inline float mulh(float hh) { seth(geth() * hh); return geth(); }
			inline FPoint mulSize(float ww, float hh) { return FPoint(mulw(ww), mulh(hh)); }
			inline FPoint mulSize(FPoint size) { return mulPos(size.x, size.y); }

			inline float divx(float xx) { setx(getx() / xx); return getx(); }
			inline float divy(float yy) { sety(gety() / yy); return gety(); }
			inline FPoint divPos(float xx, float yy) { return FPoint(divx(xx), divy(yy)); }
			inline FPoint divPos(FPoint pos) { return divPos(pos.x, pos.y); }
			inline float divw(float ww) { setw(getw() / ww); return getw(); }
			inline float divh(float hh) { seth(geth() / hh); return geth(); }
			inline FPoint divSize(float ww, float hh) { return FPoint(divw(ww), divh(hh)); }
			inline FPoint divSize(FPoint size) { return divPos(size.x, size.y); }

			inline FPoint topLeft(void) { return getPosition(); }
			inline FPoint topRight(void) { return FPoint(getx() + getw(), gety()); }
			inline FPoint bottomLeft(void) { return FPoint(getx(), gety() + geth()); }
			inline FPoint bottomRight(void) { return FPoint(getx() + getw(), gety() + geth()); }

			inline bool contains(float xx, float yy) { return xx > getx() && xx < bottomRight().x && yy > gety() && yy < bottomRight().y;  }
			inline bool contains(FPoint p) { return contains(p.x, p.y); }

			inline float getDistance(FPoint p) { return sqrt(fabs((p.x - getx()) * (p.x - getx()) + (p.y - gety()) * (p.y - gety()))); }

		protected:
			float x;
			float y;
			float w;
			float h;
	};
}
#endif
