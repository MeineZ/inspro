#pragma once
#include <inspro/hittable.hpp>

namespace insp
{

    class Sphere: public Hittable
    {
    public:
        Sphere() :
            center(0.0f, 0.0f, 0.0f),
            radius(0.0f)
        { }
        Sphere( glm::vec3 center, float radius ):
            center( center ), 
            radius( radius )
        { };

        virtual bool Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const;

        glm::vec3 center;
        float radius;
    };

    bool Sphere::Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const
    {
        glm::vec3 oc = r.GetOrigin() - center;
        float a = glm::dot( r.GetDirection(), r.GetDirection() );
        float b = glm::dot( oc, r.GetDirection() );
        float c = glm::dot( oc, oc ) - radius * radius;
        float discriminant = b * b - a * c;
        if( discriminant > 0 )
        {
            float temp = ( -b - sqrt( discriminant ) ) / a;
            if( temp < tMax && temp > tMin )
            {
                rec.t = temp;
                rec.p = r.PointAtParameter( rec.t );
                rec.normal = ( rec.p - center ) / radius;
                return true;
            }
            temp = ( -b + sqrt( discriminant ) ) / a;
            if( temp < tMax && temp > tMin )
            {
                rec.t = temp;
                rec.p = r.PointAtParameter( rec.t );
                rec.normal = ( rec.p - center ) / radius;
                return true;
            }
        }
        return false;
    }

} // insp namespace