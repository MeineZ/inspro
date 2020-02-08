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
#include <inspro/lambertian.hpp>
#include <inspro/metal.hpp>

glm::vec3 RenderColor( const insp::Ray &ray, insp::Hittable *world, std::uint8_t depth )
{
	insp::HitRecord hitRecord;
	if( world->Hit( ray, 0.001f, std::numeric_limits<float>::max(), hitRecord ) )
	{
		insp::Ray scattered;
		glm::vec3 attenuation(0.0f);

		if( depth < 50 && hitRecord.material->Scatter( ray, hitRecord, attenuation, scattered ) )
		{
			return attenuation * RenderColor( scattered, world, depth + 1 );
		}
		return attenuation;
	}
	else
	{
		glm::vec3 unitDirection = glm::normalize( ray.GetDirection() );
		float t = 0.5f * ( unitDirection.y + 1.0f );
		return ( 1.0f - t ) * glm::vec3( 1.0f ) + t * glm::vec3( 0.5f, 0.7f, 1.0f );
	}
}

int main()
{
#ifdef _DEBUG
	std::uint16_t lastProgress = 0;
	std::uint16_t currentProgress = 0;
#endif // _DEBUG

	const std::uint8_t MAX_SAMPLES = 2;

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

	insp::Hittable *list[4];
	list[0] = new insp::Sphere( glm::vec3( 0, 0, -1 ), 0.5f, new insp::Lambertian(glm::vec3( 0.8f, 0.3f, 0.3f ) ) );
	list[1] = new insp::Sphere( glm::vec3( 0, -100.5, -1 ), 100.0f, new insp::Lambertian( glm::vec3( 0.8f, 0.8f, 0.0f ) ) );
	list[2] = new insp::Sphere( glm::vec3( 1.0f, 0.0f, -1.0f ), 0.5f, new insp::Metal( glm::vec3( 0.8f, 0.6f, 0.2f ), 0.3f ) );
	list[3] = new insp::Sphere( glm::vec3( -1.0f, 0.0f, -1.0f ), 0.5f, new insp::Metal( glm::vec3( 0.8f ), 1.0f ) );
	insp::Hittable *world = new insp::HittableList( list, 4 );

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
				color += RenderColor( ray, world, 0 );
			}
			color /= static_cast<float>( MAX_SAMPLES );
			color = glm::sqrt( color );

			buffer[( ( iY * WIDTH ) + x ) * 3] = std::uint8_t( 255.99f * color[0] );
			buffer[( ( iY * WIDTH ) + x ) * 3 + 1] = std::uint8_t( 255.99f * color[1] );
			buffer[( ( iY * WIDTH ) + x ) * 3 + 2] = std::uint8_t( 255.99f * color[2] );

		#ifdef _DEBUG
			currentProgress = static_cast<std::uint16_t>(std::floor( ( ( ( static_cast<float>( y ) *static_cast<float>( WIDTH ) ) + static_cast<float>( x ) ) / static_cast<float>( WIDTH * HEIGHT ) ) * 100.0f ));

			if(currentProgress > lastProgress)
			{
				lastProgress = currentProgress;
				std::cout << lastProgress << "%" << std::endl;
			}
		#endif
		}
	}


	stbi_write_png( "final.png", WIDTH, HEIGHT, 3, buffer, 0 );

	free( buffer );
	delete list[0];
	delete list[1];
	delete list[2];
	delete list[3];
	delete world;

	return 0;
}