#include <iostream>
#include <limits>
#include <cstdint>

#include <stb_image_write.h>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <time.h>
#include <util/random.hpp>

#include <inspro/camera.hpp>
#include <inspro/ray.hpp>
#include <inspro/hittable_list.hpp>
#include <inspro/sphere.hpp>

glm::vec3 RenderColor( const insp::Ray &r, insp::Hittable *world )
{
	insp::HitRecord rec;
	if( world->Hit( r, 0.0, std::numeric_limits<float>::max(), rec ) )
	{
		return 0.5f * ( rec.normal + 1.0f );
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize( r.GetDirection() );
		float t = 0.5f * ( unitDirection.y + 1.0f );
		return ( 1.0f - t ) * glm::vec3( 1.0f ) + t * glm::vec3( 0.5f, 0.7f, 1.0f );
	}
}

int main()
{
	const std::uint8_t MAX_SAMPLES = 50;

	const std::uint16_t WIDTH = 1920;
	const std::uint16_t HEIGHT = 1080;
	std::uint8_t *buffer = static_cast<std::uint8_t *>( malloc( WIDTH * HEIGHT * 3 * sizeof( std::uint8_t ) ) );

	insp::Camera camera;
	{ // Create lookat
		glm::vec3 origin( 0.0f, 0.0f, 0.0f );
		glm::vec3 lookAt( 0.0f, 0.0f, -1.0f );
		glm::vec3 up( 0.0f, 1.0f, 0.0f );
		float aspectRatio = static_cast<float>( WIDTH ) / static_cast<float>( HEIGHT );

		camera.LookAt( origin, lookAt, up, 90, aspectRatio );
	}

	insp::Ray ray;
	glm::vec3 color;
	std::int16_t iY = 0; // To flip the buffer

	insp::Hittable *list[2];
	list[0] = new insp::Sphere( glm::vec3( 0, 0, -1 ), 0.5 );
	list[1] = new insp::Sphere( glm::vec3( 0, -100.5, -1 ), 100 );
	insp::Hittable *world = new insp::HittableList( list, 2 );

	for( std::uint16_t y = 0; y < HEIGHT; ++y )
	{
		iY = ( HEIGHT - y ) - 1;

		for( std::uint16_t x = 0; x < WIDTH; ++x )
		{
			color = glm::vec3( 0.0f );
			for( std::uint16_t sample = 0; sample < MAX_SAMPLES; ++sample )
			{
				float u = ( static_cast<float>( x ) + insp::RandomFloat() ) / static_cast<float>( WIDTH );
				float v = ( static_cast<float>( y ) + insp::RandomFloat() ) / static_cast<float>( HEIGHT );

				ray = camera.GetRay( u, v );
				color += RenderColor( ray, world );
			}
			color /= static_cast<float>( MAX_SAMPLES );

			buffer[( ( iY * WIDTH ) + x ) * 3] = std::uint8_t( 255.99f * color[0] );
			buffer[( ( iY * WIDTH ) + x ) * 3 + 1] = std::uint8_t( 255.99f * color[1] );
			buffer[( ( iY * WIDTH ) + x ) * 3 + 2] = std::uint8_t( 255.99f * color[2] );
		}
	}


	stbi_write_png( "final.png", WIDTH, HEIGHT, 3, buffer, 0 );

	free( buffer );
	delete [] list;
	delete world;

	return 0;
}