/******************************************************************************
Class: PhysicsObject
Implements:
Author: Author: Pieran Marris      <p.marris@newcastle.ac.uk> and YOU!
Description: 

This defines all the physical properties of an element in the world, such
as velocity, position, mass etc..





		(\_/)							
		( '_')						
	 /""""""""""""\=========     -----D	
	/"""""""""""""""""""""""\		
....\_@____@____@____@____@_/			

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include <nclgl\Quaternion.h>
#include <nclgl\Matrix3.h>
#include "CollisionShape.h"
#include <functional>

class PhysicsEngine;
class Object;

//Callback function called whenever a collision is detected between two objects
//Params:
//	PhysicsObject* this_obj			- The current object class that contains the callback
//	PhysicsObject* colliding_obj	- The object that is colliding with the given object
//Return:
//  True	- The physics engine should process the collision as normal
//	False	- The physics engine should drop the collision pair and not do any further collision resolution/manifold generation
//			  > This can be useful for AI to see if a player/agent is inside an area/collision volume
typedef std::function<bool(PhysicsObject* this_obj, PhysicsObject* colliding_obj)> PhysicsCollisionCallback;

enum TYPE {
	TEST,
	ORBIT,
	TARGET,
	PROJECTILE,
	STATIC,
	SUN
};

class PhysicsObject
{
	friend class PhysicsEngine;

public:
	PhysicsObject();
	virtual ~PhysicsObject();



	//<--------- GETTERS ------------->
	inline bool					IsEnabled()					const 	{ return m_Enabled; }

	inline float				GetElasticity()				const 	{ return m_Elasticity; }
	inline float				GetFriction()				const 	{ return m_Friction; }

	inline const Vector3&		GetPosition()				const 	{ return m_Position; }
	inline const Vector3&		GetLinearVelocity()			const 	{ return m_LinearVelocity; }
	inline const Vector3&		GetForce()					const 	{ return m_Force; }
	inline float				GetInverseMass()			const 	{ return m_InvMass; }

	inline const Quaternion&	GetOrientation()			const 	{ return m_Orientation; }
	inline const Vector3&		GetAngularVelocity()		const 	{ return m_AngularVelocity; }
	inline const Vector3&		GetTorque()					const 	{ return m_Torque; }
	inline const Matrix3&		GetInverseInertia()			const 	{ return m_InvInertia; }

	inline CollisionShape*		GetCollisionShape()			const 	{ return m_pColShape; }

	inline Object*				GetAssociatedObject()		const	{ return m_pParent; }

	const Matrix4&				GetWorldSpaceTransform()    const;	//Built from scratch or returned from cached value



	//<--------- SETTERS ------------->
	inline void SetElasticity(float elasticity)						{ m_Elasticity = elasticity; }
	inline void SetFriction(float friction)							{ m_Friction = friction; }

	inline void SetPosition(const Vector3& v)						{ m_Position = v;	m_wsTransformInvalidated = true; }
	inline void SetLinearVelocity(const Vector3& v)					{ m_LinearVelocity = v; }
	inline void SetForce(const Vector3& v)							{ m_Force = v; }
	inline void SetInverseMass(const float& v)						{ m_InvMass = v; }

	inline void SetOrientation(const Quaternion& v)					{ m_Orientation = v; m_wsTransformInvalidated = true; }
	inline void SetAngularVelocity(const Vector3& v)				{ m_AngularVelocity = v; }
	inline void SetTorque(const Vector3& v)							{ m_Torque = v; }
	inline void SetInverseInertia(const Matrix3& v)					{ m_InvInertia = v; }

	inline void SetCollisionShape(CollisionShape* colShape)			{ m_pColShape = colShape; }
	


	//Called automatically when PhysicsObject is created through Object::CreatePhysicsNode()
	inline void SetAssociatedObject(Object* obj)					{ m_pParent = obj; }


	//<---------- CALLBACKS ------------>
	inline void SetOnCollisionCallback(PhysicsCollisionCallback callback) { m_OnCollisionCallback = callback; }
	inline bool FireOnCollisionEvent(PhysicsObject* obj_a, PhysicsObject* obj_b)
	{
		return (m_OnCollisionCallback) ? m_OnCollisionCallback(obj_a, obj_b) : true;
	}

	TYPE getObjType() { return objType; }
	void setObjType(TYPE t) { objType = t; }

	bool isAtRest() { return atRest; }
	void setAtRest(bool b) { atRest = b; }

	float getAverageLinear() { return averageLinear; }
	float getAverageAngular() { return averageAngular; }
	void setAverageLinear(float f) { averageLinear = f; }
	void setAverageAngular(float f) { averageAngular = f; }

	float getMinX() { return minX; }
	float getMaxX() { return maxX; }
	void setMinX(float x) { minX = x; }
	void setMaxX(float x) { maxX = x; }

	float getMinZ() { return minZ; }
	float getMaxZ() { return maxZ; }
	void setMinZ(float z) { minZ = z; }
	void setMaxZ(float z) { maxZ = z; }

protected:
	Object*				m_pParent;			//Optional: Attached GameObject or NULL if none set
	bool				m_Enabled;

	mutable bool		m_wsTransformInvalidated;
	mutable Matrix4		m_wsTransform;

	float				m_Elasticity;		//Value from 0-1 definiing how much the object bounces off other objects
	float				m_Friction;			//Value from 0-1 defining how much the object can slide off other objects

	//<---------LINEAR-------------->
	Vector3		m_Position;
	Vector3		m_LinearVelocity;
	Vector3		m_Force;
	float		m_InvMass;

	//<----------ANGULAR-------------->
	Quaternion  m_Orientation;
	Vector3		m_AngularVelocity;
	Vector3		m_Torque;
	Matrix3     m_InvInertia;

	//<----------COLLISION------------>
	CollisionShape*				m_pColShape;
	PhysicsCollisionCallback	m_OnCollisionCallback;

	TYPE objType;
	bool atRest;

	float averageLinear;
	float averageAngular;

	float minX;
	float maxX;
	float minZ;
	float maxZ;
};