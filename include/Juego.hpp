#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Carta {
    sf::Sprite sprite;
    int id;                 
    bool revelada = false;
    bool emparejada = false;
};

class Juego {
public:
    void run(sf::RenderWindow& window);

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
};