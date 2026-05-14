#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "sim.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "nbody");

    const double deltaT = 3600.0/2; // physics timestep in seconds per frame, 1 = 1s/frame
    // 3600.0 1 hour per frame
    // 86400.0 1 day per frame
    // 604800.0 1 week per frame
    const float scaleFactor = 200.0f / AU; // 1 AU = 200px
    const float centerX = window.getSize().x/2.0f;
    const float centerY = window.getSize().y/2.0f;


    //init
    simulation nbody;
    auto sun = nbody.createBody("sol", 0, 0, 0, SUN_MASS_CONSTANT);
    auto earth = nbody.createBody("earth", AU, 0, 0, EARTH_MASS_CONSTANT);
    //auto moon = nbody.createBody("moon", earth->posx+384.4E6, 0, 0, MOON_MASS_CONSTANT);

    sun->setVel(0,0,0);
    earth->setVel(0, 29784.8, 0); // in m/s || y = 29784.8 is normal, this is tangential velocity
    //moon->setVel(0,29784.8+1024,0);


    //graphics
    sf::CircleShape sunGraphic(10.0f);
    sf::CircleShape earthGraphic(10.0f);
    sunGraphic.setOrigin({10.0f, 10.0f});
    earthGraphic.setOrigin({10.0f, 10.0f});


    // view options
    sf::View main(window.getDefaultView().getCenter(), window.getDefaultView().getSize());
    auto defaultSize = main.getSize();


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
                main.setCenter({centerX + static_cast<float>(earth->posx * scaleFactor),
                    centerY + static_cast<float>(earth->posy * scaleFactor)});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Slash))
            {
                //debug: focus sun
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
        earthGraphic.setPosition({
        centerX + static_cast<float>(earth->posx * scaleFactor),
        centerY + static_cast<float>(earth->posy * scaleFactor)
        });


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
        window.draw(earthGraphic);


        // end the current frame
        window.display();
    }
    return 0;
}