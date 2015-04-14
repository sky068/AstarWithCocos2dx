//
//  xAstar.h
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//调用入口函数即可，传入起始坐标（gl坐标）和tiledMap，并设置好逻辑层然后对应修改isCanMove..方法
//之后就会返回一个路径数组，里面的点是tiledCoord坐标，需要转下再用

#ifndef __findWayForTiledMap__xAstar__
#define __findWayForTiledMap__xAstar__

#include <iostream>
#include "xNode.h"
#include "cocos2d.h"
USING_NS_CC;
class xAstar:public CCObject
{
private:
    CCTMXTiledMap   *map;
    CCTMXLayer      *xLuojiLayer;
    CCArray         *openList;
    CCArray         *closeList;
    CCArray         *shortestPath;//最终路径
public:
    //入口函数，传递的点是gl坐标
    CCArray *       startFindWay(CCPoint beginPos,CCPoint endPos,CCTMXTiledMap *_map);
    xAstar(){};
    ~xAstar(){};
private:
    bool            isCanMoveAtTileCoord(cocos2d::CCPoint tileCoord); 
    CCPoint         tileCoordForPosition(cocos2d::CCPoint mpos);      //gl转tiledCoord
    CCPoint         positonForTileCoord(cocos2d::CCPoint tileCoord);  //tiledCoord 转gl
    void            insertNodeInOpenList(xNode *pNode);               //向开发列表添加节点
    bool            isValidTileCoord(CCPoint tileCoord);              //是否是有效坐标
    void            constructPathFromStep(xNode *pNode);              //返回路径
    //获取相邻节点
    CCPointArray*    walkableAdjacentTilesCoordForTIleCoord(CCPoint tileCoord);
    //计算G值
    int             costToMoveFromStepToAdjacentStep(xNode*fromStep,xNode* toNode);
    //计算H值
    int             computeHScoreFromCoord(CCPoint fromCoord,CCPoint toCoord);
    // 功能函数
    bool            arrayContainObject(CCArray *pArray,xNode *pNode);
    int             indexForNodeInArray(CCArray *pArray,xNode *pNode);
    bool            ccPointEqualPoint(CCPoint pos1,CCPoint pos2);

};
#endif /* defined(__findWayForTiledMap__xAstar__) */
