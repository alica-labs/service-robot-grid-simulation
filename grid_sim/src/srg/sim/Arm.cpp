#include "srg/sim/Arm.h"

#include "srg/sim/SimulatedAgent.h"

#include <srg/world/Cell.h>
#include <srg/World.h>

namespace srg
{
namespace sim
{
Arm::Arm(srg::sim::SimulatedAgent* robot)
        : robot(robot)
{
}

bool Arm::execute(srg::sim::containers::SimCommand sc, World* world)
{
    switch (sc.action) {
    case srg::sim::containers::Action::OPEN:
        this->open(sc.objectID, world);
        break;
    case srg::sim::containers::Action::CLOSE:
        this->close(sc.objectID, world);
        break;
    case srg::sim::containers::Action::PICKUP:
        this->pickUp(sc.objectID, world);
        break;
    case srg::sim::containers::Action::PUTDOWN:
        this->putDown(sc, world);
        break;
    default:
        return false;
    }
    return true;
}

bool Arm::open(essentials::IdentifierConstPtr objectID, World* world)
{
    world->openDoor(objectID);
    return true;
}

bool Arm::close(essentials::IdentifierConstPtr objectID, World* world)
{
    world->closeDoor(objectID);
    return true;
}

bool Arm::pickUp(essentials::IdentifierConstPtr objectID, World* world)
{
    if (robot->getCarriedObject()) {
        std::cout << "[Arm] Already carrying!" << std::endl;
        return false;
    }

    auto object = world->editObject(objectID);
    if (!object) {
        std::cout << "[Arm] Object does not exist! ID: " << *objectID << std::endl;
        return false;
    }

    srg::world::Coordinate diff = (robot->getCoordinate()- object->getCoordinate()).abs();
    if ( diff.x > 1 || diff.y > 1) {
        std::cout << "[Arm] Object too far away for picking! Distance: (" << diff.x << ", " << diff.y << ")" <<  std::endl;
        // too far away for grasping
        return false;
    }

    robot->setCarriedObject(object);
    std::cout << "[Arm] Object picked up: " << *object << std::endl;
    return true;
}

bool Arm::putDown(srg::sim::containers::SimCommand sc, World* world)
{
    if (!robot->getCarriedObject()) {
        std::cout << "[Arm] Nothing to put down!" << std::endl;
        return false;
    }

    srg::world::Coordinate diff = (robot->getCoordinate()- world::Coordinate(sc.x, sc.y)).abs();
    if ( diff.x > 1 || diff.y > 1) {
        // too far away for putDown
        std::cout << "[Arm] Target for placing the object too far away! Distance: (" << diff.x << ", " << diff.y << ")" << std::endl;
        return false;
    }

    if (world->placeObject(robot->getCarriedObject(), world::Coordinate(sc.x, sc.y))) {
        std::cout << "[Arm] Object placed at (" << sc.x << ", " << sc.y << ")" << std::endl;
        return true;
    } else {
        std::cout << "[Arm] Could not place the object here (" << sc.x << ", " << sc.y << ")" << std::endl;
        return false;
    }
}
} // namespace sim
} // namespace srg