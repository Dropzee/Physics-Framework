#pragma once

#include "Object.h"

namespace CommonUtils
{

	//Generates a unique colour based on scalar parameter in the range of 0-1.
	// - Works off HSV color format, in this case Saturation and Value are 100% and Hue is dependant on 'scalar'
	Vector4 GenColour(float scalar, float alpha);

	Object * BuildSphereObject(const std::string & name, const Vector3 & pos, float radius, bool physics_enabled, float inverse_mass, bool collidable, bool dragable, const Vector4 & color, int texID, TYPE t, bool b);
	Object * BuildCuboidObject(const std::string & name, const Vector3 & pos, Vector3 & scale, bool physics_enabled, float inverse_mass, bool collidable, bool dragable, const Vector4 & color, int texID, TYPE t, bool b);


	//Generates a default Sphere object with the parameters specified.
	Object* BuildSphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		int texID = 0,
		TYPE objType = TEST,
		bool atRest = false);

	//Generates a default Cuboid object with the parameters specified
	Object* BuildCuboidObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		int texID = 0,
		TYPE objType = TEST,
		bool atRest = false);
};