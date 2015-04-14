//
//  xNode.h
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//

#ifndef __findWayForTiledMap__xNode__
#define __findWayForTiledMap__xNode__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class xNode:public CCObject
{
public:
    CCPoint tileCoord;
    int gScore;
    int hScore;
    xNode *parent;
public:
    int getFscore();
    static xNode* createWithPosition(CCPoint _tileCoord);
    xNode();
    ~xNode();
};
#endif /* defined(__findWayForTiledMap__xNode__) */
