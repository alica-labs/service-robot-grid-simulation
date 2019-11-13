#include "srgsim/world/GUI.h"

#include "srgsim/world/Cell.h"
#include "srgsim/world/Door.h"
#include "srgsim/world/Object.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/TaskType.h"

#include <FileSystem.h>
#include <SystemConfig.h>
#include <iostream>

//#define GUI_DEBUG

namespace srgsim
{
const std::string GUI::configFolder = ".grid_sim";
const std::string GUI::windowConfigFile = "Window.conf";
GUI::GUI(std::string windowName)
{
    this->readWindowConfig();
    std::string textureFile = essentials::SystemConfig::getInstance()->getConfigPath() + "/textures/textures.png";
    std::cout << "[GUI] Info: loading textureFile '" << textureFile << "'" << std::endl;
    this->texture = new sf::Texture();
    if (!this->texture->loadFromFile(textureFile)) {
        std::cerr << "[GUI] Coudn't load the texture file " << textureFile << std::endl;
    }
    this->texture->setSmooth(true);
    this->texture->setRepeated(true);
    this->window = new sf::RenderWindow(
            sf::VideoMode(this->windowConfig->tryGet<uint32_t>(800, "xSize", NULL), this->windowConfig->tryGet<uint32_t>(800, "ySize", NULL)),
            windowName);
    this->window->setPosition(
            sf::Vector2i(this->windowConfig->tryGet<uint32_t>(20, "xPosition", NULL), this->windowConfig->tryGet<uint32_t>(20, "yPosition", NULL)));
    this->window->setActive(false);
    for (int i = 0; i != static_cast<int>(SpriteType::Last); i++) {
        sf::Sprite sprite;
        sprite.setTexture(*this->texture);
        SpriteType type = static_cast<SpriteType>(i);
        switch (type) {
        case SpriteType::Wall:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, 0, textureSize, textureSize));
            break;
        case SpriteType::DoorOpen:
            sprite.setTextureRect(sf::IntRect(textureSize * 3, 0, textureSize, textureSize));
            break;
        case SpriteType::DoorClosed:
            sprite.setTextureRect(sf::IntRect(textureSize * 3, textureSize, textureSize, textureSize));
            break;
        case SpriteType::Floor:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, textureSize, textureSize, textureSize));
            break;
        case SpriteType::Unknown:
            sprite.setTextureRect(sf::IntRect(textureSize, textureSize, textureSize, textureSize));
            break;
        case SpriteType::Robot:
            sprite.setTextureRect(sf::IntRect(0, textureSize * 2, textureSize, textureSize));
            break;
        case SpriteType::CupBlue:
            sprite.setTextureRect(sf::IntRect(textureSize, textureSize * 3, textureSize, textureSize));
            break;
        case SpriteType::CupRed:
            sprite.setTextureRect(sf::IntRect(0, textureSize * 3, textureSize, textureSize));
            break;
        case SpriteType::CupYellow:
            sprite.setTextureRect(sf::IntRect(textureSize * 2, textureSize * 3, textureSize, textureSize));
            break;
        default:
            sprite.setTextureRect(sf::IntRect(0, 0, textureSize, textureSize));
            std::cout << "[GUI] Unknown cell type " << static_cast<int>(type) << std::endl;
        }
        sprites[static_cast<int>(type)] = sprite;
    }
}

GUI::~GUI()
{
    this->storeWindowConfig();
    delete this->windowConfig;
    delete this->window;
    delete this->texture;
}

void GUI::storeWindowConfig()
{
    auto pos = this->window->getPosition();
    this->windowConfig->setCreateIfNotExistent<uint32_t>(pos.x, "xPosition", NULL);
    this->windowConfig->setCreateIfNotExistent<uint32_t>(pos.y, "yPosition", NULL);
    auto size = this->window->getSize();
    this->windowConfig->setCreateIfNotExistent<uint32_t>(size.x, "xSize", NULL);
    this->windowConfig->setCreateIfNotExistent<uint32_t>(size.y, "ySize", NULL);
    this->windowConfig->store();
}

void GUI::readWindowConfig()
{
    char* homeFolder;
    homeFolder = getenv("HOME");
    if (homeFolder == NULL) {
        std::cerr << "[GUI] Unable to read the environment variable $HOME!" << std::endl;
        return;
    }
    std::string absoluteConfigFolder = essentials::FileSystem::combinePaths(std::string(homeFolder), srgsim::GUI::configFolder);
    if (!essentials::FileSystem::pathExists(absoluteConfigFolder)) {
        essentials::FileSystem::createDirectory(absoluteConfigFolder, 755);
    }

    std::string absoluteWindowConfigFile = essentials::FileSystem::combinePaths(absoluteConfigFolder, srgsim::GUI::windowConfigFile);
    if (!essentials::FileSystem::pathExists(absoluteWindowConfigFile)) {
        this->windowConfig = new essentials::Configuration(absoluteWindowConfigFile, "");
    } else {
        this->windowConfig = new essentials::Configuration(absoluteWindowConfigFile);
    }
}

void GUI::draw(World* world)
{
    this->window->setActive(true);

    handleSFMLEvents(world);

    this->window->clear();

    {
        std::recursive_mutex& dataMutex = world->getDataMutex();
        std::lock_guard<std::recursive_mutex> guard(dataMutex);
        for (auto& pair : world->getGrid()) {
            // background sprite
            sf::Sprite sprite = getSprite(pair.second->getType());
            sprite.setPosition(pair.second->coordinate.x * scaledSpriteSize, pair.second->coordinate.y * scaledSpriteSize);
            this->window->draw(sprite);
//            std::cout << "GUI: Background Sprite: " << pair.second->getType() << " at " << pair.second->coordinate << std::endl;

            // object sprites
            for (Object* object : pair.second->getObjects()) {
                sf::Sprite sprite;
                sprite = getSprite(object);
                sprite.setPosition(object->getCell()->coordinate.x * scaledSpriteSize, object->getCell()->coordinate.y * scaledSpriteSize);
                this->window->draw(sprite);

                if (ServiceRobot* robot = dynamic_cast<ServiceRobot*>(object)) {
                    if (const Object* carriedObject = robot->getCarriedObject()) {
                        sprite = getSprite(carriedObject);
                        sprite.setPosition((robot->getCell()->coordinate.x * scaledSpriteSize) + scaledSpriteSize / 2,
                                (robot->getCell()->coordinate.y * scaledSpriteSize) + scaledSpriteSize / 2);
                        sprite.setScale(0.25, 0.25);
                        this->window->draw(sprite);
                    }
                }
#ifdef GUI_DEBUG
                std::cout << "GUI: Placing object of Type " << object->getType() << " at (" << object->getCell()->coordinate.x << ", "
                          << object->getCell()->coordinate.y << ")" << std::endl;
#endif
            }
        }

        // for debug purposes
        for (Perception perception : world->getMarkers()) {
            sf::Sprite sprite;
            if (perception.type == ObjectType::CupRed)
                sprite = getSprite(SpriteType::Unknown);
            else {
                sprite = getSprite(SpriteType::Default);
            }
            sprite.setPosition((perception.x * scaledSpriteSize) + scaledSpriteSize / 4, (perception.y * scaledSpriteSize) + scaledSpriteSize / 4);
            sprite.setScale(0.25, 0.25);
            this->window->draw(sprite);
        }
        world->getMarkers().clear();
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

sf::Sprite GUI::getSprite(SpriteType type)
{
    return sprites[static_cast<int>(type)];
}

sf::Sprite GUI::getSprite(RoomType type)
{
    if (type == RoomType::Wall) {
        return getSprite(SpriteType::Wall);
    }
    return getSprite(SpriteType::Floor);
}

sf::Sprite GUI::getSprite(const Object* object)
{
    switch (object->getType()) {
        case ObjectType::Door:
            if (static_cast<const Door*>(object)->isOpen()) {
                return getSprite(SpriteType::DoorOpen);
            } else {
                return getSprite(SpriteType::DoorClosed);
            }
        case ObjectType::CupBlue:
            return getSprite(SpriteType::CupBlue);
        case ObjectType::CupYellow:
            return getSprite(SpriteType::CupYellow);
        case ObjectType::CupRed:
            return getSprite(SpriteType::CupRed);
        case ObjectType::Robot:
            return getSprite(SpriteType::Robot);
        default:
            std::cerr << "[GUI] Unknown object type encountered!" << object->getType() << std::endl;
            return getSprite(SpriteType::Unknown);
    }

}

} // namespace srgsim
