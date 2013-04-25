/*
ENTITY GRID (server only)
EntityGrid class:
    Will contain a 2D array of EntityList objects, which stores ALL entities.
    This will only be used on the server.
Purpose:
    To spatially partition the entity list to dramatically improve performance when doing anything with entities.
    Also improves performance and simplicity of deciding what entities to send to the clients.
        A 3x3 portion (or something like that) of the entity grid will be sent to each client.
Caveats:
    Getting an entity pointer from an entity ID would require a VERY expensive search throughough the entire grid.
        This will require keeping a copy of the entity pointers in a different data container, see below.
    Entity movement code will be slightly more complex.
        When an entity is out of bounds, it will need to be moved to the proper grid location.
*/

#ifndef ENTITYGRID_H
#define ENTITYGRID_H

class EntityGrid
{
    public:
        EntityGrid();
    private:
};

#endif // ENTITYGRID_H
