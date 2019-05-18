#include "srgsim/GUI.h"

#include "srgsim/Cell.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/Simulator.h"

#include <iostream>

namespace srgsim
{
GUI::GUI()
{
    std::string textureFile = Simulator::get_selfpath() + "/textures/test_texture.png";
    std::cout << "[GUI] Info: loading textureFile '" << textureFile << "'" << std::endl;
    this->texture = new sf::Texture();
    if (!this->texture->loadFromFile(textureFile)) {
        std::cerr << "[GUI] Coudn't load the texture file " << textureFile << std::endl;
    }
    this->texture->setSmooth(true);
    this->texture->setRepeated(true);
    this->window = new sf::RenderWindow(sf::VideoMode(xResolution, yResolution), "Grid Simulator GUI");
    this->window->setActive(false);
    for (int i = 0; i != Type::Last; i++) {
        sf::Sprite sprite;
        sprite.setTexture(*this->texture);
        Type type = static_cast<Type>(i);
        switch (type) {
        case Type::Floor:
            sprite.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
            break;
        case Type::Door:
            sprite.setTextureRect(sf::IntRect(0, textureSize, textureSize, textureSize));
            break;
        case Type::Wall:
            sprite.setTextureRect(sf::IntRect(textureSize, 0, textureSize, textureSize));
            break;
        case Type::Unknown:
            sprite.setTextureRect(sf::IntRect(textureSize, textureSize, textureSize, textureSize));
            break;
        default:
            std::cout << "[GUI] Unknown cell type " << type << std::endl;
        }
        sprites[type] = sprite;
    }
}

GUI::~GUI()
{
    delete this->window;
    delete this->texture;
}

void GUI::draw(World* world)
{
    this->window->setActive(true);

    handleSFMLEvents(world);

    this->window->clear();

    for (auto& pair : world->getGrid()) {
        sf::Sprite sprite = getSprite(pair.second->type);
        sprite.setPosition(pair.second->coordinate.x * scaledSpriteSize, pair.second->coordinate.y * scaledSpriteSize);
        this->window->draw(sprite);
    }
    this->window->display();
}

void GUI::handleSFMLEvents(const World* world)
{
    sf::Event event;

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } else if (event.type == sf::Event::Resized) {
            window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
            scaleSprite(world);
        }
    }
}

void GUI::scaleSprite(const World* world)
{
    calculateSpriteSize(world);
    calculateScale();
    for (auto& sprite : sprites) {
        sprite.setScale(scaleFactor, scaleFactor);
    }
}

void GUI::calculateScale()
{
    scaleFactor = scaledSpriteSize / float(textureSize);
}

void GUI::calculateSpriteSize(const World* world)
{
    auto sizeX = float(window->getSize().x) / float(world->getSizeX());
    auto sizeY = float(window->getSize().y) / float(world->getSizeY());

    if (sizeX < sizeY) {
        scaledSpriteSize = sizeX;
    } else {
        scaledSpriteSize = sizeY;
    }
}

sf::Sprite GUI::getSprite(Type type)
{
    return sprites[type];
}
} // namespace srgsim
