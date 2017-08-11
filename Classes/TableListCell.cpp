

#include "TableListCell.h"



void TableListCell::reset()
{
    _idx = CC_INVALID_INDEX;
}

ssize_t TableListCell::getIdx() const
{
    return _idx;
}

void TableListCell::setIdx(ssize_t idx)
{
    _idx = idx;
}

