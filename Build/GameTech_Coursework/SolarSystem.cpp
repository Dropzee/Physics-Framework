#include "SolarSystem.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\OriginConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>
using namespace CommonUtils;

SolarSystem::SolarSystem(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer(NULL)
{
	speed = 0.5f;
	size = 0.5f;
}

SolarSystem::~SolarSystem()
{

}


void SolarSystem::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);
	PhysicsEngine::Instance()->setDamping(1.0f);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(50.0f, 10.0f, -50.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-5.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");

	//<--- SCENE CREATION --->

	//Planets!!!!!!!
	Object* planet1 = BuildSphereObject("SUN", Vector3(0.0f, 0.0f, 0.0f), 5.0f, true, 0.00001f, true, false, Vector4(1, 1, 1, 1), 7, ORBIT);
	planet1->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.0f));
	planet1->Physics()->SetElasticity(0.0f);
	this->AddGameObject(planet1);
	Object* planet2 = BuildSphereObject("MERCURY", Vector3(10.0f, 0.0f, 0.0f), 0.5f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 4, ORBIT);
	planet2->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 42.0f));
	planet2->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet2);
	Object* planet3 = BuildSphereObject("VENUS", Vector3(20.0f, 0.0f, 0.0f), 1.0f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 9, ORBIT);
	planet3->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 30.0f));
	planet3->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet3);
	Object* planet4 = BuildSphereObject("EARTH", Vector3(30.0f, 0.0f, 0.0f), 1.0f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 1, ORBIT);
	planet4->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 18.0f));
	planet4->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet4);
	Object* planet5 = BuildSphereObject("MARS", Vector3(40.0f, 0.0f, 0.0f), 0.7f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 3, ORBIT);
	planet5->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 16.0f));
	planet5->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet5);
	Object* planet6 = BuildSphereObject("JUPITER", Vector3(50.0f, 0.0f, 0.0f), 2.0f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 2, ORBIT);
	planet6->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 15.0f));
	planet6->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f)); 
	this->AddGameObject(planet6);
	Object* planet7 = BuildSphereObject("SATURN", Vector3(60.0f, 0.0f, 0.0f), 1.8f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 6, ORBIT);
	planet7->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 14.5f));
	planet7->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet7);
	Object* planet8 = BuildSphereObject("URANUS", Vector3(70.0f, 0.0f, 0.0f), 1.2f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 8, ORBIT);
	planet8->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 14.2f));
	planet8->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet8);
	Object* planet9 = BuildSphereObject("NEPTUNE", Vector3(80.0f, 0.0f, 0.0f), 1.2f, true, 0.1f, true, false, Vector4(1, 1, 1, 1), 5, ORBIT);
	planet9->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 14.0f));
	planet9->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
	this->AddGameObject(planet9);

	//Target
	Object* target = BuildCuboidObject("TARGET", Vector3(5.0f, 0.0f, 0.0f), Vector3(0.01f,2.5f,2.5f), true, 0.1f, true, false, Vector4(1, 1, 1, 1), 0, TARGET);
	this->AddGameObject(target);

	//PhysicsEngine::Instance()->AddConstraint(new DistanceConstraint(
	//	planet1->Physics(),													//Physics Object A
	//	target->Physics(),													//Physics Object B
	//	planet1->Physics()->GetPosition(),									//Attachment Position on Object A	-> Currently the far right edge
	//	target->Physics()->GetPosition()));	//Attachment Position on Object B	-> Currently the far left edge 

	/*PhysicsEngine::Instance()->AddConstraint(new OriginConstraint(
		planet1->Physics(),
		planet1->Physics()->GetPosition()));*/
}

void SolarSystem::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene();
}

void SolarSystem::OnUpdateScene(float dt)
{
	m_AccumTime += dt;


	// You can add status entries to the top left from anywhere in the program
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Welcome to the Game Tech Framework!");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 You can move the black car with the arrow keys");

	// You can print text using 'printf' formatting
	bool donkeys = false;
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 The %s in this scene are dragable", donkeys ? "donkeys" : "cubes");


	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);


	//Or move our car around the scene..
	{
		const float mv_speed = 10.f * dt;			//Motion: Meters per second
		const float rot_speed = 90.f * dt;			//Rotation: Degrees per second

		//Projectile
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
			Object* obj = BuildSphereObject(
				"",																	// Optional: Name
				SceneManager::Instance()->GetCamera()->GetPosition(),				// Position
				1.0f * size,														// Half-Dimensions
				true,																// Physics Enabled?
				0.1f,																// Physical Mass (must have physics enabled)
				true,																// Physically Collidable (has collision shape)
				false,																// Dragable by user?
				Vector4(1, 1, 1, 1),												// Render colour
				0,
				ORBIT);

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			obj->Physics()->SetLinearVelocity(forward * 50.0f);
			this->AddGameObject(obj);

		}
	}
}