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

    body* getBody(std::string target){
        for (auto n : this->container){
            if (n->name == target)
                return n;
        }
        return nullptr;
    }

    body* createBody(std::string name, double posx, double posy, double posz, double mass, double radius) {
        body* n = new body(std::move(name), posx, posy, posz, mass, radius); // NOTE: HEAP ALLOCATION
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
    // uses eulers method, TODO: change eventually, verlet method?
    void step(double deltaT) {
        for (auto n : container) {
            //trail
            if (n->trail.size() == TRAIL_LENGTH) n->trail.pop_front();
            n->trail.emplace_back(n->posx, n->posy);


            
            //physics (replace with tree algo at some point)
            n->clearAcc();
            for (auto other : container) {
                if (other == n) continue;
                n->applyGravity(*other);
            }
        }


        // integrate (eulers method initial, verlet after)
        for (auto n : container) {
            n->prev_posx = n->posx;
            n->prev_posy = n->posy;
            n->prev_posz = n->posz;

            if (n->initial_step){ // eulers method (can improve this to reduce upfront energy drift)
                n->velx += n->accx * deltaT;
                n->vely += n->accy * deltaT;
                n->velz += n->accz * deltaT;
                n->posx += n->velx * deltaT;
                n->posy += n->vely * deltaT;
                n->posz += n->velz * deltaT;
                n->initial_step = false;
            }
            else{
                // velocity verlet integration here
                n->posx += n->velx * deltaT + 0.5 * n->accx * deltaT * deltaT;
                n->posy += n->vely * deltaT + 0.5 * n->accy * deltaT * deltaT;
                n->posz += n->velz * deltaT + 0.5 * n->accz * deltaT * deltaT;

                double oldaccx = n->accx;
                double oldaccy = n->accy;
                double oldaccz = n->accz;
                n->clearAcc();
                for (auto other : container)
                {
                    if (other == n)
                        continue;
                    n->applyGravity(*other);
                }

                n->velx += 0.5*(n->accx + oldaccx) * deltaT;
                n->vely += 0.5 * (n->accy + oldaccy) * deltaT;
                n->velz += 0.5 * (n->accz + oldaccz) * deltaT;
            }
        }
    }
};