#include <iostream>
#include "objectallocator.h"

using namespace std;

struct PositionComponent
{
    int entityId;
    float x;
    float y;
};

int main()
{
    ObjectAllocator<PositionComponent> positionComponents(5);
    for (int i = 0; i < 10; ++i)
    {
        size_t test = positionComponents.allocate();
        cout << test << endl;
        positionComponents.erase(test);
    }
    return 0;
}
