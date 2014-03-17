#pragma once

#include "cocos2d.h"
USING_NS_CC;

class Terrain : public CCNode
{
public:
	static const int MAX_HILL_KEY_POINTS=1000;
	static const int kHillSegmentWidth=10;
	Terrain();
	virtual ~Terrain();
	CREATE_FUNC(Terrain);
	CC_SYNTHESIZE_RETAIN(CCSprite*,_stripes,Stripes);
	virtual bool init();
	virtual void draw();
	void setOffsetX(float newOffsetX);
private:
	int _offsetX;
	int _fromKeyPointI;
	int _toKeyPointI;
	CCPoint _hillKeyPoints[MAX_HILL_KEY_POINTS];
	void generateHills();
	void resetHillVertices();
};