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
}

GUI::~GUI()
{
    delete this->window;
    delete this->texture;
}

void GUI::draw(World* world)
{
    calculateSpriteSize(world);
    calculateScale();
    this->window->setActive(true);

    handleSFMLEvents();

    this->window->clear();

    const std::map<Coordinate, Cell*> grid = world->getGrid();
    for (auto pair : grid) {
        this->window->draw(getSprite(pair.second));
    }
    this->window->display();
}

void GUI::handleSFMLEvents() const
{
    sf::Event event;

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        } else if (event.type == sf::Event::Resized) {
            window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        }
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

sf::Sprite GUI::getSprite(Cell* cell)
{
    sf::Sprite sprite;
    sprite.setTexture(*this->texture);
    switch (cell->type) {
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
        std::cout << "[GUI] Unknown cell type " << cell->type << std::endl;
    }
    sprite.setScale(scaleFactor, scaleFactor);
    sprite.setPosition(cell->coordinate.x * scaledSpriteSize, cell->coordinate.y * scaledSpriteSize);
    return sprite;
}
} // namespace srgsim
