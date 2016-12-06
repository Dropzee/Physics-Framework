#include "SolarSystem.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
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
	projectile = SPHERE;
}

SolarSystem::~SolarSystem()
{

}


void SolarSystem::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");

	//<--- SCENE CREATION --->

	Object* obj;
	obj = BuildSphereObject(
		"",																	// Optional: Name
		SceneManager::Instance()->GetCamera()->GetPosition(),				// Position
		1.0f * size,														// Half-Dimensions
		true,																// Physics Enabled?
		0.1f,																// Physical Mass (must have physics enabled)
		true,																// Physically Collidable (has collision shape)
		false,																// Dragable by user?
		Vector4(1, 1, 1, 1),												// Render colour
		0);																	// Texture

	//Create Player (See OnUpdateScene)
	m_pPlayer = BuildCuboidObject(
		"Player1",					// Optional: Name
		Vector3(5.f, 0.5f, 0.0f),	// Position
		Vector3(0.5f, 0.5f, 1.0f),  // Half-Dimensions
		true,						// Physics Enabled?
		0.1f,						// Physical Mass (must have physics enabled)
		true,						// Physically Collidable (has collision shape)
		false,						// Dragable by user?
		Vector4(0.1f, 0.1f, 0.1f, 1.0f)); // Render colour
	this->AddGameObject(m_pPlayer);

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

		bool boosted = false;

		PhysicsObject* pobj = m_pPlayer->Physics();
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3() * Vector3(0.0f, 0.0f, -mv_speed));
			boosted = true;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN))
		{
			pobj->SetPosition(pobj->GetPosition() +
				pobj->GetOrientation().ToMatrix3()* Vector3(0.0f, 0.0f, mv_speed / 2.f));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), rot_speed));
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT))
		{
			pobj->SetOrientation(pobj->GetOrientation() *
				Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 1.0f, 0.0f), -rot_speed));
		}

		//Projectile
		if (Window::GetMouse()->GetWheelMovement() > 0 && speed <= 1) {
			speed += 0.05;
		}
		else if (Window::GetMouse()->GetWheelMovement() < 0 && speed >= 0.2) {
			speed -= 0.05;
		}
		if (Window::GetKeyboard()->KeyHeld(KEYBOARD_PLUS) && size <= 1) {
			size += 0.01;
		}
		else if (Window::GetKeyboard()->KeyHeld(KEYBOARD_MINUS) && size >= 0.2) {
			size -= 0.01;
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			projectile = SPHERE;
		}
		else if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			projectile = CUBE;
		}

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
			Object* obj;
			if (projectile == SPHERE) {
				obj = BuildSphereObject(
					"",																	// Optional: Name
					SceneManager::Instance()->GetCamera()->GetPosition(),				// Position
					1.0f * size,														// Half-Dimensions
					true,																// Physics Enabled?
					0.1f,																// Physical Mass (must have physics enabled)
					true,																// Physically Collidable (has collision shape)
					false,																// Dragable by user?
					Vector4(1, 1, 1, 1));												// Render colour
			}
			else {
				obj = BuildCuboidObject(
					"",																	// Optional: Name
					SceneManager::Instance()->GetCamera()->GetPosition(),				// Position
					Vector3(1.0f * size, 1.0f * size, 1.0f * size),						// Half-Dimensions
					true,																// Physics Enabled?
					0.1f,																// Physical Mass (must have physics enabled)
					true,																// Physically Collidable (has collision shape)
					false,																// Dragable by user?
					Vector4(1, 1, 1, 1));												// Render colour
			}

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			obj->Physics()->SetLinearVelocity(forward * 50.0f * speed);
			this->AddGameObject(obj);

		}

		// Also (and importantly), as the projMatrix/viewMatrix is all abstracted away
		//  we can now use debug draw tools to render geometry in world-space from anywhere
		//  in the program. Very useful for debugging!
		if (boosted)
		{
			//Draw the rocket booster on the car using NCLDebug
			Vector3 backward_dir = pobj->GetOrientation().ToMatrix3() * Vector3(0, 0, 1);
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir, 0.3f, Vector4(1.f, 0.7f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.333f, 0.26f, Vector4(0.9f, 0.5f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 1.666f, 0.23f, Vector4(0.8f, 0.3f, 0.0f, 1.0f));
			NCLDebug::DrawPoint(pobj->GetPosition() + backward_dir * 2.f, 0.2f, Vector4(0.7f, 0.2f, 0.0f, 1.0f));
		}
	}
}