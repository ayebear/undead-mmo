#include "entitylist.h"
/*
EntityList::EntityList()
{
    if(&MasterList == NULL)
    {
   	 MasterList = new std::deque<Entity>;
    }
    MainList = new std::deque<Entity*>;
    InitList = new std::deque<Entity*>;
    CleanList = new std::deque<Entity*>;
}

EntityList::~EntityList()
{
}

Entity* EntityList::insertMasterEntity(Entity key)
{
    std::deque<Entity>::iterator imax = MasterList->end();
    std::deque<Entity>::iterator imin = MasterList->begin();
    std::deque<Entity>::iterator imid = midPoint(imin,imax);

    // continually narrow search until just one element remains
    while (imin < imax)
	{

   	 imid = midPoint(imin,imax);

   	 // code must guarantee the interval is reduced at each iteration
   	 assert(imid < imax);
   	 // note: 0 <= imin < imax implies imid will always be less than imax

   	 // reduce the search
   	 if (*imid < key)
   		 imin = ++imid;
   	 else
   		 imax = imid;
    }
    // At exit of while:
    //   if A[] is empty, then imax < imin
    //   otherwise imax == imin

    // deferred test for equality
    if ((imax == imin) && (*imin <= key))
    {
   	 MasterList->insert(++imin,key);
   	 return &(*imin);
    }
    else
    {
   	 return NULL;
    }
}

std::deque<Entity>::iterator EntityList::findMasterEntity(Entity key)
{
    std::deque<Entity>::iterator imax = MasterList->end();
    std::deque<Entity>::iterator imin = MasterList->begin();
    std::deque<Entity>::iterator imid = midPoint(imin,imax);

    // continually narrow search until just one element remains
    while (imin < imax)
	{
   	 imid = midPoint(imin,imax);

   	 // code must guarantee the interval is reduced at each iteration
   	 assert(imid < imax);
   	 // note: 0 <= imin < imax implies imid will always be less than imax

   	 // reduce the search
   	 if (*imid < key)
   		 imin = ++imid;
   	 else
   		 imax = imid;
    }
    // At exit of while:
    //   if A[] is empty, then imax < imin
    //   otherwise imax == imin
    // deferred test for equality
    if ((imax == imin) && (*imin == key))
    {
   	 return imin;
    }
    return MasterList->end();
}
bool EntityList::deleteMasterEntity(Entity key)
{
    std::deque<Entity>::iterator temp = findMasterEntity(key);
    if(temp != MasterList->end())
    {
   	 MasterList->erase(temp);
   	 return true;
    }
    return false;
}

bool EntityList::deleteEntity(Entity key)
{
    std::deque<Entity*>::iterator temp = findEntity(key);
    if(temp != MainList->end())
    {
   	 MainList->erase(temp);
   	 return true;
    }
    return false;
}

std::deque<Entity*>::iterator EntityList::findEntity(Entity key)
{
    std::deque<Entity*>::iterator imax = MainList->end();
    std::deque<Entity*>::iterator imin = MainList->begin();
    std::deque<Entity*>::iterator imid = midPoint(imin,imax);

    // continually narrow search until just one element remains
    while (imin < imax)
	{
   	 imid = midPoint(imin,imax);

   	 // code must guarantee the interval is reduced at each iteration
   	 assert(imid < imax);
   	 // note: 0 <= imin < imax implies imid will always be less than imax

   	 // reduce the search
   	 if (**imid < key)
   		 imin = ++imid;
   	 else
   		 imax = imid;
    }
    // At exit of while:
    //   if A[] is empty, then imax < imin
    //   otherwise imax == imin
    // deferred test for equality
    if ((imax == imin) && (**imin == key))
    {
   	 return imin;
    }
    return MainList->end();
}

bool EntityList::insertEntity(Entity key)
{
    std::deque<Entity*>::iterator imax = MainList->end();
    std::deque<Entity*>::iterator imin = MainList->begin();
    std::deque<Entity*>::iterator imid = midPoint(imin,imax);

    // continually narrow search until just one element remains
    while (imin < imax)
	{

   	 imid = midPoint(imin,imax);

   	 // code must guarantee the interval is reduced at each iteration
   	 assert(imid < imax);
   	 // note: 0 <= imin < imax implies imid will always be less than imax

   	 // reduce the search
   	 if (**imid < key)
   		 imin = ++imid;
   	 else
   		 imax = imid;
    }
    // At exit of while:
    //   if A[] is empty, then imax < imin
    //   otherwise imax == imin

    // deferred test for equality
    if ((imax == imin) && (**imin <= key))
    {
   	 MainList->insert(++imin,&key);
   	 return true;
    }
    else
    {
   	 return false;
    }
}

void EntityList::updateList()
{
    for(std::deque<Entity*>::iterator it = InitList->begin(), endit = InitList->end();it < endit;it++)
    {
   	 (*it)->setReady(true);
   	 (*it)->_ID = new int(getOpenId());
   	 insertMasterEntity(**it);
   	 insertEntity(**it);
    }
    InitList->clear();

    for(std::deque<Entity*>::iterator it = CleanList->begin(),endit = CleanList->end();it < endit;it++)
    {
   	 deleteEntity(**it);
   	 deleteMasterEntity(**it);
    }
    CleanList->clear();
}

//Returns true if the Int is not already being used.
bool EntityList::checkIdUsage(int key)
{
    std::deque<Entity>::iterator imax = MasterList->end();
    std::deque<Entity>::iterator imin = MasterList->begin();
    std::deque<Entity>::iterator imid = midPoint(imin,imax);

    // continually narrow search until just one element remains
    while (imin < imax)
	{
   	 imid = midPoint(imin,imax);

   	 // code must guarantee the interval is reduced at each iteration
   	 assert(imid < imax);
   	 // note: 0 <= imin < imax implies imid will always be less than imax

   	 // reduce the search
   	 if (*imid->_ID < key)
   		 imin = ++imid;
   	 else
   		 imax = imid;
    }
    // At exit of while:
    //   if A[] is empty, then imax < imin
    //   otherwise imax == imin
    // deferred test for equality
    if ((imax == imin) && (*imin->_ID == key))
    {
   	 return false;
    }
    return true;
}

int EntityList::getOpenId()
{
    int id = rand()%1000000;
    while(EntityList().checkIdUsage(id) == false)
    {
   	 id += rand()%10+1;
    }
    return id;
}

std::deque<Entity>::iterator EntityList::midPoint(std::deque<Entity>::iterator min, std::deque<Entity>::iterator max)
{
    std::deque<Entity>::iterator temp = min;
    std::advance(temp,floor(std::distance(min,max)/2));
    return temp;
}

std::deque<Entity*>::iterator EntityList::midPoint(std::deque<Entity*>::iterator min, std::deque<Entity*>::iterator max)
{
    std::deque<Entity*>::iterator temp = min;
    std::advance(temp,floor(std::distance(min,max)/2));
    return temp;
}
*/
