#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

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

	Object* projectiles[6];
	int shotCount;

	int reloadTime;
	Object* reload;
	int spin;

	int score;

	Object* mccree;
	bool ultimate;
	bool autofire;
	int firetime;
};