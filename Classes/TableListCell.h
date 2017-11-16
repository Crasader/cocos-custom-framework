#ifndef __TABLELISTCELL_H__
#define __TABLELISTCELL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace cocos2d::ui;
class TableListCell: public Widget
{
public:
    CREATE_FUNC(TableListCell);
    
    TableListCell() {}
    ssize_t getIdx() const;
    void setIdx(ssize_t uIdx);
    void reset();

private:
    ssize_t _idx;
};


#endif /* __TABLELISTCELL_H__ */
