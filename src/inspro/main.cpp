// STD includes
#include <iostream>
#include <limits>

// Inspro includes
#include <inspro/object/camera.hpp>
#include <inspro/raytracing/ray.hpp>
#include <inspro/raytracing/hittable_list.hpp>
#include <inspro/object/sphere.hpp>
#include <inspro/material/lambertian.hpp>
#include <inspro/material/metal.hpp>
#include <inspro/material/dielectric.hpp>

// Util includes
#include <util/random.hpp>

// Third party includes
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <stb_image_write.h>

insp::Hittable *BasicScene()
{
	std::uint16_t numberOfBalls = 5;
	insp::Hittable **list = new insp::Hittable * [numberOfBalls];

	list[0] = new insp::Sphere( glm::vec3( 0.0f, 0.0f, -1.0f ), 0.5f, new insp::Lambertian( glm::vec3( 0.1f, 0.2f, 0.5f ) ) );
	list[1] = new insp::Sphere( glm::vec3( 0.0f, -100.5f, -1.0f ), 100.0f, new insp::Lambertian( glm::vec3( 0.8f, 0.8f, 0.0f ) ) );
	list[2] = new insp::Sphere( glm::vec3( 1.0f, 0.0f, -1.0f ), 0.5f, new insp::Metal( glm::vec3( 0.8f, 0.6f, 0.2f ), 0.3f ) );
	list[3] = new insp::Sphere( glm::vec3( -1.0f, 0.0f, -1.0f ), 0.5f, new insp::Dielectric( 1.5f ) );
	list[4] = new insp::Sphere( glm::vec3( -1.0f, 0.0f, -1.0f ), -0.45f, new insp::Dielectric( 1.5f ) );

	return new insp::HittableList( list, numberOfBalls );
}

insp::Hittable *RandomScene()
{
	std::uint16_t numberOfBalls = 500;
	insp::Hittable **list = new insp::Hittable * [numberOfBalls + 1];
	list[0] = new insp::Sphere( glm::vec3( 0.0f, -1000.0f, 0.0f ), 1000.0f, new insp::Lambertian( glm::vec3( 0.5f, 0.5f, 0.5f ) ) );
	std::uint16_t i = 1;
	for( std::int16_t a = -11; a < 11; ++a )
	{
		for( std::int16_t b = -11; b < 11; ++b )
		{
			float choose_mat = insp::RandomFloat();
			glm::vec3 center( a + 0.9f * insp::RandomFloat(), 0.2f, b + 0.9f * insp::RandomFloat() );
			if( ( glm::length( center - glm::vec3( 4.0f, 0.2f, 0.0f ) ) ) > 0.9f )
			{
				if( choose_mat < 0.8f )
				{  // diffuse
					list[i++] = new insp::Sphere( center, 0.2f,
													new insp::Lambertian( glm::vec3( insp::RandomFloat() * insp::RandomFloat(),
													insp::RandomFloat() * insp::RandomFloat(),
													insp::RandomFloat() * insp::RandomFloat() )
					)
					);
				}
				else if( choose_mat < 0.95f )
				{ // metal
					list[i++] = new insp::Sphere( center, 0.2f,
													new insp::Metal( glm::vec3( 0.5f * ( 1.0f + insp::RandomFloat() ),
													0.5f * ( 1.0f + insp::RandomFloat() ),
													0.5f * ( 1.0f + insp::RandomFloat() ) ),
													0.5f * insp::RandomFloat() ) );
				}
				else
				{  // glass
					list[i++] = new insp::Sphere( center, 0.2f, new insp::Dielectric( 1.5f ) );
				}
			}
		}
	}


	list[i++] = new insp::Sphere( glm::vec3( 0.0f, 1.0f, 0.0f ), 1.0f, new insp::Dielectric( 1.5f ) );
	list[i++] = new insp::Sphere( glm::vec3( -4.0f, 1.0f, 0.0f ), 1.0f, new insp::Lambertian( glm::vec3( 0.4f, 0.2f, 0.1f ) ) );
	list[i++] = new insp::Sphere( glm::vec3( 4.0f, 1.0f, 0.0f ), 1.0f, new insp::Metal( glm::vec3( 0.7f, 0.6f, 0.5f ), 0.0f ) );

	return new insp::HittableList( list, numberOfBalls );
}

glm::vec3 RenderColor( const insp::Ray &ray, insp::Hittable *world, std::uint8_t depth )
{
	insp::HitRecord hitRecord;
	if( world->Hit( ray, 0.001f, std::numeric_limits<float>::max(), hitRecord ) )
	{
		insp::Ray scattered;
		glm::vec3 attenuation( 0.0f );

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
	std::uint16_t lastProgress = 0;
	std::uint16_t currentProgress = 0;

	const std::uint8_t MAX_SAMPLES = 50;

	const std::uint16_t WIDTH = 1920;
	const std::uint16_t HEIGHT = 1080;

	std::uint8_t *buffer = static_cast<std::uint8_t *>( malloc( WIDTH * HEIGHT * 3 * sizeof( std::uint8_t ) ) );

	insp::Camera camera;
	{ // Create lookat
		glm::vec3 origin( -5.0f, 0.0f, 5.0f );
		glm::vec3 lookAt( 0.0f, 0.0f, -1.0f );
		glm::vec3 up( 0.0f, 1.0f, 0.0f );
		float focusDistance = glm::length( origin - lookAt );
		float aperature = 2.0f;
		float aspectRatio = static_cast<float>( WIDTH ) / static_cast<float>( HEIGHT );

		camera.LookAt( origin, lookAt, up, 20, aspectRatio, aperature, focusDistance );
	}

	insp::Hittable *world = BasicScene();
	//insp::Hittable *world = RandomScene();

	insp::Ray ray;
	glm::vec3 color;
	std::int16_t iY = 0; // To flip the buffer

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

			currentProgress = static_cast<std::uint16_t>( std::floor( ( ( ( static_cast<float>( y ) *static_cast<float>( WIDTH ) ) + static_cast<float>( x ) ) / static_cast<float>( WIDTH * HEIGHT ) ) * 100.0f ) );

			if( currentProgress > lastProgress )
			{
				lastProgress = currentProgress;
				std::cout << lastProgress << "%" << std::endl;
			}
		}
	}

	stbi_write_png( "output/final.png", WIDTH, HEIGHT, 3, buffer, 0 );

	free( buffer );
	delete world;

	return 0;
}