#pragma once
#include <utility>
#include "body.h"

class simulation {
    int numBodies = 0;
public:
    std::vector<body*> container;
    simulation() = default;
    ~simulation() {
        for (auto n : container) {
            delete n;
        }
    }

    body* createBody(std::string name, double posx, double posy, double posz, double mass) {
        body* n = new body(std::move(name), posx, posy, posz, mass);
        this->container.push_back(n);
        numBodies++;
        return n;
    }

    void step(double deltaT) {
        // eulers method, TODO: change eventually
        for (auto n : container) {
            //trail
            if (n->trail.size() == TRAIL_LENGTH) n->trail.pop();
            n->trail.emplace(n->posx, n->posy);
            //physics
            n->clearAcc();
            for (auto other : container) {
                if (other == n) continue;
                n->applyGravity(*other);
            }
        }
        // integrate
        for (auto n : container) {
            n->velx += n->accx * deltaT;
            n->vely += n->accy * deltaT;
            n->velz += n->accz * deltaT;
            n->posx += n->velx * deltaT;
            n->posy += n->vely * deltaT;
            n->posz += n->velz * deltaT;
        }
    }
};