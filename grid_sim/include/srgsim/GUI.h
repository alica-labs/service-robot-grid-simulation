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
        uint32_t xResolution = 8;
        uint32_t yResolution = 8;
        uint32_t textureSize = 60;
        float scaleFactorX = 0;
        float scaleFactorY = 0;
        float scaledSpriteXSize = 0;
        float scaledSpriteYSize = 0;

        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell);
    };
}