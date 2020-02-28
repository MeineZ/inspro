#pragma once

// Inspro includes
#include <inspro/raytracing/hittable.hpp>
#include <inspro/raytracing/aabb.hpp>


namespace insp
{
	class HittableList: public Hittable
	{
	public:
		HittableList():
			list( nullptr ),
			listSize( 0 )
		{ }

		HittableList( Hittable **l, std::uint16_t n ):
			list( l ),
			listSize( n )
		{ }

		~HittableList()
		{
			delete[] *list;
			delete list;
		}

		inline Hittable ** GetList()
		{
			return list;
		}

		inline std::uint16_t GetSize()
		{
			return listSize;
		}

		virtual bool Hit(
			const Ray &r, float tMin, float tMax, HitRecord &rec ) const;
		virtual bool BoundingBox( const float t0, const float t1, AABB &box ) const;

	private:
		Hittable **list;
		std::uint16_t listSize;
	};

	bool HittableList::Hit( const Ray &r, float tMin, float tMax,
							HitRecord &rec ) const
	{

		HitRecord tempRec;
		bool hitAnything = false;
		float closestSoFar = tMax;
		for( int i = 0; i < this->listSize; i++ )
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

	bool HittableList::BoundingBox( const float t0, const float t1, AABB &box ) const
	{
		if( this->listSize < 1 ) return false;
		AABB tempBox;
		bool firstTrue = list[0]->BoundingBox( t0, t1, tempBox );
		if( !firstTrue )
		{
			return false;
		}
		else
		{
			box = tempBox;
		}

		for( std::uint16_t i = 1; i < this->listSize; ++i )
		{
			if( list[i]->BoundingBox( t0, t1, tempBox ) )
			{
				box = AABB::SurroundingBox( box, tempBox );
			}
			else
			{
				return false;
			}
		}
		return true;
	}

} // insp namespace