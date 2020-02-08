#pragma once

// Inspro includes
#include <inspro/ray.hpp>

namespace insp
{
    class Material;

    struct HitRecord
    {
        insp::Material *material;
        glm::vec3 position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 normal{ 0.0f, 0.0f, 0.0f };
        float t = 0.0f;
    };

    class Hittable
    {
    public:
        virtual bool Hit(
            const Ray &r, float tMin, float tMax, HitRecord &rec ) const = 0;
    };

} // insp namespace