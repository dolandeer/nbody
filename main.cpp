#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "sim.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "nbody");
    window.setFramerateLimit(60); // this is unfortunately not perfect, 
    // meaning its impossible to map a single frame to a real simulation time with this approach

    double deltaT = DAY_PER_SECOND*7*2; // physics timestep in years, 1 frame = 1 year.
    // default is (1/365.25)/60 or approx 1 day per second
    const float scaleFactor = 200.0f; // 1 AU = 200px
    const float centerX = window.getSize().x/2.0f;
    const float centerY = window.getSize().y/2.0f;

    //init
    simulation nbody;
    auto sun = nbody.createBody("sol", 0, 0, 0, SUN_MASS_CONSTANT);
    auto earth = nbody.createBody("earth", EARTH_ORBIT_RADIUS, 0, 0, EARTH_MASS_CONSTANT);
    auto moon = nbody.createBody("moon", EARTH_ORBIT_RADIUS+MOON_ORBIT_RADIUS, 0, 0, MOON_MASS_CONSTANT);


    double totalMass = SUN_MASS_CONSTANT + EARTH_MASS_CONSTANT + MOON_MASS_CONSTANT;
    sun->setVel(0, -((EARTH_MASS_CONSTANT * EARTH_ORBIT_V) / totalMass), 0);
    earth->setVel(0, (SUN_MASS_CONSTANT / totalMass) * EARTH_ORBIT_V, 0);
    moon->setVel(0, earth->vely + MOON_ORBIT_V, 0);

    //graphics
    sf::CircleShape sunGraphic(1.0f);
    //sf::CircleShape earthGraphic(1.0f);
    sunGraphic.setOrigin({1.0f, 1.0f});
    //earthGraphic.setOrigin({1.0f, 1.0f});


    // view options
    sf::View main(window.getDefaultView().getCenter(), window.getDefaultView().getSize());
    auto defaultSize = main.getSize();
    std::string target = "sol";

    while (window.isOpen())
    {
        window.setView(main);

        // check all the window's events that were triggered since the last iteration of the loop
        // input handler
        while (const std::optional event = window.pollEvent())
        {
            // request to close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            //input handling
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal))
            {
                //zoom in
                main.zoom(1/1.25f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen))
            {
                //zoom out
                main.zoom(1.25f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                //move left
                main.move({-10,0});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                //move right
                main.move({10,0});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                //move up
                main.move({0,-10});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                //move down
                main.move({0,10});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backslash)) {
                //debug: focus earth
                target = "earth";
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash))
            {
                //debug: focus sun
                target = "sol";
                main.setCenter({centerX + static_cast<float>(sun->posx * scaleFactor),
                                centerY + static_cast<float>(sun->posy * scaleFactor)});
            }
        }

        //physics handling below
        nbody.step(deltaT);


        // clear the window with black color
        window.clear(sf::Color::Black);

        // graphics
        sunGraphic.setPosition({
        centerX + static_cast<float>(sun->posx * scaleFactor),
        centerY + static_cast<float>(sun->posy * scaleFactor)
        });
        /*
        earthGraphic.setPosition({
        centerX + static_cast<float>(earth->posx * scaleFactor),
        centerY + static_cast<float>(earth->posy * scaleFactor)
        });
        */

        // keep centered
        auto targetBody = nbody.getBody(target);
        if (target == "sol" || targetBody == nullptr)
        {
            // pass
        }
        else
        {
            main.setCenter({centerX + static_cast<float>(targetBody->posx * scaleFactor),
                            centerY + static_cast<float>(targetBody->posy * scaleFactor)});
        }

        // draw
        //handle trail
        for (const auto n : nbody.getContainer()) {
            sf::VertexArray lines(sf::PrimitiveType::LineStrip, n->trail.size());
            for (int i = 0; i < n->trail.size(); i++) {
                lines[i].position = sf::Vector2f(
                    centerX + static_cast<float>(n->trail[i].first  * scaleFactor),
                    centerY + static_cast<float>(n->trail[i].second * scaleFactor)
                );
            }
            window.draw(lines);
        }
        window.draw(sunGraphic);
        //window.draw(earthGraphic);
        std::cout << earth->posx - sun->posx << ", " << earth->posy - sun->posy << std::endl;

        // end the current frame
        window.display();
    }
    return 0;
}