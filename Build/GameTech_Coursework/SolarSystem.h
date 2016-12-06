#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

enum SHAPE {
	SPHERE,
	CUBE
};

class SolarSystem : public Scene
{
public:
	SolarSystem(const std::string& friendly_name);
	virtual ~SolarSystem();

	virtual void OnInitializeScene()	 override;
	virtual void OnCleanupScene()		 override;
	virtual void OnUpdateScene(float dt) override;

protected:
	float m_AccumTime;
	Object* m_pPlayer;

	//Projecticle Settings
	SHAPE projectile;
	float size;
	float speed;
};