#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>

enum SHAPE {
	SPHERE,
	CUBE
};

class TestScene : public Scene
{
public:
	TestScene(const std::string& friendly_name);
	virtual ~TestScene();

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