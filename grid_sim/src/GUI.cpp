#include "GUI.h"

#include "Cell.h"
#include "SRGEnums.h"
#include "Simulator.h"

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
    sprite.setScale(scaleFactorX, scaleFactorY);
    sprite.setPosition(cell->coordinate.x * scaledSpriteSizeX, cell->coordinate.y * scaledSpriteSizeY);
    return sprite;
}

void GUI::draw(World* world)
{
    if (this->window->getSize().x <= this->window->getSize().y) {
        std::cout << " x <= y " << this->window->getSize().x << " " << this->window->getSize().y << std::endl;
        if (world->getSizeX() > world->getSizeY()) {
            std::cout << "w x < y " << world->getSizeX() << " " << world->getSizeY() << std::endl;
            this->scaledSpriteSizeX = float(this->window->getSize().x) / float(world->getSizeX());
            this->scaledSpriteSizeY = scaledSpriteSizeX * float(this->window->getSize().x) / float(this->window->getSize().y);
        } else {
            std::cout << "w x > y " << world->getSizeX() << " " << world->getSizeY() << std::endl;
            this->scaledSpriteSizeX = float(this->window->getSize().x) / float(world->getSizeY());
            this->scaledSpriteSizeY = scaledSpriteSizeX * float(this->window->getSize().x) / float(this->window->getSize().y);
        }
    } else {
       std::cout << "x > y " << this->window->getSize().x << " " << this->window->getSize().y << std::endl;
        if (world->getSizeX() > world->getSizeY()) {
            this->scaledSpriteSizeY = float(this->window->getSize().y) / float(world->getSizeX());
            this->scaledSpriteSizeX = scaledSpriteSizeY * float(this->window->getSize().y) / float(this->window->getSize().x);
        } else {
            this->scaledSpriteSizeY = float(this->window->getSize().y) / float(world->getSizeY());
            this->scaledSpriteSizeX = scaledSpriteSizeY * float(this->window->getSize().y) / float(this->window->getSize().x);
        }
    }
    this->scaleFactorX = scaledSpriteSizeX / float(textureSize);
    this->scaleFactorY = scaledSpriteSizeY / float(textureSize);
    this->window->setActive(true);

    sf::Event event;

    while (this->window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window->close();
        }
    }

    this->window->clear();

    const std::map<Coordinate, Cell*> grid = world->getGrid();
    for (auto pair : grid) {
        this->window->draw(getSprite(pair.second));
    }
    this->window->display();
}
} // namespace srgsim
