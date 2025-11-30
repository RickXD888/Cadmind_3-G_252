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
    // modo: 0 = solitario (12 cartas), 1 = 1v1 (24 cartas)
    void run(sf::RenderWindow& window, int modo = 0, const std::string& name1 = "", const std::string& name2 = "");
    // Mostrar pantalla de victoria de ejemplo (para debug/testing)
    void showVictoryTest(sf::RenderWindow& window);

    // Volumen (permitir sincronizar con el menú)
    void setMasterVolume(float v);
    float getMasterVolume() const;

private:
    sf::Texture texturaFondo;
    sf::Texture texturaReverso;
    std::vector<sf::Texture> texturasCartas;

    std::vector<Carta> cartas;
    std::vector<int> seleccionadas;
    
    sf::Clock relojEspera;    // Para el error al fallar par
    bool bloqueado = false;   // Para no dar clic mientras espera

    // --- NUEVO: CRONÓMETRO Y UI ---
    sf::Font font;
    sf::Text textoTiempo;      // Muestra el tiempo en vivo
    sf::Text textoVictoria;    // "FELICIDADES"
    sf::Text textoTiempoFinal; // "Tardaste: XX segundos"
    sf::Text botonMenu;        // Botón para salir

    sf::Clock relojJuego;      // El cronómetro real
    bool juegoIniciado = false;
    bool juegoTerminado = false;
    int paresEncontrados = 0;

    // --- Modo 1v1 ---
    int modoJuego = 0; // 0 solitario, 1 1v1
    int currentPlayer = 0;
    int score[2] = {0,0};
    // Mantener un historial de pares por jugador (se muestran apilados)
    std::vector<sf::Sprite> pairedSprites[2];
    sf::Text textoJugador[2];
    sf::Text textoScore[2];
    sf::Text textoVol;
    float masterVolume = 100.0f; // 0..100
    // Placeholders for sounds (no files added yet)
    sf::SoundBuffer sonidoFlipBuffer;
    sf::SoundBuffer sonidoMatchBuffer;
    sf::SoundBuffer sonidoFailBuffer;
    sf::Sound sonidoFlip;
    sf::Sound sonidoMatch;
    sf::Sound sonidoFail;
    sf::Music gameMusic;
    // Trofeo para pantalla de victoria
    sf::Texture trophyTexture;
    sf::Sprite trophySprite;
};