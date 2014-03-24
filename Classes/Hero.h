#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class Hero : public CCSprite
{
public:
	Hero();
	virtual ~Hero();

	static Hero* createWithWorld(b2World* world);
	bool initWithWorld(b2World* world);

	virtual void update(float dt);
	void createBody();

	CC_SYNTHESIZE_READONLY(bool,_awake,Awake);

	void wake();
	void dive();
	void limitVelocity();
	void nodive();
	void runForceAnimation();
	void runNormalAnimation();

private:
	b2World* _world;
	b2Body* _body;
	static const int NUM_PREV_VELS=5;
	b2Vec2 _prevVels[NUM_PREV_VELS];
	int _nextVel;
	CCAnimation* _normalAnim;
	CCAction* _normalAnimate;
};