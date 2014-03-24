#include "Hero.h"
#include "box2dConfig.h"

Hero::Hero()
{
	_world=NULL;
	_body=NULL;
	_awake=false;
	_nextVel=0;
	memset(_prevVels,NULL,sizeof(b2Vec2)*NUM_PREV_VELS);
	_normalAnim=NULL;
	_normalAnimate=NULL;
}

Hero::~Hero()
{
	CC_SAFE_RELEASE_NULL(_normalAnim);
}

Hero* Hero::createWithWorld( b2World* world )
{
	Hero* pRet=new Hero();
	if(pRet&&pRet-> initWithWorld(world)){
		pRet->autorelease();
	}else{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

bool Hero::initWithWorld( b2World* world )
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrameName("seal1.png"));
		_world=world;
		this->createBody();
		_normalAnim=CCAnimation::create();
		_normalAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("seal1.png"));
		_normalAnim->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("seal2.png"));
		_normalAnim->retain();
		_normalAnim->setDelayPerUnit(0.1f);
		bRet=true;
	} while (0);
	return bRet;
}

void Hero::update( float dt )
{
	this->setPosition(ccp(_body->GetPosition().x*PTM_RATIO,_body->GetPosition().y*PTM_RATIO));
	b2Vec2 vel=_body->GetLinearVelocity();
	//为了修正海豹贴地滑行时的身体颤抖，使用6次内的速度的加权平均值来计算角度
	b2Vec2 weightVel=vel;
	for(int i=0;i<NUM_PREV_VELS;i++){
		weightVel+=_prevVels[i];
	}
	weightVel=b2Vec2(weightVel.x/NUM_PREV_VELS,weightVel.y/NUM_PREV_VELS);
	_prevVels[_nextVel++]=vel;
	_nextVel=_nextVel>=NUM_PREV_VELS?0:_nextVel;
	
	//float angle=ccpToAngle(ccp(vel.x,vel.y));
	float angle=ccpToAngle(ccp(weightVel.x,weightVel.y));
	if(_awake){
		this->setRotation(-1*CC_RADIANS_TO_DEGREES(angle));
	}
}

void Hero::createBody()
{
	float radius=16.0f;
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	CCPoint startPosition=ccp(0,winSize.height/2+radius);

	b2BodyDef bd;
	bd.linearDamping=0.1f;
	bd.fixedRotation=true;
	bd.type=b2_dynamicBody;
	bd.position.Set(startPosition.x/PTM_RATIO,startPosition.y/PTM_RATIO);
	_body=_world->CreateBody(&bd);
	b2CircleShape shape;
	shape.m_radius=radius/PTM_RATIO;
	b2FixtureDef fd;
	fd.shape=&shape;
	fd.density=1.0/CC_CONTENT_SCALE_FACTOR();
	fd.friction=0.2f;
	fd.restitution=0;
	_body->CreateFixture(&fd);
}

void Hero::wake()
{
	_awake=true;
	_body->SetActive(true);
	_body->ApplyLinearImpulse(b2Vec2(1,2),_body->GetPosition());
}

void Hero::dive()
{
	_body->ApplyForce(b2Vec2(5,-110),_body->GetPosition());
}

void Hero::limitVelocity()
{
	if(!_awake)
		return;
	const float minVelocityX=5;
	const float minVelocityY=-40;
	b2Vec2 vel=_body->GetLinearVelocity();
	vel.x=vel.x<minVelocityX?minVelocityX:vel.x;
	vel.y=vel.y<minVelocityY?minVelocityY:vel.y;
	_body->SetLinearVelocity(vel);
}

void Hero::nodive()
{
	this->runNormalAnimation();
}

void Hero::runForceAnimation()
{
	if(_normalAnimate){
		this->stopAction(_normalAnimate);
	}
	_normalAnimate=NULL;
	this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("seal_downhill.png"));
}

void Hero::runNormalAnimation()
{
	if(_normalAnimate||!_awake)
		return;
	_normalAnimate=CCRepeatForever::create(CCAnimate::create(_normalAnim));
	this->runAction(_normalAnimate);
}
