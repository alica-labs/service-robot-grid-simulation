#pragma once

#include "srg/viz/Marker.h"
#include "srg/viz/SpriteType.h"

#include <srg/World.h>
#include <srg/world/RoomType.h>

#include <SystemConfig.h>

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

namespace srg
{
class GUI
{
public:
    GUI(std::string windowName);

    ~GUI();

    void addMarker(viz::Marker marker);
    void draw(srg::World* world);

private:
    void readWindowConfig();
    void storeWindowConfig();
    sf::Sprite getSprite(viz::SpriteType type);
    sf::Sprite getSprite(world::RoomType type);
    sf::Sprite getSprite(const world::Object* object);
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
    float zoomFactor = 0;

    sf::Sprite sprites[static_cast<int>(viz::SpriteType::Last)];

    sf::Texture* texture;
    sf::RenderWindow* window;
    sf::View* view;
    std::vector<viz::Marker> markers;
};
} // namespace srg