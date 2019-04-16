#include "Simulator.h"

#include <SFML/Graphics.hpp>

#include <unistd.h>
#include <limits.h>
#include <iostream>

std::string get_selfpath() {
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff)-1);
    if (len != -1) {
        buff[len] = '\0';
        std::string exePath = std::string(buff);

        return exePath.substr(0, exePath.find_last_of("/"));
    }
    /* handle error condition */
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(120, 120), "Grid Simulator GUI");

    sf::Texture texture;
    std::string executabelPath = get_selfpath();
    std::cout << "[GridSimGUI] executabelPath: " << executabelPath << std::endl;
    if (!texture.loadFromFile(executabelPath + "/textures/test_texture.png", sf::IntRect(30, 30, 60, 60))) {
        std::cerr << "[GridSimGUI] Coudn't load the texture!" << std::endl;
    }
    texture.setSmooth(true);
    texture.setRepeated(true);

    sf::Sprite sprite;
    sprite.setTexture(texture);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}