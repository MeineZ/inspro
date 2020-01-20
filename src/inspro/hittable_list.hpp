#pragma once
#include <inspro/hittable.hpp>

namespace insp
{
    class HittableList: public Hittable
    {
    public:
        HittableList() :
            list(nullptr),
            listSize(0)
        { }

        HittableList( Hittable **l, int n ) :
            list(l),
            listSize(n)
        { }

        virtual bool Hit(
            const Ray &r, float tMin, float tMax, HitRecord &rec ) const;

        Hittable **list;
        int listSize;
    };

    bool HittableList::Hit( const Ray &r, float tMin, float tMax,
                            HitRecord &rec ) const
    {

        HitRecord tempRec;
        bool hitAnything = false;
        float closestSoFar = tMax;
        for( int i = 0; i < listSize; i++ )
        {
            if( list[i]->Hit( r, tMin, closestSoFar, tempRec ) )
            {
                hitAnything = true;
                closestSoFar = tempRec.t;
                rec = tempRec;
            }
        }
        return hitAnything;
    }

} // insp namespace