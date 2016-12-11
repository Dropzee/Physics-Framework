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

	//Sun
	Object* sun = BuildSphereObject("SUN", Vector3(0.0f, 0.0f, 0.0f), 5.0f, true, 0.00001f, true, false, Vector4(1, 1, 1, 1), 7, ORBIT);
	sun->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.0f));
	this->AddGameObject(sun);
	
	//Planets!!!!!!!
	float sizes[8] = {0.5f, 1.0f, 1.0f, 0.7f, 2.0f, 1.8f, 1.2f, 1.2f};
	int texIDs[8] = {4, 9, 1, 3, 2, 6, 8, 5};
	Object * planet;
	for (int i = 0; i < 8; i++) {
		planet = BuildSphereObject("PLANET",
			Vector3((i+1) * 10.0f, 0.0f, 0.0f),
			sizes[i],
			true,
			0.1f,
			true, 
			false,
			Vector4(1, 1, 1, 1),
			texIDs[i],
			ORBIT);
		planet->Physics()->SetLinearVelocity(Vector3(0.0f, 0.0f, 20.0f));
		planet->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.2f));
		this->AddGameObject(planet);

		//Fix Orbit Distance (Stops planets being absorbed by sun over long run times)
		PhysicsEngine::Instance()->AddConstraint(new DistanceConstraint(sun->Physics(), planet->Physics(), sun->Physics()->GetPosition(), planet->Physics()->GetPosition()));
	}

	//Target
	Object* target = BuildCuboidObject("TARGET", Vector3(5.0f, 0.0f, 0.0f), Vector3(0.01f,2.5f,2.5f), true, 0.1f, true, false, Vector4(1, 1, 1, 1), 0, TARGET);
	this->AddGameObject(target);

	//Rotate the target with the sun
	PhysicsEngine::Instance()->AddConstraint(new DistanceConstraint(sun->Physics(), target->Physics(), sun->Physics()->GetPosition(), target->Physics()->GetPosition()));	

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
				PROJECTILE);

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			obj->Physics()->SetLinearVelocity(forward * 50.0f);
			this->AddGameObject(obj);

		}
	}
}