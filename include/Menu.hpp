/**
 * Menu.hpp
 * Declaración de la clase Menu que muestra la interfaz principal del juego,
 * permite seleccionar el modo (Solitario / 1v1), introducir nombres para 1v1
 * y controlar el volumen de la música.
 */
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Menu {
public:
    /**
     * Constructor del menú.
     * @param width  Ancho de la ventana esperada (para posicionamiento inicial)
     * @param height Alto de la ventana esperada
     */
    Menu(float width, float height);

    /**
     * Ejecuta la pantalla del menú en la ventana indicada. Bloquea hasta que
     * el usuario elija una opción.
     * @return 0 = SOLITARIO, 1 = 1v1, 2 = SALIR (o cierre de ventana)
     */
    int run(sf::RenderWindow& window);

    // Volumen del menú
    float getVolume() const;
    void setVolume(float v);

    // Lectura de nombres introducidos por el usuario en el submenú 1v1
    std::string getPlayerName1() const { return playerName1; }
    std::string getPlayerName2() const { return playerName2; }
    // Selección de mazo (1 = baraja-1, 2 = baraja-2)
    int getSelectedDeck() const { return selectedDeck; }

private:
    // Recursos gráficos del menú
    sf::Texture textureFondo; // Imagen de fondo cargada
    sf::Sprite spriteFondo;   // Sprite para dibujar el fondo
    sf::Font font;
    sf::Text title;
    sf::Text playText;
    sf::Text soloText; // Texto "SOLITARIO" en el submenú
    sf::Text vsText;   // Texto "1 VS 1" en el submenú
    sf::Text exitText;
    sf::Text backText; // Botón para volver desde el submenú
    bool showSubmenu = false;
    // Maestros / Deck selection
    sf::Text decksText; // Texto para abrir selector de mazos
    bool showDecksMenu = false;
    sf::Texture deck1Texture;
    sf::Texture deck2Texture;
    sf::Sprite deck1Sprite;
    sf::Sprite deck2Sprite;
    sf::Text deckBackText; // Botón volver dentro del selector de mazos
    int selectedDeck = 1;
    // (logo en el menú eliminado; la aplicación usa icono de ventana)
    sf::Music menuMusic;
    sf::Text textoVol; // Texto que muestra el volumen actual
    float menuVolume = 100.0f;

    // Nombres de jugadores y elementos de input del submenú 1v1
    std::string playerName1;
    std::string playerName2;
    bool showNameInputs = false;
    sf::Text inputNameText1;
    sf::Text inputNameText2;
    sf::Text startButtonText;

    // Estado de foco y caret para los inputs de texto
    int activeInput = 0; // 0 = primer campo, 1 = segundo campo
    sf::Clock caretClock; // controla parpadeo del caret
    bool caretVisible = false;
    float caretInterval = 0.5f; // intervalo de parpadeo en segundos

    // Cajas visuales y etiquetas asociadas a los inputs
    sf::RectangleShape inputBox1;
    sf::RectangleShape inputBox2;
    sf::Text labelPlayer1;
    sf::Text labelPlayer2;
};
