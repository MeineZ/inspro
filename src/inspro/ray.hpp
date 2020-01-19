#pragma once
#include <inspro/Vec3.hpp>

namespace insp
{
    class Ray
    {
    public:
        Ray()
        { }

        Ray( const Vec3 &origin, const Vec3 &direction ):
            origin( origin ),
            direction( direction )
        { }

        Vec3 GetOrigin() const
        {
            return origin;
        }
        Vec3 GetDirection() const
        {
            return direction;
        }
        Vec3 PointAtParameter( float t ) const
        {
            return origin + t * direction;
        }

    private:
        Vec3 origin;
        Vec3 direction;
    };

} // insp namespace