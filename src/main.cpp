/**
 * main.cpp
 * Punto de entrada de la aplicación CardMind.
 *
 * Este fichero crea la ventana SFML, instancia las pantallas `Menu` y `Juego`
 * y ejecuta el bucle principal que alterna entre mostrar el menú y ejecutar
 * el juego según la elección del usuario.
 */

#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Juego.hpp"

int main() {
    // Crear ventana inicial para el menú (tamaño reducido por comodidad)
    sf::RenderWindow window(sf::VideoMode(800, 600), "CardMind");
    window.setFramerateLimit(60);

    // Instancias principales
    Menu menu(800, 600);
    Juego juego;

    // Bucle principal: mostrar menú, ejecutar juego y regresar
    while (window.isOpen()) {
        // Mostrar el menú y esperar elección del usuario
        int opcion = menu.run(window);

        if (opcion == 0 || opcion == 1 || opcion == 3) {
            // Sincronizar volumen desde el menú hacia el juego
            juego.setMasterVolume(menu.getVolume());

            if (opcion == 3) {
                // Opción de prueba: mostrar la pantalla de victoria
                juego.showVictoryTest(window);
            } else {
                // Obtener nombres (si se introdujeron) y ejecutar el juego
                std::string n1 = menu.getPlayerName1();
                std::string n2 = menu.getPlayerName2();
                juego.run(window, opcion, n1, n2);
            }

            // Al volver desde el juego, restaurar el tamaño y la vista para el menú
            window.setSize(sf::Vector2u(800, 600));
            sf::View view = window.getDefaultView();
            view.setSize(800, 600);
            view.setCenter(400, 300);
            window.setView(view);

            // Intentar centrar la ventana en el monitor
            auto desktop = sf::VideoMode::getDesktopMode();
            window.setPosition(sf::Vector2i(desktop.width/2 - 400, desktop.height/2 - 300));

            // Sincronizar volumen hacia el menú al regresar
            menu.setVolume(juego.getMasterVolume());
        } else {
            // El usuario eligió salir -> cerrar la ventana y terminar
            window.close();
        }
    }

    return 0;
}