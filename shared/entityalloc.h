#ifndef ENTITYALLOC_H
#define ENTITYALLOC_H

#include "entity.h"

// Would be nice to make this a static function part of the Entity class
// Too bad it would cause a circular dependency issue...
Entity* allocateEntity(int);

#endif
