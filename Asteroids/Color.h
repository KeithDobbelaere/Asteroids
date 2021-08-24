#pragma once

#include <SFML/Graphics/Color.hpp>


////////////////////////////////////////////////////////////
/// \brief Extended definitions for SFML
////////////////////////////////////////////////////////////
namespace ext
{
	////////////////////////////////////////////////////////////
	/// \brief Extended RGBA color definitions
	////////////////////////////////////////////////////////////
	struct Color
	{
		Color() = delete;

		static constexpr sf::Uint32 AliceBlue =				0xFFF0F8FF; //Definition for AliceBlue color
		static constexpr sf::Uint32 AntiqueWhite =			0xFAEBD7FF; //Definition for AntiqueWhite color
		static constexpr sf::Uint32 Aqua =					0xFFFF00FF; //Definition for Aqua color
		static constexpr sf::Uint32 Aquamarine =			0x7FFFD4FF; //Definition for Aquamarine color
		static constexpr sf::Uint32 Azure =					0xF0FFFFFF; //Definition for Azure color
		static constexpr sf::Uint32 Beige =					0xF5F5DCFF; //Definition for Beige color
		static constexpr sf::Uint32 Bisque =				0xFFE4C4FF; //Definition for Bisque color
		static constexpr sf::Uint32 Black =					0x000000FF; //Definition for Black color
		static constexpr sf::Uint32 BlanchedAlmond =		0xFFEBCDFF; //Definition for BlanchedAlmond color
		static constexpr sf::Uint32 Blue =					0x0000FFFF; //Definition for Blue color
		static constexpr sf::Uint32 BlueViolet =			0x8A2BE2FF; //Definition for BlueViolet color
		static constexpr sf::Uint32 Brown =					0xA52A2AFF; //Definition for Brown color
		static constexpr sf::Uint32 BurlyWood =				0xDEB887FF; //Definition for BurlyWood color
		static constexpr sf::Uint32 CadetBlue =				0x5F9EA0FF; //Definition for CadetBlue color
		static constexpr sf::Uint32 Chartreuse =			0x7FFF00FF; //Definition for Chartreuse color
		static constexpr sf::Uint32 Chocolate =				0xD2691EFF; //Definition for Chocolate color
		static constexpr sf::Uint32 Coral =					0xFF7F50FF; //Definition for Coral color
		static constexpr sf::Uint32 CornflowerBlue =		0x6495EDFF; //Definition for CornflowerBlue color
		static constexpr sf::Uint32 Cornsilk =				0xFFF8DCFF; //Definition for Cornsilk color
		static constexpr sf::Uint32 Crimson =				0xDC143CFF; //Definition for Crimson color
		static constexpr sf::Uint32 Cyan =					0x00FFFFFF; //Definition for Cyan color
		static constexpr sf::Uint32 DarkBlue =				0x00008BFF; //Definition for DarkBlue color
		static constexpr sf::Uint32 DarkCyan =				0x008B8BFF; //Definition for DarkCyan color
		static constexpr sf::Uint32 DarkGoldenrod =			0xB8860BFF; //Definition for DarkGoldenrod color
		static constexpr sf::Uint32 DarkGray =				0xA9A9A9FF; //Definition for DarkGray color
		static constexpr sf::Uint32 DarkGreen =				0x006400FF; //Definition for DarkGreen color
		static constexpr sf::Uint32 DarkKhaki =				0xBDB76BFF; //Definition for DarkKhaki color
		static constexpr sf::Uint32 DarkMagenta =			0x8B008BFF; //Definition for DarkMagenta color
		static constexpr sf::Uint32 DarkOliveGreen =		0x556B2FFF; //Definition for DarkOliveGreen color
		static constexpr sf::Uint32 DarkOrange =			0xFF8C00FF; //Definition for DarkOrange color
		static constexpr sf::Uint32 DarkOrchid =			0x9932CCFF; //Definition for DarkOrchid color
		static constexpr sf::Uint32 DarkRed =				0x8B0000FF; //Definition for DarkRed color
		static constexpr sf::Uint32 DarkSalmon =			0xE9967AFF; //Definition for DarkSalmon color
		static constexpr sf::Uint32 DarkSeaGreen =			0x8FBC8FFF; //Definition for DarkSeaGreen color
		static constexpr sf::Uint32 DarkSlateBlue =			0x483D8BFF; //Definition for DarkSlateBlue color
		static constexpr sf::Uint32 DarkSlateGray =			0x2F4F4FFF; //Definition for DarkSlateGray color
		static constexpr sf::Uint32 DarkTurquoise =			0x00CED1FF; //Definition for DarkTurquoise color
		static constexpr sf::Uint32 DarkViolet =			0x9400D3FF; //Definition for DarkViolet color
		static constexpr sf::Uint32 DeepPink =				0xFF1493FF; //Definition for DeepPink color
		static constexpr sf::Uint32 DeepSkyBlue =			0x00BFFFFF; //Definition for DeepSkyBlue color
		static constexpr sf::Uint32 DimGray =				0x696969FF; //Definition for DimGray color
		static constexpr sf::Uint32 DodgerBlue =			0x1E90FFFF; //Definition for DodgerBlue color
		static constexpr sf::Uint32 Firebrick =				0xB22222FF; //Definition for Firebrick color
		static constexpr sf::Uint32 FloralWhite =			0xFFFAF0FF; //Definition for FloralWhite color
		static constexpr sf::Uint32 ForestGreen =			0x228B22FF; //Definition for ForestGreen color
		static constexpr sf::Uint32 Fuchsia =				0xFF00FFFF; //Definition for Fuchsia color
		static constexpr sf::Uint32 Gainsboro =				0xDCDCDCFF; //Definition for Gainsboro color
		static constexpr sf::Uint32 GhostWhite =			0xF8F8FFFF; //Definition for GhostWhite color
		static constexpr sf::Uint32 Gold =					0xFFD700FF; //Definition for Gold color
		static constexpr sf::Uint32 Goldenrod =				0xDAA520FF; //Definition for Goldenrod color
		static constexpr sf::Uint32 Gray =					0x808080FF; //Definition for Gray color
		static constexpr sf::Uint32 Green =					0x00FF00FF; //Definition for Green color
		static constexpr sf::Uint32 GreenYellow =			0xADFF2FFF; //Definition for GreenYellow color
		static constexpr sf::Uint32 Honeydew =				0xF0FFF0FF; //Definition for Honeydew color
		static constexpr sf::Uint32 HotPink =				0xFF69B4FF; //Definition for HotPink color
		static constexpr sf::Uint32 IndianRed =				0xCD5C5CFF; //Definition for IndianRed color
		static constexpr sf::Uint32 Indigo =				0x4B0082FF; //Definition for Indigo color
		static constexpr sf::Uint32 Ivory =					0xFFFFF0FF; //Definition for Ivory color
		static constexpr sf::Uint32 Khaki =					0xF0E68CFF; //Definition for Khaki color
		static constexpr sf::Uint32 Lavender =				0xE6E6FAFF; //Definition for Lavender color
		static constexpr sf::Uint32 LavenderBlush =			0xFFF0F5FF; //Definition for LavenderBlush color
		static constexpr sf::Uint32 LawnGreen =				0x7CFC00FF; //Definition for LawnGreen color
		static constexpr sf::Uint32 LemonChiffon =			0xFFFACDFF; //Definition for LemonChiffon color
		static constexpr sf::Uint32 LightBlue =				0xADD8E6FF; //Definition for LightBlue color
		static constexpr sf::Uint32 LightCoral =			0xF08080FF; //Definition for LightCoral color
		static constexpr sf::Uint32 LightCyan =				0xE0FFFFFF; //Definition for LightCyan color
		static constexpr sf::Uint32 LightGoldenrodYellow =	0xFAFAD2FF; //Definition for LightGoldenrodYellow color
		static constexpr sf::Uint32 LightGray =				0xD3D3D3FF; //Definition for LightGray color
		static constexpr sf::Uint32 LightGreen =			0x90EE90FF; //Definition for LightGreen color
		static constexpr sf::Uint32 LightPink =				0xFFB6C1FF; //Definition for LightPink color
		static constexpr sf::Uint32 LightSalmon =			0xFFA07AFF; //Definition for LightSalmon color
		static constexpr sf::Uint32 LightSeaGreen =			0x20B2AAFF; //Definition for LightSeaGreen color
		static constexpr sf::Uint32 LightSkyBlue =			0x87CEFAFF; //Definition for LightSkyBlue color
		static constexpr sf::Uint32 LightSlateGray =		0x778899FF; //Definition for LightSlateGray color
		static constexpr sf::Uint32 LightSteelBlue =		0xB0C4DEFF; //Definition for LightSteelBlue color
		static constexpr sf::Uint32 LightYellow =			0xFFFFE0FF; //Definition for LightYellow color
		static constexpr sf::Uint32 Lime =					0x00FF00FF; //Definition for Lime color
		static constexpr sf::Uint32 LimeGreen =				0x32CD32FF; //Definition for LimeGreen color
		static constexpr sf::Uint32 Linen =					0xFAF0E6FF; //Definition for Linen color
		static constexpr sf::Uint32 Magenta =				   Fuchsia; //Definition for Magenta color
		static constexpr sf::Uint32 Maroon =				0x800000FF; //Definition for Maroon color
		static constexpr sf::Uint32 MediumAquamarine =		0x66CDAAFF; //Definition for MediumAquamarine color
		static constexpr sf::Uint32 MediumBlue =			0x0000CDFF; //Definition for MediumBlue color
		static constexpr sf::Uint32 MediumOrchid =			0xBA55D3FF; //Definition for MediumOrchid color
		static constexpr sf::Uint32 MediumPurple =			0x9370DBFF; //Definition for MediumPurple color
		static constexpr sf::Uint32 MediumSeaGreen =		0x3CB371FF; //Definition for MediumSeaGreen color
		static constexpr sf::Uint32 MediumSlateBlue =		0x7B68EEFF; //Definition for MediumSlateBlue color
		static constexpr sf::Uint32 MediumSpringGreen =		0x00FA9AFF; //Definition for MediumSpringGreen color
		static constexpr sf::Uint32 MediumTurquoise =		0x48D1CCFF; //Definition for MediumTurquoise color
		static constexpr sf::Uint32 MediumVioletRed =		0xC71585FF; //Definition for MediumVioletRed color
		static constexpr sf::Uint32 MidnightBlue =			0x191970ff; //Definition for MidnightBlue color
		static constexpr sf::Uint32 MintCream =				0xF5FFFAFF; //Definition for MintCream color
		static constexpr sf::Uint32 MistyRose =				0xFFE4E1FF; //Definition for MistyRose color
		static constexpr sf::Uint32 Moccasin =				0xFFE4B5FF; //Definition for Moccasin color
		static constexpr sf::Uint32 NavajoWhite =			0xFFDEADFF; //Definition for NavajoWhite color
		static constexpr sf::Uint32 Navy =					0x000080FF; //Definition for Navy color
		static constexpr sf::Uint32 OldLace =				0xFDF5E6FF; //Definition for OldLace color
		static constexpr sf::Uint32 Olive =					0x808000FF; //Definition for Olive color
		static constexpr sf::Uint32 OliveDrab =				0x6B8E23FF; //Definition for OliveDrab color
		static constexpr sf::Uint32 Orange =				0xFFA500FF; //Definition for Orange color
		static constexpr sf::Uint32 OrangeRed =				0xFF4500FF; //Definition for OrangeRed color
		static constexpr sf::Uint32 Orchid =				0xDA70D6FF; //Definition for Orchid color
		static constexpr sf::Uint32 PaleGoldenrod =			0xEEE8AAFF; //Definition for PaleGoldenrod color
		static constexpr sf::Uint32 PaleGreen =				0x98FB98FF; //Definition for PaleGreen color
		static constexpr sf::Uint32 PaleTurquoise =			0xAFEEEEFF; //Definition for PaleTurquoise color
		static constexpr sf::Uint32 PaleVioletRed =			0xDB7093FF; //Definition for PaleVioletRed color
		static constexpr sf::Uint32 PapayaWhip =			0xFFEFD5FF; //Definition for PapayaWhip color
		static constexpr sf::Uint32 PeachPuff =				0xFFDAB9FF; //Definition for PeachPuff color
		static constexpr sf::Uint32 Peru =					0xCD853FFF; //Definition for Peru color
		static constexpr sf::Uint32 Pink =					0xFFC0CBFF; //Definition for Pink color
		static constexpr sf::Uint32 Plum =					0xDDA0DDFF; //Definition for Plum color
		static constexpr sf::Uint32 PowderBlue =			0xB0E0E6FF; //Definition for PowderBlue color
		static constexpr sf::Uint32 Purple =				0x800080FF; //Definition for Purple color
		static constexpr sf::Uint32 Red =					0xFF0000FF; //Definition for Red color
		static constexpr sf::Uint32 RosyBrown =				0xBC8F8FFF; //Definition for RosyBrown color
		static constexpr sf::Uint32 RoyalBlue =				0x4169E1FF; //Definition for RoyalBlue color
		static constexpr sf::Uint32 SaddleBrown =			0x8B4513FF; //Definition for SaddleBrown color
		static constexpr sf::Uint32 Salmon =				0xFA8072FF; //Definition for Salmon color
		static constexpr sf::Uint32 SandyBrown =			0xF4A460FF; //Definition for SandyBrown color
		static constexpr sf::Uint32 SeaGreen =				0x2E8B57FF; //Definition for SeaGreen color
		static constexpr sf::Uint32 SeaShell =				0xFFF5EEFF; //Definition for SeaShell color
		static constexpr sf::Uint32 Sienna =				0xA0522DFF; //Definition for Sienna color
		static constexpr sf::Uint32 Silver =				0xC0C0C0FF; //Definition for Silver color
		static constexpr sf::Uint32 SkyBlue =				0x87CEEBFF; //Definition for SkyBlue color
		static constexpr sf::Uint32 SlateBlue =				0x6A5ACDFF; //Definition for SlateBlue color
		static constexpr sf::Uint32 SlateGray =				0x708090FF; //Definition for SlateGray color
		static constexpr sf::Uint32 Snow =					0xFFFAFAFF; //Definition for Snow color
		static constexpr sf::Uint32 SpringGreen =			0x00FF7FFF; //Definition for SpringGreen color
		static constexpr sf::Uint32 SteelBlue =				0x4682B4FF; //Definition for SteelBlue color
		static constexpr sf::Uint32 Tan =					0xD2B48CFF; //Definition for Tan color
		static constexpr sf::Uint32 Teal =					0x008080FF; //Definition for Teal color
		static constexpr sf::Uint32 Thistle =				0xD8BFD8FF; //Definition for Thistle color
		static constexpr sf::Uint32 Tomato =				0xFF6347FF; //Definition for Tomato color
		static constexpr sf::Uint32 Transparent =			0x00000000; //Definition for Transparent color
		static constexpr sf::Uint32 Turquoise =				0x40E0D0FF; //Definition for Turquoise color
		static constexpr sf::Uint32 Violet =				0xEE82EEFF; //Definition for Violet color
		static constexpr sf::Uint32 Wheat =					0xF5DEB3FF; //Definition for Wheat color
		static constexpr sf::Uint32 White =					0xFFFFFFFF; //Definition for White color
		static constexpr sf::Uint32 WhiteSmoke =			0xF5F5F5FF; //Definition for WhiteSmoke color
		static constexpr sf::Uint32 Yellow =				0xFFFF00FF; //Definition for Yellow color
		static constexpr sf::Uint32 YellowGreen =			0x9ACD32FF; //Definition for YellowGreen color


		/////////////////////////////////////////////////////////////////////////
		/// \relates Color
		/// \brief Helper function to multiply RGBA integer components.
		/// 
		/// This operator returns the component-wise multiplication
		/// (also called "modulation") of a color with a single multiplier.
		/// 
		/// \param The RGBA formatted integer to multiply
		/// \param The multiplier
		/// 
		/// \return Color represented as a 32-bit unsigned integer
		/////////////////////////////////////////////////////////////////////////
		static sf::Uint32 multiply(const sf::Uint32& color, const float multiplier)
		{
			unsigned char r, g, b, a;
			r = char(((color & 0xff000000) >> 24) * multiplier);
			g = char(((color & 0x00ff0000) >> 16) * multiplier);
			b = char(((color & 0x0000ff00) >> 8) * multiplier);
			a = char(((color & 0x000000ff) >> 0) * multiplier);
			return int(r << 24) + int(g << 16) + int(b << 8) + a;
		}


		/////////////////////////////////////////////////////////////////////////
		/// \relates Color
		/// \brief Helper function to multiply Color components.
		/// 
		/// This operator returns the component-wise multiplication
		/// (also called "modulation") of a color with a single multiplier.
		/// 
		/// \param The Color object to multiply
		/// \param The multiplier
		/// 
		/// \return Color represented as a 32-bit unsigned integer
		/////////////////////////////////////////////////////////////////////////
		static sf::Uint32 multiply(const sf::Color& sfColor, const float multiplier)
		{
			sf::Uint32 color = sfColor.toInteger();
			return multiply(color, multiplier);
		}
	};
} // namespace ext
