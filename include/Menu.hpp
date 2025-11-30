#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Menu {
public:
    Menu(float width, float height);
    // 0 = solitario, 1 = 1v1, 2 = salir
    int run(sf::RenderWindow& window);

    // Volumen del menú
    float getVolume() const;
    void setVolume(float v);
    std::string getPlayerName1() const { return playerName1; }
    std::string getPlayerName2() const { return playerName2; }

private:
    sf::Texture textureFondo; // La imagen en memoria
    sf::Sprite spriteFondo;   // El objeto que dibuja la imagen
    sf::Font font;
    sf::Text title;
    sf::Text playText;
    sf::Text soloText; // SOLITARIO (submenu)
    sf::Text vsText;   // 1 VS 1 (submenu)
    sf::Text exitText;
    bool showSubmenu = false;
    sf::Music menuMusic;
    sf::Text textoVol;
    float menuVolume = 100.0f;
    // Nombre de jugadores para 1v1 (inputs del submenu)
    std::string playerName1;
    std::string playerName2;
    // Mostrar inputs para nombres
    bool showNameInputs = false;
    sf::Text inputNameText1;
    sf::Text inputNameText2;
    sf::Text startButtonText;

    
};
