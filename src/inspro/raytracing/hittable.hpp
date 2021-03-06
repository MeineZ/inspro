#pragma once

// Inspro includes
#include <inspro/raytracing/ray.hpp>
#include <inspro/raytracing/aabb.hpp>


namespace insp
{
    class Material;

    struct HitRecord
    {
        insp::Material *material = nullptr;
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
        float t = 0.0f;
    };

    class Hittable
    {
    public:
        virtual bool Hit(const Ray &r, float tMin, float tMax, HitRecord &rec ) const = 0;
        virtual bool BoundingBox( const float t0, const float t1, AABB &box ) const = 0;
    };

} // insp namespace