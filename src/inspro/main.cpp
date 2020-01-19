#include <iostream>
#include <cstdint>

#include <stb_image_write.h>

#include <inspro/vec3.hpp>
#include <inspro/ray.hpp>

bool HitSphere( const insp::Vec3 &center, float radius, const insp::Ray &r )
{
	insp::Vec3 oc = r.GetOrigin() - center;
	float a = insp::dot( r.GetDirection(), r.GetDirection() );
	float b = 2.0f * dot( oc, r.GetDirection() );
	float c = dot( oc, oc ) - radius * radius;
	float discriminant = b * b - 4.0f * a * c;
	return ( discriminant > 0 );
}

insp::Vec3 RenderColor( const insp::Ray &r )
{
	if( HitSphere( insp::Vec3( 0, 0, -1 ), 0.5, r ) )
		return insp::Vec3( 1, 0, 0 );
	insp::Vec3 unit_direction = unit_vector( r.GetDirection() );
	float t = 0.5f * ( unit_direction.y() + 1.0f );
	return ( 1.0f - t ) * insp::Vec3( 1.0f, 1.0f, 1.0f ) + t * insp::Vec3( 0.5f, 0.7f, 1.0f );
}

int main()
{
	const std::uint16_t width = 200;
	const std::uint16_t height = 100;

	std::uint8_t buffer[height][width][3];

	insp::Ray ray;
	insp::Vec3 color;
	std::int16_t iHeight = 0;

	insp::Vec3 lower_left_corner( -2.0, -1.0, -1.0 );
	insp::Vec3 horizontal( 4.0, 0.0, 0.0 );
	insp::Vec3 vertical( 0.0, 2.0, 0.0 );
	insp::Vec3 origin( 0.0, 0.0, 0.0 );

	for( std::uint16_t y = 0; y < height; ++y )
	{
		for( std::uint16_t x = 0; x < width; ++x )
		{
			float u = float( x ) / float( width );
			float v = float( y ) / float( height );
			ray = insp::Ray( origin, lower_left_corner + u * horizontal + v * vertical );
			color = RenderColor( ray );

			iHeight = (height - y) - 1;
			buffer[iHeight][x][0] = std::uint8_t( 255.99f * color[0] );
			buffer[iHeight][x][1] = std::uint8_t( 255.99f * color[1] );
			buffer[iHeight][x][2] = std::uint8_t( 255.99f * color[2] );
		}
	}


	stbi_write_png( "final.png", width, height, 3, buffer, 0 );

	return 0;
}