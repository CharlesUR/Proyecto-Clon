/*******************************************************************************************

*   Para compilar, presionar F5. antuna

********************************************************************************************/

#include "raylib.h"

#define VEL 236
Texture2D textura;
float deltaTime;

//----------------------------------------------------------------------------------
// Definicion de clases.
//----------------------------------------------------------------------------------
typedef enum{
    Menu_Principal,
    Juego,
    Game_Over
} Escenas;

Escenas escenaActual;

typedef enum{
    Quieto,
    Arriba,
    Izquierda,
    Abajo,
    Derecha
} Direcciones;

typedef enum{
    Normal,
    Rapido,
    Lento
} EstadoVelocidad;

class Jugador{
    private:
        Rectangle hitbox;
        Color colorMoto;

    public:
        Direcciones direccion;
        EstadoVelocidad velocidadActual;

        Jugador(float _height, float _width, Vector2 _coord, Color _color){
            hitbox.height = _height;
            hitbox.width = _width;
            hitbox.x = _coord.x;
            hitbox.y = _coord.y;
            colorMoto = _color;
            direccion = Quieto;
            velocidadActual = Normal;
        }
        Jugador(float _height, float _width, float _x, float _y, Color _color){
            hitbox.height = _height;
            hitbox.width = _width;
            hitbox.x = _x;
            hitbox.y = _y;
            colorMoto = _color;
            direccion = Quieto;
            velocidadActual = Normal;
        }
        void dibujar_Jugador(){
            float x = textura.width;
            x *= direccion == Izquierda? -1:1;
            
            float r = 0;
            //if(direccion == Abajo) r = 90;
            //else if (direccion == Arriba) r = -90;
            //else r = 0;

            DrawTexturePro(textura, (Rectangle){0, 0, x, (float)textura.height}, hitbox, (Vector2){0,0}, r, colorMoto);
        }
        void mover_Lateral(float velocidad){
            hitbox.x += velocidad;
        }
        void mover_Vertical(float velocidad){
            hitbox.y -= velocidad;
        }
        void mover(){
            float velocidad = VEL * deltaTime;
            
            if(velocidadActual == Rapido) velocidad *= 1.4;
            else if(velocidadActual == Lento) velocidad *= 0.55;

            if(direccion == Arriba) mover_Vertical(velocidad);
            else if(direccion == Abajo) mover_Vertical(-velocidad);
            else if(direccion == Derecha) mover_Lateral(velocidad);
            else if(direccion == Izquierda) mover_Lateral(-velocidad);
            
        }
        float pos_x(){
            return hitbox.x;
        }
        float pos_y(){
            return hitbox.y;
        }
};

//----------------------------------------------------------------------------------
// Definicion de variables y constantes.
//----------------------------------------------------------------------------------
const int screenWidth = 800;
const int screenHeight = 450;


Jugador P1 = Jugador(50, 50, screenWidth/2-25, screenHeight/2-25, PURPLE);
Camera2D camera = {(Vector2){screenWidth/2, screenHeight/2}, (Vector2){P1.pos_x()+25, P1.pos_y()+25}, 0, 1}; // Para un jugador nomas

//----------------------------------------------------------------------------------
// Definicion de funciones.
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);          // Update and draw one frame
void limites();

//----------------------------------------------------------------------------------
// Programa pincipal.
//----------------------------------------------------------------------------------
int main()
{
    // Inicializacion.
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Clon");
    
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);           // Hace que ninguna tecla cierre el juego. Por defecto es ESC, pero sera la pausa.
    SetTargetFPS(60);               // Limita los fps a 60.
    
    escenaActual = Menu_Principal;
    textura = LoadTexture("resources/moto.png");
    //--------------------------------------------------------------------------------------

    // Loop principal del juego.
    while (!WindowShouldClose())    // Detecta el boton de cerrar ventana.
    {
        if(escenaActual == Menu_Principal){
            P1 = Jugador(50, 50, screenWidth/2-25, screenHeight/2-25, PURPLE);
            BeginDrawing();
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("CLON", screenWidth/2, 12, 30, ORANGE);
                DrawText("CLON", screenWidth/2-2, 10, 30, WHITE);
                DrawText("WASD - Movimiento", 30, 40, 25, WHITE);
                DrawText("CTRL - Acelerar", 30, 70, 25, WHITE);
                DrawText("SHIFT - Desacelerar", 30, 100, 25, WHITE);
                DrawText("Space - Velocidad Normal", 30, 130, 25, WHITE);
                DrawText("Espacio para continuar...", 53, 224, 30, BLUE);
                DrawText("Espacio para continuar...", 50, 220, 30, WHITE);
                if(IsKeyPressed(KEY_SPACE)) escenaActual = Juego;
            EndDrawing();
        } else if(escenaActual == Juego){
            UpdateDrawFrame();
        } else if(escenaActual == Game_Over){
            BeginDrawing();
                DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
                DrawText("GAME OVER", screenWidth/2, 12, 30, RED);
                DrawText("Espacio para continuar...", 53, 224, 30, BLUE);
                DrawText("Espacio para continuar...", 50, 220, 30, WHITE);
                if(IsKeyPressed(KEY_SPACE)) escenaActual = Menu_Principal;
            EndDrawing();
        }
    }

    // Liberacion de memoria.
    //--------------------------------------------------------------------------------------
    CloseWindow();                  // Cierra la ventana y el contexto OpenGL
    //--------------------------------------------------------------------------------------

    return 0;
}

// Actualiza y dibuja los frames.
static void UpdateDrawFrame(void)
{
    // Actualizacion de cosas.
    //----------------------------------------------------------------------------------
    deltaTime = GetFrameTime();

    if(IsKeyPressed(KEY_W)){
        if(P1.direccion != Arriba) P1.direccion = Arriba;
        else if(P1.velocidadActual != Rapido) P1.velocidadActual = Rapido;
        else P1.velocidadActual = Normal;
    } 
    if(IsKeyPressed(KEY_S)){
        if(P1.direccion != Abajo) P1.direccion = Abajo;
        else if(P1.velocidadActual != Rapido) P1.velocidadActual = Rapido;
        else P1.velocidadActual = Normal;
    }
    if(IsKeyPressed(KEY_A)){
        if(P1.direccion != Izquierda) P1.direccion = Izquierda;
        else if(P1.velocidadActual != Rapido) P1.velocidadActual = Rapido;
        else P1.velocidadActual = Normal;
    } 
    if(IsKeyPressed(KEY_D)){
        if(P1.direccion != Derecha) P1.direccion = Derecha;
        else if(P1.velocidadActual != Rapido) P1.velocidadActual = Rapido;
        else P1.velocidadActual = Normal;
    } 

    if(IsKeyPressed(KEY_LEFT_SHIFT)){
        if(P1.velocidadActual != Lento) P1.velocidadActual = Lento;
        else P1.velocidadActual = Normal;
    }

    if(IsKeyPressed(KEY_LEFT_CONTROL)){
        if(P1.velocidadActual != Rapido) P1.velocidadActual = Rapido;
        else P1.velocidadActual = Normal;
    }

    if(IsKeyPressed(KEY_SPACE)){
        P1.velocidadActual = Normal;
    }

    P1.mover();

    limites();
    //camera.target = (Vector2){P1.pos_x()+25, P1.pos_y()+25}; // Seguir al jugador

    //----------------------------------------------------------------------------------

    // Dibujar frames.
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(BLACK);
        BeginMode2D(camera);
    
            // Limites del mapa
            DrawLine(1, 0, 1, screenHeight, RED);
            DrawLine(0, 2, screenWidth, 2, RED);
            DrawLine(screenWidth, 0, screenWidth, screenHeight, RED);
            DrawLine(0, screenHeight, screenWidth, screenHeight, RED);
            
            // Cruz central.
            //DrawLine(0, screenHeight/2, screenWidth, screenHeight/2, GREEN);
            //DrawLine(screenWidth/2, 0, screenWidth/2, screenHeight, GREEN);

            DrawText(TextFormat("Velocidad: %s", P1.velocidadActual==Rapido?"Rapido":P1.velocidadActual==Lento?"Lento":"Normal"), 20, 20,20, GREEN);

            P1.dibujar_Jugador();
        
        EndMode2D();

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void limites(){
    if(P1.pos_x() < 0 || P1.pos_x()+50 > screenWidth || P1.pos_y() < 0 || P1.pos_y()+50 > screenHeight){
        escenaActual = Game_Over;
    }
}