#include "PhysicsEngine.h"
#include "Object.h"
#include "CollisionDetectionSAT.h"
#include "NCLDebug.h"
#include <nclgl\Window.h>
#include <omp.h>
#include <algorithm>


void PhysicsEngine::SetDefaults()
{
	m_DebugDrawFlags = NULL;
	m_IsPaused = false;
	m_UpdateTimestep = 1.0f / 60.f;
	m_UpdateAccum = 0.0f;
	m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_DampingFactor =  0.999f;
	score = 0;
}

PhysicsEngine::PhysicsEngine()
{
	SetDefaults();
}

PhysicsEngine::~PhysicsEngine()
{
	RemoveAllPhysicsObjects();
}

void PhysicsEngine::AddPhysicsObject(PhysicsObject* obj)
{
	m_PhysicsObjects.push_back(obj);
}

void PhysicsEngine::RemovePhysicsObject(PhysicsObject* obj)
{
	//Lookup the object in question
	auto found_loc = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), obj);

	//If found, remove it from the list
	if (found_loc != m_PhysicsObjects.end())
	{
		m_PhysicsObjects.erase(found_loc);
	}
}

void PhysicsEngine::RemoveAllPhysicsObjects()
{
	//Delete and remove all constraints/collision manifolds
	for (Constraint* c : m_vpConstraints)
	{
		delete c;
	}
	m_vpConstraints.clear();

	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();


	//Delete and remove all physics objects
	// - we also need to inform the (possible) associated game-object
	//   that the physics object no longer exists
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		if (obj->m_pParent != NULL) obj->m_pParent->m_pPhysicsObject = NULL;
		delete obj;
	}
	m_PhysicsObjects.clear();
}


void PhysicsEngine::Update(float deltaTime)
{
	const int max_updates_per_frame = 5;

	if (!m_IsPaused)
	{
		m_UpdateAccum += deltaTime;
		for (int i = 0; (m_UpdateAccum >= m_UpdateTimestep) && i < max_updates_per_frame; ++i)
		{
			m_UpdateAccum -= m_UpdateTimestep;
			if (!m_IsPaused) UpdatePhysics(); //Additional check here incase physics was paused mid-update and the contents of the physics need to be displayed
		}

		if (m_UpdateAccum >= m_UpdateTimestep)
		{
			NCLERROR("Physics too slow to run in real time!");
			//Drop Time in the hope that it can continue to run in real-time
			m_UpdateAccum = 0.0f;
		}
	}
}


void PhysicsEngine::UpdatePhysics()
{
	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();

	//Check for collisions
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	//Solve collision constraints
	SolveConstraints();

	//Update movement
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		UpdatePhysicsObject(obj);
	}
}


void PhysicsEngine::SolveConstraints()
{
	//Optional step to allow constraints to 
	// precompute values based off current velocities 
	// before they are updated in the main loop below.
	for (Manifold* m : m_vpManifolds)		m->PreSolverStep(m_UpdateTimestep);
	for (Constraint* c : m_vpConstraints)	c->PreSolverStep(m_UpdateTimestep);

	// Solve all Constraints and Collision Manifolds
	/*for (Manifold* m : m_vpManifolds)		m->ApplyImpulse();
	for (Constraint* c : m_vpConstraints)	c->ApplyImpulse();*/
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i)
	{
		for (Manifold * m : m_vpManifolds)
		{
			m->ApplyImpulse();
		}
		for (Constraint * c : m_vpConstraints)
		{
			c->ApplyImpulse();

		}
	}
}


void PhysicsEngine::UpdatePhysicsObject(PhysicsObject* obj)
{

	if (obj->moveFlag) {
		obj->SetPosition(Vector3(1000.f, 1000.f, 1000.f));
	}

	//Gravity
	if (obj->getObjType() == TEST) {
		if (obj->m_InvMass > 0.0f) {
			obj->m_LinearVelocity += m_Gravity * m_UpdateTimestep;
		}
	}
	if (obj->getObjType() == SUN) {
		Vector3 dist, nDist, force;
		for each (PhysicsObject* obj2 in m_PhysicsObjects) {
			if (obj != obj2) {
				dist = obj2->GetPosition() - obj->GetPosition();
				nDist = dist;
				nDist.Normalise();
				force += nDist * G / (obj->GetInverseMass() * obj2->GetInverseMass() * Vector3::Dot(dist, dist));
				obj->SetForce(force);
			}
		}
	}

	//Semi-Implicit
	obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;

	//Damping
	obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;

	//Update Pos
	obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;

	//Angular Rot
	obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;

	//Damping
	obj->m_AngularVelocity = obj->m_AngularVelocity * m_DampingFactor;

	//Orientation
	obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * (obj->m_AngularVelocity * m_UpdateTimestep * 0.5f);
	obj->m_Orientation.Normalise();

	//Invalidiate world transform
	obj->m_wsTransformInvalidated = true;
}

//Sort for sweep
bool compareFunc(PhysicsObject * left, PhysicsObject * right) {

	if (left->GetCollisionShape() != NULL && right->GetCollisionShape() != NULL) {
		Vector3 *xMinL, *xMaxL, *xMinR, *xMaxR, *zMinL, *zMaxL, *zMinR, *zMaxR;
		xMinL = &Vector3();
		xMaxL = &Vector3();
		xMinR = &Vector3();
		xMaxR = &Vector3();
		zMinL = &Vector3();
		zMaxL = &Vector3();
		zMinR = &Vector3();
		zMaxR = &Vector3();

		left->GetCollisionShape()->GetMinMaxVertexOnAxis(left, Vector3(1, 0, 0), xMinL, xMaxL);
		left->setMinX(xMinL->x);
		left->setMaxX(xMaxL->x);
		right->GetCollisionShape()->GetMinMaxVertexOnAxis(right, Vector3(1, 0, 0), xMinR, xMaxR);
		right->setMinX(xMinR->x);
		right->setMaxX(xMaxR->x);

		left->GetCollisionShape()->GetMinMaxVertexOnAxis(left, Vector3(0, 0, 1), zMinL, zMaxL);
		left->setMinZ(zMinL->z);
		left->setMaxZ(zMaxL->z);
		right->GetCollisionShape()->GetMinMaxVertexOnAxis(right, Vector3(0, 0, 1), zMinR, zMaxR);
		right->setMinZ(zMinR->z);
		right->setMaxZ(zMaxR->z);

		return zMinL->z < zMinR->z;
	}
	return true;
	
}

void PhysicsEngine::BroadPhaseCollisions()
{
	m_BroadphaseCollisionPairs.clear();

	PhysicsObject *m_pObj1, *m_pObj2;
	//	The broadphase needs to build a list of all potentially colliding objects in the world,
	//	which then get accurately assesed in narrowphase. If this is too coarse then the system slows down with
	//	the complexity of narrowphase collision checking, if this is too fine then collisions may be missed.

	//Rest State
	for each(PhysicsObject * po in m_PhysicsObjects) {
		if (po->getObjType() != STATIC && po->getObjType() != SUN) {
			po->setAverageAngular(updateAverage(po->getAverageAngular(), po->GetAngularVelocity().LengthSquared()));
			po->setAverageLinear(updateAverage(po->getAverageLinear(), po->GetLinearVelocity().LengthSquared()));
			if (po->getAverageLinear() < 0.001f && po->getAverageAngular() < 0.1f) {
				po->SetLinearVelocity(Vector3());
				po->SetAngularVelocity(Vector3());
				po->setAtRest(true);
				if (po->getObjType() == TEST) { //Only change colour if in test scene
					po->GetAssociatedObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.f));
				}
			}
			else {
				po->setAtRest(false);
				if (po->getObjType() == TEST) { //Only change colour if in test scene
					po->GetAssociatedObject()->SetColour(Vector4(0.8f, 0.2f, 0.f, 1.f));
				}
			}
		}
	}

	if (m_PhysicsObjects.size() > 0)
	{
		//Sort for sweep
		std::sort(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), compareFunc);

		for (size_t i = 0; i < m_PhysicsObjects.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_PhysicsObjects.size(); ++j)
			{
				m_pObj1 = m_PhysicsObjects[i];
				m_pObj2 = m_PhysicsObjects[j];

				if (m_pObj1->isAtRest() && m_pObj2->isAtRest()) {
					continue;
				}

				//Check they both atleast have collision shapes
				if (m_pObj1->GetCollisionShape() != NULL
					&& m_pObj2->GetCollisionShape() != NULL)
				{

					//Check based on sort
					if (m_pObj1->getMaxZ() < m_pObj2->getMinZ()) {
						break;
					}

					//Also check X axis
					if (m_pObj1->getMaxX() > m_pObj2->getMinX()) {
						CollisionPair cp;
						cp.pObjectA = m_pObj1;
						cp.pObjectB = m_pObj2;
						m_BroadphaseCollisionPairs.push_back(cp);
					}
				}

			}
		}
	}
}


void PhysicsEngine::NarrowPhaseCollisions()
{
	if (m_BroadphaseCollisionPairs.size() > 0)
	{
		//Collision data to pass between detection and manifold generation stages.
		CollisionData colData;				

		//Collision Detection Algorithm to use
		CollisionDetectionSAT colDetect;	

		// Iterate over all possible collision pairs and perform accurate collision detection
		for (size_t i = 0; i < m_BroadphaseCollisionPairs.size(); ++i)
		{
			CollisionPair& cp = m_BroadphaseCollisionPairs[i];

			CollisionShape *shapeA = cp.pObjectA->GetCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->GetCollisionShape();

			colDetect.BeginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->GetCollisionShape(),
				cp.pObjectB->GetCollisionShape());

			//--TUTORIAL 4 CODE--
			// Detects if the objects are colliding - Seperating Axis Theorem
			if (colDetect.AreColliding(&colData))
			{
				//Draw collision data to the window if requested
				// - Have to do this here as colData is only temporary. 
				if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS)
				{
					NCLDebug::DrawPointNDT(colData._pointOnPlane, 0.1f, Vector4(0.5f, 0.5f, 1.0f, 1.0f));
					NCLDebug::DrawThickLineNDT(colData._pointOnPlane, colData._pointOnPlane - colData._normal * colData._penetration, 0.05f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
				}

				//Check to see if any of the objects have collision callbacks that dont want the objects to physically collide
				bool okA = cp.pObjectA->FireOnCollisionEvent(cp.pObjectA, cp.pObjectB);
				bool okB = cp.pObjectB->FireOnCollisionEvent(cp.pObjectB, cp.pObjectA);

				if (okA && okB)
				{
					if (cp.pObjectA->isAtRest()) {
						cp.pObjectA->setAtRest(false);
					}
					if (cp.pObjectB->isAtRest()) {
						cp.pObjectB->setAtRest(false);
					}
					if (cp.pObjectA->getObjType()  == PROJECTILE) {
						if (cp.pObjectB->getObjType() == TARGET) {
							score += 100;
							/*float dist = (cp.pObjectA->GetPosition() - cp.pObjectB->GetPosition()).Length();
							if ((int)dist != 0) {
							score += 10 * (100 / (int)dist);
							}
							else {
							score += 1000;
							}*/
						}
						cp.pObjectA->moveFlag = true;
						//cp.pObjectA->SetPosition(Vector3(1000.f, 1000.f, 1000.f));
					}
					else if (cp.pObjectB->getObjType() == PROJECTILE) {
						if (cp.pObjectA->getObjType() == TARGET) {
							score += 100;
							/*float dist = (cp.pObjectA->GetPosition() - cp.pObjectB->GetPosition()).Length();
							if ((int)dist != 0) {
								score += 10 * (100 / (int)dist);
							}
							else {
								score += 1000;
							}*/
						}
						cp.pObjectB->moveFlag = true;
						//cp.pObjectB->SetPosition(Vector3(1000.f, 1000.f, 1000.f));
					}
					else {
						//-- TUTORIAL 5 CODE --
						// Build full collision manifold that will also handle the collision response between the two objects in the solver stage
						Manifold* manifold = new Manifold();
						manifold->Initiate(cp.pObjectA, cp.pObjectB);

						// Construct contact points that form the perimeter of the collision manifold
						colDetect.GenContactPoints(manifold);

						// Add to list of manifolds that need solving
						m_vpManifolds.push_back(manifold);
					}
				}
			}
		}

	}
}


void PhysicsEngine::DebugRender()
{
	// Draw all collision manifolds
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_MANIFOLD)
	{
		for (Manifold* m : m_vpManifolds)
		{
			m->DebugDraw();
		}
	}

	// Draw all constraints
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_CONSTRAINT)
	{
		for (Constraint* c : m_vpConstraints)
		{
			c->DebugDraw();
		}
	}

	// Draw all associated collision shapes
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES)
	{
		for (PhysicsObject* obj : m_PhysicsObjects)
		{
			if (obj->GetCollisionShape() != NULL)
			{
				obj->GetCollisionShape()->DebugDraw(obj);
			}
		}
	}
}

void PhysicsEngine::notAtRest(){
	for each (PhysicsObject* obj in m_PhysicsObjects) {
		obj->setAtRest(false);
	}
}

void PhysicsEngine::SetPaused(bool paused) { 
	m_IsPaused = paused; 
	if (!paused) { 
		notAtRest(); 
	} 
}

float PhysicsEngine::updateAverage(float var, float val) {
	var -= var / 15.f;
	var += val / 15.f;
	return var;
}

