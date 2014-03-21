#pragma once

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "box2dConfig.h"
#if ENABLE_BOX2D_DEBUG_DRAW
#include "Box2DTestBed/GLES-Render.h"
#endif
USING_NS_CC;

#define kMaxHillVertices 4000
#define kMaxBorderVertices 800

class Terrain : public CCNode
{
public:
	static const int MAX_HILL_KEY_POINTS=1000;
	static const int kHillSegmentWidth=10;
	Terrain();
	virtual ~Terrain();
	//CREATE_FUNC(Terrain);
	static Terrain* createWithWorld(b2World* world);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_stripes,Stripes);
	virtual bool init(b2World* world);
	virtual void draw();
	void setOffsetX(float newOffsetX);
private:
	b2World* _world;
	b2Body* _body;
	int _offsetX;
	int _fromKeyPointI;
	int _toKeyPointI;
	CCPoint _hillKeyPoints[MAX_HILL_KEY_POINTS];
	GLESDebugDraw* m_debugDraw;
	void generateHills();
	void resetHillVertices();
	void resetBox2DBody();

	int _nHillVertices;
	cocos2d::CCPoint _hillVertices[kMaxHillVertices];
	cocos2d::CCPoint _hillTexCoords[kMaxHillVertices];
	int _nBorderVertices;
	cocos2d::CCPoint _borderVertices[kMaxBorderVertices];
};