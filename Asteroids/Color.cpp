#include "Color.h"


ext::Color::Color() :
	integer(0x000000FF)
{
}

ext::Color::Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) :
	r(red),
	g(green),
	b(blue),
	a(alpha)
{
}

ext::Color::Color(float red, float green, float blue, float alpha) :
	r((Uint8)std::round(red * 255.f)),
	g((Uint8)std::round(green * 255.f)),
	b((Uint8)std::round(blue * 255.f)),
	a((Uint8)std::round(alpha * 255.f))
{
}

ext::Color::Color(const Uint32& color) :
	integer(color)
{
}

ext::Color::Color(const HSPColor& color)
{
	*this = HSPtoRGB(color);
}

ext::Color::Color(const sf::Color& c) :
	integer(c.toInteger())
{
}

ext::HSPColor ext::Color::toHSP() const
{
	return RGBtoHSP(*this);
}

ext::Color ext::Color::blend(const Color& other, float threshold) const
{
	const auto& t = threshold;
	Color ret;
	ret.r = blendColorVal(r, other.r, t);
	ret.g = blendColorVal(g, other.g, t);
	ret.b = blendColorVal(b, other.b, t);
	ret.a = blendAlphaVal(a, other.a, t);
	return ret;
}

ext::Uint8 ext::Color::blendColorVal(Uint8 a, Uint8 b, float t) const
{
	return (Uint8)std::sqrt((1 - t) * (a * a) + t * (b * b));
}

ext::Uint8 ext::Color::blendAlphaVal(Uint8 a, Uint8 b, float t) const
{
	return Uint8((1.f - t) * a + t * b);
}

ext::HSPColor::HSPColor() :
	h(0), s(0), b(0), a(1.f)
{
}

ext::HSPColor::HSPColor(float hue, float saturation, float brightness, float alpha) :
	h(hue), s(saturation), b(brightness), a(alpha)
{
}

ext::HSPColor::HSPColor(const Color& color)
{
	*this = RGBtoHSP(color);
}

ext::Color ext::HSPColor::toRGB() const
{
	return HSPtoRGB(*this);
}

bool ext::operator==(ext::Color lhs, ext::Color rhs) noexcept
{
	return lhs.integer == rhs.integer;
}

bool ext::operator!=(ext::Color lhs, ext::Color rhs) noexcept
{
	return !(lhs == rhs);
}

ext::Color ext::operator+(const Color& left, const Color& right)
{
	return Color(Uint8(std::min(int(left.r) + right.r, 255)),
		Uint8(std::min(int(left.g) + right.g, 255)),
		Uint8(std::min(int(left.b) + right.b, 255)),
		Uint8(std::min(int(left.a) + right.a, 255)));
}

ext::Color ext::operator-(const Color& left, const Color& right)
{
	return Color(Uint8(std::max(int(left.r) - right.r, 0)),
		Uint8(std::max(int(left.g) - right.g, 0)),
		Uint8(std::max(int(left.b) - right.b, 0)),
		Uint8(std::max(int(left.a) - right.a, 0)));
}

ext::Color& ext::operator+=(Color& left, const Color& right)
{
	return left = left + right;
}

ext::Color& ext::operator-=(Color& left, const Color& right)
{
	return left = left - right;
}

ext::Color ext::HSPtoRGB(float hue, float saturation, float brightness, float alpha)
{
	const float&
		Pr = HSPColor::Pr,
		Pg = HSPColor::Pg,
		Pb = HSPColor::Pb;

	//Check values are within expected range
	hue = hue < 0 ? 0 : hue > 360.f ? 360.f : hue;
	saturation = saturation < 0 ? 0 : saturation > 1.f ? 1.f : saturation;
	brightness = brightness < 0 ? 0 : brightness > 1.f ? 1.f : brightness;
	alpha = alpha < 0 ? 0 : alpha > 1.f ? 1.f : alpha;
	float minOverMax = 1.f - saturation;
	float R, G, B;
	if (minOverMax > 0) {
		float part;
		if (hue < (1.f / 6.f)) { //R>G>B
			hue = 6.f * (hue - 0); part = 1.f + hue * (1.f / minOverMax - 1.f);
			B = brightness / std::sqrt(Pr / minOverMax / minOverMax + Pg * part * part + Pb);
			R = B / minOverMax;
			G = B + hue * (R - B);
		}
		else if (hue < (2.f / 6.f)) { //G>R>B
			hue = 6.f * (-hue + (2.f / 6.f)); part = 1.f + hue * (1.f / minOverMax - 1.f);
			B = brightness / std::sqrt(Pg / minOverMax / minOverMax + Pr * part * part + Pb);
			G = B / minOverMax;
			R = B + hue * (G - B);
		}
		else if (hue < (3.f / 6.f)) {   //  G>B>R
			hue = 6.f * (hue - (2.f / 6.f)); part = 1.f + hue * (1.f / minOverMax - 1.f);
			R = brightness / std::sqrt(Pg / minOverMax / minOverMax + Pb * part * part + Pr);
			G = R / minOverMax;
			B = R + hue * (G - R);
		}
		else if (hue < (4.f / 6.f)) { //B>G>R
			hue = 6.f * (-hue + (4.f / 6.f)); part = 1.f + hue * (1.f / minOverMax - 1.f);
			R = brightness / std::sqrt(Pb / minOverMax / minOverMax + Pg * part * part + Pr);
			B = R / minOverMax;
			G = R + hue * (B - R);
		}
		else if (hue < (5.f / 6.f)) { //B>R>G
			hue = 6.f * (hue - (4.f / 6.f)); part = 1.f + hue * (1.f / minOverMax - 1.f);
			G = brightness / std::sqrt(Pb / minOverMax / minOverMax + Pr * part * part + Pg);
			B = G / minOverMax;
			R = G + hue * (B - G);
		}
		else { //R>B>G
			hue = 6.f * (-hue + 1.f); part = 1.f + hue * (1.f / minOverMax - 1.f);
			G = brightness / std::sqrt(Pr / minOverMax / minOverMax + Pb * part * part + Pg);
			R = G / minOverMax;
			B = G + hue * (R - G);
		}
	}
	else {
		if (hue < (1.f / 6.f)) { //R>G>B
			hue = 6.f * (hue - 0);
			R = std::sqrt(brightness* brightness / (Pr + Pg * hue * hue));
			G = R * hue;
			B = 0;
		}
		else if (hue < (2.f / 6.f)) { //G>R>B
			hue = 6.f * (-hue + (2.f / 6.f));
			G = std::sqrt(brightness * brightness / (Pg + Pr * hue * hue));
			R = G * hue;
			B = 0;
		}
		else if (hue < (3.f / 6.f)) { //G>B>R
			hue = 6.f * (hue - (2.f / 6.f));
			G = std::sqrt(brightness * brightness / (Pg + Pb * hue * hue));
			B = G * hue;
			R = 0;
		}
		else if (hue < (4.f / 6.f)) { //B>G>R
			hue = 6.f * (-hue + (4.f / 6.f));
			B = std::sqrt(brightness * brightness / (Pb + Pg * hue * hue));
			G = B * hue;
			R = 0;
		}
		else if (hue < (5.f / 6.f)) { //B>R>G
			hue = 6.f * (hue - (4.f / 6.f));
			B = std::sqrt(brightness * brightness / (Pb + Pr * hue * hue));
			R = B * hue;
			G = 0;
		}
		else { //R>B>G
			hue = 6.f * (-hue + 1.f);
			R = std::sqrt(brightness * brightness / (Pr + Pb * hue * hue));
			B = R * hue;
			G = 0;
		}
	}
	return Color(R, G, B, alpha);
}

ext::Color ext::HSPtoRGB(const HSPColor& color) {
	return HSPtoRGB(color.h, color.s, color.p, color.a);
}

ext::HSPColor ext::RGBtoHSP(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	HSPColor ret;
	const float&
		Pr = HSPColor::Pr,
		Pg = HSPColor::Pg,
		Pb = HSPColor::Pb;

	const float
		r = (float)red / 255.f,
		g = (float)green / 255.f,
		b = (float)blue / 255.f;

	ret.a = alpha;
	//  Calculate the Perceived brightness.
	ret.p = std::sqrt(r * r * Pr + g * g * Pg + b * b * Pb);

	//  Calculate the Hue and Saturation.  (This part works
	//  the same way as in the HSV/b and HSL systems???.)
	float max = std::max(std::max(r, g), b);
	float min = std::min(std::min(r, g), b);
	if (max == min) {
		ret.h = 0; ret.s = 0;
		return ret;
	}
	if (r == max) {   //  r is largest
		if (g == min) {
			ret.h = (6.f / 6.f) - (1.f / 6.f) * (b - g) / (r - g); ret.s = 1.f - g / r;
		}
		else {
			ret.h = (0.f / 6.f) + (1.f / 6.f) * (g - b) / (r - b); ret.s = 1.f - b / r;
		}
	}
	else if (g == max) {   //  g is largest
		if (b == min) {
			ret.h = (2.f / 6.f) - (1.f / 6.f) * (r - b) / (g - b); ret.s = 1.f - b / g;
		}
		else {
			ret.h = (2.f / 6.f) + (1.f / 6.f) * (b - r) / (g - r); ret.s = 1.f - r / g;
		}
	}
	else {   //  b is largest
		if (r == min) {
			ret.h = (4.f / 6.f) - (1.f / 6.f) * (g - r) / (b - r); ret.s = 1.f - r / b;
		}
		else {
			ret.h = (4.f / 6.f) + (1.f / 6.f) * (r - g) / (b - g); ret.s = 1.f - g / b;
		}
	}
	return ret;
}

ext::HSPColor ext::RGBtoHSP(const ext::Color& color) {
	return RGBtoHSP(color.r, color.g, color.b, color.a);
}
