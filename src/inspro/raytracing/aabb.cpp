#pragma once
// Class include
#include <inspro/raytracing/aabb.hpp>

// Inspro includes
#include <inspro/raytracing/ray.hpp>
#include <inspro/raytracing/hittable.hpp>

// Util includes
#include <util/math/generic.hpp>

namespace insp
{
    AABB::AABB() :
        min( 0.0f ),
        max( 0.0f )
    { }

    AABB::AABB( const glm::vec3 &min, const glm::vec3 &max ) :
        min( min ),
        max( max )
    { }

    bool AABB::Hit( const insp::Ray &ray, float tmin, float tmax ) const
    {
        for( int a = 0; a < 3; a++ )
        {
            float invD = 1.0f / ray.GetDirection().x;
            float t0 = ( this->min.x - ray.GetOrigin().x ) * invD;
            float t1 = ( this->max.x - ray.GetOrigin().x ) * invD;
            if( invD < 0.0f )
                std::swap( t0, t1 );
            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if( tmax <= tmin )
                return false;
        }
        return true;
    }

    AABB AABB::SurroundingBox( AABB box0, AABB box1 )
    {
        glm::vec3 small( ffmin( box0.min.x, box1.min.x ),
                    ffmin( box0.min.y, box1.min.y ),
                    ffmin( box0.min.z, box1.min.z ) );
        glm::vec3 big( ffmax( box0.max.x, box1.max.x ),
                  ffmax( box0.max.y, box1.max.y ),
                  ffmax( box0.max.z, box1.max.z ) );
        return AABB( small, big );
    }

    int AABB::BoxCompare( const void *a, const void *b, std::uint16_t axis )
    {
        AABB boxLeft, boxRight;
        Hittable *hittableA = *(Hittable **) a;
        Hittable *hittableB = *(Hittable **) b;

        if( !hittableA->BoundingBox( 0, 0, boxLeft ) || !hittableB->BoundingBox( 0, 0, boxRight ) )
        {
            assert( false );
        }

        if( boxLeft.min[axis] - boxRight.min[axis] < 0.0f )
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    int AABB::BoxCompareX( const void *a, const void *b )
    {
        return AABB::BoxCompare(a, b, 0);
    }
    int AABB::BoxCompareY( const void *a, const void *b )
    {
        return AABB::BoxCompare( a, b, 1 );
    }
    int AABB::BoxCompareZ( const void *a, const void *b )
    {
        return AABB::BoxCompare( a, b, 2 );
    }

} // insp namespace