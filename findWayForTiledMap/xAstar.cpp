//
//  xAstar.cpp
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//

#include "xAstar.h"
CCArray* xAstar::startFindWay(cocos2d::CCPoint beginPos, cocos2d::CCPoint endPos,CCTMXTiledMap *_map)
{
    //创建开方和关闭列表
    openList=CCArray::create();
    openList->retain();
    closeList=CCArray::create();
    closeList->retain();
    map=_map;
    xLuojiLayer=map->layerNamed("luoji");
    shortestPath=NULL;
    //设置起始坐标（tiledCoord）
    CCPoint fromTileCoor=tileCoordForPosition(beginPos);
    CCPoint toTileCoor  =tileCoordForPosition(endPos);
    CCLog("xAstar_toTileCoord:(%g,%g)",toTileCoor.x,toTileCoor.y);
    if (ccPointEqualPoint(fromTileCoor, toTileCoor))
    {
        CCLog("xAstar:起始点相同，结束寻路!");
        return NULL;//起始点相同
    }
    if (!isCanMoveAtTileCoord(toTileCoor))
    {
        CCLog("xAstar:终点是障碍，结束寻路!");
        return NULL;//终点是障碍
    }
    xNode *node=xNode::createWithPosition(fromTileCoor);
    this->insertNodeInOpenList(node);
    do
    {
        //获取最小 f值的节点
        xNode *currentNode=(xNode*)openList->objectAtIndex(0);
        //把当前点添加到关闭列表
        closeList->addObject(currentNode);
        CCLOG("closeListNum:%d",closeList->count());

        openList->removeObjectAtIndex(0);
        //如果当前节点和目标节点相同，则找到路径，结束
        CCLOG("当前(%g,%g),目标(%g,%g)",currentNode->tileCoord.x,currentNode->tileCoord.y,toTileCoor.x,toTileCoor.y);
        if (ccPointEqualPoint(currentNode->tileCoord, toTileCoor))
        {
            this->constructPathFromStep(currentNode);
            openList=NULL;
            closeList=NULL;
            break;
        }
        //获取相邻的节点
        CCPointArray *adjSteps=walkableAdjacentTilesCoordForTIleCoord(currentNode->tileCoord);
        for (int i=0; i<adjSteps->count(); i++)
        {
            CCLOG("(%g,%g)",adjSteps->getControlPointAtIndex(i).x,adjSteps->getControlPointAtIndex(i).y);
            xNode *lnode=xNode::createWithPosition(adjSteps->getControlPointAtIndex(i));
            //判断节点是否已经在关闭列表里
            if (arrayContainObject(closeList, lnode))
            {
                lnode->release();
                continue;
            }
            //计算当前点到下一步的花费
            int moveCost = costToMoveFromStepToAdjacentStep(currentNode, lnode);
            //判断节点是否已经在开方列表里
            int index = indexForNodeInArray(openList, lnode);
            if (index== -1) //没有在开方列表里，添加进去
            {
                lnode->parent = currentNode;
                //节点的G值等于它父节点的G值和从父节点到该节点的花费:G=parent G + cost
                lnode->gScore=currentNode->gScore+moveCost;
                
                //估算到达目的点的成本H，按照瓦片地图坐标和麦哈顿法进行计算
                lnode->hScore=computeHScoreFromCoord(lnode->tileCoord, toTileCoor);
                // Adding it with the function which is preserving the list ordered by F score
                this->insertNodeInOpenList(lnode);
                lnode->release();
            }
            else
            {
                //已经存在于开方列表
                lnode->release();
                lnode=(xNode*)openList->objectAtIndex(index);     //检索旧节点，其花费已经计算过
                // Check to see if the G score for that step is lower if we use the current step to get there
                if ((currentNode->gScore+moveCost)<lnode->gScore)
                {
                    // The G score is equal to the parent G score + the cost to move from the parent to it
					lnode->gScore = currentNode->gScore + moveCost;
					
                    //因为G值改变了，所以F值可能也变了，因此为了维护开放列表我们要删除改节点，再通过插入函数重新把它插入到开放列表里，此插入函数通过F值进行维护
					// 在将节点从开放列表中删除之前先retain下
                    lnode->retain();
                    // 现在将节点从开放列表中删除，而不用担心该节点被释放掉
					openList->removeObjectAtIndex(index);
					// 通过对F值比较插入函数重新将该节点插入到开放列表里
					this->insertNodeInOpenList(lnode);
					// 因为之前retain过，现在release下
                    lnode->release();
                 }
            }
        }
        CCLOG("openListCount:%d",openList->count());
    }while (openList->count()>0);
    
    if (shortestPath==NULL) {
        CCLog("no path found");
        return NULL;
    }
    return shortestPath;
}
//计算移动消耗G值
int xAstar::costToMoveFromStepToAdjacentStep(xNode *fromStep, xNode *toNode)
{
    // Because we can't move diagonaly and because terrain is just walkable or unwalkable the cost is always the same.
	// But it have to be different if we can move diagonaly and/or if there is swamps, hills, etc...
	return 1;//因为我们不允许斜着走，所以上下花费都是1
}
//计算到终点的预算消耗H值
int xAstar::computeHScoreFromCoord(cocos2d::CCPoint fromCoord, cocos2d::CCPoint toCoord)
{
    //麦哈顿估算法
    // Here we use the Manhattan method, which calculates the total number of step moved horizontally and vertically to reach the
	// final desired step from the current step, ignoring any obstacles that may be in the way
    return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}
//获取相邻的节点上、下、左、右
CCPointArray* xAstar::walkableAdjacentTilesCoordForTIleCoord(cocos2d::CCPoint tileCoord)
{
    CCPointArray *tmp=CCPointArray::create(4);
    tmp->retain();
    //up
    CCPoint p=ccp(tileCoord.x, tileCoord.y-1);
    if (isValidTileCoord(p) && isCanMoveAtTileCoord(p))
    {
        tmp->addControlPoint(p);
    }
    //left
    p = ccp(tileCoord.x-1, tileCoord.y);
    if (isValidTileCoord(p) && isCanMoveAtTileCoord(p)) {
        tmp->addControlPoint(p);
    }
    //bottom
    p=ccp(tileCoord.x, tileCoord.y+1);
    if (isValidTileCoord(p) && isCanMoveAtTileCoord(p)) {
        tmp->addControlPoint(p);
    }
    //right
    p=ccp(tileCoord.x+1, tileCoord.y);
    if (isValidTileCoord(p) && isCanMoveAtTileCoord(p)) {
        tmp->addControlPoint(p);
    }
    return tmp;
 }
//返回路径
void xAstar::constructPathFromStep(xNode *pNode)
{
    shortestPath = CCArray::create();
    shortestPath->retain();
    do {
        if (pNode->parent != NULL)
        {
            shortestPath->insertObject(pNode, 0);//go backward，the last one is the origin position;
        }
        pNode=pNode->parent;    //go backward
    } while (pNode != NULL);
}
//判断是否是有效的地图格
bool xAstar::isValidTileCoord(cocos2d::CCPoint tileCoord)
{
    if (tileCoord.x<0 || tileCoord.y<0 || tileCoord.x>=map->getMapSize().width || tileCoord.y>=map->getMapSize().height) {
        return false;
    }
    else
    {
        return true;
    }
}
//判断地图格是否可行
bool xAstar::isCanMoveAtTileCoord(cocos2d::CCPoint tileCoord)
{
    CCLog("xAstar_mapPos:(%g,%g)",tileCoord.x,tileCoord.y);
    int tileGid=xLuojiLayer->tileGIDAt(tileCoord);
    if (tileGid) {
        CCDictionary *dic=map->propertiesForGID(tileGid);
        if (dic) {
            const CCString *str=dic->valueForKey("isPeng");
            int x=strcmp(str->getCString(), "true");
            if (str && x==0) {
                //closeList->addObject(xNode::createWithPosition(mpos));
                return false;
            }
        }
    }
    return true;
}
//转成地图坐标
CCPoint xAstar::tileCoordForPosition(cocos2d::CCPoint mpos)
{
    int x=mpos.x/map->getTileSize().width;
    int y=((map->getMapSize().height*map->getTileSize().height)-mpos.y)/map->getTileSize().height;
    return ccp(x, y);
}
//转成gl坐标
CCPoint xAstar::positonForTileCoord(cocos2d::CCPoint tileCoord)
{
    int x = (tileCoord.x * map->getTileSize().width) + map->getTileSize().width/2;
    int y = (map->getMapSize().height * map->getTileSize().height) - (tileCoord.y * map->getTileSize().height) - map->getTileSize().height/2;
    return ccp(x, y);
}
//获取最小F值的节点保存到开发列表
void xAstar::insertNodeInOpenList(xNode *pnode)
{
    int stepFscore=pnode->getFscore();
    int count=openList->count();
    int i=0;
    for (; i<count; i++) {
        if (stepFscore<=((xNode*)openList->objectAtIndex(i))->getFscore()) {
            break;
        }
    }
    openList->insertObject(pnode, i);
}
//--------------------自定义函数---------------------------//
#pragma mark--自定义功能函数

//判断两点是否相同
bool xAstar::ccPointEqualPoint(cocos2d::CCPoint pos1, cocos2d::CCPoint pos2)
{
    if ((pos1.x==pos2.x) && (pos1.y==pos2.y)) {
        return true;
    }
    else
    {
        return false;
    }
}
//判断列表里是否包含节点
bool xAstar::arrayContainObject(cocos2d::CCArray *pArray, xNode *pNode)
{
    CCObject *node;
    bool flag = false;
    CCARRAY_FOREACH(pArray, node)
    {
        xNode *tem=(xNode*)node;
        if (ccPointEqualPoint(tem->tileCoord, pNode->tileCoord))
        {
            flag=true;
            break;
        }
    }
    return flag;
}
//返回列表里某个节点的索引值
int xAstar::indexForNodeInArray(cocos2d::CCArray *pArray, xNode *pNode)
{
    int     i=0;
    xNode  *temp;
    for (; i<pArray->count(); i++)
    {
        temp=(xNode*)pArray->objectAtIndex(i);
        if (ccPointEqualPoint(temp->tileCoord, pNode->tileCoord))
        {
            break;
        }
    }
    if (i==pArray->count())
    {
        return -1;
    }
    else
    {
        return i;
    }
}