/**
 * Juego.cpp
 * Implementación de la lógica del juego de memoria (concentración).
 *
 * Este archivo contiene la inicialización de recursos (texturas, audio),
 * la creación y posicionamiento del tablero de cartas, la lógica de selección
 * y emparejado, y la pantalla de victoria. Está preparado para correr en una
 * ventana 1920x1080 dedicada durante la ejecución del juego.
 */

#include "Juego.hpp"
#include <iostream>
#include <algorithm> // Para std::shuffle
#include <random>    // Necesario para el generador moderno
#include <ctime>
#include <iomanip>   // Para formatear string
#include <sstream>   // Para stringstream

/**
 * run: Ejecuta el bucle principal del juego.
 * - Ajusta la ventana a 1920x1080 para la experiencia de juego.
 * - Carga recursos (fuente, texturas, sonidos, música).
 * - Inicializa el tablero y entra en el bucle de eventos y renderizado.
 *
 * Parámetros:
 * - window: ventana SFML donde se dibuja el juego
 * - modo: 0 = solitario (12 cartas), 1 = 1v1 (24 cartas)
 * - name1, name2: nombres opcionales para el modo 1v1
 */
void Juego::run(sf::RenderWindow& window, int modo, const std::string& name1, const std::string& name2) {
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

    // Guardar modo
    modoJuego = modo;

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

    // Texto volumen
    textoVol.setFont(font);
    textoVol.setCharacterSize(20);
    textoVol.setFillColor(sf::Color::White);
    textoVol.setPosition(50, 70);
    textoVol.setString("Vol: 100%");

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
    if (!texturaFondo.loadFromFile("assets/imagenes/backgrounds/Space Background_1920x1080.png")) {
        if (!texturaFondo.loadFromFile("assets/imagenes/menu/space-1.png")) {
             std::cout << "ERROR: Sin fondo." << std::endl;
        }
    }
    sf::Sprite spriteFondo(texturaFondo);
    if (texturaFondo.getSize().x > 0) {
        spriteFondo.setScale(1920.0f/texturaFondo.getSize().x, 1080.0f/texturaFondo.getSize().y);
    }

    // Cargar reverso dependiendo del mazo seleccionado
    bool loadedBack = false;
    if (selectedDeck == 1) {
        if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja-1/back.png")) loadedBack = true;
        else if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja-1/back1.1.png")) loadedBack = true;
    } else {
        // baraja 2: hay varios nombres (bakc1.1.png con tipografía errónea en el repo, back.png, etc.)
        if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja - 2/bakc1.1.png")) loadedBack = true;
        else if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja - 2/back1.1.png")) loadedBack = true;
        else if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja - 2/back.png")) loadedBack = true;
        else if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja-2/back.png")) loadedBack = true;
        else if (texturaReverso.loadFromFile("assets/imagenes/cards/baraja-2/bakc1.1.png")) loadedBack = true;
    }
    if (!loadedBack) {
        sf::Image img; img.create(140, 190, sf::Color::Magenta);
        texturaReverso.loadFromImage(img);
    }
    texturaReverso.setSmooth(true);

    // CARGAR SONIDOS
    if (!sonidoFlipBuffer.loadFromFile("assets/music/cartoon-flip_0mhhTzj.mp3")) {
        std::cout << "Warning: no se pudo cargar assets/music/cartoon-flip_0mhhTzj.mp3" << std::endl;
    } else {
        sonidoFlip.setBuffer(sonidoFlipBuffer);
        sonidoFlip.setVolume(masterVolume);
    }

    if (!sonidoMatchBuffer.loadFromFile("assets/music/papa-louie-customer-overjoyed.mp3")) {
        std::cout << "Warning: no se pudo cargar assets/music/papa-louie-customer-overjoyed.mp3" << std::endl;
    } else {
        sonidoMatch.setBuffer(sonidoMatchBuffer);
        sonidoMatch.setVolume(masterVolume);
    }

    if (!sonidoFailBuffer.loadFromFile("assets/music/solidworks-error-badunk.mp3")) {
        std::cout << "Warning: no se pudo cargar assets/music/solidworks-error-badunk.mp3" << std::endl;
    } else {
        sonidoFail.setBuffer(sonidoFailBuffer);
        sonidoFail.setVolume(masterVolume);
    }

    // Cargar música de juego
    if (!gameMusic.openFromFile("assets/music/interstellar-drip-ii-337275.mp3")) {
        std::cout << "Warning: no se pudo cargar assets/music/interstellar-drip-ii-337275.mp3" << std::endl;
    } else {
        gameMusic.setLoop(true);
        gameMusic.setVolume(masterVolume);
        gameMusic.play();
    }

    texturasCartas.clear();
    // Cargamos un conjunto básico de texturas según el mazo seleccionado.
    // Para la baraja 2 los nombres son 1.1.png .. 1.12.png en carpeta "baraja - 2".
    for (int i = 1; i <= 12; i++) {
        sf::Texture t;
        bool loaded = false;
        if (selectedDeck == 2) {
            // intentar varios patrones para baraja 2 (con espacio en nombre de carpeta)
            std::string r1 = "assets/imagenes/cards/baraja - 2/1." + std::to_string(i) + ".png";
            std::string r2 = "assets/imagenes/cards/baraja-2/1." + std::to_string(i) + ".png";
            std::string r3 = "assets/cards/baraja - 2/1." + std::to_string(i) + ".png";
            if (t.loadFromFile(r1) || t.loadFromFile(r2) || t.loadFromFile(r3)) {
                loaded = true;
            }
        } else {
            // baraja 1 suele tener nombres 1.png o 1.1.png
            std::string r1 = "assets/imagenes/cards/baraja-1/" + std::to_string(i) + ".png";
            std::string r2 = "assets/imagenes/cards/baraja-1/1." + std::to_string(i) + ".png";
            std::string r3 = "assets/cards/baraja-1/" + std::to_string(i) + ".png";
            std::string r4 = "assets/cards/baraja-1/1." + std::to_string(i) + ".png";
            if (t.loadFromFile(r1) || t.loadFromFile(r2) || t.loadFromFile(r3) || t.loadFromFile(r4)) {
                loaded = true;
            }
        }
        if (loaded) {
            t.setSmooth(true);
            texturasCartas.push_back(t);
        }
    }

    if (texturasCartas.empty()) return;

    // Cargar textura de trofeo (opcional)
    if (!trophyTexture.loadFromFile("assets/imagenes/trofeo.png")) {
        // no fatal, solo advertencia
        std::cout << "Warning: no se pudo cargar assets/imagenes/trofeo.jpg" << std::endl;
    } else {
        trophySprite.setTexture(trophyTexture);
        // escalar trofeo a un tamaño razonable
        float tw = trophyTexture.getSize().x;
        float th = trophyTexture.getSize().y;
        if (tw > 0 && th > 0) {
            float desired = 180.0f;
            float s = desired / std::max(tw, th);
            trophySprite.setScale(s, s);
        }
    }

    // -------------------------------------------------
    // 3. INICIALIZAR LÓGICA
    // -------------------------------------------------
    cartas.clear();
    seleccionadas.clear();
    juegoIniciado = false;
    juegoTerminado = false;
    paresEncontrados = 0;
    bloqueado = false;
    // Reiniciar estado 1v1
    score[0] = score[1] = 0;
    currentPlayer = 0;
    pairedSprites[0].clear();
    pairedSprites[1].clear();

    // Inicializa el tablero según el modo.
    // p1/p2 se pasan por si se quiere usar el nombre al inicializar (no obligatorio).
    auto initBoard = [&](const std::string& p1, const std::string& p2) {
        cartas.clear();
        seleccionadas.clear();
        juegoIniciado = false;
        juegoTerminado = false;
        paresEncontrados = 0;
        bloqueado = false;

        // Reiniciar 1v1
        score[0] = score[1] = 0;
        currentPlayer = 0;
        pairedSprites[0].clear();
        pairedSprites[1].clear();

        int paresObjetivo = (modoJuego == 1) ? 12 : 6;
        for (int i = 0; i < paresObjetivo; i++) {
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

        // POSICIONAMIENTO recalculado más abajo
    };

    // Initialize board with optional player names (not used here yet)
    initBoard("Jugador 1", "Jugador 2");

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartas.begin(), cartas.end(), g);

    // Variables accesibles para tamaño de cartas calculado
    float currentTargetWidth = 0.0f;
    float currentTargetHeight = 0.0f;

    // POSICIONAMIENTO: lambda para (re)posicionar y escalar las cartas
    // según el número de columnas/filas esperado para el modo.
    auto positionBoard = [&]() {
        int cols = 4;
        int rows = 3;
        if (modoJuego == 1) {
            cols = 6;
            rows = 4;
        }
        const float spacing = 30.0f;

        float rawW = texturaReverso.getSize().x;
        float rawH = texturaReverso.getSize().y;
        if (rawH == 0) rawH = 100;

        float uiReserveTop = 160.0f;
        float uiReserveBottom = 120.0f;
        float availableHeight = 1080.0f - uiReserveTop - uiReserveBottom;
        float targetHeight = (availableHeight - (rows - 1) * spacing) / rows;
        float maxCardHeight = (modoJuego == 1) ? targetHeight : std::min(280.0f, targetHeight);

        float scaleBase = (rawH > 0) ? (maxCardHeight / rawH) : 1.0f;
        if (scaleBase <= 0) scaleBase = 1.0f;

        float targetWidth = rawW * scaleBase;
        float totalW = cols * targetWidth + (cols - 1) * spacing;
        float maxAvailableW = 1920.0f - 100.0f;
        if (totalW > maxAvailableW && totalW > 0) {
            float shrink = maxAvailableW / totalW;
            targetWidth *= shrink;
            maxCardHeight *= shrink;
        }

        float targetHeightFinal = maxCardHeight;
        // actualizar variables externas para uso al revelar/ocultar cartas
        currentTargetWidth = targetWidth;
        currentTargetHeight = targetHeightFinal;
        float totalH = rows * targetHeightFinal + (rows - 1) * spacing;
        float startX = (1920 - (cols * targetWidth + (cols - 1) * spacing)) / 2;
        float startY = (1080 - totalH) / 2;

        for (int i = 0; i < cartas.size(); i++) {
            int r = i / cols;
            int c = i % cols;
            float x = startX + c * (targetWidth + spacing);
            float y = startY + r * (targetHeightFinal + spacing);
            cartas[i].sprite.setPosition(x, y);
            cartas[i].sprite.setScale(targetWidth / rawW, targetHeightFinal / rawH);
        }
    };

    // Posicionar cartas inicialmente
    positionBoard();

    // Prepare player names from parameters
    std::string playerNames[2] = {"Jugador 1", "Jugador 2"};
    if (!name1.empty()) playerNames[0] = name1;
    if (!name2.empty()) playerNames[1] = name2;

    // Botón de 'Jugar de nuevo' local
    sf::Text botonReiniciar;
    botonReiniciar.setFont(font);
    botonReiniciar.setString("JUGAR DE NUEVO");
    botonReiniciar.setCharacterSize(40);
    botonReiniciar.setFillColor(sf::Color::Yellow);
    sf::FloatRect rr = botonReiniciar.getLocalBounds();
    botonReiniciar.setOrigin(rr.left + rr.width/2.0f, rr.top + rr.height/2.0f);
    botonReiniciar.setPosition(1920/2.0f, 680);

    // --- Selector de mazo en juego (toggle con tecla D) ---
    bool showDeckSelectorInGame = false;
    sf::Texture deckThumb1Tex, deckThumb2Tex;
    sf::Sprite deckThumb1Spr, deckThumb2Spr;
    // intentar cargar miniaturas sencillas
    if (!deckThumb1Tex.loadFromFile("assets/imagenes/cards/baraja-1/1.png")) {
        // fallback
        deckThumb1Tex = texturasCartas.empty() ? sf::Texture() : texturasCartas[0];
    }
    if (!deckThumb2Tex.loadFromFile("assets/imagenes/cards/baraja - 2/1.1.png")) {
        deckThumb2Tex = deckThumb1Tex;
    }
    deckThumb1Spr.setTexture(deckThumb1Tex);
    deckThumb2Spr.setTexture(deckThumb2Tex);

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
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
                if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.stop();
                return;
            }

            // Control de volumen: Flecha arriba / Flecha abajo
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Up) {
                masterVolume = std::min(100.0f, masterVolume + 5.0f);
                textoVol.setString(std::string("Vol: ") + std::to_string((int)masterVolume) + "%");
                sonidoFlip.setVolume(masterVolume);
                sonidoMatch.setVolume(masterVolume);
                sonidoFail.setVolume(masterVolume);
                if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.setVolume(masterVolume);
            }
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Down) {
                masterVolume = std::max(0.0f, masterVolume - 5.0f);
                textoVol.setString(std::string("Vol: ") + std::to_string((int)masterVolume) + "%");
                sonidoFlip.setVolume(masterVolume);
                sonidoMatch.setVolume(masterVolume);
                sonidoFail.setVolume(masterVolume);
                if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.setVolume(masterVolume);
            }

            // Tecla D para abrir selector de mazo en partida
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::D) {
                showDeckSelectorInGame = !showDeckSelectorInGame;
                // si abrimos, pausar música y mostrar overlays; si cerramos, continuar
                if (showDeckSelectorInGame) {
                    if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.pause();
                } else {
                    if (gameMusic.getStatus() == sf::Music::Paused) gameMusic.play();
                }
            }

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Si el selector de mazos en partida está activo, procesar clicks aquí primero
                if (showDeckSelectorInGame) {
                    if (deckThumb1Spr.getGlobalBounds().contains(mousePos)) {
                        setSelectedDeck(1);
                        initBoard(playerNames[0], playerNames[1]);
                        positionBoard();
                        showDeckSelectorInGame = false;
                        if (gameMusic.getStatus() == sf::Music::Paused) gameMusic.play();
                        continue;
                    } else if (deckThumb2Spr.getGlobalBounds().contains(mousePos)) {
                        setSelectedDeck(2);
                        initBoard(playerNames[0], playerNames[1]);
                        positionBoard();
                        showDeckSelectorInGame = false;
                        if (gameMusic.getStatus() == sf::Music::Paused) gameMusic.play();
                        continue;
                    } else {
                        // clic fuera: cerrar selector
                        showDeckSelectorInGame = false;
                        if (gameMusic.getStatus() == sf::Music::Paused) gameMusic.play();
                        continue;
                    }
                }

                // --- SI EL JUEGO TERMINÓ: DETECTAR CLIC EN BOTONES ---
                if (juegoTerminado) {
                    if (botonReiniciar.getGlobalBounds().contains(mousePos)) {
                        // Reiniciar tablero y reposicionar cartas
                        initBoard(playerNames[0], playerNames[1]);
                        positionBoard();
                        // continuar el bucle sin salir
                        continue;
                    }

                    if (botonMenu.getGlobalBounds().contains(mousePos)) {
                        if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.stop();
                        return; // Sale de Juego::run y regresa al Menu
                    }
                    continue; // Ignorar otros clics en cartas
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
                        // reproducir sonido de flip
                        if (sonidoFlip.getBuffer() != nullptr) sonidoFlip.play();
                        if (cartas[i].id < texturasCartas.size()) {
                            sf::Texture& texNueva = texturasCartas[cartas[i].id];
                            cartas[i].sprite.setTexture(texNueva, true); // Reset recorte
                            float sX = currentTargetWidth / texNueva.getSize().x;
                            float sY = currentTargetHeight / texNueva.getSize().y;
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

                                if (modoJuego == 1) {
                                    // Dar punto al jugador actual y guardar la imagen del par en su historial
                                    score[currentPlayer]++;
                                    int matchedId = cartas[i].id;
                                    if (matchedId >= 0 && matchedId < texturasCartas.size()) {
                                        sf::Sprite s;
                                        s.setTexture(texturasCartas[matchedId]);
                                        // escalar a miniatura fija
                                        float desiredW = 80.0f;
                                        float desiredH = 110.0f;
                                        float tw = texturasCartas[matchedId].getSize().x;
                                        float th = texturasCartas[matchedId].getSize().y;
                                        float scaleX = (tw>0)?(desiredW / tw):1.0f;
                                        float scaleY = (th>0)?(desiredH / th):1.0f;
                                        s.setScale(scaleX, scaleY);
                                        pairedSprites[currentPlayer].push_back(s);
                                    }
                                }

                                // reproducir sonido de match (cada vez que se hace un par)
                                if (sonidoMatch.getBuffer() != nullptr) sonidoMatch.play();

                                // --- VERIFICAR VICTORIA ---
                                int objetivo = (modoJuego == 1) ? 12 : 6;
                                if (paresEncontrados == objetivo) {
                                    juegoTerminado = true;
                                    if (modoJuego == 0) {
                                        sf::Time total = relojJuego.getElapsedTime();
                                        std::stringstream ss;
                                        ss << "Tardaste: " << std::fixed << std::setprecision(2) << total.asSeconds() << " segundos";
                                        textoTiempoFinal.setString(ss.str());
                                        sf::FloatRect fr = textoTiempoFinal.getLocalBounds();
                                        textoTiempoFinal.setOrigin(fr.left + fr.width/2.0f, fr.top + fr.height/2.0f);
                                        textoTiempoFinal.setPosition(1920/2.0f, 1080/2.0f);
                                    }
                                    // reproducir sonido de victoria en general
                                    if (sonidoMatch.getBuffer() != nullptr) sonidoMatch.play();
                                }

                            } else {
                                // ERROR
                                bloqueado = true;
                                relojEspera.restart();
                                // reproducir sonido de fallo
                                if (sonidoFail.getBuffer() != nullptr) sonidoFail.play();

                                // En 1v1, si falla, se pasará el turno al otro jugador
                                if (modoJuego == 1) {
                                    // la lógica de pase ocurre cuando se oculten de nuevo (en la sección de bloqueo)
                                }
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
                float sX = currentTargetWidth / texturaReverso.getSize().x;
                float sY = currentTargetHeight / texturaReverso.getSize().y;
                cartas[index].sprite.setScale(sX, sY);
            }
            seleccionadas.clear();
            bloqueado = false; 

            // Si en modo 1v1 ocurrió un fallo, pasar turno
            if (modoJuego == 1) {
                currentPlayer = 1 - currentPlayer;
            }
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

        // Interfaz de Tiempo (solo en solitario)
        if (modoJuego == 0) window.draw(textoTiempo);
        // Volumen
        window.draw(textoVol);

        // UI 1v1: nombres, scores y últimas cartas emparejadas
        if (modoJuego == 1) {
            // Nombres (usar nombres provistos desde el menú)
            textoJugador[0].setFont(font);
            textoJugador[0].setString(playerNames[0]);
            textoJugador[0].setCharacterSize(30);
            textoJugador[0].setFillColor((currentPlayer==0)?sf::Color::Yellow:sf::Color::White);
            textoJugador[0].setPosition(50, 10);

            textoJugador[1].setFont(font);
            textoJugador[1].setString(playerNames[1]);
            textoJugador[1].setCharacterSize(30);
            textoJugador[1].setFillColor((currentPlayer==1)?sf::Color::Yellow:sf::Color::White);
            textoJugador[1].setPosition(1920 - 200, 10);

            textoScore[0].setFont(font);
            textoScore[0].setString(std::to_string(score[0]));
            textoScore[0].setCharacterSize(24);
            textoScore[0].setFillColor(sf::Color::White);
            textoScore[0].setPosition(50, 45);

            textoScore[1].setFont(font);
            textoScore[1].setString(std::to_string(score[1]));
            textoScore[1].setCharacterSize(24);
            textoScore[1].setFillColor(sf::Color::White);
            textoScore[1].setPosition(1920 - 200, 45);

            window.draw(textoJugador[0]);
            window.draw(textoJugador[1]);
            window.draw(textoScore[0]);
            window.draw(textoScore[1]);

            // Mostrar historial de pares por jugador en grid 7 filas x 2 columnas
            const float desiredW = 80.0f;
            const float desiredH = 110.0f;
            const float gap = 8.0f;
            const int colsGrid = 2;
            const int rowsGrid = 7;
            float baseX0 = 50.0f;
            float baseY = 80.0f;

            for (size_t j = 0; j < pairedSprites[0].size(); ++j) {
                int col = j % colsGrid;
                int row = j / colsGrid;
                if (row >= rowsGrid) break; // no mostrar más de 7 filas
                float x = baseX0 + col * (desiredW + gap);
                float y = baseY + row * (desiredH + gap);
                pairedSprites[0][j].setPosition(x, y);
                window.draw(pairedSprites[0][j]);
            }

            // Jugador 2 a la derecha
            float baseX1 = 1920.0f - 200.0f - (colsGrid * desiredW + (colsGrid - 1) * gap);
            for (size_t j = 0; j < pairedSprites[1].size(); ++j) {
                int col = j % colsGrid;
                int row = j / colsGrid;
                if (row >= rowsGrid) break;
                float x = baseX1 + col * (desiredW + gap);
                float y = baseY + row * (desiredH + gap);
                pairedSprites[1][j].setPosition(x, y);
                window.draw(pairedSprites[1][j]);
            }
        }

        // Pantalla de Victoria
        if (juegoTerminado) {
            // Fondo oscuro semitransparente detrás del texto
            sf::RectangleShape overlay(sf::Vector2f(1920, 1080));
            overlay.setFillColor(sf::Color(0, 0, 0, 200)); // Negro transparente
            window.draw(overlay);

            if (modoJuego == 1) {
                // Mostrar ganador, conteo de pares y trofeo centrados verticalmente
                // Determinar ganador usando nombres reales
                std::string victoriaStr;
                if (score[0] > score[1]) victoriaStr = std::string("GANA: ") + playerNames[0];
                else if (score[1] > score[0]) victoriaStr = std::string("GANA: ") + playerNames[1];
                else victoriaStr = "EMPATE";

                // Título de victoria
                textoVictoria.setString(victoriaStr);
                sf::FloatRect vr = textoVictoria.getLocalBounds();
                textoVictoria.setOrigin(vr.left + vr.width/2.0f, vr.top + vr.height/2.0f);
                textoVictoria.setPosition(1920/2.0f, 220.0f);
                window.draw(textoVictoria);

                // Conteo de pares en una sola línea
                std::stringstream ss;
                ss << playerNames[0] << ": " << score[0] << " pares   -   " << playerNames[1] << ": " << score[1] << " pares";
                textoTiempoFinal.setCharacterSize(40);
                textoTiempoFinal.setString(ss.str());
                sf::FloatRect fr = textoTiempoFinal.getLocalBounds();
                textoTiempoFinal.setOrigin(fr.left + fr.width/2.0f, fr.top + fr.height/2.0f);
                textoTiempoFinal.setPosition(1920/2.0f, 320.0f);
                window.draw(textoTiempoFinal);

                // Mostrar trofeo centrado debajo de los textos con separación
                float trophyY = 400.0f;
                if (trophyTexture.getSize().x > 0) {
                    sf::FloatRect tb = trophySprite.getGlobalBounds();
                    float trophyX = 1920/2.0f - tb.width/2.0f;
                    trophySprite.setPosition(trophyX, trophyY);
                    window.draw(trophySprite);
                }

                // Botón 'Jugar de nuevo' y 'Volver' debajo del trofeo con separación
                botonReiniciar.setPosition(1920/2.0f, trophyY + 220.0f);
                // Dibujar un fondo/halo alrededor del botón y efecto hover
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                sf::FloatRect localBounds = botonReiniciar.getLocalBounds();
                float bw = localBounds.width;
                float bh = localBounds.height;
                sf::RectangleShape highlight(sf::Vector2f(bw + 24.0f, bh + 16.0f));
                highlight.setOrigin((bw + 24.0f)/2.0f, (bh + 16.0f)/2.0f);
                highlight.setPosition(botonReiniciar.getPosition());
                highlight.setFillColor(sf::Color(0,0,0,150));
                highlight.setOutlineThickness(2.0f);
                if (botonReiniciar.getGlobalBounds().contains(mousePos)) {
                    highlight.setOutlineColor(sf::Color::Blue);
                    botonReiniciar.setFillColor(sf::Color::Blue);
                } else {
                    highlight.setOutlineColor(sf::Color::Yellow);
                    botonReiniciar.setFillColor(sf::Color::Yellow);
                }
                window.draw(highlight);
                window.draw(botonReiniciar);
                botonMenu.setPosition(1920/2.0f, trophyY + 300.0f);
                window.draw(botonMenu);
            } else {
                // Solitario: separar textos y botón igual que en 1v1
                textoVictoria.setPosition(1920/2.0f, 220.0f);
                window.draw(textoVictoria);
                textoTiempoFinal.setPosition(1920/2.0f, 300.0f);
                window.draw(textoTiempoFinal);
                botonMenu.setPosition(1920/2.0f, 620);
                window.draw(botonMenu);
            }
        }

        // Dibujar selector de mazos en partida si está activo
        if (showDeckSelectorInGame) {
            sf::RectangleShape overlay(sf::Vector2f(1920, 1080));
            overlay.setFillColor(sf::Color(0,0,0,200));
            window.draw(overlay);

            // calcular escala para thumbs (hacer mazo 1 más pequeño)
            float targetH1 = 300.0f; // reducir mazo 1
            if (deckThumb1Tex.getSize().y > 0) {
                float s1 = targetH1 / deckThumb1Tex.getSize().y;
                deckThumb1Spr.setScale(s1, s1);
            }
            if (deckThumb2Tex.getSize().y > 0) {
                float s2 = (deckThumb1Spr.getGlobalBounds().height > 0) ? (deckThumb1Spr.getGlobalBounds().height / deckThumb2Tex.getSize().y) : 2.0f;
                deckThumb2Spr.setScale(s2, s2);
            }
            // positions
            float centerX = 1920/2.0f;
            float spacingInGame = 120.0f;
            deckThumb1Spr.setPosition(centerX - deckThumb1Spr.getGlobalBounds().width - spacingInGame, 280);
            deckThumb2Spr.setPosition(centerX + spacingInGame, 280);
            // draw
            window.draw(deckThumb1Spr);
            window.draw(deckThumb2Spr);

            // labels
            sf::Text lbl1("Mazo 1", font); lbl1.setCharacterSize(32); lbl1.setFillColor(sf::Color::White);
            sf::FloatRect l1b = lbl1.getLocalBounds(); lbl1.setOrigin(l1b.left + l1b.width/2.0f, l1b.top + l1b.height/2.0f);
            lbl1.setPosition(deckThumb1Spr.getPosition().x + deckThumb1Spr.getGlobalBounds().width/2.0f, deckThumb1Spr.getPosition().y + deckThumb1Spr.getGlobalBounds().height + 30);
            sf::Text lbl2("Mazo 2", font); lbl2.setCharacterSize(32); lbl2.setFillColor(sf::Color::White);
            sf::FloatRect l2b = lbl2.getLocalBounds(); lbl2.setOrigin(l2b.left + l2b.width/2.0f, l2b.top + l2b.height/2.0f);
            lbl2.setPosition(deckThumb2Spr.getPosition().x + deckThumb2Spr.getGlobalBounds().width/2.0f, deckThumb2Spr.getPosition().y + deckThumb2Spr.getGlobalBounds().height + 30);
            window.draw(lbl1);
            window.draw(lbl2);
        }

        window.display();
    }
}

    void Juego::setMasterVolume(float v) {
        if (v < 0.0f) v = 0.0f;
        if (v > 100.0f) v = 100.0f;
        masterVolume = v;
        if (gameMusic.getStatus() == sf::Music::Playing) gameMusic.setVolume(masterVolume);
        if (sonidoFlip.getBuffer() != nullptr) sonidoFlip.setVolume(masterVolume);
        if (sonidoMatch.getBuffer() != nullptr) sonidoMatch.setVolume(masterVolume);
        if (sonidoFail.getBuffer() != nullptr) sonidoFail.setVolume(masterVolume);
    }

    float Juego::getMasterVolume() const {
        return masterVolume;
    }

    void Juego::showVictoryTest(sf::RenderWindow& window) {
        // Asegurar tamaño/cámara
        window.setSize(sf::Vector2u(1920, 1080));
        sf::View view = window.getDefaultView();
        view.setSize(1920,1080); view.setCenter(1920/2.0f, 1080/2.0f); window.setView(view);

        // Cargar recursos mínimos (fuente y trofeo)
        sf::Font testFont;
        if (!testFont.loadFromFile("assets/tipografia/MTCORSVA.ttf")) {
            // si falla, usar font miembro (si existe)
            testFont = font;
        }

        sf::Texture testTrofeo;
        sf::Sprite testSprite;
        if (testTrofeo.loadFromFile("assets/imagenes/trofeo.jpg")) {
            testSprite.setTexture(testTrofeo);
            float s = 180.0f / std::max(testTrofeo.getSize().x, testTrofeo.getSize().y);
            testSprite.setScale(s,s);
        }

        sf::Text title("GANA: Jugador 2", testFont);
        title.setCharacterSize(100); title.setFillColor(sf::Color::Green);
        sf::FloatRect tr = title.getLocalBounds(); title.setOrigin(tr.left + tr.width/2.0f, tr.top + tr.height/2.0f);
        title.setPosition(1920/2.0f, 220.0f);

        sf::Text counts("", testFont);
        counts.setCharacterSize(40); counts.setFillColor(sf::Color::White);
        counts.setString("Jugador 1: 4 pares   -   Jugador 2: 8 pares");
        sf::FloatRect cr = counts.getLocalBounds(); counts.setOrigin(cr.left + cr.width/2.0f, cr.top + cr.height/2.0f);
        counts.setPosition(1920/2.0f, 320.0f);

        sf::Text volver("VOLVER AL MENU", testFont);
        volver.setCharacterSize(50); volver.setFillColor(sf::Color::Yellow);
        sf::FloatRect br = volver.getLocalBounds(); volver.setOrigin(br.left + br.width/2.0f, br.top + br.height/2.0f);
        volver.setPosition(1920/2.0f, 620.0f);

        // Bucle simple hasta click en volver o cerrar
        while (window.isOpen()) {
            sf::Event e;
            while (window.pollEvent(e)) {
                if (e.type == sf::Event::Closed) return;
                if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (volver.getGlobalBounds().contains(mp)) return;
                }
            }

            

            window.clear();
            // background dim
            sf::RectangleShape overlay(sf::Vector2f(1920,1080)); overlay.setFillColor(sf::Color(0,0,0,120));
            window.draw(overlay);
            window.draw(title);
            window.draw(counts);
            if (testTrofeo.getSize().x > 0) {
                sf::FloatRect tb = testSprite.getGlobalBounds(); testSprite.setPosition(1920/2.0f - tb.width/2.0f, 400.0f);
                window.draw(testSprite);
            }
            window.draw(volver);
            window.display();
        }
    }

    