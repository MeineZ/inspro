#pragma once
#include <inspro/ray.hpp>

namespace insp
{

    struct HitRecord
    {
        float t = 0.0f;
        glm::vec3 position { 0.0f, 0.0f, 0.0f };
        glm::vec3 normal { 0.0f, 0.0f, 0.0f };
    };

    class Hittable
    {
    public:
        virtual bool Hit(
            const Ray &r, float tMin, float tMax, HitRecord &rec ) const = 0;
    };

} // insp namespace