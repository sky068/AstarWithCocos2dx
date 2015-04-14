//
//  GameScene.h
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//

#ifndef __findWayForTiledMap__GameScene__
#define __findWayForTiledMap__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "xAstar.h"
USING_NS_CC;
class pGame:public CCLayer
{
public:
    CCTMXTiledMap   *xMap;
    CCTMXLayer      *xLuojiLayer;
    CCTMXLayer      *xSpriteLayer;
    CCSprite        *xPlayer;
    CCArray         *pathArray;
public:
    bool        init();
    static      CCScene* scene();
    CREATE_FUNC(pGame);
    void        onEnter();
    void        onExit();
    bool        ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
    void        ccTouchEnded(CCTouch *pTouch,CCEvent *pEvent);
    CCPoint     tileCoordForPosition(CCPoint mpos);         //gl坐标转地图坐标
    CCPoint     positonForTileCoord(CCPoint tileCoord);     //tileCoord转gl坐标
    void        moveForPoints();                            //按轨迹移动
    
};
#endif /* defined(__findWayForTiledMap__GameScene__) */
