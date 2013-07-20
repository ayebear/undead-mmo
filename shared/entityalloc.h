// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ENTITYALLOC_H
#define ENTITYALLOC_H

#include "entity.h"

// Would be nice to make this a static function part of the Entity class
// Too bad it would cause a circular dependency issue...
Entity* allocateEntity(int);

#endif
