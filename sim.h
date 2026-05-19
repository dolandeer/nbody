#pragma once
#include <utility>
#include "body.h"

class simulation {
private:
    std::vector<body*> container;
public:
    simulation() = default;
    ~simulation() {
        for (auto n : container) {
            delete n;
        }
    }

    //helpers
    const std::vector<body*>& getContainer() const {
        return this->container;
    }

    const body* getBody(std::string target){
        for (auto n : this->container){
            if (n->name == target)
                return n;
        }
        return nullptr;
    }

    body* createBody(std::string name, double posx, double posy, double posz, double mass) {
        body* n = new body(std::move(name), posx, posy, posz, mass); // NOTE: HEAP ALLOCATION
        this->container.push_back(n);
        return n;
    }

    double totalMass() const {
        double out = 0.0;
        for (const auto n : container) {
            out += n->getMass();
        }
        return out;
    }


    //physics operations
    // one step happens per frame, deltaT determines the size of this step
    // uses eulers method, TODO: change eventually
    void step(double deltaT) {
        for (auto n : container) {
            //trail
            if (n->trail.size() == TRAIL_LENGTH) n->trail.pop_front();
            n->trail.emplace_back(n->posx, n->posy);

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