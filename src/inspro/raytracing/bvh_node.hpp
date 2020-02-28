#pragma once

// Inspro includes
#include <inspro/raytracing/hittable.hpp>
#include <inspro/raytracing/aabb.hpp>

// Third party includes
#include <glm/vec3.hpp>


namespace insp
{
    class BVHNode: public Hittable
    {
    public:
        BVHNode();
        BVHNode( Hittable **hittable, std::uint16_t n, float t0, float t1 );
        ~BVHNode();

        virtual bool Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const;
        virtual bool BoundingBox( const float t0, const float t1, AABB &aabb ) const;

    private:
        Hittable *left;
        Hittable *right;
        AABB box;
    };

} // insp namespace