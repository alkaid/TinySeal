#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::HelloWorld()
{
	_world=NULL;
	_backgournd=NULL;
	_terrain=NULL;
	m_debugDraw=NULL;
	_hero=NULL;
	_tapDown=false;
}

HelloWorld::~HelloWorld()
{

}

bool HelloWorld::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		bRet=true;
	} while (0);
	return bRet;
}

void HelloWorld::draw()
{
	//
	// IMPORTANT:
	// This is only for debug purposes
	// It is recommend to disable it
	//
	CCLayer::draw();

#if 0
	ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

	kmGLPushMatrix();

	_world->DrawDebugData();

	kmGLPopMatrix();
#endif
}

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("TinySeal.mp3", true);
	this->setupWorld();
	this->setScale(1.0);
	_terrain=Terrain::createWithWorld(_world);
	this->addChild(_terrain,-10);
	genBackground();
	this->setTouchEnabled(true);
	this->scheduleUpdate();

	_hero=Hero::createWithWorld(_world);
	_terrain->getBatchNode()->addChild(_hero);
}

void HelloWorld::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCLayer::ccTouchesBegan(pTouches,pEvent);
	genBackground();
	_tapDown=true;
	_hero->runForceAnimation();
	//生成测试刚体
	/*CCTouch* anyTouch=static_cast<CCTouch*>( pTouches->anyObject() );
	CCPoint touchLocation=_terrain->convertTouchToNodeSpace(anyTouch);
	this->createTestBodyAtPosition(touchLocation);*/
}

cocos2d::ccColor4F HelloWorld::randomBrightColor()
{
	int requireBright=192;
	while(true){
		ccColor4B rcolor=ccc4(rand()%255,rand()%255,rand()%255,255);
		if(rcolor.a>requireBright||rcolor.b>requireBright||rcolor.g>requireBright){
			return ccc4FFromccc4B(rcolor);
		}
	}
}

void HelloWorld::genBackground()
{
	if(_backgournd){
		_backgournd->removeFromParentAndCleanup(true);
	}

	//int nStrips=rand()%5*2;
	//ccColor4F c1=this->randomBrightColor();
	//ccColor4F c2=this->randomBrightColor();
	//_backgournd=this->spriteWithColor1(c1,c2,512,512,nStrips);
	//CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	//_backgournd->setPosition(ccp(winSize.width/2,winSize.height/2));
	////GL_LINEAR：当以比原始大小更小或者更大的尺寸，来显示纹理时，采用邻近像素的加权平均值。
	////GL_REPEAT：当需要显示纹理边界之外时，显示的是平铺纹理。
	//ccTexParams tp={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};

	//_backgournd->getTexture()->setTexParameters(&tp);
	//this->addChild(_backgournd);

	//this->setScale(0.5);

	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	ccColor4F brightColor=this->randomBrightColor();
	_backgournd=this->spriteWithColor(brightColor,512,512);
	_backgournd->setPosition(ccp(winSize.width/2,winSize.height/2));
	ccTexParams tp={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
	_backgournd->getTexture()->setTexParameters(&tp);

	this->addChild(_backgournd,-100);

	ccColor4F c3=this->randomBrightColor();
	ccColor4F c4=this->randomBrightColor();
	CCSprite* stripes=this->spriteWithColor1(c3,c4,512,512,4);
	ccTexParams tp2={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};
	stripes->getTexture()->setTexParameters(&tp2);
	_terrain->setStripes(stripes);
}

CCSprite* HelloWorld::spriteWithColor( ccColor4F color,float textureWidth,float textureHeight )
{
	CCRenderTexture *rt=CCRenderTexture::create(textureWidth,textureHeight);
	rt->beginWithClear(color.r,color.g,color.b,color.a);
	
	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();
	//画渐变矩形
	CCPoint vertices[4];
	ccColor4F colors[4];
	int nVertices=0;
	float gradientAlpha=0.7f;
	vertices[nVertices]=ccp(0,0);
	colors[nVertices++]=ccc4f(0,0,0,0);
	vertices[nVertices]=ccp(0,textureHeight);
	colors[nVertices++]=ccc4f(0,0,0,gradientAlpha);
	vertices[nVertices]=ccp(textureWidth,0);
	colors[nVertices++]=ccc4f(0,0,0,0);
	vertices[nVertices]=ccp(textureWidth,textureHeight);
	colors[nVertices++]=ccc4f(0,0,0,gradientAlpha);
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position|kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position,2,GL_FLOAT,GL_FALSE,0,vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color,4,GL_FLOAT,GL_FALSE,0,colors);
	glBlendFunc(CC_BLEND_SRC,CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP,0,(GLsizei)nVertices);
	//噪音背景
	CCSprite* noise=CCSprite::create("noise.png");
	noise->setPosition(ccp(textureWidth/2,textureHeight/2));
	ccBlendFunc blendFunc={GL_DST_COLOR,GL_ZERO};
	noise->setBlendFunc(blendFunc);
	noise->visit();

	rt->end();
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());
}

CCSprite* HelloWorld::spriteWithColor1( ccColor4F c1,ccColor4F c2,float textureWidth,float textureHeight,int nStripes )
{
	CCRenderTexture *rt=CCRenderTexture::create(textureWidth,textureHeight);
	rt->beginWithClear(c1.r,c1.g,c1.b,c1.a);
	
	this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
	CC_NODE_DRAW_SETUP();
	//画条带
	CCPoint *vertices = new CCPoint[nStripes*6];
	ccColor4F* colors = new ccColor4F[nStripes*6];
	float x1=-textureHeight;
	float x2;// x1+textureHeight
	float y1=textureHeight;
	float y2=0;
	float stripWith=textureWidth/nStripes;
	float dx=stripWith*2;
	int nVertices=0;
	for(int i=0;i<nStripes;i++){
		x2=x1+textureHeight;
		vertices[nVertices]=ccp(x1,y1);
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		vertices[nVertices]=ccp(x1+stripWith,y1);
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		vertices[nVertices]=ccp(x2,y2);
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		vertices[nVertices]=vertices[nVertices-2];
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		vertices[nVertices]=vertices[nVertices-2];
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		vertices[nVertices]=ccp(x2+stripWith,y2);
		colors[nVertices++]=ccc4f(c2.r,c2.g,c2.b,c2.a);
		x1+=dx;
	}
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position|kCCVertexAttribFlag_Color);
	glVertexAttribPointer(kCCVertexAttrib_Position,2,GL_FLOAT,GL_FALSE,0,vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color,4,GL_FLOAT,GL_FALSE,0,colors);
	glDrawArrays(GL_TRIANGLES,0,(GLsizei)nVertices);
	//噪音背景
	CCSprite* noise=CCSprite::create("noise.png");
	noise->setPosition(ccp(textureWidth/2,textureHeight/2));
	ccBlendFunc blendFunc={GL_DST_COLOR,CC_BLEND_DST};
	noise->setBlendFunc(blendFunc);
	noise->visit();
	//渐变叠加
	float gradientAlpha = 0.7f;
	nVertices = 0;
	vertices[nVertices] = ccp(0, 0);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);
	vertices[nVertices] = ccp(textureWidth, 0);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);
	vertices[nVertices] = ccp(0, textureHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, gradientAlpha);
	vertices[nVertices] = ccp(textureWidth, textureHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, gradientAlpha);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
	// 顶部高光
	float borderHeight = textureHeight / 16;
	float borderAlpha = 0.3f;
	nVertices = 0;
	vertices[nVertices] = ccp(0, 0);
	colors[nVertices++] = ccc4f(1, 1, 1, borderAlpha);
	vertices[nVertices] = ccp(textureWidth, 0);
	colors[nVertices++] = ccc4f(1, 1, 1, borderAlpha);
	vertices[nVertices] = ccp(0, borderHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);
	vertices[nVertices] = ccp(textureWidth, borderHeight);
	colors[nVertices++] = ccc4f(0, 0, 0, 0);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	rt->end();
	//cleanup
	CC_SAFE_DELETE(vertices);
	CC_SAFE_DELETE(colors);
	return CCSprite::createWithTexture(rt->getSprite()->getTexture());

}

void HelloWorld::update( float dt )
{
	//更新hero
	if(_tapDown){
		if(!_hero->getAwake()){
			_hero->wake();
			_tapDown=false;
		}else{
			_hero->dive();
		}
	}else{
		_hero->nodive();
	}
	_hero->limitVelocity();
	//更新box2d
	static const double MAX_CYCLES_PER_FRAME=5.0f;
	static const double UPDATE_INTERVAL=1.0f/60.0f;
	static double timeAccumulator=0;
	static const int32 velocityInterations=3;
	static const int32 positionInterations=2;
	timeAccumulator+=dt;
	//如果box2d的step要追赶的次数(即上一帧和这一帧之内累加的时间/频率UPDATE_INTERVAL)超过了限制MAX_CYCLES_PER_FRAME，则放弃追赶，只执行一次step
	if(timeAccumulator>MAX_CYCLES_PER_FRAME*UPDATE_INTERVAL){
		timeAccumulator=UPDATE_INTERVAL;
	}
	//追赶补上未执行的step
	while(timeAccumulator>=UPDATE_INTERVAL){
		timeAccumulator-=UPDATE_INTERVAL;
		_world->Step(UPDATE_INTERVAL,velocityInterations,positionInterations);
		_world->ClearForces();
	}
	//移动背景 移动山丘
	/*float pixPerSec=100;
	static float offset=0;
	offset+=pixPerSec*dt;*/
	//以上固定移动山丘距离改为根据英雄坐标移动
	_hero->update(dt);
		//画面缩放
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	float scale=winSize.height*3.0f/4.0f/_hero->getPositionY();
	_terrain->setScale(scale>1?1:scale);
	float offset=_hero->getPositionX();
	CCSize size=_backgournd->getTextureRect().size;
	_backgournd->setTextureRect(CCRectMake(offset*0.7,0,size.width,size.height));
	//this->setPositionX(-offset);
	_terrain->setOffsetX(offset);
}

void HelloWorld::onExit()
{
	this->unscheduleUpdate();
#if 0
	CC_SAFE_DELETE(m_debugDraw);
#endif
	CC_SAFE_DELETE(_world);
	CCLayer::onExit();
}

void HelloWorld::setupWorld()
{
	b2Vec2 gravity=b2Vec2(0.0f,-7.0f);
	_world=new b2World(gravity);
	_world->SetAllowSleeping(true);
#if 0
	m_debugDraw=new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(m_debugDraw);
	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	m_debugDraw->SetFlags(flags);
#endif
}

void HelloWorld::createTestBodyAtPosition( CCPoint location )
{
	b2BodyDef testBodyDef;
	testBodyDef.type=b2_dynamicBody;
	testBodyDef.position.Set(location.x/PTM_RATIO,location.y/PTM_RATIO);
	b2Body *testBody=_world->CreateBody(&testBodyDef);
	b2CircleShape testBodyShape;
	testBodyShape.m_radius=25.0f/PTM_RATIO;
	b2FixtureDef testBodyFixtureDef;
	testBodyFixtureDef.density=1.0;
	testBodyFixtureDef.friction=2.0;
	testBodyFixtureDef.shape=&testBodyShape;
	testBodyFixtureDef.restitution=0.5;
	testBody->CreateFixture(&testBodyFixtureDef);
}

void HelloWorld::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	_tapDown=false;
	_hero->runNormalAnimation();
}

void HelloWorld::ccTouchesCancelled( CCSet *pTouches, CCEvent *pEvent )
{
	_tapDown=false;
	_hero->runNormalAnimation();
}
