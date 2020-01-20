#include <iostream>
#include <limits>
#include <cstdint>

#include <stb_image_write.h>

#include <inspro/vec3.hpp>
#include <inspro/ray.hpp>
#include <inspro/hittable_list.hpp>
#include <inspro/sphere.hpp>

insp::Vec3 RenderColor( const insp::Ray &r, insp::Hittable *world )
{
	insp::HitRecord rec;
	if( world->Hit( r, 0.0, std::numeric_limits<float>::max(), rec ) )
	{
		return 0.5 * insp::Vec3( rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1 );
	}
	else
	{
		insp::Vec3 unit_direction = insp::unit_vector( r.GetDirection() );
		float t = 0.5f * ( unit_direction.y() + 1.0f );
		return ( 1.0f - t ) * insp::Vec3( 1.0f, 1.0f, 1.0f ) + t * insp::Vec3( 0.5f, 0.7f, 1.0f );
	}
}

int main()
{
	const std::uint16_t width = 200;
	const std::uint16_t height = 100;

	std::uint8_t buffer[height][width][3];

	insp::Ray ray;
	insp::Vec3 color;
	std::int16_t iHeight = 0;

	insp::Vec3 lowerLeftCorner( -2.0, -1.0, -1.0 );
	insp::Vec3 horizontal( 4.0, 0.0, 0.0 );
	insp::Vec3 vertical( 0.0, 2.0, 0.0 );
	insp::Vec3 origin( 0.0, 0.0, 0.0 );

	insp::Hittable *list[2];
	list[0] = new insp::Sphere( insp::Vec3( 0, 0, -1 ), 0.5 );
	list[1] = new insp::Sphere( insp::Vec3( 0, -100.5, -1 ), 100 );
	insp::Hittable *world = new insp::HittableList( list, 2 );

	for( std::uint16_t y = 0; y < height; ++y )
	{
		for( std::uint16_t x = 0; x < width; ++x )
		{
			float u = float( x ) / float( width );
			float v = float( y ) / float( height );
			ray = insp::Ray( origin, lowerLeftCorner + u * horizontal + v * vertical );

			insp::Vec3 p = ray.PointAtParameter( 2.0f );
			color = RenderColor( ray, world );

			iHeight = (height - y) - 1;
			buffer[iHeight][x][0] = std::uint8_t( 255.99f * color[0] );
			buffer[iHeight][x][1] = std::uint8_t( 255.99f * color[1] );
			buffer[iHeight][x][2] = std::uint8_t( 255.99f * color[2] );
		}
	}


	stbi_write_png( "final.png", width, height, 3, buffer, 0 );

	return 0;
}