#pragma once

// Inspro includes
#include <inspro/raytracing/hittable.hpp>
#include <inspro/material/material.hpp>


namespace insp
{

	class Sphere: public Hittable
	{
	public:
		Sphere():
			center( 0.0f, 0.0f, 0.0f ),
			radius( 0.0f ),
			material( nullptr )
		{ }
		Sphere( glm::vec3 center, float radius, Material *material ):
			center( center ),
			radius( radius ),
			material( material )
		{ };

		~Sphere()
		{
			delete material;
		}

		virtual bool Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const;

		glm::vec3 center;
		float radius;
		insp::Material *material;
	};

	bool Sphere::Hit( const Ray &r, float tMin, float tMax, HitRecord &rec ) const
	{
		glm::vec3 oc = r.GetOrigin() - this->center;
		float a = glm::dot( r.GetDirection(), r.GetDirection() );
		float b = glm::dot( oc, r.GetDirection() );
		float c = glm::dot( oc, oc ) - this->radius * this->radius;
		float discriminant = b * b - a * c;
		if( discriminant > 0 )
		{
			float temp = ( -b - sqrt( discriminant ) ) / a;
			if( temp < tMax && temp > tMin )
			{
				rec.t = temp;
				rec.position = r.PointAtParameter( rec.t );
				rec.normal = ( rec.position - this->center ) / this->radius;
				rec.material = this->material;
				return true;
			}
			temp = ( -b + sqrt( discriminant ) ) / a;
			if( temp < tMax && temp > tMin )
			{
				rec.t = temp;
				rec.position = r.PointAtParameter( rec.t );
				rec.normal = ( rec.position - this->center ) / this->radius;
				rec.material = this->material;
				return true;
			}
		}
		return false;
	}

} // insp namespace