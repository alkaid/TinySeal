#include "HelloWorldScene.h"

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
	_backgournd=NULL;
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

void HelloWorld::onEnter()
{
	CCLayer::onEnter();
	genBackground();
	this->setTouchEnabled(true);
	this->scheduleUpdate();
}

void HelloWorld::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCLayer::ccTouchesBegan(pTouches,pEvent);
	genBackground();
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
	//_backgournd=this->spriteWithColor(randomBrightColor(),512,512);
	int nStrips=rand()%5*2;
	ccColor4F c1=this->randomBrightColor();
	ccColor4F c2=this->randomBrightColor();
	_backgournd=this->spriteWithColor1(c1,c2,512,512,nStrips);
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	_backgournd->setPosition(ccp(winSize.width/2,winSize.height/2));
	//GL_LINEAR：当以比原始大小更小或者更大的尺寸，来显示纹理时，采用邻近像素的加权平均值。
	//GL_REPEAT：当需要显示纹理边界之外时，显示的是平铺纹理。
	ccTexParams tp={GL_LINEAR,GL_LINEAR,GL_REPEAT,GL_REPEAT};

	_backgournd->getTexture()->setTexParameters(&tp);
	this->addChild(_backgournd);

	this->setScale(0.5);
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
	ccBlendFunc blendFunc={GL_DST_COLOR,GL_ZERO};
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
	float pixPerSec=100;
	static float offset=0;
	offset+=pixPerSec*dt;
	CCSize size=_backgournd->getTextureRect().size;
	_backgournd->setTextureRect(CCRectMake(offset,0,size.width,size.height));
}

void HelloWorld::onExit()
{
	this->unscheduleUpdate();
}
