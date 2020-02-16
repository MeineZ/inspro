#pragma once
// Inspro includes
#include <inspro/raytracing/ray.hpp>
#include <inspro/material/material.hpp>

// Util includes
#include <util/random.hpp>

// Third party includes
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


namespace insp
{
    class Metal: public Material
    {
    public:
        Metal( const glm::vec3 &albedo, float fuzziness ): albedo( albedo ), fuzziness( fuzziness )
        { 
            if( this->fuzziness > 1.0f )
            {
                this->fuzziness = 1.0f;
            }
        }

        virtual bool Scatter( const insp::Ray &rayIn, const insp::HitRecord &hitRecord, glm::vec3 &attenuation, insp::Ray &rayOut ) const
        {
            glm::vec3 reflected = glm::reflect( glm::normalize( rayIn.GetDirection() ), hitRecord.normal );
            rayOut = Ray( hitRecord.position, reflected + this->fuzziness * RandomInUnitSphere());
            attenuation = this->albedo;
            return (dot(rayOut.GetDirection(), hitRecord.normal) > 0.0f);
        }

    private:
        glm::vec3 albedo;
        float fuzziness;
    };

} // insp namespace