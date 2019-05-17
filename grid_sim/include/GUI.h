#pragma once

#include "World.h"

#include <SFML/Graphics.hpp>

#include <string>

namespace srgsim {
    class GUI {
    public:
        GUI();
        ~GUI();
        void draw(srgsim::World* world);
    private:
        uint32_t xResolution = 800;
        uint32_t yResolution = 800;
        uint32_t textureSize = 60;
        float scaleFactorX = 0;
        float scaleFactorY = 0;
        float scaledSpriteSizeX = 0;
        float scaledSpriteSizeY = 0;

        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell);
    };
}