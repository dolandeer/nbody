#include <iostream>
#include <SFML/Graphics.hpp>
#include "loop.h"
#define AU_1 1.496e+11 // in meters || 1 au = 200px

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "nbody");
    sf::Clock clock;

    double physicsAccumulator = 0.0;
    const double simSpeed = 10000000.0; // speed factor (without modifying physics) || 10000000.0 seems to be good, 500000.0
    const double deltaT = 3600.0; // physics timestep in seconds, 1 = 1s
    // 3600.0 1 hour per frame
    // 86400.0 1 day per frame
    // 604800.0 1 week per frame

    const float scaleFactor = 200.0f / AU_1; // 1 AU = 200px
    const float centerX = 800 / 2.0f;
    const float centerY = 600 / 2.0f;


    //init
    loop nbody;
    auto sun = nbody.createBody("sol", 0, 0, 0, SUN_MASS_CONSTANT);
    auto earth = nbody.createBody("earth", AU_1, 0, 0, EARTH_MASS_CONSTANT);
    //auto moon = nbody.createBody("moon", earth->posx+384.4E6, 0, 0, MOON_MASS_CONSTANT);
    sun->setVel(0,0,0);
    earth->setVel(0, 29784.8, 0); // in m/s || y = 29784.8 is normal, this is tangential velocity
    //moon->setVel(0,29784.8+1024,0);
    //std::cout << earth->gravitationalForce(*sun) << std::endl;

    //graphics
    sf::CircleShape sunGraphic(109.0f);
    sf::CircleShape earthGraphic(1.0f);
    //sf::CircleShape moonGraphic(.1f);
    sunGraphic.setFillColor(sf::Color(255, 255, 0));
    earthGraphic.setFillColor(sf::Color(100, 250, 50));
    //moonGraphic.setFillColor(sf::Color(255, 255, 255));
    sunGraphic.setOrigin({109.0f, 109.0f});
    earthGraphic.setOrigin({1.0f, 1.0f});
    //moonGraphic.setOrigin({0.5f,0.5f});
    sunGraphic.setPosition({
    centerX + static_cast<float>(sun->posx * scaleFactor),
    centerY + static_cast<float>(sun->posy * scaleFactor)
    });
    earthGraphic.setPosition({
    centerX + static_cast<float>(earth->posx * scaleFactor),
    centerY + static_cast<float>(earth->posy * scaleFactor)
    });
    /*
    moonGraphic.setPosition({
    centerX + static_cast<float>(moon->posx * scaleFactor),
    centerY + static_cast<float>(moon->posy * scaleFactor)
    });
    */


    //view options
    /*
    sf::View view2;
    view2.setCenter({centerX + static_cast<float>(earth->posx * scaleFactor),
        centerY + static_cast<float>(earth->posy * scaleFactor)});
    view2.setSize({200.f, 200.f});
    view2.zoom(0.3f);
    view2.setViewport(sf::FloatRect({0.75f, 0.f}, {0.25f, 0.25f}));
    */

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        // essentially the input handler
        while (const std::optional event = window.pollEvent())
        {
            // request to close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }


        //physics handling below
        physicsAccumulator += clock.restart().asSeconds() * simSpeed;
        if (physicsAccumulator > deltaT * 1000) physicsAccumulator = deltaT * 1000;
        while (physicsAccumulator >= deltaT) {
            nbody.step(deltaT);
            physicsAccumulator -= deltaT;
        }

        //fixed AU->PX
        sunGraphic.setPosition({
        centerX + static_cast<float>(sun->posx * scaleFactor),
        centerY + static_cast<float>(sun->posy * scaleFactor)
        });
        earthGraphic.setPosition({
        centerX + static_cast<float>(earth->posx * scaleFactor),
        centerY + static_cast<float>(earth->posy * scaleFactor)
        });
        /*
        moonGraphic.setPosition({
        centerX + static_cast<float>(moon->posx * scaleFactor),
        centerY + static_cast<float>(moon->posy * scaleFactor)
        });
        */

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything below

        //viewport stuff
        /*
        window.setView(view2);
        view2.setCenter({centerX + static_cast<float>(earth->posx * scaleFactor),
        centerY + static_cast<float>(earth->posy * scaleFactor)});
        window.draw(earthGraphic);
        window.draw(moonGraphic);
        // restore the default view
        window.setView(window.getDefaultView());
        */


        window.draw(sunGraphic);
        window.draw(earthGraphic);
        //window.draw(moonGraphic);

        //handle trail
        for (const auto n : nbody.container) {
            sf::VertexArray lines(sf::PrimitiveType::LineStrip, n->trail.size());
            auto tempTrail = n->trail; // copy since we're popping
            for (int i = 0; !tempTrail.empty(); i++) {
                auto p = tempTrail.front();
                tempTrail.pop();
                lines[i].position = sf::Vector2f(
                    centerX + static_cast<float>(p.first * scaleFactor),
                    centerY + static_cast<float>(p.second * scaleFactor)
                );
            }
            window.draw(lines);
        }

        // end the current frame
        window.display();
    }
    return 0;
}