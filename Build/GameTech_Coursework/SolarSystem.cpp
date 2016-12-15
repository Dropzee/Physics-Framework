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
	, shotCount(0)
	, reloadTime(300)
	, score(0)
	, ultimate(false)
	, autofire(false)
	, firetime(60)
{
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

	//Initialize Client Network
	if (m_Network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");

		//Attempt to connect to the server on localhost:1234
		m_pServerConnection = m_Network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}

	m_AccumTime = 0.0f;

	//Example usage of Log and LogE inside NCLDebug functionality
	NCLDebug::Log("This is a log entry");
	NCLERROR("THIS IS AN ERROR!");

	//<--- SCENE CREATION --->

	//Sun
	sun = BuildSphereObject("SUN", Vector3(0.0f, 0.0f, 0.0f), 5.0f, true, 0.00001f, true, false, Vector4(1, 1, 1, 1), 7, SUN);
	sun->Physics()->SetAngularVelocity(Vector3(0.0f, -0.5f, 0.0f));
	this->AddGameObject(sun);
	
	//Planets!!!!!!!
	float sizes[8] = {0.5f, 1.0f, 1.0f, 0.7f, 2.0f, 1.8f, 1.2f, 1.2f};
	int texIDs[8] = {4, 9, 1, 3, 2, 6, 8, 5};
	Object * planet;
	for (int i = 0; i < 8; i++) {
		planet = BuildSphereObject("PLANET",
			Vector3((i+1) * 10.0f + 5.f, 0.0f, 0.0f),
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
	target = BuildCuboidObject("TARGET", Vector3(6.0f, 0.0f, 0.0f), Vector3(0.5f,2.5f,2.5f), true, 0.0f, true, false, Vector4(1, 1, 1, 1), 11, TARGET);
	this->AddGameObject(target);

	//Reload Symbol
	reload = BuildCuboidObject("RELOAD", Vector3(-1000.0f, -1000.0f, -1000.0f), Vector3(0.01f, 0.2f, 0.2f), true, 0.0f, true, false, Vector4(1, 1, 1, 1), 12, STATIC);
	this->AddGameObject(reload);

	//...it's high noon...
	mccree = BuildCuboidObject("McCree", Vector3(-1000.0f, 1000.0f, -1000.0f), Vector3(0.001f, 0.2f, 0.2f), true, 0.0f, true, false, Vector4(1, 1, 1, 1), 13, STATIC);
	this->AddGameObject(mccree);

	//networking... joy
	m_NetworkObj = BuildCuboidObject("NETWORK", Vector3(0.f, 7.f, 0.f), Vector3(1.0f, 1.f, 1.f), true, 0.0f, true, false, Vector4(1, 1, 1, 1), 10, STATIC);
	this->AddGameObject(m_NetworkObj);
}

void SolarSystem::OnCleanupScene()
{
	Scene::OnCleanupScene();
}

void SolarSystem::OnUpdateScene(float dt)
{
	m_AccumTime += dt;

	//Update Network
	auto callback = std::bind(
		&SolarSystem::ProcessNetworkEvent,	// Function to call
		this,								// Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	m_Network.ServiceNetwork(dt, callback);
	
	//Add Debug Information to screen
	uint8_t ip1 = m_pServerConnection->address.host & 0xFF;
	uint8_t ip2 = (m_pServerConnection->address.host >> 8) & 0xFF;
	uint8_t ip3 = (m_pServerConnection->address.host >> 16) & 0xFF;
	uint8_t ip4 = (m_pServerConnection->address.host >> 24) & 0xFF;

	NCLDebug::DrawTextWs(m_NetworkObj->Physics()->GetPosition() + Vector3(0.f, 0.5f, 0.f), 14.f, TEXTALIGN_CENTRE, Vector4(),
		"Peer: %u.%u.%u.%u:%u", ip1, ip2, ip3, ip4, m_pServerConnection->address.port);

	Vector4 status_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	NCLDebug::AddStatusEntry(status_color, "Network Traffic");
	NCLDebug::AddStatusEntry(status_color, "    Incoming: %5.2fKbps", m_Network.m_IncomingKb);
	NCLDebug::AddStatusEntry(status_color, "    Outgoing: %5.2fKbps", m_Network.m_OutgoingKb);

	//Rotate Target with Planet
	Vector3 radius = Vector3(6.f, 0.f, 0.f);
	Matrix3 rot = sun->Physics()->GetOrientation().ToMatrix3();
	target->Physics()->SetOrientation(sun->Physics()->GetOrientation());
	target->Physics()->SetPosition(rot * radius);

	sun->Physics()->SetPosition(Vector3(0.f, 0.f, 0.f));

	score += PhysicsEngine::Instance()->getScoreUpdate();
	spin = (spin + 2) % 360; 

	Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
	Vector3 forward = Vector3(-view._13, -view._23, -view._33);

	if (ultimate) {

		mccree->Physics()->SetPosition(SceneManager::Instance()->GetCamera()->GetPosition() + forward * 2 + Vector3(-view._13 - 2.0f, -view._23 - 1.2f, 0));
		Matrix4 mat4view = Matrix4(view);
		Quaternion q = Quaternion::FromMatrix(mat4view);
		mccree->Physics()->SetOrientation(q * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), -90.0f));

		if (firetime % 10 == 0) {
			autofire = true;
		}
		else {
			autofire = false;
		}
		firetime--;
		if (firetime < 0) {
			mccree->Physics()->SetPosition(Vector3(-1000, 1000, -1000));
		}
	}
	
	if (shotCount == 6) {

		reload->Physics()->SetPosition(SceneManager::Instance()->GetCamera()->GetPosition() + forward * 2);
		Matrix4 mat4view = Matrix4(view) * Matrix4::Rotation(spin, forward);
		Quaternion q = Quaternion::FromMatrix(mat4view);
		reload->Physics()->SetOrientation(q * Quaternion::AxisAngleToQuaterion(Vector3(0, 1, 0), -90.0f));
		
		reloadTime--;

		if (reloadTime == 0) {
			reload->Physics()->SetPosition(Vector3(-1000, -1000, -1000));
			reloadTime = 300;
			shotCount = 0;
			ultimate = false;
			autofire = false;
			firetime = 60;
		}
		
	}

	// You can add status entries to the top left from anywhere in the program
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Controls:");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 Movement - W/A/S/D");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 Camera Rotate - HOLD RMB");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 Fire Projectile - F");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 Rapid Fire - M");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.4f, 0.4f, 1.0f), "   \x01 Quit - ECS");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Score: " + to_string(score));
	if (shotCount != 6) {
		NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Ammo: " + to_string(6 - shotCount));
	}
	else {
		NCLDebug::AddStatusEntry(Vector4(1.0f, 0.2f, 0.2f, 1.0f), "Ammo: RELOADING!!!");
	}

	// Lets sun a little bit...
	Vector3 invLightDir = Matrix4::Rotation(15.f * dt, Vector3(0.0f, 1.0f, 0.0f)) * SceneManager::Instance()->GetInverseLightDirection();
	SceneManager::Instance()->SetInverseLightDirection(invLightDir);



	//Or move our car around the scene..
	{
		const float mv_speed = 10.f * dt;			//Motion: Meters per second
		const float rot_speed = 150.f * dt;			//Rotation: Degrees per second

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M) && shotCount != 6 && !ultimate) {
			ultimate = true;
		}

		//Projectile
		if ((Window::GetKeyboard()->KeyTriggered(KEYBOARD_F) && shotCount != 6) || (autofire && shotCount != 6)) {
			
			if(Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
				firetime -= 10;
			}

			Matrix3 view = Matrix3(SceneManager::Instance()->GetCamera()->BuildViewMatrix());
			Vector3 forward = Vector3(-view._13, -view._23, -view._33);

			if(projectiles[shotCount] == NULL){
			Object* projectile = BuildSphereObject(
					"",																	// Optional: Name
					SceneManager::Instance()->GetCamera()->GetPosition(),				// Position
					0.1f,																// Half-Dimensions
					true,																// Physics Enabled?
					0.1f,																// Physical Mass (must have physics enabled)
					true,																// Physically Collidable (has collision shape)
					false,																// Dragable by user?
					Vector4(1, 1, 1, 1),												// Render colour
					10,
					PROJECTILE,
					false);
				projectiles[shotCount] = projectile;
				this->AddGameObject(projectile);
			}
			else {
				projectiles[shotCount]->Physics()->SetPosition(SceneManager::Instance()->GetCamera()->GetPosition());
			}

			projectiles[shotCount]->Physics()->SetLinearVelocity(forward * 50.0f);
			shotCount++;
		}
	}
}

void SolarSystem::ProcessNetworkEvent(const ENetEvent& evnt)
{
	switch (evnt.type)
	{
		//New connection request or an existing peer accepted our connection request
	case ENET_EVENT_TYPE_CONNECT:
	{
		if (evnt.peer == m_pServerConnection)
		{
			NCLDebug::Log("Network: Successfully connected to server!");

			//Send a 'hello' packet
			char* text_data = "Hellooo!";
			ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
			enet_peer_send(m_pServerConnection, 0, packet);
		}
	}
	break;


	//Server has sent us a new packet
	case ENET_EVENT_TYPE_RECEIVE:
	{
		if (evnt.packet->dataLength == sizeof(Vector3))
		{
			Vector3 pos;
			memcpy(&pos, evnt.packet->data, sizeof(Vector3));
			m_NetworkObj->Physics()->SetPosition(pos);
		}
		else
		{
			NCLERROR("Recieved Invalid Network Packet!");
		}

	}
	break;


	//Server has disconnected
	case ENET_EVENT_TYPE_DISCONNECT:
	{
		NCLDebug::Log("Network: Server has disconnected!");
	}
	break;
	}
}