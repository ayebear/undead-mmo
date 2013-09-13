#include "range.h"

Range::Range()
{
    reset();
}

bool Range::hasRange() const
{
    return (type != NoType);
}

void Range::reset()
{
    dec1 = 0;
    dec2 = 0;
    rule1 = NoRule;
    rule2 = NoRule;
    type = NoType;
}

Range::Type Range::getType()
{
    return type;
}
