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
        uint32_t xResolution = 800;
        uint32_t yResolution = 800;
        uint32_t textureSize = 60;
        float scaleFactor = 0;
        float scaledSpriteSize = 0;

        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell, uint32_t x, uint32_t y, uint32_t worldSize);
    };
}