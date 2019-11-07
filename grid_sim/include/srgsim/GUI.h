#pragma once

#include "srgsim/world/World.h"
#include "srgsim/world/TaskType.h"

#include <SFML/Graphics.hpp>
#include <SystemConfig.h>

#include <string>
#include <unordered_map>

namespace srgsim
{
class GUI
{
public:
    GUI();
    ~GUI();
    void draw(srgsim::World* world);

private:
    void readWindowConfig();
    void storeWindowConfig();
    sf::Sprite getSprite(SpriteObjectType type);
    void scaleSprite(const World* world);

    void handleSFMLEvents(const World* world);
    void calculateScale();
    void calculateSpriteSize(const World* world);

    essentials::Configuration* windowConfig;
    static const std::string configFolder;
    static const std::string windowConfigFile;

    uint32_t textureSize = 60;
    float scaleFactor = 0;
    float scaledSpriteSize = 0;

    sf::Sprite sprites[static_cast<int>(SpriteObjectType::Last)];
    sf::Texture* texture;
    sf::RenderWindow* window;

};
} // namespace srgsim