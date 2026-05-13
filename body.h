#pragma once
#include <cstdint>
#include <utility>
#include <vector>
#include <string>
#include <queue>
#define G_CONSTANT 6.67300E-11
#define EARTH_MASS_CONSTANT 5.97219E24 // in kg
#define SUN_MASS_CONSTANT 1.989E30 // kg
#define MOON_MASS_CONSTANT 7.34767309E22
#define TRAIL_LENGTH 3000

#ifndef NBODY_BODY_H
#define NBODY_BODY_H

struct body {
private:
    double mass;
public:
    std::string name;
    double posx, posy, posz; // one unit of pos = 1m (ex x=0 and x=1 are 1m apart)
    double velx, vely, velz;
    double accx, accy, accz;

    std::queue<std::pair<double, double>> trail;
    //const
    body(std::string name, double posx, double posy, double posz, double mass) {
        this->name = std::move(name);
        this->posx = posx;
        this->posy = posy;
        this->posz = posz;
        this->mass = mass;
        velx = vely = velz = 0.0;
        accx = accy = accz = 0.0;
    }

    //functions
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

    double gravitationalForce(body& other) {
        double r = distanceFrom(other);
        return G_CONSTANT * ((this->mass*other.getMass())/(r*r));
    }

    double distanceFrom(body& other) {
        double posx1, posy1, posz1, posx2, posy2, posz2;
        posx1 = this->posx; posy1 = this->posy; posz1 = this->posz;
        posx2 = other.posx; posy2 = other.posy; posz2 = other.posz;
        return sqrt(((posx1-posx2)*(posx1-posx2)) + ((posy1-posy2)*(posy1-posy2)) + ((posz1-posz2)*(posz1-posz2)));
    }

    void applyGravity(body& other) {
        if (trail.size() == TRAIL_LENGTH) { // trail length
            trail.pop();
        }
        trail.emplace(posx, posy);
        double disx = other.posx - this->posx;
        double disy = other.posy - this->posy;
        double disz = other.posz - this->posz;
        double r = distanceFrom(other);
        double F = G_CONSTANT * ((this->mass*other.getMass())/(r*r));
        double acc = F/this->mass;
        this->accx += acc * (disx / r);
        this->accy += acc * (disy / r);
        this->accz += acc * (disz / r);
    }
};

#endif //NBODY_BODY_H