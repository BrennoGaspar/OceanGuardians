/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Simplified template for game
 * development in C using Raylib (https://www.raylib.com/).
 * * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*---------------------------------------------
 * Library headers.
 *-------------------------------------------*/
#include "raylib/raylib.h"
#include "raylib/raymath.h" // Adicionado para Vector2 e funções relacionadas

/*---------------------------------------------
 * Project headers.
 *-------------------------------------------*/

/*---------------------------------------------
 * Macros. 
 *-------------------------------------------*/

/*--------------------------------------------
 * Constants. 
 *------------------------------------------*/
const int PARADO = 0;
const int RODANDO = 1;
int ESTADO = PARADO; 

const int LIXO_WIDTH = 40;
const int LIXO_HEIGHT = 40;

/*---------------------------------------------
 * Custom types (enums, structs, unions, etc.)
 *-------------------------------------------*/
typedef struct Jogador {
    int spriteX;
    int spriteY;
    float vel;
    Vector2 pos;
    Vector2 dim;
    Texture2D sprite;
} Jogador;

typedef enum TipoDoLixo {
    PLASTICO, VIDRO, METAL, PAPEL,
} TipoDoLixo;

typedef struct Lixo {
    Vector2 pos;
    TipoDoLixo type;
    Texture2D sprite;
    bool active; // Checa se o lixo esta na tela 
} Lixo;

/*---------------------------------------------
 * Global variables.
 *-------------------------------------------*/
Jogador jogador;
Texture2D background;
Texture2D fire;
Font tituloFont;
Texture2D papelLixo;
Texture2D vidroLixo;
Texture2D plasticoLixo;
Texture2D metalLixo; 

#define MAX_LIXOS 20 // O maximo de lixos que podem aparecer na tela

Lixo itensLixo[MAX_LIXOS];
Texture2D spritesLixo[4]; // Array para os 4 tipos de lixo

/*---------------------------------------------
 * Function prototypes. 
 *-------------------------------------------*/
/**
 * @brief Reads user input and updates the state of the game.
 */
void update( float delta );
    
/**
 * @brief Draws the state of the game.
 */
void draw( void );
void draw_menu(void);
void draw_gameplay(void);

void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta);

/**
 * @brief Game entry point.
 */
int main( void ) {
    
    // antialiasing
    SetConfigFlags( FLAG_MSAA_4X_HINT );

    // creates a new window 800 pixels wide and 600 pixels high
    InitWindow( 800, 600, "Ocean Guardians - O Jogo" );

    // init audio device only if your game uses sounds
    //InitAudioDevice();

    // FPS: frames per second
    SetTargetFPS( 60 );

    // Load all game resources here
    background = LoadTexture( "resources/images/fundo.jpg" );
    fire = LoadTexture( "resources/images/fire.png" );
    tituloFont = LoadFont("resources/font/Asimovian-Regular.ttf");
    papelLixo = LoadTexture("resources/images/paperGarbage.png");
    vidroLixo = LoadTexture("resources/images/glassGarbage.png");
    plasticoLixo = LoadTexture("resources/images/plasticGarbage.png");
    metalLixo = LoadTexture("resources/images/metalGarbage.png");
    jogador.sprite = LoadTexture("resources/images/player.png");
    jogador.pos = (Vector2){ 400, 500 };    
    jogador.dim = (Vector2){ 225, 225 };   //tamanho do mergulhador   
    jogador.vel = 200; // velocidade do mergulhador

    spritesLixo[PLASTICO] = plasticoLixo;
    spritesLixo[VIDRO] = vidroLixo;
    spritesLixo[PAPEL] = papelLixo;
    spritesLixo[METAL] = metalLixo;

    //loop array lixo
    for (int i = 0; i < MAX_LIXOS; i++) {
       itensLixo[i].active = false;
    } 

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        draw();
    }

    // Unload resources before closing
    UnloadTexture(background);
    UnloadFont(tituloFont);
    UnloadTexture(fire);
    UnloadTexture(papelLixo);
    UnloadTexture(vidroLixo);
    UnloadTexture(plasticoLixo);
    UnloadTexture(metalLixo);

    //Liberação da textura do mergulhador
    UnloadTexture(jogador.sprite);
    // close audio device only if your game uses sounds
    CloseWindow();

    return 0;

}

void update( float delta ) {
    if (ESTADO == PARADO) {
        Rectangle iniciar = { 310, 267, 180, 50 };
        bool isCollision = CheckCollisionPointRec( GetMousePosition(), iniciar );
        
        if( isCollision ){
            if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
                ESTADO = RODANDO;
            }
        }
    } else if (ESTADO == RODANDO) {
        
        AtualizarJogador(&jogador, KEY_A, KEY_D, KEY_W, KEY_S, delta);
        // if (IsKeyDown(KEY_D)) {
        //     jogador.spriteX += 100 * delta;
        // }

        // apertar G para spawnar o lixo 
        if(IsKeyPressed(KEY_G)){
            for(int i = 0; i < MAX_LIXOS; i++){
                if(!itensLixo[i].active){
                    itensLixo[i].active = true;
                    itensLixo[i].pos.x = GetRandomValue(50, GetScreenWidth() - 50); 
                    itensLixo[i].pos.y = GetRandomValue(200, GetScreenHeight() - 50);
                    
                    int tipoAleatorio = GetRandomValue (0, 3); 
                    itensLixo[i].type = (TipoDoLixo)tipoAleatorio;
                    itensLixo[i].sprite = spritesLixo[tipoAleatorio]; 
                    break;
                }
            }
        }
    }
}

void draw( void ) {
    BeginDrawing();
    ClearBackground( WHITE );

    if(ESTADO == PARADO) {
        draw_menu();
    } else if (ESTADO == RODANDO) {
        draw_gameplay();
    }

    EndDrawing();
}

void draw_menu(void) {
    // Fundo
    Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0.0f, 0.0f };
    Color transparentWhite = ColorAlpha(WHITE, 0.9f);
    DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, transparentWhite);

    // Titulo e botao
    Vector2 tituloPos = { GetScreenWidth() / 2 - MeasureTextEx(tituloFont, "Ocean Guardian", 100, 1).x / 2, 100 };
    DrawTextEx(tituloFont, "Ocean Guardian", tituloPos, 100, 1, WHITE);
    
    DrawRectangle(310, 267, 180, 50, BLUE);
    int iniciarTextWidth = MeasureText("INICIAR", 30);
    DrawText("INICIAR", GetScreenWidth()/2 - iniciarTextWidth/2, 280, 30, WHITE);

    // Textos secundarios
    DrawRectangle( (GetScreenWidth() / 2 - 175), (GetScreenHeight() / 2 + 75), 370, 90, BLUE );
    DrawText("Aperte Q para descartar o lixo", GetScreenWidth()/2 - 150 , 390, 20, WHITE);
    DrawText("Aperte E para pegar o lixo", GetScreenWidth()/2 - 150 , 410, 20, WHITE);
    DrawText("WASD para movimentacao", GetScreenWidth()/2 - 150 , 430, 20, WHITE);

    Rectangle fireSourceRec = { 0, 0, (float)fire.width, (float)fire.height};
    Rectangle fireDestRec = { 5, GetScreenHeight() / 2 - 110, 250, 190 };
    Vector2 fireOrigin = { 0 , 0 };
    DrawTexturePro(fire, fireSourceRec, fireDestRec, fireOrigin, 0, WHITE);
    DrawText("Melhor", 75, GetScreenHeight()/2 - 20, 20, WHITE);
    DrawText("Pontuacao:", 75, GetScreenHeight()/2 - 5, 20, WHITE);
    DrawText("XXX", 105, GetScreenHeight()/2 + 20, 20, WHITE);

    DrawText("Desenvolvido por estudantes do segundo semestre de ciencia da computacao", 10, 580, 19, BLACK);
}

void draw_gameplay(void) {
    
    Rectangle sourceRecBackground = { 0.0f, 0.0f, (float)background.width, (float)background.height };
    Rectangle destRecBackground = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 originBackground = { 0.0f, 0.0f };
    DrawTexturePro(background, sourceRecBackground, destRecBackground, originBackground, 0.0f, WHITE);

    // mergulhador(player)
    Rectangle source = { 0.0f, 0.0f, (float)jogador.sprite.width, (float)jogador.sprite.height };
    Rectangle dest = { jogador.pos.x, jogador.pos.y, jogador.dim.x, jogador.dim.y };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(jogador.sprite, source, dest, origin, 0.0f, WHITE);
    
    // geracao do lixo na tela
    for (int i = 0; i < MAX_LIXOS; i++) {
        if (itensLixo[i].active) {
            Rectangle source = {0.0f, 0.0f, (float)itensLixo[i].sprite.width,
            (float)itensLixo[i].sprite.height };

            Rectangle dest = {itensLixo[i].pos.x, itensLixo[i].pos.y, LIXO_WIDTH, LIXO_HEIGHT};

            Vector2 origin = {0.0f, 0.0f};

            DrawTexturePro(itensLixo[i].sprite, source, dest, origin, 0.0f, WHITE);
        }
    }
}

// Função para movimento do jogador.
void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta){

    // Movimento do jogador
    if ( IsKeyDown( teclaEsquerda ) ) {
        jogador->pos.x -= jogador->vel * delta;
    }

    if ( IsKeyDown( teclaDireita ) ) {
        jogador->pos.x += jogador->vel * delta;
    } 

    if (IsKeyDown(teclaCima)) {
        jogador->pos.y -= jogador->vel * delta;
    }

    if (IsKeyDown(teclaBaixo)) {
        jogador->pos.y += jogador->vel * delta;
    } 

    // Verificação de limites para manter o jogador na tela
    // Limite esquerdo
    if ( jogador->pos.x < 0 ) {
        jogador->pos.x = 0;
    }
    
    // Limite direito
    if ( jogador->pos.x + jogador->dim.x > GetScreenWidth() ) {
        jogador->pos.x = GetScreenWidth() - jogador->dim.x;
    }

    // Limite superior
    if ( jogador->pos.y < 0 ) {
        jogador->pos.y = 0;
    }
    
    // Limite inferior
    if ( jogador->pos.y + jogador->dim.y > GetScreenHeight() ) {
        jogador->pos.y = GetScreenHeight() - jogador->dim.y;
    }
}
