#ifndef ENTITYLIVING_H
#define ENTITYLIVING_H

#include "entity.h"

class EntityLiving: public Entity
{
    public:
        EntityLiving();
    protected:
        float HP;
};

#endif // ENTITYLIVING_H
