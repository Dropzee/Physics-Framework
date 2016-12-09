#include "TestScene.h"

#include <nclgl\Vector4.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\DistanceConstraint.h>
#include <ncltech\SceneManager.h>
#include <ncltech\CommonUtils.h>



using namespace CommonUtils;

TestScene::TestScene(const std::string& friendly_name)
	: Scene(friendly_name)
	, m_AccumTime(0.0f)
	, m_pPlayer(NULL)
{
	speed = 0.5f;
	size = 0.5f;
	projectile = SPHERE;
}

TestScene::~TestScene()
{

}


void TestScene::OnInitializeScene()
{
	//Disable the physics engine (We will be starting this later!)
	PhysicsEngine::Instance()->SetPaused(true);
	PhysicsEngine::Instance()->setSpace(false);

	//Set the camera position
	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(15.0f, 10.0f, -15.0f));
	SceneManager::Instance()->GetCamera()->SetYaw(140.f);
	SceneManager::Instance()->GetCamera()->SetPitch(-20.f);

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");
	


//<--- SCENE CREATION --->
	//Create Ground
	//this->AddGameObject(BuildCuboidObject("Ground", Vector3(0.0f, -1.0f, 0.0f), Vector3(20.0f, 1.0f, 20.0f), true, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

	//Create Environment
	this->AddGameObject(BuildCuboidObject("Ground", Vector3(0.0f, -1.0f, 0.0f), Vector3(20.0f, 1.0f, 20.0f), true, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));
	this->AddGameObject(BuildCuboidObject("Wall1", Vector3(-21.0f, 10.0f, 0.0f), Vector3(1.0f, 10.0f, 20.0f), true, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));
	this->AddGameObject(BuildCuboidObject("Wall2", Vector3(0.0f, 10.0f, 21.0f), Vector3(20.0f, 10.0f, 1.0f), true, 0.0f, true, false, Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

	//Create Bouncing Spheres
	for (int i = 0; i < 3; ++i)
	{
		Vector4 colour = CommonUtils::GenColour(0.7f + i * 0.05f, 1.0f);
		Object* obj = CommonUtils::BuildSphereObject(
			"",
			Vector3(5.0f + i * 5.0f, 5.0f, 15.0f),
			1.0f,
			true,
			0.1f,
			true,
			true,
			colour);
		obj->Physics()->SetFriction(0.1f);
		obj->Physics()->SetElasticity(i * 0.2f + 0.5f);
		this->AddGameObject(obj);
	}

	//Create Bouncing Cubes
	for (int i = 0; i < 3; ++i)
	{
		Vector4 colour = CommonUtils::GenColour(0.7f + i * 0.05f, 1.0f);
		Object* obj = CommonUtils::BuildCuboidObject(
			"",
			Vector3(5.0f + i * 5.0f, 5.0f, 5.0f),
			Vector3(1.f, 1.f, 1.f),
			true,
			0.1f,
			true,
			true,
			colour);
		obj->Physics()->SetFriction(0.1f);
		obj->Physics()->SetElasticity(i * 0.2f + 0.5f);
		this->AddGameObject(obj);
	}

	//Create Ramp
	Object* ramp = CommonUtils::BuildCuboidObject(
		"Ramp",
		Vector3(-15.0f, 3.5f, -10.0f),
		Vector3(4.0f, 0.5f, 8.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(1.0f, 0.7f, 1.0f, 1.0f));
	ramp->Physics()->SetOrientation(Quaternion::AxisAngleToQuaterion(Vector3(0.0f, 0.0f, 1.0f), -20.0f));
	ramp->Physics()->SetFriction(1.0f);
	this->AddGameObject(ramp);


	auto create_ball_cube = [&](const Vector3& offset, const Vector3& scale, float ballsize)
	{
		const int dims = 5;
		const Vector4 col = Vector4(1.0f, 0.5f, 0.2f, 1.0f);

		for (int x = 0; x < 1; ++x)
		{
			for (int y = 0; y < dims; ++y)
			{
				for (int z = 0; z < dims * 5; ++z)
				{
					Vector3 pos = offset + Vector3(scale.x *x, scale.y * y, scale.z * z);
					Object* sphere = BuildSphereObject(
						"",					// Optional: Name
						pos,				// Position
						ballsize,			// Half-Dimensions
						true,				// Physics Enabled?
						1.0f,				// Physical Mass (must have physics enabled)
						true,				// Physically Collidable (has collision shape)
						false,				// Dragable by user?
						col);// Render colour
					this->AddGameObject(sphere);
				}
			}
		}
	};

	//Create Test Ball Pit
	create_ball_cube(Vector3(0.0f, 0.5f, -16.0f), Vector3(0.5f, 0.5f, 0.5f), 0.1f);
}

void TestScene::OnCleanupScene()
{
	//Just delete all created game objects 
	//  - this is the default command on any Scene instance so we don't really need to override this function here.
	Scene::OnCleanupScene(); 
}

void TestScene::OnUpdateScene(float dt)
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
				obj = BuildSphereObject("", SceneManager::Instance()->GetCamera()->GetPosition(), 1.0f * size, true, 0.05f, true, false, Vector4(1, 1, 1, 1));
			}
			else{
				obj = BuildCuboidObject("",	SceneManager::Instance()->GetCamera()->GetPosition(), Vector3(1.0f * size, 1.0f * size, 1.0f * size), true, 0.1f, true, false, Vector4(1, 1, 1, 1));
			}

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			obj->Physics()->SetLinearVelocity(forward * 50.0f * speed);
			this->AddGameObject(obj);

		}
	}
}