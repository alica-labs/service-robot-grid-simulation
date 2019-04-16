#pragma once

#include "World.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace grid_sim {
    class GUI {
    public:
        GUI();
        ~GUI();
        void draw(grid_sim::World* world);
    private:
        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell, u_int32_t x, u_int32_t y);
    };
}