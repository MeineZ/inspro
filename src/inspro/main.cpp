#include <iostream>
#include <limits>
#include <cstdint>

#include <stb_image_write.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <inspro/ray.hpp>
#include <inspro/hittable_list.hpp>
#include <inspro/sphere.hpp>

// #define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
// #define GLM_FORCE_SIMD_AVX2

glm::vec3 RenderColor( const insp::Ray &r, insp::Hittable *world )
{
	insp::HitRecord rec;
	if( world->Hit( r, 0.0, std::numeric_limits<float>::max(), rec ) )
	{
		return 0.5f * (rec.normal + 1.0f);
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize(r.GetDirection());
		float t = 0.5f * ( unitDirection.y + 1.0f );
		return ( 1.0f - t ) * glm::vec3(1.0f) + t * glm::vec3( 0.5f, 0.7f, 1.0f );
	}
}

int main()
{
	const std::uint16_t width = 1920;
	const std::uint16_t height = 1080;
	std::uint8_t * buffer = static_cast<std::uint8_t*>(malloc( width * height * 3 * sizeof( std::uint8_t )));

	float aspectRatio = static_cast<float>(height) / static_cast<float>(width);

	glm::vec3 horizontal( 4.0, 0.0, 0.0 );
	glm::vec3 vertical( 0.0, aspectRatio * 4.0f, 0.0 );
	glm::vec3 lowerLeftCorner( horizontal.x * -0.5f, vertical.y * -0.5f, -1.0 );


	insp::Ray ray;
	glm::vec3 color;
	std::int16_t iY = 0;

	glm::vec3 origin( 0.0, 0.0, 0.0 );

	insp::Hittable *list[2];
	list[0] = new insp::Sphere( glm::vec3( 0, 0, -1 ), 0.5 );
	list[1] = new insp::Sphere( glm::vec3( 0, -100.5, -1 ), 100 );
	insp::Hittable *world = new insp::HittableList( list, 2 );

	for( std::uint16_t y = 0; y < height; ++y )
	{
		for( std::uint16_t x = 0; x < width; ++x )
		{
			float u = float( x ) / float( width );
			float v = float( y ) / float( height );
			ray = insp::Ray( origin, lowerLeftCorner + u * horizontal + v * vertical );

			glm::vec3 p = ray.PointAtParameter( 2.0f );
			color = RenderColor( ray, world );

			iY = (height - y) - 1;
			buffer[(( iY * width ) + x) * 3] = std::uint8_t( 255.99f * color[0] );
			buffer[( ( iY * width ) + x ) * 3 + 1] = std::uint8_t( 255.99f * color[1] );
			buffer[( ( iY * width ) + x ) * 3 + 2] = std::uint8_t( 255.99f * color[2] );
		}
	}


	stbi_write_png( "final.png", width, height, 3, buffer, 0 );

	free( buffer );

	return 0;
}