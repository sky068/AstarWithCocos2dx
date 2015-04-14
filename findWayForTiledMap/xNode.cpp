//
//  xNode.cpp
//  findWayForTiledMap
//
//  Created by SKY on 13-10-14.
//
//

#include "xNode.h"
xNode* xNode::createWithPosition(cocos2d::CCPoint _tileCoord)
{
    xNode *node=new xNode;
    if (node)
    {
        node->tileCoord=_tileCoord;
        node->gScore=0;
        node->hScore=0;
        node->parent=NULL;
        node->autorelease();
        return node;
    }
    else
    {
        CC_SAFE_DELETE(node);
        return NULL;
    }
}
int xNode::getFscore()
{
    int sc=gScore+hScore;
    return sc;
}
xNode::xNode()
{
    
};
xNode::~xNode()
{
    
}