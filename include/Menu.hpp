#pragma once
#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(float width, float height);
    int run(sf::RenderWindow& window); // 0 = jugar, 1 = salir

private:
    sf::Texture textureFondo; // La imagen en memoria
    sf::Sprite spriteFondo;   // El objeto que dibuja la imagen
    sf::Font font;
    sf::Text title;
    sf::Text playText;
    sf::Text exitText;
};
