// STD includes
#include <iostream>
#include <limits>
#include <atomic>
#include <thread>
#include <algorithm>
#include <chrono>

// Inspro includes
#include <inspro/object/camera.hpp>
#include <inspro/raytracing/ray.hpp>
#include <inspro/raytracing/hittable_list.hpp>
#include <inspro/raytracing/bvh_node.hpp>
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

const std::uint16_t OUTPUT_WIDTH = 1920;
const std::uint16_t OUTPUT_HEIGHT = 1080;

const std::uint8_t MAX_SAMPLES = 128;
const std::int32_t THREAD_BUFFER_SIZE = 128;
const std::int32_t MAX_INDEX = static_cast<std::int32_t>( std::ceil( static_cast<double>( OUTPUT_WIDTH ) / static_cast<double>( THREAD_BUFFER_SIZE ) ) *std::ceil( static_cast<double>( OUTPUT_HEIGHT ) / static_cast<double>( THREAD_BUFFER_SIZE ) ) );

namespace glob
{
	std::atomic<std::uint16_t> index;
}


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
	std::uint16_t numberOfBalls = 501;
	insp::Hittable **list = new insp::Hittable * [numberOfBalls];
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

	return new insp::HittableList( list, i );
}

glm::vec3 RenderColor( const insp::Ray &ray, const insp::Hittable *world, std::uint8_t depth )
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

void TraceRays( std::uint8_t *buffer, const insp::Camera * camera, const insp::Hittable * world)
{
	
	std::int32_t start[2], end[2];

	auto getArea = [] ( std::int32_t *start, std::int32_t *end) {
		std::int32_t currentIndex = glob::index++;
		std::int32_t currentPixel = ( THREAD_BUFFER_SIZE * currentIndex );

		if( currentIndex >= MAX_INDEX )
		{
			start[0] = start[1] = end[0] = end[1] = -1;
			return;
		}

		std::cout << MAX_INDEX - currentIndex << " buffers left!" << std::endl;

		start[0] = static_cast<std::int32_t>( currentPixel % OUTPUT_WIDTH );
		start[1] = static_cast<std::int32_t>( ( currentPixel / OUTPUT_WIDTH ) * THREAD_BUFFER_SIZE );
		end[0] = std::min(start[0] + THREAD_BUFFER_SIZE, static_cast<std::int32_t>(OUTPUT_WIDTH) );
		end[1] = std::min(start[1] + THREAD_BUFFER_SIZE, static_cast<std::int32_t>( OUTPUT_HEIGHT ));
	};

	std::uint16_t startingIndex = glob::index;

	insp::Ray ray;
	glm::vec3 color;
	std::uint32_t pixelPos;
	std::uint16_t iY = 0; // To flip the buffer

	do
	{
		getArea( start, end );

		if( start[0] == -1 )
		{
			break;
		}

		for( std::int32_t y = start[1]; y < end[1]; ++y )
		{
			iY = static_cast<std::uint16_t>( ( static_cast<std::int32_t>(OUTPUT_HEIGHT) - y ) - static_cast<std::int32_t>( 1 ));

			for( std::int32_t x = start[0]; x < end[0]; ++x )
			{
				color = glm::vec3( 0.0f );
				for( std::uint16_t sample = 0; sample < MAX_SAMPLES; ++sample )
				{
					float u = ( static_cast<float>( x ) + insp::RandomFloat() ) / static_cast<float>( OUTPUT_WIDTH );
					float v = ( static_cast<float>( y ) + insp::RandomFloat() ) / static_cast<float>( OUTPUT_HEIGHT );

					ray = camera->GetRay( u, v );
					color += RenderColor( ray, world, 0 );
				}
				color /= static_cast<float>( MAX_SAMPLES );
				color = glm::sqrt( color );

				pixelPos = static_cast<std::uint32_t>(( ( iY * OUTPUT_WIDTH ) + x ) * 3);

				buffer[pixelPos] = std::uint8_t( 255.99f * color[0] );
				buffer[pixelPos + 1] = std::uint8_t( 255.99f * color[1] );
				buffer[pixelPos + 2] = std::uint8_t( 255.99f * color[2] );
			}
		}

	} while(true);

	std::cout << "Thread " << startingIndex << " done!\n";
}

int main()
{
	std::uint8_t *buffer = static_cast<std::uint8_t *>( malloc( OUTPUT_WIDTH * OUTPUT_HEIGHT * 3 * sizeof( std::uint8_t ) ) );

	insp::Camera camera;
	{ // Create lookat
		glm::vec3 origin( 0.0f, 0.0f, 5.0f );
		glm::vec3 lookAt( 0.0f, 0.0f, -1.0f );
		glm::vec3 up( 0.0f, 1.0f, 0.0f );
		float focusDistance = glm::length( origin - lookAt );
		float aperature = 2.0f;
		float aspectRatio = static_cast<float>( OUTPUT_WIDTH ) / static_cast<float>( OUTPUT_HEIGHT );

		camera.LookAt( origin, lookAt, up, 20, aspectRatio, aperature, focusDistance );
	}

	// Create the scene
	//insp::HittableList *world = static_cast<insp::HittableList*>(RandomScene());
	insp::HittableList *world = static_cast<insp::HittableList*>(BasicScene());

	// Create the BVH
	insp::BVHNode *bvh = new insp::BVHNode( world->GetList(), world->GetSize(), 0.0f, 0.0f);

	// Create threads and run the raytracer
	glob::index = 0;

	const std::uint32_t numberOfThreads = std::thread::hardware_concurrency();
	std::thread **threadPool = static_cast<std::thread**>(malloc( numberOfThreads * sizeof(std::thread*)));

	for( std::uint32_t i = 0; i < numberOfThreads; ++i )
	{
		threadPool[i] = new std::thread( &TraceRays, buffer, &camera, bvh );
	}

	for( std::uint32_t i = 0; i < numberOfThreads; ++i )
	{
		threadPool[i]->join();
		delete threadPool[i];
	}

	stbi_write_png( "output/final.png", OUTPUT_WIDTH, OUTPUT_HEIGHT, 3, buffer, 0 );

	free( buffer );
	free(threadPool);
	delete bvh;
	delete world;

	return 0;
}