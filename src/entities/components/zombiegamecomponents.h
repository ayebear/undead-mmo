#ifndef ZOMBIEGAMECOMPONENTS_H
#define ZOMBIEGAMECOMPONENTS_H

#include <OCS\Component.hpp>

struct Position : public ocs::Component<Position>
{

};

struct Velocity : public ocs::Component<Velocity>
{

};

struct Renderable : public ocs::Component<Renderable>
{

};

struct Damager : public ocs::Component<Damager>
{

};

struct Collidable : public ocs::Component<Collidable>
{

};

struct RigidBody : public ocs::Component<RigidBody> {}

struct Health : public ocs::Component<Health>
{

};

struct InfectionLevel : public ocs::Component<InfectionLevel>
{

};

#endif
