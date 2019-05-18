#pragma once

#include "World.h"
#include "srgsim/SRGEnums.h"

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

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

        sf::Sprite getSprite(Type type);

        void calculateScale();

        void handleSFMLEvents(const World* world);

        void calculateSpriteSize(const World *world);

        sf::Sprite sprites[Type::Last];

        void scaleSprite(const World *world);
    };
}