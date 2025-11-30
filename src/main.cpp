#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Juego.hpp"

int main() {
    // 1. Crear ventana inicial
    sf::RenderWindow window(sf::VideoMode(800, 600), "CardMind");
    window.setFramerateLimit(60);

    // Creamos las pantallas
    Menu menu(800, 600);
    Juego juego;

    // --- ESTE ES EL BUCLE INFINITO QUE FALTABA ---
    while (window.isOpen()) {
        // 1. Mostramos el Menú y esperamos a que el usuario elija
        int opcion = menu.run(window);

        if (opcion == 0 || opcion == 1 || opcion == 3) {
            // --- JUGAR ---
            // Sincronizar volumen del menú al juego
            juego.setMasterVolume(menu.getVolume());

            if (opcion == 3) {
                // Mostrar pantalla de victoria de prueba
                juego.showVictoryTest(window);
            } else {
                // Obtener nombres desde el menú si se ingresaron
                std::string n1 = menu.getPlayerName1();
                std::string n2 = menu.getPlayerName2();
                juego.run(window, opcion, n1, n2);
            }

            // --- AL VOLVER (Cuando juego.run termina) ---
            // Restauramos el tamaño de la ventana para el menú (800x600)
            window.setSize(sf::Vector2u(800, 600));

            // Ajustamos la "cámara" (View) para que no se vea estirado
            sf::View view = window.getDefaultView();
            view.setSize(800, 600);
            view.setCenter(400, 300);
            window.setView(view);

            // Centramos la ventana pequeña en el monitor
            auto desktop = sf::VideoMode::getDesktopMode();
            window.setPosition(sf::Vector2i(desktop.width/2 - 400, desktop.height/2 - 300));
            // Al volver del juego, sincronizar volumen del juego al menú
            menu.setVolume(juego.getMasterVolume());
        }
        else {
            // --- SALIR ---
            // Si en el menú eligieron salir, cerramos la ventana
            window.close();
        }
    }

    return 0;
}