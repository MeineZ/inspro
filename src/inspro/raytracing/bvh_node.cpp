#pragma once
// Class include
#include <inspro/raytracing/bvh_node.hpp>

// Inspro includes
#include <inspro/raytracing/ray.hpp>

// Util includes
#include <util/defines.hpp>
#include <util/random.hpp>

namespace insp
{
	BVHNode::BVHNode():
		left( nullptr ),
		right( nullptr ),
		box()
	{ }

	BVHNode::BVHNode( Hittable **hittable, std::uint16_t n, float t0, float t1 )
	{
		std::uint16_t axis = static_cast<std::uint16_t>( 3.0f * RandomFloat() );
		
		switch( axis )
		{
			case 0:
			{
				qsort( hittable, n, sizeof( Hittable * ), AABB::BoxCompareX );
				break;
			}
			case 1:
			{
				qsort( hittable, n, sizeof( Hittable * ), AABB::BoxCompareY );
				break;
			}
			default:
				qsort( hittable, n, sizeof( Hittable * ), AABB::BoxCompareZ );
		}
		

		if( n == 1 )
		{
			this->left = this->right = hittable[0];
		}
		else if( n == 2 )
		{
			this->left = hittable[0];
			this->right = hittable[1];
		}
		else
		{
			std::uint16_t halfN = static_cast<std::uint16_t>( std::floor( double( n ) / 2.0 ));
			this->left = new BVHNode( hittable, halfN, t0, t1 );
			this->right = new BVHNode( hittable + halfN, n - halfN, t0, t1 );
		}

		AABB boxLeft, boxRight;

		if( !this->left->BoundingBox( t0, t1, boxLeft ) ||
			!this->right->BoundingBox( t0, t1, boxRight ) )
		{
			assert( false );
		}

		this->box = AABB::SurroundingBox( boxLeft, boxRight );
	}

	BVHNode::~BVHNode()
	{
		BVHNode *node = nullptr;
		if( this->left != this->right) 
		{
			node = reinterpret_cast<BVHNode *>( this->right );
			if( node != nullptr )
			{
				delete this->right;
			}
		}

		node = reinterpret_cast<BVHNode *>( this->left );
		if( node != nullptr )
		{
			delete this->left;
		}

		this->left = nullptr;
		this->right = nullptr;
	}

	bool BVHNode::Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const
	{
		if( this->box.Hit( r, tMin, tMax ) )
		{
			HitRecord leftRec, rightRec;
			bool hitLeft = this->left->Hit( r, tMin, tMax, leftRec );
			bool hitRight = this->right->Hit( r, tMin, tMax, rightRec );

			if( hitLeft && hitRight )
			{
				if( leftRec.t < rightRec.t )
				{
					rec = leftRec;
				}
				else
				{
					rec = rightRec;
				}
				return true;
			}
			else if( hitLeft )
			{
				rec = leftRec;
				return true;
			}
			else if( hitRight )
			{
				rec = rightRec;
				return true;
			}
		}
		return false;
	}

	bool BVHNode::BoundingBox( const float t0, const float t1, AABB &aabb ) const
	{
		UNUSED( t0 );
		UNUSED( t1 );

		aabb = this->box;
		return true;
	}

} // insp namespace