#pragma once

#include <SFML/Graphics/Color.hpp>

#include <algorithm>
#include <type_traits>
#include <cmath>

////////////////////////////////////////////////////////////
/// \brief Extended definitions for SFML
////////////////////////////////////////////////////////////
namespace ext
{
	typedef sf::Uint32 Uint32;
	typedef sf::Uint8 Uint8;

	struct Color;
	struct HSPColor;

	// Helper fuctions
	Color HSPtoRGB(float hue, float saturation, float brightness, float alpha = 1.f);
	Color HSPtoRGB(const HSPColor& color);
	HSPColor RGBtoHSP(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha = 255);
	HSPColor RGBtoHSP(const ext::Color& color);

	////////////////////////////////////////////////////////////
	/// \brief Extended RGBA color definitions
	////////////////////////////////////////////////////////////
	struct Color/* : public sf::Color*/
	{
		Color();
		Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
		Color(float red, float green, float blue, float alpha);
		Color(const Uint32& color);
		Color(const HSPColor& color);
		Color(const sf::Color& c);

		HSPColor toHSP() const;

		[[nodiscard]] Color blend(const Color& other, float threshold = .5f) const;

		static constexpr Uint32 AliceBlue =				0xFFF0F8FF; //RGBA integer definition for AliceBlue color
		static constexpr Uint32 AntiqueWhite =			0xFAEBD7FF; //RGBA integer definition for AntiqueWhite color
		static constexpr Uint32 Aqua =					0xFFFF00FF; //RGBA integer definition for Aqua color
		static constexpr Uint32 Aquamarine =			0x7FFFD4FF; //RGBA integer definition for Aquamarine color
		static constexpr Uint32 Azure =					0xF0FFFFFF; //RGBA integer definition for Azure color
		static constexpr Uint32 Beige =					0xF5F5DCFF; //RGBA integer definition for Beige color
		static constexpr Uint32 Bisque =				0xFFE4C4FF; //RGBA integer definition for Bisque color
		static constexpr Uint32 Black =					0x000000FF; //RGBA integer definition for Black color
		static constexpr Uint32 BlanchedAlmond =		0xFFEBCDFF; //RGBA integer definition for BlanchedAlmond color
		static constexpr Uint32 Blue =					0x0000FFFF; //RGBA integer definition for Blue color
		static constexpr Uint32 BlueViolet =			0x8A2BE2FF; //RGBA integer definition for BlueViolet color
		static constexpr Uint32 Brown =					0xA52A2AFF; //RGBA integer definition for Brown color
		static constexpr Uint32 BurlyWood =				0xDEB887FF; //RGBA integer definition for BurlyWood color
		static constexpr Uint32 CadetBlue =				0x5F9EA0FF; //RGBA integer definition for CadetBlue color
		static constexpr Uint32 Chartreuse =			0x7FFF00FF; //RGBA integer definition for Chartreuse color
		static constexpr Uint32 Chocolate =				0xD2691EFF; //RGBA integer definition for Chocolate color
		static constexpr Uint32 Coral =					0xFF7F50FF; //RGBA integer definition for Coral color
		static constexpr Uint32 CornflowerBlue =		0x6495EDFF; //RGBA integer definition for CornflowerBlue color
		static constexpr Uint32 Cornsilk =				0xFFF8DCFF; //RGBA integer definition for Cornsilk color
		static constexpr Uint32 Crimson =				0xDC143CFF; //RGBA integer definition for Crimson color
		static constexpr Uint32 Cyan =					0x00FFFFFF; //RGBA integer definition for Cyan color
		static constexpr Uint32 DarkBlue =				0x00008BFF; //RGBA integer definition for DarkBlue color
		static constexpr Uint32 DarkCyan =				0x008B8BFF; //RGBA integer definition for DarkCyan color
		static constexpr Uint32 DarkGoldenrod =			0xB8860BFF; //RGBA integer definition for DarkGoldenrod color
		static constexpr Uint32 DarkGray =				0xA9A9A9FF; //RGBA integer definition for DarkGray color
		static constexpr Uint32 DarkGreen =				0x006400FF; //RGBA integer definition for DarkGreen color
		static constexpr Uint32 DarkKhaki =				0xBDB76BFF; //RGBA integer definition for DarkKhaki color
		static constexpr Uint32 DarkMagenta =			0x8B008BFF; //RGBA integer definition for DarkMagenta color
		static constexpr Uint32 DarkOliveGreen =		0x556B2FFF; //RGBA integer definition for DarkOliveGreen color
		static constexpr Uint32 DarkOrange =			0xFF8C00FF; //RGBA integer definition for DarkOrange color
		static constexpr Uint32 DarkOrchid =			0x9932CCFF; //RGBA integer definition for DarkOrchid color
		static constexpr Uint32 DarkRed =				0x8B0000FF; //RGBA integer definition for DarkRed color
		static constexpr Uint32 DarkSalmon =			0xE9967AFF; //RGBA integer definition for DarkSalmon color
		static constexpr Uint32 DarkSeaGreen =			0x8FBC8FFF; //RGBA integer definition for DarkSeaGreen color
		static constexpr Uint32 DarkSlateBlue =			0x483D8BFF; //RGBA integer definition for DarkSlateBlue color
		static constexpr Uint32 DarkSlateGray =			0x2F4F4FFF; //RGBA integer definition for DarkSlateGray color
		static constexpr Uint32 DarkTurquoise =			0x00CED1FF; //RGBA integer definition for DarkTurquoise color
		static constexpr Uint32 DarkViolet =			0x9400D3FF; //RGBA integer definition for DarkViolet color
		static constexpr Uint32 DeepPink =				0xFF1493FF; //RGBA integer definition for DeepPink color
		static constexpr Uint32 DeepSkyBlue =			0x00BFFFFF; //RGBA integer definition for DeepSkyBlue color
		static constexpr Uint32 DimGray =				0x696969FF; //RGBA integer definition for DimGray color
		static constexpr Uint32 DodgerBlue =			0x1E90FFFF; //RGBA integer definition for DodgerBlue color
		static constexpr Uint32 Firebrick =				0xB22222FF; //RGBA integer definition for Firebrick color
		static constexpr Uint32 FloralWhite =			0xFFFAF0FF; //RGBA integer definition for FloralWhite color
		static constexpr Uint32 ForestGreen =			0x228B22FF; //RGBA integer definition for ForestGreen color
		static constexpr Uint32 Fuchsia =				0xFF00FFFF; //RGBA integer definition for Fuchsia color
		static constexpr Uint32 Gainsboro =				0xDCDCDCFF; //RGBA integer definition for Gainsboro color
		static constexpr Uint32 GhostWhite =			0xF8F8FFFF; //RGBA integer definition for GhostWhite color
		static constexpr Uint32 Gold =					0xFFD700FF; //RGBA integer definition for Gold color
		static constexpr Uint32 Goldenrod =				0xDAA520FF; //RGBA integer definition for Goldenrod color
		static constexpr Uint32 Gray =					0x808080FF; //RGBA integer definition for Gray color
		static constexpr Uint32 Green =					0x00FF00FF; //RGBA integer definition for Green color
		static constexpr Uint32 GreenYellow =			0xADFF2FFF; //RGBA integer definition for GreenYellow color
		static constexpr Uint32 Honeydew =				0xF0FFF0FF; //RGBA integer definition for Honeydew color
		static constexpr Uint32 HotPink =				0xFF69B4FF; //RGBA integer definition for HotPink color
		static constexpr Uint32 IndianRed =				0xCD5C5CFF; //RGBA integer definition for IndianRed color
		static constexpr Uint32 Indigo =				0x4B0082FF; //RGBA integer definition for Indigo color
		static constexpr Uint32 Ivory =					0xFFFFF0FF; //RGBA integer definition for Ivory color
		static constexpr Uint32 Khaki =					0xF0E68CFF; //RGBA integer definition for Khaki color
		static constexpr Uint32 Lavender =				0xE6E6FAFF; //RGBA integer definition for Lavender color
		static constexpr Uint32 LavenderBlush =			0xFFF0F5FF; //RGBA integer definition for LavenderBlush color
		static constexpr Uint32 LawnGreen =				0x7CFC00FF; //RGBA integer definition for LawnGreen color
		static constexpr Uint32 LemonChiffon =			0xFFFACDFF; //RGBA integer definition for LemonChiffon color
		static constexpr Uint32 LightBlue =				0xADD8E6FF; //RGBA integer definition for LightBlue color
		static constexpr Uint32 LightCoral =			0xF08080FF; //RGBA integer definition for LightCoral color
		static constexpr Uint32 LightCyan =				0xE0FFFFFF; //RGBA integer definition for LightCyan color
		static constexpr Uint32 LightGoldenrodYellow =	0xFAFAD2FF; //RGBA integer definition for LightGoldenrodYellow color
		static constexpr Uint32 LightGray =				0xD3D3D3FF; //RGBA integer definition for LightGray color
		static constexpr Uint32 LightGreen =			0x90EE90FF; //RGBA integer definition for LightGreen color
		static constexpr Uint32 LightPink =				0xFFB6C1FF; //RGBA integer definition for LightPink color
		static constexpr Uint32 LightSalmon =			0xFFA07AFF; //RGBA integer definition for LightSalmon color
		static constexpr Uint32 LightSeaGreen =			0x20B2AAFF; //RGBA integer definition for LightSeaGreen color
		static constexpr Uint32 LightSkyBlue =			0x87CEFAFF; //RGBA integer definition for LightSkyBlue color
		static constexpr Uint32 LightSlateGray =		0x778899FF; //RGBA integer definition for LightSlateGray color
		static constexpr Uint32 LightSteelBlue =		0xB0C4DEFF; //RGBA integer definition for LightSteelBlue color
		static constexpr Uint32 LightYellow =			0xFFFFE0FF; //RGBA integer definition for LightYellow color
		static constexpr Uint32 Lime =					0x00FF00FF; //RGBA integer definition for Lime color
		static constexpr Uint32 LimeGreen =				0x32CD32FF; //RGBA integer definition for LimeGreen color
		static constexpr Uint32 Linen =					0xFAF0E6FF; //RGBA integer definition for Linen color
		static constexpr Uint32 Magenta =				   Fuchsia; //RGBA integer definition for Magenta color
		static constexpr Uint32 Maroon =				0x800000FF; //RGBA integer definition for Maroon color
		static constexpr Uint32 MediumAquamarine =		0x66CDAAFF; //RGBA integer definition for MediumAquamarine color
		static constexpr Uint32 MediumBlue =			0x0000CDFF; //RGBA integer definition for MediumBlue color
		static constexpr Uint32 MediumOrchid =			0xBA55D3FF; //RGBA integer definition for MediumOrchid color
		static constexpr Uint32 MediumPurple =			0x9370DBFF; //RGBA integer definition for MediumPurple color
		static constexpr Uint32 MediumSeaGreen =		0x3CB371FF; //RGBA integer definition for MediumSeaGreen color
		static constexpr Uint32 MediumSlateBlue =		0x7B68EEFF; //RGBA integer definition for MediumSlateBlue color
		static constexpr Uint32 MediumSpringGreen =		0x00FA9AFF; //RGBA integer definition for MediumSpringGreen color
		static constexpr Uint32 MediumTurquoise =		0x48D1CCFF; //RGBA integer definition for MediumTurquoise color
		static constexpr Uint32 MediumVioletRed =		0xC71585FF; //RGBA integer definition for MediumVioletRed color
		static constexpr Uint32 MidnightBlue =			0x191970ff; //RGBA integer definition for MidnightBlue color
		static constexpr Uint32 MintCream =				0xF5FFFAFF; //RGBA integer definition for MintCream color
		static constexpr Uint32 MistyRose =				0xFFE4E1FF; //RGBA integer definition for MistyRose color
		static constexpr Uint32 Moccasin =				0xFFE4B5FF; //RGBA integer definition for Moccasin color
		static constexpr Uint32 NavajoWhite =			0xFFDEADFF; //RGBA integer definition for NavajoWhite color
		static constexpr Uint32 Navy =					0x000080FF; //RGBA integer definition for Navy color
		static constexpr Uint32 OldLace =				0xFDF5E6FF; //RGBA integer definition for OldLace color
		static constexpr Uint32 Olive =					0x808000FF; //RGBA integer definition for Olive color
		static constexpr Uint32 OliveDrab =				0x6B8E23FF; //RGBA integer definition for OliveDrab color
		static constexpr Uint32 Orange =				0xFFA500FF; //RGBA integer definition for Orange color
		static constexpr Uint32 OrangeRed =				0xFF4500FF; //RGBA integer definition for OrangeRed color
		static constexpr Uint32 Orchid =				0xDA70D6FF; //RGBA integer definition for Orchid color
		static constexpr Uint32 PaleGoldenrod =			0xEEE8AAFF; //RGBA integer definition for PaleGoldenrod color
		static constexpr Uint32 PaleGreen =				0x98FB98FF; //RGBA integer definition for PaleGreen color
		static constexpr Uint32 PaleTurquoise =			0xAFEEEEFF; //RGBA integer definition for PaleTurquoise color
		static constexpr Uint32 PaleVioletRed =			0xDB7093FF; //RGBA integer definition for PaleVioletRed color
		static constexpr Uint32 PapayaWhip =			0xFFEFD5FF; //RGBA integer definition for PapayaWhip color
		static constexpr Uint32 PeachPuff =				0xFFDAB9FF; //RGBA integer definition for PeachPuff color
		static constexpr Uint32 Peru =					0xCD853FFF; //RGBA integer definition for Peru color
		static constexpr Uint32 Pink =					0xFFC0CBFF; //RGBA integer definition for Pink color
		static constexpr Uint32 Plum =					0xDDA0DDFF; //RGBA integer definition for Plum color
		static constexpr Uint32 PowderBlue =			0xB0E0E6FF; //RGBA integer definition for PowderBlue color
		static constexpr Uint32 Purple =				0x800080FF; //RGBA integer definition for Purple color
		static constexpr Uint32 Red =					0xFF0000FF; //RGBA integer definition for Red color
		static constexpr Uint32 RosyBrown =				0xBC8F8FFF; //RGBA integer definition for RosyBrown color
		static constexpr Uint32 RoyalBlue =				0x4169E1FF; //RGBA integer definition for RoyalBlue color
		static constexpr Uint32 SaddleBrown =			0x8B4513FF; //RGBA integer definition for SaddleBrown color
		static constexpr Uint32 Salmon =				0xFA8072FF; //RGBA integer definition for Salmon color
		static constexpr Uint32 SandyBrown =			0xF4A460FF; //RGBA integer definition for SandyBrown color
		static constexpr Uint32 SeaGreen =				0x2E8B57FF; //RGBA integer definition for SeaGreen color
		static constexpr Uint32 SeaShell =				0xFFF5EEFF; //RGBA integer definition for SeaShell color
		static constexpr Uint32 Sienna =				0xA0522DFF; //RGBA integer definition for Sienna color
		static constexpr Uint32 Silver =				0xC0C0C0FF; //RGBA integer definition for Silver color
		static constexpr Uint32 SkyBlue =				0x87CEEBFF; //RGBA integer definition for SkyBlue color
		static constexpr Uint32 SlateBlue =				0x6A5ACDFF; //RGBA integer definition for SlateBlue color
		static constexpr Uint32 SlateGray =				0x708090FF; //RGBA integer definition for SlateGray color
		static constexpr Uint32 Snow =					0xFFFAFAFF; //RGBA integer definition for Snow color
		static constexpr Uint32 SpringGreen =			0x00FF7FFF; //RGBA integer definition for SpringGreen color
		static constexpr Uint32 SteelBlue =				0x4682B4FF; //RGBA integer definition for SteelBlue color
		static constexpr Uint32 Tan =					0xD2B48CFF; //RGBA integer definition for Tan color
		static constexpr Uint32 Teal =					0x008080FF; //RGBA integer definition for Teal color
		static constexpr Uint32 Thistle =				0xD8BFD8FF; //RGBA integer definition for Thistle color
		static constexpr Uint32 Tomato =				0xFF6347FF; //RGBA integer definition for Tomato color
		static constexpr Uint32 Transparent =			0x00000000; //RGBA integer definition for Transparent color
		static constexpr Uint32 Turquoise =				0x40E0D0FF; //RGBA integer definition for Turquoise color
		static constexpr Uint32 Violet =				0xEE82EEFF; //RGBA integer definition for Violet color
		static constexpr Uint32 Wheat =					0xF5DEB3FF; //RGBA integer definition for Wheat color
		static constexpr Uint32 White =					0xFFFFFFFF; //RGBA integer definition for White color
		static constexpr Uint32 WhiteSmoke =			0xF5F5F5FF; //RGBA integer definition for WhiteSmoke color
		static constexpr Uint32 Yellow =				0xFFFF00FF; //RGBA integer definition for Yellow color
		static constexpr Uint32 YellowGreen =			0x9ACD32FF; //RGBA integer definition for YellowGreen color

		inline operator sf::Color() const { return sf::Color(integer); }

		union {
			struct
			{
				Uint8 a;
				Uint8 b;
				Uint8 g;
				Uint8 r;
			};
			Uint32 integer;
		};

	private:
		Uint8 blendColorVal(Uint8 a, Uint8 b, float t) const;
		Uint8 blendAlphaVal(Uint8 a, Uint8 b, float t) const;
	};


	////////////////////////////////////////////////////////////
	/// \brief Color Class for converting RGB to/from HSP format
	////////////////////////////////////////////////////////////
	struct HSPColor {
		HSPColor();
		HSPColor(float hue, float saturation, float brightness, float alpha);
		HSPColor(const Color& color);

		Color toRGB() const;

		friend Color HSPtoRGB(float hue, float saturation, float brightness, float alpha);
		friend HSPColor RGBtoHSP(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

		float h;
		float s;
		union {
			float p;
			float b;
		};
		float a;

	private:
		// Percieved brightness ratios for RGB color channels
		static constexpr float Pr = .299f;
		static constexpr float Pg = .587f;
		static constexpr float Pb = .114f;
	};

	bool operator==(Color lhs, Color rhs) noexcept;
	bool operator!=(Color lhs, Color rhs) noexcept;
	Color operator +(const Color& left, const Color& right);
	Color operator -(const Color& left, const Color& right);
	Color& operator +=(Color& left, const Color& right);
	Color& operator -=(Color& left, const Color& right);

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr >
	inline Color operator+(const Color& color, const Number& num)
	{
		return Color(Uint8(Number(color.r) + num),
			Uint8(Number(color.g) + num),
			Uint8(Number(color.b) + num),
			color.a);
	}

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr >
	inline Color operator-(const Color& color, const Number& num)
	{
		return Color(Uint8(Number(color.r) - num),
			Uint8(Number(color.g) - num),
			Uint8(Number(color.b) - num),
			color.a);
	}


	/////////////////////////////////////////////////////////////////////////
	/// \relates ext::Color
	/// \brief Helper function to multiply Color components.
	/// 
	/// This function returns the component-wise multiplication
	/// (also called "modulation") of a color with a single multiplier.
	/// 
	/// \param The ext::Color object to multiply
	/// \param The multiplier
	/// 
	/// \return ext::Color object
	/////////////////////////////////////////////////////////////////////////
	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr>
	inline ext::Color multiply(const ext::Color& color, Number num)
	{
		return ext::Color((ext::Uint8)std::round(Number(color.r) * num),
			(ext::Uint8)std::round(Number(color.g) * num),
			(ext::Uint8)std::round(Number(color.b) * num),
			color.a);
	}

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr>
	inline Color operator*(const Color& color, Number num)
	{
		return multiply<Number>(color, num);
	}

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr >
	inline Color& operator+=(Color& color, const Number& num)
	{
		return color = color + num;
	}

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr >
	inline Color& operator-=(Color& color, const Number& num)
	{
		return color = color - num;
	}

	template<typename Number, typename std::enable_if<std::is_arithmetic<Number>::value>::type* = nullptr >
	inline Color& operator*=(Color& color, const Number& num)
	{
		return color = color * num;
	}
} // namespace ext