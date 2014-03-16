#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
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

private:
	CCSprite* _backgournd;
	ccColor4F randomBrightColor();
	void genBackground();
	CCSprite* spriteWithColor(ccColor4F color,float textureWith,float textureHeight);
	CCSprite* spriteWithColor1(ccColor4F c1,ccColor4F c2,float textureWith,float textureHeight,int nStripes);
};

#endif // __HELLOWORLD_SCENE_H__
