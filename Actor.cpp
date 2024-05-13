// Actor.cpp
#include "Actor.h"

Actor::Actor(glm::vec3 position)
    : rb(10.0f, 0.0f, true, glm::vec3(0.5f), position) {}