#pragma once
// Inspro includes
#include <inspro/raytracing/ray.hpp>
#include <inspro/material/material.hpp>

// Util includes
#include <util/defines.hpp>
#include <util/random.hpp>

// Third party includes
#include <glm/vec3.hpp>


namespace insp
{
    class Lambertian : public Material
    {
    public:
        Lambertian( const glm::vec3 &albedo ): albedo( albedo )
        { }

        virtual bool Scatter( const insp::Ray &rayIn, const insp::HitRecord &hitRecord, glm::vec3 &attenuation, insp::Ray &rayOut ) const
        {
            UNUSED( rayIn );

            glm::vec3 target = hitRecord.position + hitRecord.normal + RandomInUnitSphere();
            rayOut = Ray(hitRecord.position, target - hitRecord.position);
            attenuation = this->albedo;
            return true;
        }

    private:
        glm::vec3 albedo;
    };

} // insp namespace