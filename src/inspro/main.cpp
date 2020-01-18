#include <iostream>
#include <cstdint>

#include <stb_image_write.h>

int main()
{


	const std::uint8_t width = 200;
	const std::uint8_t height = 100;

	std::uint8_t buffer[height][width][3];

	for( int y = height - 1; y >= 0; --y )
	{
		for( int x = 0; x < width; ++x )
		{
			buffer[y][x][0] = std::uint8_t( 255.99 * ( float( x ) / float( width ) ) );
			buffer[y][x][1] = std::uint8_t( 255.99 * ( float( y ) / float( height ) ) );
			buffer[y][x][2] = std::uint8_t( 255.99 * 0.2f );
		}
	}


	stbi_write_png( "final.png", width, height, 3, buffer, 0 );

	return 0;
}