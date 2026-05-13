#pragma once
#include <utility>

#include "body.h"

class loop {
    int numBodies = 0;
public:
    std::vector<body*> container;
    loop() = default;
    ~loop() {
        for (auto n : container) {
            delete n;
        }
    }

    body* createBody(std::string name, double posx, double posy, double posz, double mass) {
        body* n = new body(std::move(name), posx, posy, posz, mass);
        this->container.push_back(n);
        return n;
    }

    void step(double deltaT) {
        for (int i = 0; i < container.size(); i++) {
            auto n = container[i];
            n->clearAcc();
            for (int j = 0; j < container.size(); j++) {
                auto other = container[j];
                if (j == i) continue;
                n->applyGravity(*other);
            }
        }

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