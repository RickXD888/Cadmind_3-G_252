# Cadmind — Juego CardMind
Proyecto: trabajo práctico (3er parcial) — versión local del juego de emparejar cartas.

Descripción
 - CardMind es un juego de memoria (concentración) implementado en C++ usando SFML (gráficos y audio) y Box2D enlazado en el Makefile.
 - Soporta dos modos: Solitario (12 cartas) y 1 vs 1 (24 cartas). Incluye música, efectos, control de volumen y UI básica para nombres de jugadores.

Capturas / Diagramas (añadir imágenes aquí)

![Pantalla principal](assets/imagenes/menu/screenshot-main.png)

![Tablero 1v1](assets/imagenes/menu/screenshot-1v1.png)

Requisitos
 - Sistema: Windows o Linux con herramienta `make` y g++ con soporte C++17.
 - Dependencias de runtime / desarrollo:
	 - SFML (graphics, window, system, audio)
	 - Box2D (opcional según Makefile)

Compilar y ejecutar
 - Desde PowerShell (en la raíz del repo):
 ```powershell
 cd 'f:\progra\Cadmind_3-G_252'
 make            # compila ambos ejecutables
 make run        # ejecuta bin/CardMind.exe
 # o para forzar rebuild completo:
 make actualizar-force
 ```

Estructura del proyecto (resumen)
 - `src/` : código fuente C++ (Menu, Juego, main, test)
	 - `main.cpp` — bucle principal que alterna Menu <-> Juego
	 - `Menu.cpp` / `Menu.hpp` — pantalla de menú, selección de modo, entrada de nombres
	 - `Juego.cpp` / `Juego.hpp` — lógica del juego, render, audio, 1v1 y pantalla de victoria
 - `include/` : headers auxiliares (Ball, Ground, GameWindow, PhysicsSpace...)
 - `assets/` : imágenes, música y tipografías
	 - `assets/imagenes/cards/baraja-1/` — texturas de cartas
	 - `assets/imagenes/trofeo.jpg` — trofeo para pantalla de victoria
	 - `assets/music/` — pistas y efectos usados

Controles y uso
 - Menú: usar el ratón para seleccionar `JUGAR`, elegir `SOLITARIO` o `1 VS 1`.
 - En 1v1: hacer clic en las cajas `Jugador 1:` / `Jugador 2:` y escribir el nombre; hacer clic en `COMENZAR` para iniciar.
 - Volumen: flecha arriba/abajo para subir/bajar el volumen tanto en menú como en el juego.
 - Durante el juego: clic en las cartas para voltearlas. En 1v1, si fallas pasa el turno.
 - Salir: tecla `Esc` o `SALIR` en el menú.

Notas de implementación (compacta)
 - UI: SFML `sf::Text`, `sf::Sprite` para textos e imágenes; `sf::Music` y `sf::Sound` para audio.
 - Layout: cartas escaladas dinámicamente según filas/columnas (12 o 24 cartas) para adaptarse a la pantalla 1920x1080.
 - 1v1: conserva `score[2]`, `pairedSprites[2]` con miniaturas de las cartas ya emparejadas; `currentPlayer` gestiona el turno.
 - Volumen: `Menu::getVolume()` / `Juego::setMasterVolume()` sincronizan volumen entre pantallas.

Posibles mejoras (lista corta)
 - Mejorar edición de textos (selección, pegar, caret más robusto). Actualmente soporta clic para foco y backspace.
 - Transiciones suaves y animaciones de color al hacer hover.
 - Guardado de resultados/estadísticas en archivo.

Depuración y ejecución
 - Si hay errores de enlace, asegúrate de tener SFML instalado y que las rutas del linker en el Makefile coincidan con tu sistema.
 - Mensajes de advertencia sobre assets faltantes aparecen en la consola; verifica que `assets/` contenga las rutas esperadas.

Contacto / mantenimiento
 - Autor: (añadir nombre)
 - Para cambios: editar `src/`, compilar con `make` y probar con `make run`.

Licencia
 - (añadir licencia si procede)

