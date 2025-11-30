#include "Menu.hpp"
#include <iostream> // Para mensajes de error en consola
#include <algorithm>

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


    // --- BOTÓN JUGAR (Menú principal) ---
    playText.setFont(font);
    playText.setString("JUGAR");
    playText.setCharacterSize(40);
    playText.setFillColor(sf::Color::Yellow);
    sf::FloatRect playRect = playText.getLocalBounds();
    playText.setOrigin(playRect.left + playRect.width / 2.0f, playRect.top + playRect.height / 2.0f);
    playText.setPosition(width / 2.0f, 250); // Altura media

    // --- SUBMENÚ (mismos estilos, se muestran al pulsar JUGAR) ---
    soloText.setFont(font);
    soloText.setString("SOLITARIO");
    soloText.setCharacterSize(36);
    soloText.setFillColor(sf::Color::Yellow);
    sf::FloatRect soloRect = soloText.getLocalBounds();
    soloText.setOrigin(soloRect.left + soloRect.width / 2.0f, soloRect.top + soloRect.height / 2.0f);
    soloText.setPosition(width / 2.0f, 240);

    vsText.setFont(font);
    vsText.setString("1 VS 1");
    vsText.setCharacterSize(36);
    vsText.setFillColor(sf::Color::Cyan);
    sf::FloatRect vsRect = vsText.getLocalBounds();
    vsText.setOrigin(vsRect.left + vsRect.width / 2.0f, vsRect.top + vsRect.height / 2.0f);
    vsText.setPosition(width / 2.0f, 300);

    // Inputs de nombre (inicializados vacíos)
    playerName1 = "Jugador 1";
    playerName2 = "Jugador 2";
    inputNameText1.setFont(font);
    inputNameText1.setCharacterSize(28);
    inputNameText1.setFillColor(sf::Color::White);
    inputNameText1.setPosition(width/2.0f - 200, 240);
    inputNameText1.setString(playerName1);

    inputNameText2.setFont(font);
    inputNameText2.setCharacterSize(28);
    inputNameText2.setFillColor(sf::Color::White);
    inputNameText2.setPosition(width/2.0f - 200, 300);
    inputNameText2.setString(playerName2);

    startButtonText.setFont(font);
    startButtonText.setCharacterSize(32);
    startButtonText.setFillColor(sf::Color::Yellow);
    startButtonText.setString("COMENZAR");
    sf::FloatRect stb = startButtonText.getLocalBounds();
    startButtonText.setOrigin(stb.left + stb.width/2.0f, stb.top + stb.height/2.0f);
    startButtonText.setPosition(width/2.0f, 380);


    // --- BOTÓN SALIR ---
    exitText.setFont(font);
    exitText.setString("SALIR");
    exitText.setCharacterSize(40);
    exitText.setFillColor(sf::Color::Red);

    sf::FloatRect exitRect = exitText.getLocalBounds();
    exitText.setOrigin(exitRect.left + exitRect.width / 2.0f, exitRect.top + exitRect.height / 2.0f);
    exitText.setPosition(width / 2.0f, 350); // Altura baja (menos separado)
}

int Menu::run(sf::RenderWindow& window) {
    // Iniciar música del menú
    if (!menuMusic.openFromFile("assets/music/calm-space-music-312291.mp3")) {
        std::cout << "Warning: no se pudo cargar assets/music/calm-space-music-312291.mp3" << std::endl;
    } else {
        menuMusic.setLoop(true);
        menuMusic.setVolume(menuVolume);
        menuMusic.play();
    }

    // Texto volumen (pequeño)
    textoVol.setFont(font);
    textoVol.setCharacterSize(18);
    textoVol.setFillColor(sf::Color::White);
    textoVol.setPosition(20, 560);
    textoVol.setString(std::string("Vol: ") + std::to_string((int)menuVolume) + "%");

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();

            if (e.type == sf::Event::MouseButtonPressed) {
                // Obtenemos coordenadas del mouse
                auto x = sf::Mouse::getPosition(window).x;
                auto y = sf::Mouse::getPosition(window).y;

                // Si está abierto el submenú, detectar selección ahí
                if (showSubmenu) {
                    if (!showNameInputs) {
                        if (soloText.getGlobalBounds().contains(x, y)) {
                            if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.stop();
                            return 0; // Solitario
                        }
                        if (vsText.getGlobalBounds().contains(x, y)) {
                            // Abrir inputs de nombres in-place
                            showNameInputs = true;
                            // reset default names for fresh input
                            playerName1 = "";
                            playerName2 = "";
                            inputNameText1.setString(playerName1);
                            inputNameText2.setString(playerName2);
                            continue;
                        }
                    } else {
                        // Nombre inputs visibles: detectar clicks en start button
                        if (startButtonText.getGlobalBounds().contains(x, y)) {
                            if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.stop();
                            // copy input texts to playerName fields
                            playerName1 = inputNameText1.getString();
                            playerName2 = inputNameText2.getString();
                            return 1; // 1v1 comenzar con nombres
                        }
                        // Si clic fuera del área de inputs, cerrar inputs
                        sf::FloatRect r1(inputNameText1.getPosition().x, inputNameText1.getPosition().y, 400, 40);
                        sf::FloatRect r2(inputNameText2.getPosition().x, inputNameText2.getPosition().y, 400, 40);
                        if (!r1.contains(x,y) && !r2.contains(x,y)) {
                            showNameInputs = false;
                            showSubmenu = false;
                        }
                    }
                } else {
                    // Detectar clics en el menú principal
                    if (playText.getGlobalBounds().contains(x, y)) {
                        showSubmenu = true; // Abrir submenú
                        continue;
                    }

                    if (exitText.getGlobalBounds().contains(x, y)) {
                        if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.stop();
                        return 2; // Salir
                    }
                }
            }

            // Control de volumen en el menú con flechas
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up) {
                menuVolume = std::min(100.0f, menuVolume + 5.0f);
                if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.setVolume(menuVolume);
                textoVol.setString(std::string("Vol: ") + std::to_string((int)menuVolume) + "%");
            }
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down) {
                menuVolume = std::max(0.0f, menuVolume - 5.0f);
                if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.setVolume(menuVolume);
                textoVol.setString(std::string("Vol: ") + std::to_string((int)menuVolume) + "%");
            }

            // Manejo de entrada de texto para nombres cuando están visibles
            if (showNameInputs && e.type == sf::Event::TextEntered) {
                if (e.text.unicode == 8) { // backspace: apply to second name if not empty else first
                    std::string s2 = inputNameText2.getString();
                    if (!s2.empty()) s2.pop_back();
                    else {
                        std::string s1 = inputNameText1.getString();
                        if (!s1.empty()) s1.pop_back();
                        inputNameText1.setString(s1);
                    }
                    inputNameText2.setString(inputNameText2.getString());
                } else if (e.text.unicode < 128) {
                    char ch = static_cast<char>(e.text.unicode);
                    // If first name length < 12 append there, else append to second
                    std::string s1 = inputNameText1.getString();
                    std::string s2 = inputNameText2.getString();
                    if (s1.size() < 12) s1.push_back(ch);
                    else if (s2.size() < 12) s2.push_back(ch);
                    inputNameText1.setString(s1);
                    inputNameText2.setString(s2);
                }
            }
        }

        window.clear(sf::Color::Black);

        // --- DIBUJAR ---
        // 1. El fondo va primero para que quede atrás
        window.draw(spriteFondo);

        // 2. Los textos van encima
        window.draw(title);
        // Si no hay submenú, dibujar menú principal
        if (!showSubmenu) {
            window.draw(playText);
            window.draw(exitText);
            window.draw(textoVol);
        } else {
            // Dibujar submenú con mismas posiciones y fondo
            if (!showNameInputs) {
                window.draw(soloText);
                window.draw(vsText);
            } else {
                // Dibujar inputs de nombres y botón comenzar
                window.draw(inputNameText1);
                window.draw(inputNameText2);
                window.draw(startButtonText);
            }
            window.draw(textoVol);
        }

        window.display();
    }

    return 1; // Retorna salir si se cierra la ventana forzosamente
}

float Menu::getVolume() const {
    return menuVolume;
}

void Menu::setVolume(float v) {
    if (v < 0.0f) v = 0.0f;
    if (v > 100.0f) v = 100.0f;
    menuVolume = v;
    if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.setVolume(menuVolume);
    // textoVol se actualiza en run() donde la fuente ya está inicializada
}