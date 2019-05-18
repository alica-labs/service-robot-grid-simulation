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
        float scaleFactor = 0;
        float scaledSpriteSize = 0;

        sf::RenderWindow* window;
        sf::Texture* texture;

        sf::Sprite getSprite(Cell* cell);

        void calculateScale();

        void handleSFMLEvents() const;

        void calculateSpriteSize(const World *world);
    };
}