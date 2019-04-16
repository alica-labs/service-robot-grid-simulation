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
        u_int32_t xResolution = 800;
        u_int32_t yResolution = 800;
        u_int32_t textureSize = 60;
        float scaleFactor = 0;
        float scaledSpriteSize = 0;

        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell, u_int32_t x, u_int32_t y, u_int32_t worldSize);
    };
}