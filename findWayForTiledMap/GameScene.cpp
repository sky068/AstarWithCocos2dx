//
//  GameScene.cpp
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//

#include "GameScene.h"
CCScene* pGame::scene()
{
    CCScene*scene=CCScene::create();
    pGame *layer=pGame::create();
    scene->addChild(layer);
    return scene;
}
bool pGame::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    else
    {
        //add map
        //xMap=CCTMXTiledMap::create("findWayMap.tmx");  //地图一
        xMap=CCTMXTiledMap::create("astarMap.tmx");     //地图二
        addChild(xMap);
        xLuojiLayer=xMap->layerNamed("luoji");
        xLuojiLayer->setVisible(false);
        CCTMXObjectGroup *objGP=xMap->objectGroupNamed("sprite");
        CCDictionary *dicPos=objGP->objectNamed("startPosition");
        //add player
        xPlayer=CCSprite::create("player.png");
        xMap->addChild(xPlayer);
        xPlayer->setPosition(ccp(dicPos->valueForKey("x")->floatValue(), dicPos->valueForKey("y")->floatValue()));
        //
        return true;
    }
}
void pGame::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}
void pGame::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
bool pGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pos=pTouch->getLocation();
    CCLOG("(%g,%g)",pos.x,pos.y);
    CCPoint playerPos=xPlayer->getPosition();
    xAstar *astar=new xAstar;
    pathArray=astar->startFindWay(playerPos, pos, xMap);
    delete astar;
    return true;
}
void pGame::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    schedule(schedule_selector(pGame::moveForPoints), 0.20f);
}
//按照轨迹移动
#pragma mark--按路径移动
void pGame::moveForPoints()
{
    if (pathArray)
    {
        if (pathArray->count()>0)
        {
            xNode *node=(xNode*)pathArray->objectAtIndex(0);
            CCPoint pos=node->tileCoord;
            pos=this->positonForTileCoord(pos);
            CCMoveTo *mto=CCMoveTo::create(0.20, pos);
            xPlayer->runAction(mto);
            pathArray->removeObjectAtIndex(0);
        }
        else
        {
            unschedule(schedule_selector(pGame::moveForPoints));
            CCLog("到达目的地！");
        }
    }
}

//转成地图坐标
CCPoint pGame::tileCoordForPosition(cocos2d::CCPoint mpos)
{
    int x=mpos.x/xMap->getTileSize().width;
    int y=((xMap->getMapSize().height*xMap->getTileSize().height)-mpos.y)/xMap->getTileSize().height;
    return ccp(x, y);
}
//转成gl坐标
CCPoint pGame::positonForTileCoord(cocos2d::CCPoint tileCoord)
{
    int x = (tileCoord.x * xMap->getTileSize().width) + xMap->getTileSize().width/2;
    int y = (xMap->getMapSize().height * xMap->getTileSize().height) - (tileCoord.y * xMap->getTileSize().height) - xMap->getTileSize().height/2;
    return ccp(x, y);
}
