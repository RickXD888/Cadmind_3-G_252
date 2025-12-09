/**
 * main.cpp
 * Punto de entrada de la aplicación CardMind.
 *
 * Este fichero crea la ventana SFML, instancia las pantallas `Menu` y `Juego`
 * y ejecuta el bucle principal que alterna entre mostrar el menú y ejecutar
 * el juego según la elección del usuario.
 */

#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "Juego.hpp"
#include <iostream>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

int main() {
    // Crear ventana inicial para el menú (tamaño reducido por comodidad)
    sf::RenderWindow window(sf::VideoMode(800, 600), "CardMind");
    window.setFramerateLimit(60);

    // Cargar y establecer icono de la ventana (usar sólo PNG en tiempo de ejecución)
    sf::Image windowIcon;
    bool loadedIcon = false;
    const char* candidates[] = {
        "assets/imagenes/menu/logo.png",
        "assets/images/logo.png",
        nullptr
    };

    for (int i = 0; candidates[i] != nullptr; ++i) {
        if (windowIcon.loadFromFile(candidates[i])) {
            window.setIcon(windowIcon.getSize().x, windowIcon.getSize().y, windowIcon.getPixelsPtr());
            std::cout << "Icono PNG cargado: " << candidates[i] << std::endl;
            loadedIcon = true;
            break;
        }
    }

    if (!loadedIcon) {
        std::cout << "No se pudo cargar el icono PNG (buscando logo.png en assets)." << std::endl;
    }

#if defined(_WIN32)
    // Además de setIcon de SFML, también creamos un HICON y lo aplicamos
    // vía Win32 para aumentar la probabilidad de que Windows muestre
    // el icono en la barra de tareas (explorer). Esto es opcional y
    // sólo se ejecuta en Windows.
    if (loadedIcon) {
        HWND hwnd = (HWND)window.getSystemHandle();
        unsigned int w = windowIcon.getSize().x;
        unsigned int h = windowIcon.getSize().y;

        // Crear DIB seccion 32bpp top-down
        BITMAPV5HEADER bi;
        ZeroMemory(&bi, sizeof(bi));
        bi.bV5Size = sizeof(BITMAPV5HEADER);
        bi.bV5Width = (LONG)w;
        bi.bV5Height = -(LONG)h; // top-down
        bi.bV5Planes = 1;
        bi.bV5BitCount = 32;
        bi.bV5Compression = BI_BITFIELDS;
        bi.bV5RedMask   = 0x00FF0000;
        bi.bV5GreenMask = 0x0000FF00;
        bi.bV5BlueMask  = 0x000000FF;

        void* pvBits = nullptr;
        HDC hdc = GetDC(NULL);
        HBITMAP hbm = CreateDIBSection(hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &pvBits, NULL, 0);
        ReleaseDC(NULL, hdc);

        if (hbm && pvBits) {
            // Copiar pixels (sf::Image usa RGBA)
            const sf::Uint8* src = windowIcon.getPixelsPtr();
            BYTE* dst = (BYTE*)pvBits;
            for (unsigned int yy = 0; yy < h; ++yy) {
                for (unsigned int xx = 0; xx < w; ++xx) {
                    size_t i = (yy * w + xx) * 4;
                    // BGRA
                    dst[i + 0] = src[i + 2]; // B
                    dst[i + 1] = src[i + 1]; // G
                    dst[i + 2] = src[i + 0]; // R
                    dst[i + 3] = src[i + 3]; // A
                }
            }

            // Crear máscara monocroma (todo 0 -> usa canal alpha)
            HBITMAP hbmMask = CreateBitmap((int)w, (int)h, 1, 1, NULL);

            ICONINFO ii;
            ZeroMemory(&ii, sizeof(ii));
            ii.fIcon = TRUE;
            ii.hbmColor = hbm;
            ii.hbmMask = hbmMask;

            HICON hIcon = CreateIconIndirect(&ii);
            if (hIcon) {
                // Aplicar tanto ICON_SMALL como ICON_BIG
                SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                // No destruimos hIcon inmediatamente: DestroyIcon al salir
            }

            // Liberar bitmaps (CreateIconIndirect copia los datos internamente)
            DeleteObject(hbmMask);
            DeleteObject(hbm);
        }
    }
#endif

    // Instancias principales
    Menu menu(800, 600);
    Juego juego;

    // Bucle principal: mostrar menú, ejecutar juego y regresar
    while (window.isOpen()) {
        // Mostrar el menú y esperar elección del usuario
        int opcion = menu.run(window);

        if (opcion == 0 || opcion == 1 || opcion == 3) {
            // Sincronizar volumen desde el menú hacia el juego
            juego.setMasterVolume(menu.getVolume());

            if (opcion == 3) {
                // Opción de prueba: mostrar la pantalla de victoria
                juego.showVictoryTest(window);
            } else {
                // Obtener nombres (si se introdujeron) y ejecutar el juego
                std::string n1 = menu.getPlayerName1();
                std::string n2 = menu.getPlayerName2();
                // Sincronizar mazo seleccionado desde el menú
                juego.setSelectedDeck(menu.getSelectedDeck());
                juego.run(window, opcion, n1, n2);
            }

            // Al volver desde el juego, restaurar el tamaño y la vista para el menú
            window.setSize(sf::Vector2u(800, 600));
            sf::View view = window.getDefaultView();
            view.setSize(800, 600);
            view.setCenter(400, 300);
            window.setView(view);

            // Intentar centrar la ventana en el monitor
            auto desktop = sf::VideoMode::getDesktopMode();
            window.setPosition(sf::Vector2i(desktop.width/2 - 400, desktop.height/2 - 300));

            // Sincronizar volumen hacia el menú al regresar
            menu.setVolume(juego.getMasterVolume());
        } else {
            // El usuario eligió salir -> cerrar la ventana y terminar
            window.close();
        }
    }

    return 0;
}