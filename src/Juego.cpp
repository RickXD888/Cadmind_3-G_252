#include "Juego.hpp"
#include <iostream>

void Juego::run(sf::RenderWindow& window) {
    // -------------------------------------------------
    // 1. CAMBIO DE RESOLUCIÓN (De 800x600 a 1280x720)
    // -------------------------------------------------
    // Cambiamos el tamaño físico de la ventana
    window.setSize(sf::Vector2u(1280, 720));
    
    // IMPORTANTE: Ajustamos la "cámara" (View) para que coincida con el nuevo tamaño.
    // Si no haces esto, SFML estirará la imagen de 800 a 1280 y se verá borroso.
    sf::View view = window.getDefaultView();
    view.setSize(1280, 720);
    view.setCenter(1280 / 2.0f, 720 / 2.0f);
    window.setView(view);

    // Opcional: Centrar la ventana en el monitor de nuevo (para que no se vaya de lado)
    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        desktop.width / 2 - 1280 / 2,
        desktop.height / 2 - 720 / 2
    ));

    // -------------------------------------------------
    // 2. CARGAR RECURSOS
    // -------------------------------------------------
    sf::Texture backgroundTexture;
    // Asegúrate de que la ruta sea correcta según tu carpeta
    if (!backgroundTexture.loadFromFile("assets/imagenes/backgrounds/Space Background_1280x720.png")) {
        std::cout << "ERROR: No se pudo cargar assets/backgrounds/space1.png" << std::endl;
        // Si falla, intentamos cargar el fondo del menú como respaldo o dejarlo vacío
        if (!backgroundTexture.loadFromFile("assets/fondo_pixel.png")) {
             std::cout << "ERROR CRITICO: No hay imagenes de fondo." << std::endl;
        }
    }

    sf::Sprite background(backgroundTexture);

    // Escalar el fondo exactamente a 1280x720
    sf::Vector2u bgSize = backgroundTexture.getSize();
    if (bgSize.x > 0) {
        float scaleX = 1280.0f / bgSize.x;
        float scaleY = 720.0f / bgSize.y;
        background.setScale(scaleX, scaleY);
    }

    // -------------------------------------------------
    // 3. CREAR EL TABLERO (Configuración Original Grande)
    // -------------------------------------------------
    const int rows = 3;
    const int cols = 4;
    const float cellWidth = 150;  // Tamaño original que querías
    const float cellHeight = 200; // Tamaño original que querías
    const float spacing = 20;

    std::vector<sf::RectangleShape> slots;

    // Calcular tamaño total del bloque de cartas
    float boardWidth = cols * cellWidth + (cols - 1) * spacing;
    float boardHeight = rows * cellHeight + (rows - 1) * spacing;

    // Centrar en la pantalla de 1280x720
    float startX = (1280 - boardWidth) / 2;
    float startY = (720 - boardHeight) / 2;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            sf::RectangleShape slot(sf::Vector2f(cellWidth, cellHeight));
            
            // Color blanco transparente (Alpha = 40)
            slot.setFillColor(sf::Color(255, 255, 255, 40)); 
            slot.setOutlineColor(sf::Color::White);
            slot.setOutlineThickness(2);

            float x = startX + c * (cellWidth + spacing);
            float y = startY + r * (cellHeight + spacing);
            slot.setPosition(x, y);

            slots.push_back(slot);
        }
    }

    // -------------------------------------------------
    // 4. BUCLE DEL JUEGO
    // -------------------------------------------------
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            // Tecla ESC para salir (Opcional)
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
                // Al salir, podrías querer regresar la ventana a 800x600 si vuelves al menú
                return; 
            }
        }

        window.clear(); // Limpia (negro por defecto)

        // Dibujar Fondo
        if (bgSize.x > 0) window.draw(background);

        // Dibujar Cartas
        for (auto& slot : slots) {
            window.draw(slot);
        }

        window.display();
    }
}