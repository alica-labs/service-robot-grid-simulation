#include "GUI.h"

#include "Cell.h"
#include "SRGEnums.h"
#include "Simulator.h"

#include <iostream>

namespace srgsim{
    GUI::GUI() {
        std::string textureFile = Simulator::get_selfpath()+ "/textures/test_texture.png";
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

    GUI::~GUI() {
        delete this->window;
        delete this->texture;
    }

    sf::Sprite GUI::getSprite(Cell* cell) {
        sf::Sprite sprite;
        sprite.setTexture(*this->texture);
        switch(cell->type) {
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
        sprite.setPosition(cell->coordinate.x * scaledSpriteXSize, cell->coordinate.y * scaledSpriteYSize);
        return sprite;
    }

    void GUI::draw(World* world) {
        this->window->setSize({world->getSizeX()* textureSize, world->getSizeY()* textureSize});
        this->scaledSpriteXSize = float(xResolution) / float(world->getSizeX());
        this->scaledSpriteYSize = float(yResolution) / float(world->getSizeY());
        this->scaleFactorX = scaledSpriteXSize/float(textureSize);
        this->scaleFactorY = scaledSpriteYSize/float(textureSize);
        this->window->setActive(true);

        sf::Event event;

        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                this->window->close();
            }
        }

        this->window->clear();

        const std::map<Coordinate, Cell*> grid = world->getGrid();
        for(auto pair : grid) {
            this->window->draw(getSprite(pair.second));
        }
        this->window->display();
    }
}
