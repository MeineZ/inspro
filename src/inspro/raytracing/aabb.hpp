#pragma once

// Third party includes
#include <glm/vec3.hpp>


namespace insp
{
    class Ray;

    class AABB
    {
    public:
        AABB();
        AABB( const glm::vec3 &min, const glm::vec3 &max );

        bool Hit( const insp::Ray &r, float tmin, float tmax ) const; 
        static AABB SurroundingBox( AABB box0, AABB box1 );

        static int BoxCompareX( const void *a, const void *b );
        static int BoxCompareY( const void *a, const void *b );
        static int BoxCompareZ( const void *a, const void *b );

    private:
        static int BoxCompare( const void *a, const void *b, std::uint16_t axis );
        glm::vec3 min;
        glm::vec3 max;
    };

} // insp namespace