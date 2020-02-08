#pragma once

// Inspro includes
#include <inspro/ray.hpp>
#include <inspro/hittable.hpp>

// Third party includes
#include <glm/vec3.hpp>

namespace insp
{
    class Material
    {
    public:
        virtual bool Scatter( const insp::Ray &rayIn, const insp::HitRecord &hitRecord, glm::vec3 &attenuation, insp::Ray &rayOut ) const = 0;
    };

} // insp namespace