#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <string>
#include <deque>
#include "constants.h"

struct body {
private:
    double mass;
public:
    std::string name;
    double posx, posy, posz; // one unit of pos = 1m (ex x=0 and x=1 are 1m apart)
    double velx, vely, velz;
    double accx, accy, accz;
    std::deque<std::pair<double, double>> trail;

    // constructor
    body(std::string name, double posx, double posy, double posz, double mass) {
        this->name = std::move(name);
        this->posx = posx;
        this->posy = posy;
        this->posz = posz;
        this->mass = mass;
        velx = vely = velz = 0.0;
        accx = accy = accz = 0.0;
    }

    // functions
    // helpers
    double getMass(){return this->mass;}
    void clearAcc() {
        accx = accy = accz = 0.0;
    }
    void setMass(double newMass){this->mass = newMass;}
    void setVel(double newX, double newY, double newZ) {
        this->velx = newX;
        this->vely = newY;
        this->velz = newZ;
    }

    void applyGravity(body& other) {
        double disx = other.posx - this->posx;
        double disy = other.posy - this->posy;
        double disz = other.posz - this->posz;
        double r2 = ((disx*disx)+(disy*disy)+(disz*disz));
        double r = sqrt(r2);
        double F = G_CONSTANT * ((this->mass*other.getMass())/(r2));
        double acc = F/this->mass;
        this->accx += acc * (disx / r);
        this->accy += acc * (disy / r);
        this->accz += acc * (disz / r);
    }
};
