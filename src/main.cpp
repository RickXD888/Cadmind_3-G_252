#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Juego.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "CardMind");

    Menu menu(800, 600);
    Juego juego;

    int opcion = menu.run(window);

    if (opcion == 0)
        juego.run(window);

    return 0;
}
