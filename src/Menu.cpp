#include "Menu.hpp"
#include <iostream> // Para mensajes de error en consola
#include <algorithm>
#include <cctype>

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
    vsText.setFillColor(sf::Color::Yellow);
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
    inputNameText1.setOrigin(0,0);
    inputNameText1.setString(playerName1);

    inputNameText2.setFont(font);
    inputNameText2.setCharacterSize(28);
    inputNameText2.setFillColor(sf::Color::White);
    inputNameText2.setPosition(width/2.0f - 200, 300);
    inputNameText2.setOrigin(0,0);
    inputNameText2.setString(playerName2);

    startButtonText.setFont(font);
    startButtonText.setCharacterSize(32);
    startButtonText.setFillColor(sf::Color::Yellow);
    startButtonText.setString("COMENZAR");
    sf::FloatRect stb = startButtonText.getLocalBounds();
    startButtonText.setOrigin(stb.left + stb.width/2.0f, stb.top + stb.height/2.0f);
    startButtonText.setPosition(width/2.0f, 380);

    // Input boxes and labels
    inputBox1.setSize(sf::Vector2f(420.0f, 36.0f));
    inputBox1.setFillColor(sf::Color(0,0,0,80));
    inputBox1.setOutlineThickness(2);
    inputBox1.setOutlineColor(sf::Color::White);
    inputBox1.setPosition(width/2.0f - 200, 236);

    inputBox2.setSize(sf::Vector2f(420.0f, 36.0f));
    inputBox2.setFillColor(sf::Color(0,0,0,80));
    inputBox2.setOutlineThickness(2);
    inputBox2.setOutlineColor(sf::Color::White);
    inputBox2.setPosition(width/2.0f - 200, 296);

    labelPlayer1.setFont(font);
    labelPlayer1.setCharacterSize(24);
    labelPlayer1.setFillColor(sf::Color::Yellow);
    labelPlayer1.setString("Jugador 1:");
    labelPlayer1.setPosition(width/2.0f - 340, 240);

    labelPlayer2.setFont(font);
    labelPlayer2.setCharacterSize(24);
    labelPlayer2.setFillColor(sf::Color::Yellow);
    labelPlayer2.setString("Jugador 2:");
    labelPlayer2.setPosition(width/2.0f - 340, 300);

    // no decorative box for start button (user requested)


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
                            activeInput = 0;
                            caretClock.restart();
                            caretVisible = true;
                            continue;
                        }
                    } else {
                        // Nombre inputs visibles: detectar clicks en start button
                        if (startButtonText.getGlobalBounds().contains(x, y)) {
                            if (menuMusic.getStatus() == sf::Music::Playing) menuMusic.stop();
                            // copy input texts to playerName fields
                                std::string t1 = inputNameText1.getString();
                                std::string t2 = inputNameText2.getString();
                                if (t1.empty()) t1 = "Jugador 1";
                                if (t2.empty()) t2 = "Jugador 2";
                                playerName1 = t1;
                                playerName2 = t2;
                            return 1; // 1v1 comenzar con nombres
                        }
                        // Detectar clicks en cada campo para dar foco
                            sf::FloatRect r1 = inputBox1.getGlobalBounds();
                            sf::FloatRect r2 = inputBox2.getGlobalBounds();
                        if (r1.contains(x,y)) {
                            activeInput = 0; caretClock.restart(); caretVisible = true; continue;
                        } else if (r2.contains(x,y)) {
                            activeInput = 1; caretClock.restart(); caretVisible = true; continue;
                        }
                        // Si clic fuera del área de inputs, cerrar inputs
                            if (!r1.contains((float)x,(float)y) && !r2.contains((float)x,(float)y)) {
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
                // Backspace
                if (e.text.unicode == 8) {
                    if (activeInput == 0) {
                        std::string s1 = inputNameText1.getString();
                        if (!s1.empty()) s1.pop_back();
                        inputNameText1.setString(s1);
                    } else {
                        std::string s2 = inputNameText2.getString();
                        if (!s2.empty()) s2.pop_back();
                        inputNameText2.setString(s2);
                    }
                } else if (e.text.unicode < 128) {
                    char ch = static_cast<char>(e.text.unicode);
                    if (!isprint((unsigned char)ch)) continue;
                    if (activeInput == 0) {
                        std::string s1 = inputNameText1.getString();
                        if (s1.size() < 12) s1.push_back(ch);
                        inputNameText1.setString(s1);
                    } else {
                        std::string s2 = inputNameText2.getString();
                        if (s2.size() < 12) s2.push_back(ch);
                        inputNameText2.setString(s2);
                    }
                }
            }
        }

        // Actualizar parpadeo del caret
        if (showNameInputs) {
            float t = caretClock.getElapsedTime().asSeconds();
            int phase = (int)(t / caretInterval);
            caretVisible = (phase % 2) == 0;
        } else {
            caretVisible = false;
        }

        // Hover: cambiar color al pasar el ratón sobre opciones
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (!showSubmenu) {
            if (playText.getGlobalBounds().contains(mousePos)) playText.setFillColor(sf::Color::Blue); else playText.setFillColor(sf::Color::Yellow);
            if (exitText.getGlobalBounds().contains(mousePos)) exitText.setFillColor(sf::Color::Magenta); else exitText.setFillColor(sf::Color::Red);
        } else {
            if (!showNameInputs) {
                if (soloText.getGlobalBounds().contains(mousePos)) soloText.setFillColor(sf::Color::Blue); else soloText.setFillColor(sf::Color::Yellow);
                // vsText: amarillo por defecto, azul al hover
                if (vsText.getGlobalBounds().contains(mousePos)) vsText.setFillColor(sf::Color::Blue); else vsText.setFillColor(sf::Color::Yellow);
            } else {
                if (startButtonText.getGlobalBounds().contains(mousePos)) startButtonText.setFillColor(sf::Color::Blue); else startButtonText.setFillColor(sf::Color::Yellow);
                // resaltar cajas si se pasa por encima
                if (inputBox1.getGlobalBounds().contains(mousePos)) inputBox1.setOutlineColor(sf::Color::Cyan);
                else if (activeInput != 0) inputBox1.setOutlineColor(sf::Color::White);
                if (inputBox2.getGlobalBounds().contains(mousePos)) inputBox2.setOutlineColor(sf::Color::Cyan);
                else if (activeInput != 1) inputBox2.setOutlineColor(sf::Color::White);
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
                // Dibujar etiquetas, cajas e inputs
                window.draw(labelPlayer1);
                window.draw(labelPlayer2);
                // cajas
                // change outline color if focused
                if (activeInput == 0) inputBox1.setOutlineColor(sf::Color::Cyan); else inputBox1.setOutlineColor(sf::Color::White);
                if (activeInput == 1) inputBox2.setOutlineColor(sf::Color::Cyan); else inputBox2.setOutlineColor(sf::Color::White);
                window.draw(inputBox1);
                window.draw(inputBox2);
                // texto dentro de cajas (alineado con padding)
                inputNameText1.setPosition(inputBox1.getPosition().x + 8, inputBox1.getPosition().y + 2);
                inputNameText2.setPosition(inputBox2.getPosition().x + 8, inputBox2.getPosition().y + 2);
                window.draw(inputNameText1);
                window.draw(inputNameText2);

                // Dibujar caret en el campo activo
                if (caretVisible) {
                    if (activeInput == 0) {
                        sf::FloatRect b = inputNameText1.getGlobalBounds();
                        float cx = b.left + b.width + 6;
                        float cy = inputBox1.getPosition().y + 2;
                        sf::RectangleShape caret(sf::Vector2f(3, inputNameText1.getCharacterSize()+4));
                        caret.setPosition(cx, cy);
                        caret.setFillColor(sf::Color::White);
                        window.draw(caret);
                    } else {
                        sf::FloatRect b = inputNameText2.getGlobalBounds();
                        float cx = b.left + b.width + 6;
                        float cy = inputBox2.getPosition().y + 2;
                        sf::RectangleShape caret(sf::Vector2f(3, inputNameText2.getCharacterSize()+4));
                        caret.setPosition(cx, cy);
                        caret.setFillColor(sf::Color::White);
                        window.draw(caret);
                    }
                }

                // Dibujar start button box + texto
                // posicionar startButtonBox si es necesario
                    // No se dibuja startButtonBox
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