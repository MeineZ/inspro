#include <iostream>
#include <cstdint>

#include <stb_image_write.h>
#include <inspro/vec3.hpp>

int main()
{
	const std::uint16_t width = 200;
	const std::uint16_t height = 100;

	std::uint8_t buffer[height][width][3];

	vec3 color;
	std::int16_t iHeight = 0;


	for( std::uint16_t y = 0; y < height; ++y )
	{
		for( std::uint16_t x = 0; x < width; ++x )
		{
			color = vec3( float( x ) / float( width ), float( y ) / float( height ), 0.2f );
			iHeight = (height - y) - 1;
			buffer[iHeight][x][0] = std::uint8_t( 255.99f * ( float( x ) / float( width ) ) );
			buffer[iHeight][x][1] = std::uint8_t( 255.99f * ( float( y ) / float( height ) ) );
			buffer[iHeight][x][2] = std::uint8_t( 255.99f * 0.2f );
		}
	}


	stbi_write_png( "final.png", width, height, 3, buffer, 0 );

	return 0;
}