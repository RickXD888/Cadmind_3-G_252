/**
 * Juego.hpp
 * Declaración de la clase Juego.
 *
 * Esta clase implementa la lógica principal del juego de cartas (memoria),
 * soportando dos modos: solitario (12 cartas) y 1 vs 1 (24 cartas). Contiene
 * los miembros necesarios para manejar el tablero, el estado del juego,
 * la UI y el audio.
 *
 * Todas las descripciones de miembros y métodos están en español para facilitar
 * la lectura y mantenimiento por el equipo.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

struct Carta {
    sf::Sprite sprite;
    int id;                 
    bool revelada = false;
    bool emparejada = false;
};

class Juego {
public:
    /**
     * Ejecuta el bucle principal del juego.
     * @param window Ventana SFML donde dibujar y recibir eventos.
     * @param modo  0 = solitario (12 cartas), 1 = 1v1 (24 cartas)
     * @param name1 Nombre del jugador 1 (opcional, usado en 1v1)
     * @param name2 Nombre del jugador 2 (opcional, usado en 1v1)
     */
    void run(sf::RenderWindow& window, int modo = 0, const std::string& name1 = "", const std::string& name2 = "");

    /**
     * Muestra una pantalla de victoria de prueba (útil para testing desde el menú).
     */
    void showVictoryTest(sf::RenderWindow& window);

    // Control de volumen (para sincronizar con la UI del menú)
    void setMasterVolume(float v);
    float getMasterVolume() const;

private:
    // Recursos gráficos
    sf::Texture texturaFondo;
    sf::Texture texturaReverso;
    std::vector<sf::Texture> texturasCartas; // Texturas de las cartas por id

    // Estado del tablero
    std::vector<Carta> cartas;
    std::vector<int> seleccionadas; // índices de cartas seleccionadas

    // Mecanismo de bloqueo cuando se produce un fallo en el emparejado
    sf::Clock relojEspera;
    bool bloqueado = false;

    // --- UI y cronómetro ---
    sf::Font font;
    sf::Text textoTiempo;      // Muestra el tiempo en vivo (solitario)
    sf::Text textoVictoria;    // Mensaje de victoria
    sf::Text textoTiempoFinal; // Texto con tiempo final o conteos
    sf::Text botonMenu;        // Texto que actúa como botón "Volver al menú"

    sf::Clock relojJuego;      // Cronómetro del juego
    bool juegoIniciado = false;
    bool juegoTerminado = false;
    int paresEncontrados = 0;

    // --- Estado para 1v1 ---
    int modoJuego = 0; // 0 = solitario, 1 = 1v1
    int currentPlayer = 0;
    int score[2] = {0,0};
    // Miniaturas de pares encontrados por cada jugador (se dibujan en la UI)
    std::vector<sf::Sprite> pairedSprites[2];
    sf::Text textoJugador[2];
    sf::Text textoScore[2];
    sf::Text textoVol;

    float masterVolume = 100.0f; // Volumen maestro 0..100

    // Audio
    sf::SoundBuffer sonidoFlipBuffer;
    sf::SoundBuffer sonidoMatchBuffer;
    sf::SoundBuffer sonidoFailBuffer;
    sf::Sound sonidoFlip;
    sf::Sound sonidoMatch;
    sf::Sound sonidoFail;
    sf::Music gameMusic;

    // Imagen del trofeo para la pantalla de victoria
    sf::Texture trophyTexture;
    sf::Sprite trophySprite;
    // Selección de mazo (1 o 2)
    int selectedDeck = 1;
public:
    void setSelectedDeck(int d) { if (d==1||d==2) selectedDeck = d; }
};