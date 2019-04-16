#include "GUI.h"

#include <Simulator.h>

#include <iostream>

namespace grid_sim{
    GUI::GUI() {
        std::string textureFile = grid_sim::Simulator::get_selfpath()+ "/textures/test_texture.png";
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

    sf::Sprite GUI::getSprite(Cell* cell, u_int32_t x, u_int32_t y, u_int32_t worldSize) {
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
            default:
                std::cout << "[GUI] Unknown cell type " << cell->type << std::endl;
        }
        sprite.setScale(scaleFactor, scaleFactor);
        sprite.setPosition(x*scaledSpriteSize,y*scaledSpriteSize);
        return sprite;
    }

    void GUI::draw(World* world) {
        this->scaledSpriteSize = float (xResolution)/float(world->getSize());
        this->scaleFactor = scaledSpriteSize/float(textureSize);
        this->window->setActive(true);

        sf::Event event;

        while (this->window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                this->window->close();
            }
        }

        this->window->clear();

        const std::vector<std::vector<Cell*>>& grid = world->getGrid();
        for(u_int32_t i = 0; i < grid.size(); i++) {
            const std::vector<Cell*>& column = grid[i];
            for (u_int32_t j = 0; j < column.size(); j++) {
                this->window->draw(getSprite(column[j], i, j, world->getSize()));
            }
        }

        this->window->display();
    }
}
