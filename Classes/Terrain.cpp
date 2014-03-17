#include "Terrain.h"

USING_NS_CC;


Terrain::Terrain()
{
	_offsetX=0;
	_stripes=NULL;
	_fromKeyPointI=0;
	_toKeyPointI=0;
}

Terrain::~Terrain()
{
	setStripes(NULL);
}

bool Terrain::init()
{
	bool bRet=false;
	do 
	{
		CC_BREAK_IF(!CCNode::init());
		this->generateHills();
		this->resetHillVertices();
		bRet=true;
	} while (0);
	return bRet;
}

void Terrain::draw()
{
	CCNode::draw();
	for(int i=MAX(1,_fromKeyPointI);i<=_toKeyPointI;++i){
		//����ɽ��
		ccDrawColor4F(1,0,0,1);
		ccDrawLine(_hillKeyPoints[i-1],_hillKeyPoints[i]);
		//TODO ����ƽ����б�¡�ɽ������б�µģ�����������ֱ��ֱ�µ�ֱ�ߡ�һ���취��ʹ�����Һ�����ɽ������.���ȣ���Ҫ�ֶλ����ߣ���ˣ���Ҫÿ10���㴴��һ�����Ρ�ͬ���ģ���Ҫһ���������������ߣ���ˣ����Խ�PI�������ε��������õ�ÿ����ĽǶȡ�Ȼ����cos(0)��Ӧp0��y��ֵ����cos(PI)��Ӧp1��y��ֵ��Ҫ������һ�㣬������cos(angle)������p1��p0֮������һ�루ͼ�ϵ�ampl��������cos(0)=1����cos(PI)=-1�����ԣ�ampl��p0����-ampl��p1�����������е����꣬�Ϳ��Եõ���Ҫ��y��ֵ
		ccDrawColor4F(1.0, 1.0, 1.0, 1.0);
		//ֻ�������Һ���ͼxΪ0~PI,yΪ-1~1��������� ȡ(0,1)ΪP0�� (-1,PI)Ϊp1�㣬 ӳ�䵽cocos2dx������_hillKeyPoints[i-1],_hillKeyPoints[i]��������
		CCPoint p0=_hillKeyPoints[i-1];
		CCPoint p1=_hillKeyPoints[i];
		float ampl=(p0.y-p1.y)/2; //amplΪp0��p1��y�����ӳ�䵽���Һ��������Ǹ����� ��Ϊ (p0.y-p1.y)/(1-(-1))
		float ymid=(p0.y+p1.y)/2; //ymidΪp0��p1֮��y���м���yֵ��ymidʵ������Ϊ��ӳ��Ϊ���Һ���ͼ�е�X���yֵ(��0)��
		//����Ԥ���¶����µ�֮��ֶε�ÿ������ֵ����ֶ��� Ȼ����ӳ�䵽���Һ���ͼ�е�0~PI����
		int hSegments=floorf((p1.x-p0.x)/kHillSegmentWidth);
		float dx=(p1.x-p0.x)/hSegments;
		float da=M_PI/hSegments;
		CCPoint pt0=p0;
		CCPoint pt1;
		for (int j=0;j<hSegments;j++)
		{
			pt1.x=pt0.x+dx;
			//y��ֵ���ǣ�ymid - ampl,ymid + ampl)����(p0.y, p1.y)  һ��cos������ֵ���ǣ�-1,1)��ô��cos����ֵ����(p0.y,p1.y���أ�������y = ymid + x * cos(alpha);
			pt1.y=ampl*cosf(da*j) + ymid;	//�ü��㷽��ӳ�䵽���Һ���ͼ��Ϊ cos(x)+0
			ccDrawLine(pt0,pt1);
			pt0=pt1;
		}
	}
}

void Terrain::generateHills()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	//TODO ʹ��Sergey���㷨������ʵ�� Sergey�㷨���������
	float minDX = 160;
	float minDY = 60;
	int rangeDX = 80;
	int rangeDY = 40;

	float x = -minDX;
	float y = winSize.height / 2;

	float dy, ny;
	float sign = 1; // +1 - going up, -1 - going  down
	float paddingTop = 20;
	float paddingBottom = 20;

	for (int i = 0; i < MAX_HILL_KEY_POINTS; ++i)
	{
		_hillKeyPoints[i] = ccp(x, y);
		if (i == 0)
		{
			x = 0;
			y = winSize.height / 2;
		} 
		else
		{
			x += rand() % rangeDX + minDX;
			while (true)
			{
				dy = rand() % rangeDY + minDY;
				ny = y + dy * sign;
				if (ny < winSize.height - paddingTop && ny > paddingBottom)
				{
					break;
				}
			}
			y = ny;
		}
		sign *= -1;
	}
}

void Terrain::setOffsetX( float newOffsetX )
{
	_offsetX=newOffsetX;
	setPosition(-_offsetX*this->getScale(),0);
	resetHillVertices();
}

void Terrain::resetHillVertices()
{
	CCSize winSize=CCDirector::sharedDirector()->getWinSize();
	while(_hillKeyPoints[_fromKeyPointI].x<_offsetX-winSize.width/8/this->getScale()){
		_fromKeyPointI++;
	}
	while(_hillKeyPoints[_toKeyPointI].x<_offsetX+winSize.width*9/8/this->getScale()){
		_toKeyPointI++;
	}
}

