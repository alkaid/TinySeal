#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__



#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "box2dConfig.h"
#include "Terrain.h"
#if ENABLE_BOX2D_DEBUG_DRAW
#include "Box2DTestBed/GLES-Render.h"
#endif

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
	HelloWorld();
	virtual ~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	virtual void onEnter();
	virtual void onExit();
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void update(float dt);
	virtual void draw();

private:
	b2World* _world;
	CCSprite* _backgournd;
	Terrain* _terrain;
	GLESDebugDraw* m_debugDraw;
	ccColor4F randomBrightColor();
	void genBackground();
	CCSprite* spriteWithColor(ccColor4F color,float textureWith,float textureHeight);
	CCSprite* spriteWithColor1(ccColor4F c1,ccColor4F c2,float textureWith,float textureHeight,int nStripes);
	void setupWorld();
	void createTestBodyAtPosition(CCPoint location);
};

#endif // __HELLOWORLD_SCENE_H__
