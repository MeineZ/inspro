#pragma once
// Inspro includes
#include <inspro/raytracing/ray.hpp>
#include <inspro/material/material.hpp>

// Util includes
#include <util/math/geometric.hpp>

// Third party includes
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


namespace insp
{
    class Dielectric : public Material
    {
    public:
        Dielectric( const float refractionIndex ): refractionIndex( refractionIndex )
        { }

        virtual bool Scatter( const insp::Ray &rayIn, const insp::HitRecord &hitRecord, glm::vec3 &attenuation, insp::Ray &rayOut ) const
        {
            glm::vec3 outwardNormal;
            glm::vec3 refracted;
            glm::vec3 reflected = glm::reflect( rayIn.GetDirection(), hitRecord.normal );
            float niOverNt;
            attenuation = glm::vec3( 1.0f );

            float reflectProbability;
            float cosine;

            if( glm::dot( rayIn.GetDirection(), hitRecord.normal ) > 0 )
            {
                outwardNormal = -hitRecord.normal;
                niOverNt = this->refractionIndex;
                cosine = this->refractionIndex * glm::dot( rayIn.GetDirection(), hitRecord.normal ) / glm::length(rayIn.GetDirection());
            }
            else
            {
                outwardNormal = hitRecord.normal;
                niOverNt = 1.0f / this->refractionIndex;
                cosine = -glm::dot( rayIn.GetDirection(), hitRecord.normal ) / glm::length( rayIn.GetDirection() );
            }

            refracted = glm::refract( glm::normalize(rayIn.GetDirection()), outwardNormal, niOverNt );

            if( glm::length(refracted) > 0.0f)
            {
                reflectProbability = insp::Schlick( cosine, this->refractionIndex );
            }
            else
            {
                reflectProbability = 1.0f;
            }

            if( insp::RandomFloat() < reflectProbability )
            {
                rayOut = Ray( hitRecord.position, reflected );
            }
            else
            {
                rayOut = Ray( hitRecord.position, refracted );
            }

            return true;
        }

    private:
        float refractionIndex;
    };

} // insp namespace