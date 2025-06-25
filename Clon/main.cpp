#include <iostream> // Incluye funcionalidades de entrada/salida estándar (cout, cin, etc) HOLA SAMI
#include <vector> // Incluye soporte para usar vectores dinámicos (como arreglos que cambian de tamaño)
#include "raylib.h" // Librería externa usada para crear gráficos, cargar imágenes, manejar entrada de teclado, etc.

using namespace std; // Permite evitar el prefijo 'std::' al usar cosas como cout, vector, string

/* ---------------------------------------------- VARIABLES GLOBALES --------------------------------------------------*/

#pragma region //VARIABLES GLOBALES

Font fuente; // Variable global para guardar la fuente personalizada que se usará en el texto del juego

#pragma endregion

/* ---------------------------------------------- ESTRUCTURAS/CLASES --------------------------------------------------*/

#pragma region //ESTRUCTURAS/CLASES

typedef enum Pantalla { // Se define un tipo de dato enum llamado Pantalla para identificar en qué pantalla está el juego
    MENU_PRINCIPAL, // Menú principal del juego
    MENU_OPCIONES,  // Menú de configuración
    MENU_CREDITOS,  // Pantalla de créditos
    JUEGO,          // Pantalla del juego en sí
    SALIR           // Opción para cerrar el juego
} Pantalla;

Pantalla pantallaActual = MENU_PRINCIPAL; // Variable que guarda en qué pantalla nos encontramos actualmente

#pragma endregion

/* --------------------------------------------------- VECTORES -----------------------------------------------------*/

#pragma region //VECTORES
vector<Texture2D> fondos; // Vector global que almacena las texturas de fondo que se irán rotando en el menú

vector<string> menuPrincipal = { "Nueva Partida", "Continuar", "Opciones", "Creditos", "Salir" }; // Lista con las opciones del menú principal
int opc = 0; // Índice para saber qué opción del menú principal está seleccionada

vector<string> menuOpciones = {"Musica", "Fx", "Pantalla"}; // Lista con las opciones del submenú de opciones
int opcOpciones = 0; // Índice para saber qué opción del menú de opciones está seleccionada

#pragma endregion

/* ---------------------------------------------- TAMAÑO DE PANTALLA --------------------------------------------------*/

#pragma region //TAMAÑO de PANTALLA

const int screenWidth = 1280; // Ancho de la ventana principal en píxeles
const int screenHeight = 720; // Alto de la ventana principal en píxeles

#pragma endregion

/* --------------------------------------------------- FUNCIONES ------------------------------------------------------*/

//ENTRADAS por TECLADO

#pragma region //FUNCIONES ACTUALIZAR

void ActualizarMenuPrincipal() { // Función que maneja la lógica al presionar teclas en el menú principal
    int tamMenuPrincipal = menuPrincipal.size(); // Guarda el número de opciones del menú principal

    if (IsKeyPressed(KEY_DOWN)) opc++; // Si el jugador presiona la tecla abajo, aumenta el índice de la opción seleccionada
    if (IsKeyPressed(KEY_UP)) opc--; // Si presiona arriba, disminuye el índice

    if (opc < 0) opc = menuPrincipal.size() - 1; // Si se pasa del límite inferior, vuelve al final del menú
    if (opc >= tamMenuPrincipal) opc = 0; // Si se pasa del último, vuelve al inicio

    if (IsKeyPressed(KEY_ENTER)) { // Si se presiona ENTER, se ejecuta una acción según la opción seleccionada
        switch (opc) { // Estructura que compara el valor de opc
            case 0: pantallaActual = JUEGO; break; // Si es "Nueva Partida", va al juego
            case 1: pantallaActual = JUEGO; break; // "Continuar" también va al juego (placeholder por ahora)
            case 2: pantallaActual = MENU_OPCIONES; break; // Ir a menú de opciones
            case 3: pantallaActual = MENU_CREDITOS; break; // Ir a créditos
            case 4: pantallaActual = SALIR; break; // Salir del juego
        }
    }
}

void ActualizarOpciones() { // Función que controla la navegación dentro del menú de configuración
    int tamMenuOpciones = menuOpciones.size(); // Número de opciones disponibles
    
    if (IsKeyPressed(KEY_ESCAPE)) pantallaActual = MENU_PRINCIPAL; // Si se presiona ESC, se vuelve al menú principal
    
    if (IsKeyPressed(KEY_DOWN)) opcOpciones++; // Avanzar en opciones
    if (IsKeyPressed(KEY_UP)) opcOpciones--; // Retroceder en opciones
    if (opcOpciones < 0) opcOpciones = menuOpciones.size() - 1; // Si se pasa hacia arriba, volver al final
    if (opcOpciones >= tamMenuOpciones) opcOpciones = 0; // Si se pasa del final, volver al inicio
}

void ActualizarCreditos() { // Permite salir de la pantalla de créditos
    if (IsKeyPressed(KEY_ESCAPE)) pantallaActual = MENU_PRINCIPAL; // Volver al menú principal al presionar ESC
}

void ActualizarJuego() { // Permite regresar al menú principal desde el juego
    if (IsKeyPressed(KEY_ESCAPE)) pantallaActual = MENU_PRINCIPAL; // ESC vuelve al menú principal
}

void ActualizarPantalla() { // Llama a la función de lógica según la pantalla actual
    switch (pantallaActual) { // Evalúa en qué estado estamos
        case MENU_PRINCIPAL: ActualizarMenuPrincipal(); break; // Menú principal
        case MENU_OPCIONES:  ActualizarOpciones(); break; // Opciones
        case MENU_CREDITOS:  ActualizarCreditos(); break; // Créditos
        case JUEGO:          ActualizarJuego(); break; // Juego
        case SALIR:          CloseWindow(); break; // Salir
    }
}

#pragma endregion

//VISUALMENTE MENU

#pragma region //FUNCIONES DIBUJAR

void DibujarMenuPrincipal() { // Función que dibuja las opciones del menú principal en la pantalla
    int fontSize = 40; // Tamaño de la fuente
    int spacing = 60; // Espacio vertical entre cada opción
    int tamMenuPrincipal = menuPrincipal.size(); // Número total de opciones del menú principal

    for (int i = 0; i < tamMenuPrincipal; i++) { // Bucle para recorrer cada opción del menú
        Color color = (i == opc) ? GREEN : DARKGRAY; // Si la opción está seleccionada, la dibuja en verde; si no, en gris

        int textWidth = MeasureTextEx(fuente, menuPrincipal[i].c_str(), fontSize, 2).x; // Calcula el ancho del texto actual
        DrawTextEx( // Dibuja el texto de la opción en pantalla
            fuente, // Fuente usada
            menuPrincipal[i].c_str(), // Convierte el string a const char*
            (Vector2){screenWidth / 2.0f - textWidth / 2.0f, screenHeight / 3.0f + i * spacing}, // Posición centrada en pantalla
            fontSize, // Tamaño del texto
            2, // Espaciado entre letras
            color // Color determinado anteriormente
        );
    }
}

void DibujarOpciones() { // Función que dibuja las opciones del menú de configuración
    int fontSize = 40; // Tamaño de fuente
    int spacing = 60; // Espacio entre líneas
    int tamOpciones = menuOpciones.size(); // Total de opciones en este menú

    for (int i = 0; i < tamOpciones; i++) { // Recorre cada opción del menú de configuración
        Color color = (i == opcOpciones) ? RED : DARKGRAY; // Marca en rojo la opción seleccionada, el resto en gris

        int textWidth = MeasureTextEx(fuente, menuOpciones[i].c_str(), fontSize, 2).x; // Calcula el ancho del texto
        DrawTextEx( // Dibuja el texto en pantalla
            fuente, // Fuente personalizada
            menuOpciones[i].c_str(), // Opción actual como const char*
            (Vector2){screenWidth / 2.0f - textWidth / 2.0f, screenHeight / 3.0f + i * spacing}, // Posición centrada
            fontSize, // Tamaño
            2, // Espaciado entre letras
            color // Color seleccionado
        );
    }
}

void DibujarCreditos() { // Dibuja la pantalla de créditos
    DrawText("CREDITOS: Charles y sus compas", 300, screenHeight / 2, 30, DARKPURPLE); // Texto fijo en la pantalla
}

void DibujarJuego() { // Pantalla temporal del juego que indica que se está "jugando"
    DrawText("JUGANDO... (Pulsa ESC para volver)", 300, screenHeight / 2, 30, DARKGREEN); // Mensaje en pantalla
}

void DibujarPantalla() { // Llama a la función de dibujo de la pantalla activa
    switch (pantallaActual) {
        case MENU_PRINCIPAL: DibujarMenuPrincipal(); break; // Dibujar menú
        case MENU_OPCIONES:  DibujarOpciones(); break; // Dibujar opciones
        case MENU_CREDITOS:  DibujarCreditos(); break; // Dibujar créditos
        case JUEGO:          DibujarJuego(); break; // Dibujar juego
        default: break; // Nada
    }
}

#pragma endregion

/* ------------------------------------------------- MAIN/PRINCIPAL --------------------------------------------------*/

#pragma region //INT MAIN

int main(void) { // Función principal, punto de inicio del programa
    Texture2D fondo[4]; // Arreglo temporal de texturas de fondo
    string cadNumImg; // Para construir el nombre del archivo de imagen
    string cadena; // Ruta completa de la imagen
    const char *cargarImg; // Conversión a const char* para LoadTexture

    InitWindow(screenWidth, screenHeight, "BRAINROT FIGHTER"); // Inicializa la ventana
    SetExitKey(KEY_NULL); // Desactiva tecla predeterminada de salida (ESC)

    fuente = LoadFontEx("C:/Users/uri18/Desktop/CODE_VIDEOGAMES/PROYECTO CLON/Proyecto-Clon/Clon/resources/fonts/AcehDarusalam.ttf", 40, 0, 0); // Carga la fuente personalizada

    for (int i = 0; i < 4; i++) { // Bucle para cargar 4 imágenes de fondo
        cadNumImg = to_string(i+1); // Convierte i+1 a string ("1", "2", etc.)
        cadena = "C:/Users/uri18/Desktop/CODE_VIDEOGAMES/PROYECTO CLON/Proyecto-Clon/Clon/resources/img/" + cadNumImg + ".png"; // Construye la ruta del archivo
        cargarImg = cadena.c_str(); // Convierte a const char*
        fondo[i] = LoadTexture(cargarImg); // Carga la imagen como textura
    }

    float tiempoCambio = 5.0f; // Tiempo entre cambios de fondo
    float tiempoActual = 0.0f; // Tiempo acumulado desde el último cambio
    int fondoActual = 0; // Índice del fondo actualmente mostrado

    for (int i = 0; i < 4; i++) { // Agrega los fondos cargados al vector global
        fondos.push_back(fondo[i]);
    }

    SetTargetFPS(60); // Define que el juego corra a 60 frames por segundo

/* ------------------------------------------------- CICLO PRINCIPAL -----------------------------------------------------*/

    while (!WindowShouldClose() && pantallaActual != SALIR) { // Bucle principal, se ejecuta hasta cerrar la ventana o elegir SALIR
        BeginDrawing(); // Comienza el dibujo de frame

            tiempoActual += GetFrameTime(); // Suma el tiempo transcurrido desde el último frame
            if (tiempoActual >= tiempoCambio) { // Si pasó suficiente tiempo...
                tiempoActual = 0.0f; // Reinicia contador
                fondoActual = (fondoActual + 1) % fondos.size(); // Cambia al siguiente fondo (cíclico)
            }

            ClearBackground(RAYWHITE); // Borra todo lo que había antes con color blanco (opcional si el fondo cubre todo)

            DrawTexturePro( // Dibuja el fondo actual adaptado al tamaño de la pantalla
                fondos[fondoActual], // Textura a dibujar
                { 0, 0, (float)fondos[fondoActual].width, (float)fondos[fondoActual].height }, // Rectángulo de origen (completo)
                { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, // Rectángulo destino (pantalla completa)
                { 0, 0 }, // Origen (offset de dibujo)
                0.0f, // Rotación
                WHITE // Color (sin modificar la textura)
            );

            ActualizarPantalla(); // Ejecuta la lógica de la pantalla actual
            DibujarPantalla(); // Dibuja la pantalla actual

        EndDrawing(); // Finaliza el frame
    }

    for (auto& fondo : fondos) { // Libera memoria de las texturas al cerrar
        UnloadTexture(fondo);
    }

    CloseWindow(); // Cierra la ventana y libera recursos de Raylib
    return 0; // Devuelve 0 para indicar salida correcta
}

#pragma endregion
