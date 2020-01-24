#pragma once
#include <glm/vec3.hpp>

namespace insp
{
    class Ray
    {
    public:
        Ray() :
			origin( 0.0f, 0.0f, 0.0f ),
            direction( 0.0f, 0.0f, -1.0f )
        { }

        Ray( const glm::vec3 &origin, const glm::vec3 &direction ):
            origin( origin ),
            direction( direction )
        { }

        glm::vec3 GetOrigin() const
        {
            return origin;
        }
        glm::vec3 GetDirection() const
        {
            return direction;
        }
        glm::vec3 PointAtParameter( float t ) const
        {
            return origin + t * direction;
        }

    private:
        glm::vec3 origin;
        glm::vec3 direction;
    };

} // insp namespace