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
        Sphere( Vec3 center, float radius ): 
            center( center ), 
            radius( radius )
        { };

        virtual bool Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const;

        Vec3 center;
        float radius;
    };

    bool Sphere::Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const
    {
        Vec3 oc = r.GetOrigin() - center;
        float a = dot( r.GetDirection(), r.GetDirection() );
        float b = dot( oc, r.GetDirection() );
        float c = dot( oc, oc ) - radius * radius;
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