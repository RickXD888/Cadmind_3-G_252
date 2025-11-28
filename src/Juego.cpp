#include "Juego.hpp"
#include <iostream>
#include <algorithm> // Para std::shuffle
#include <random>    // Necesario para el generador moderno
#include <ctime>
#include <iomanip>   // Para formatear string
#include <sstream>   // Para stringstream

void Juego::run(sf::RenderWindow& window) {
    // -------------------------------------------------
    // 1. CONFIGURACION VENTANA (1920x1080)
    // -------------------------------------------------
    window.setSize(sf::Vector2u(1920, 1080));
    
    sf::View view = window.getDefaultView();
    view.setSize(1920, 1080);
    view.setCenter(1920 / 2.0f, 1080 / 2.0f);
    window.setView(view);

    auto desktop = sf::VideoMode::getDesktopMode();
    int posX = (desktop.width > 1920) ? (desktop.width/2 - 1920/2) : 0;
    int posY = (desktop.height > 1080) ? (desktop.height/2 - 1080/2) : 0;
    window.setPosition(sf::Vector2i(posX, posY));

    // -------------------------------------------------
    // 2. CARGAR RECURSOS Y UI
    // -------------------------------------------------
    
    // FUENTE
    if (!font.loadFromFile("assets/tipografia/MTCORSVA.ttf")) {
        std::cout << "ERROR: No se encontro assets/arial.ttf" << std::endl;
    }

    // CONFIGURAR TEXTOS UI
    // 1. Cronómetro en vivo (Esquina superior izquierda)
    textoTiempo.setFont(font);
    textoTiempo.setCharacterSize(40);
    textoTiempo.setFillColor(sf::Color::White);
    textoTiempo.setPosition(50, 30);
    textoTiempo.setString("Tiempo: 0.0s");

    // 2. Texto de Victoria
    textoVictoria.setFont(font);
    textoVictoria.setString("!FELICIDADES!");
    textoVictoria.setCharacterSize(100);
    textoVictoria.setFillColor(sf::Color::Green);
    // Centrar
    sf::FloatRect tr = textoVictoria.getLocalBounds();
    textoVictoria.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
    textoVictoria.setPosition(1920/2.0f, 1080/2.0f - 150);

    // 3. Texto Tiempo Final
    textoTiempoFinal.setFont(font);
    textoTiempoFinal.setCharacterSize(60);
    textoTiempoFinal.setFillColor(sf::Color::White);
    // Posición temporal (se ajustará al ganar)
    textoTiempoFinal.setPosition(1920/2.0f, 1080/2.0f);

    // 4. Botón Volver al Menú
    botonMenu.setFont(font);
    botonMenu.setString("VOLVER AL MENU");
    botonMenu.setCharacterSize(50);
    botonMenu.setFillColor(sf::Color::Yellow);
    sf::FloatRect br = botonMenu.getLocalBounds();
    botonMenu.setOrigin(br.left + br.width/2.0f, br.top + br.height/2.0f);
    botonMenu.setPosition(1920/2.0f, 1080/2.0f + 150);


    // CARGAR IMÁGENES
    if (!texturaFondo.loadFromFile("assets/imagenes/backgrounds/space-1.1.png")) {
        if (!texturaFondo.loadFromFile("assets/imagenes/menu/space-1.png")) {
             std::cout << "ERROR: Sin fondo." << std::endl;
        }
    }
    sf::Sprite spriteFondo(texturaFondo);
    if (texturaFondo.getSize().x > 0) {
        spriteFondo.setScale(1920.0f/texturaFondo.getSize().x, 1080.0f/texturaFondo.getSize().y);
    }

    if (!texturaReverso.loadFromFile("assets/imagenes/cards/baraja-1/back.png")) {
        sf::Image img; img.create(140, 190, sf::Color::Magenta);
        texturaReverso.loadFromImage(img);
    }
    texturaReverso.setSmooth(true);

    texturasCartas.clear();
    for (int i = 1; i <= 6; i++) {
        sf::Texture t;
        std::string ruta = "assets/imagenes/cards/baraja-1/1." + std::to_string(i) + ".png";
        if (t.loadFromFile(ruta)) {
            t.setSmooth(true); 
            texturasCartas.push_back(t);
        }
    }

    if (texturasCartas.empty()) return;

    // -------------------------------------------------
    // 3. INICIALIZAR LÓGICA
    // -------------------------------------------------
    cartas.clear();
    seleccionadas.clear();
    juegoIniciado = false;
    juegoTerminado = false;
    paresEncontrados = 0;
    bloqueado = false;

    for (int i = 0; i < texturasCartas.size(); i++) {
        for (int k = 0; k < 2; k++) {
            Carta c;
            c.id = i; 
            c.sprite.setTexture(texturaReverso); 
            cartas.push_back(c);
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartas.begin(), cartas.end(), g);

    // POSICIONAMIENTO
    const int cols = 4;
    const int rows = 3;
    const float spacing = 30.0f;
    
    float rawW = texturaReverso.getSize().x;
    float rawH = texturaReverso.getSize().y;
    if (rawH == 0) rawH = 100;

    // Cartas grandes para Full HD
    float maxCardHeight = 280.0f; 
    float scaleBase = (rawH > maxCardHeight) ? maxCardHeight / rawH : 1.0f;
    if (rawH < maxCardHeight) scaleBase = maxCardHeight / rawH;

    float targetWidth = rawW * scaleBase;
    float targetHeight = rawH * scaleBase;

    float totalW = cols * targetWidth + (cols - 1) * spacing;
    float totalH = rows * targetHeight + (rows - 1) * spacing;
    float startX = (1920 - totalW) / 2;
    float startY = (1080 - totalH) / 2;

    for (int i = 0; i < cartas.size(); i++) {
        int r = i / cols;
        int c = i % cols;
        float x = startX + c * (targetWidth + spacing);
        float y = startY + r * (targetHeight + spacing);
        
        cartas[i].sprite.setPosition(x, y);
        cartas[i].sprite.setScale(targetWidth / rawW, targetHeight / rawH);
    }

    // -------------------------------------------------
    // 5. BUCLE DE JUEGO
    // -------------------------------------------------
    while (window.isOpen()) {
        
        // --- ACTUALIZAR TIEMPO ---
        if (juegoIniciado && !juegoTerminado) {
            sf::Time tiempo = relojJuego.getElapsedTime();
            std::stringstream ss;
            ss << "Tiempo: " << std::fixed << std::setprecision(1) << tiempo.asSeconds() << "s";
            textoTiempo.setString(ss.str());
        }

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            
            // ESC para salir rápido
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) return;

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // --- SI EL JUEGO TERMINÓ: DETECTAR CLIC EN BOTÓN MENU ---
                if (juegoTerminado) {
                    if (botonMenu.getGlobalBounds().contains(mousePos)) {
                        return; // Sale de Juego::run y regresa al Menu
                    }
                    continue; // Ignorar clics en cartas
                }

                if (bloqueado) continue; 

                // --- LÓGICA DE CARTAS ---
                for (int i = 0; i < cartas.size(); i++) {
                    if (cartas[i].sprite.getGlobalBounds().contains(mousePos) && 
                        !cartas[i].revelada && !cartas[i].emparejada) {
                        
                        // INICIAR RELOJ AL PRIMER CLIC
                        if (!juegoIniciado) {
                            juegoIniciado = true;
                            relojJuego.restart();
                        }

                        // REVELAR CARTA
                        cartas[i].revelada = true;
                        if (cartas[i].id < texturasCartas.size()) {
                            sf::Texture& texNueva = texturasCartas[cartas[i].id];
                            cartas[i].sprite.setTexture(texNueva, true); // Reset recorte
                            float sX = targetWidth / texNueva.getSize().x;
                            float sY = targetHeight / texNueva.getSize().y;
                            cartas[i].sprite.setScale(sX, sY);
                        }
                        
                        seleccionadas.push_back(i);

                        if (seleccionadas.size() == 2) {
                            if (cartas[seleccionadas[0]].id == cartas[seleccionadas[1]].id) {
                                // MATCH
                                cartas[seleccionadas[0]].emparejada = true;
                                cartas[seleccionadas[1]].emparejada = true;
                                seleccionadas.clear();
                                paresEncontrados++;

                                // --- VERIFICAR VICTORIA ---
                                if (paresEncontrados == 6) { // 6 pares = 12 cartas
                                    juegoTerminado = true;
                                    sf::Time total = relojJuego.getElapsedTime();
                                    
                                    // Preparar texto final
                                    std::stringstream ss;
                                    ss << "Tardaste: " << std::fixed << std::setprecision(2) << total.asSeconds() << " segundos";
                                    textoTiempoFinal.setString(ss.str());
                                    
                                    // Centrar texto final
                                    sf::FloatRect fr = textoTiempoFinal.getLocalBounds();
                                    textoTiempoFinal.setOrigin(fr.left + fr.width/2.0f, fr.top + fr.height/2.0f);
                                    textoTiempoFinal.setPosition(1920/2.0f, 1080/2.0f);
                                }

                            } else {
                                // ERROR
                                bloqueado = true;
                                relojEspera.restart();
                            }
                        }
                    }
                }
            }
        }

        // --- MANEJO DE ERROR (VOLVER A OCULTAR) ---
        if (bloqueado && relojEspera.getElapsedTime().asSeconds() > 1.0f) {
            for (int index : seleccionadas) {
                cartas[index].revelada = false;
                cartas[index].sprite.setTexture(texturaReverso, true);
                float sX = targetWidth / texturaReverso.getSize().x;
                float sY = targetHeight / texturaReverso.getSize().y;
                cartas[index].sprite.setScale(sX, sY);
            }
            seleccionadas.clear();
            bloqueado = false; 
        }

        // --- EFECTO HOVER EN BOTON MENU (Solo si ganó) ---
        if (juegoTerminado) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (botonMenu.getGlobalBounds().contains(mousePos)) {
                botonMenu.setFillColor(sf::Color::Cyan);
            } else {
                botonMenu.setFillColor(sf::Color::Yellow);
            }
        }

        // --- DIBUJAR ---
        window.clear();
        
        if (texturaFondo.getSize().x > 0) window.draw(spriteFondo);
        
        // Dibujar cartas
        for (auto& c : cartas) {
            window.draw(c.sprite);
        }

        // Interfaz de Tiempo
        window.draw(textoTiempo);

        // Pantalla de Victoria
        if (juegoTerminado) {
            // Fondo oscuro semitransparente detrás del texto
            sf::RectangleShape overlay(sf::Vector2f(1920, 1080));
            overlay.setFillColor(sf::Color(0, 0, 0, 200)); // Negro transparente
            window.draw(overlay);

            window.draw(textoVictoria);
            window.draw(textoTiempoFinal);
            window.draw(botonMenu);
        }

        window.display();
    }
}