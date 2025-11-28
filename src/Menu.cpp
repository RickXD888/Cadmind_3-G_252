#include "Menu.hpp"
#include <iostream> // Para mensajes de error en consola

Menu::Menu(float width, float height) {
    // ---------------------------------------------------------
    // 1. CONFIGURACIÓN DEL FONDO (Pixel Art)
    // ---------------------------------------------------------
    // Carga la imagen. Asegúrate de que exista "assets/fondo_pixel.png"
    if (!textureFondo.loadFromFile("assets/imagenes/menu/space-2.png")) {
        std::cout << "Error: No se pudo cargar assets/fondo_pixel.png" << std::endl;
        // Si falla, no crashea, solo se verá negro
    }

    // Asignamos la textura al sprite
    spriteFondo.setTexture(textureFondo);

    // ESCALADO: Esto estira la imagen para que cubra toda la ventana (width x height)
    sf::Vector2u size = textureFondo.getSize();
    float scaleX = width / size.x;
    float scaleY = height / size.y;
    spriteFondo.setScale(scaleX, scaleY);


    // ---------------------------------------------------------
    // 2. CONFIGURACIÓN DE TEXTOS (Centrados Perfectamente)
    // ---------------------------------------------------------
    if (!font.loadFromFile("assets/tipografia/MTCORSVA.ttf")) {
        std::cout << "Error: No se pudo cargar assets/arial.ttf" << std::endl;
    }

    // --- TITULO (CARDMIND) ---
    title.setFont(font);
    title.setString("CARDMIND");
    title.setCharacterSize(50); // Tamaño grande
    title.setFillColor(sf::Color::White);
    
    // Centrado Matemático:
    // 1. Obtenemos el rectángulo que envuelve al texto
    sf::FloatRect titleRect = title.getLocalBounds();
    // 2. Ponemos el "punto de origen" en el centro exacto del texto
    title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    // 3. Colocamos el texto en la mitad de la pantalla (width/2) y a una altura fija (80)
    title.setPosition(width / 2.0f, 80);


    // --- BOTÓN JUGAR ---
    playText.setFont(font);
    playText.setString("JUGAR");
    playText.setCharacterSize(40);
    playText.setFillColor(sf::Color::Yellow);

    sf::FloatRect playRect = playText.getLocalBounds();
    playText.setOrigin(playRect.left + playRect.width / 2.0f, playRect.top + playRect.height / 2.0f);
    playText.setPosition(width / 2.0f, 250); // Altura media


    // --- BOTÓN SALIR ---
    exitText.setFont(font);
    exitText.setString("SALIR");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::Red);

    sf::FloatRect exitRect = exitText.getLocalBounds();
    exitText.setOrigin(exitRect.left + exitRect.width / 2.0f, exitRect.top + exitRect.height / 2.0f);
    exitText.setPosition(width / 2.0f, 350); // Altura baja
}

int Menu::run(sf::RenderWindow& window) {
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::MouseButtonPressed) {
                // Obtenemos coordenadas del mouse
                auto x = sf::Mouse::getPosition(window).x;
                auto y = sf::Mouse::getPosition(window).y;

                // Detectar clics en los botones
                if (playText.getGlobalBounds().contains(x, y))
                    return 0; // Jugar

                if (exitText.getGlobalBounds().contains(x, y))
                    return 1; // Salir
            }
        }

        window.clear(sf::Color::Black);

        // --- DIBUJAR ---
        // 1. El fondo va primero para que quede atrás
        window.draw(spriteFondo);

        // 2. Los textos van encima
        window.draw(title);
        window.draw(playText);
        window.draw(exitText);

        window.display();
    }

    return 1; // Retorna salir si se cierra la ventana forzosamente
}