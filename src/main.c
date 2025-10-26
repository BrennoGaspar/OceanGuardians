/**
 * @file main.c
 * @author Prof. Dr. David Buzatto
 * @brief Main function and logic for the game. Simplified template for game
 * development in C using Raylib (https://www.raylib.com/).
 * @copyright Copyright (c) 2025
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
const int GAME_WIN = 2;
const int GAME_LOSE = 3;
int ESTADO = PARADO;

const int LIXO_WIDTH = 30;
const int LIXO_HEIGHT = 35;

const int LIXEIRA_WIDTH = 85;
const int LIXEIRA_HEIGHT = 105;

/*---------------------------------------------
 * Custom types (enums, structs, unions, etc.)
 *-------------------------------------------*/
typedef enum TipoDoLixo {
    PLASTICO, VIDRO, METAL, PAPEL, NENHUM
} TipoDoLixo;

typedef struct Jogador {
    int spriteX;
    int spriteY;
    float vel;
    TipoDoLixo tipoLixo;
    int pontuacao;
    int melhorPontuacao;
    Vector2 pos;
    Vector2 dim;
    Texture2D sprite;
    // Campos para animação
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int totalFrames;
    float frameTimer;
    float frameSpeed;
    bool isMoving;
    bool isFlipped; // Controla a direção do sprite
} Jogador;

typedef struct Lixo {
    Vector2 pos;
    TipoDoLixo type;
    Texture2D sprite;
    bool active; // Checa se o lixo esta na tela
} Lixo;

typedef struct Lixeira {
    Rectangle rect;      // Posição e tamanho (será a hitbox)
    TipoDoLixo type;     // Tipo de lixo que ela aceita
    Texture2D sprite;    // A imagem da lixeira
} Lixeira;


/*---------------------------------------------
 * Global variables.
 *-------------------------------------------*/
Jogador jogador;
Texture2D background;
Texture2D start;
Texture2D fire;
Texture2D legenda;
Texture2D placaLegenda;
Texture2D madeira;
Font tituloFont;
Texture2D papelLixo;
Texture2D vidroLixo;
Texture2D plasticoLixo;
Texture2D metalLixo;
Texture2D frame;
Texture2D hand;
Music musica;
Sound somDescarteCerto;
Sound somDescarteErrado;

float tempoRestante = 180.0f; // tempo em segundos

#define MAX_LIXOS 1 // O maximo de lixos que podem aparecer na tela
#define NUM_LIXEIRAS 4

Lixo itensLixo[MAX_LIXOS]; // Array para os lixos
Texture2D spritesLixo[4]; // Array para os 4 tipos de lixo

Texture2D lixeiraPlastico;
Texture2D lixeiraVidro;
Texture2D lixeiraMetal;
Texture2D lixeiraPapel;
Lixeira lixeiras [NUM_LIXEIRAS];

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
void draw_win(void);
void draw_lose(void);

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
    InitAudioDevice();

    // FPS: frames per second
    SetTargetFPS( 60 );

    // Load all game resources here
    background = LoadTexture( "resources/images/fundo.jpg" );
    fire = LoadTexture( "resources/images/fire.png" );
    legenda = LoadTexture( "resources/images/legenda.png" );
    placaLegenda = LoadTexture( "resources/images/placa_legenda.png" );
    madeira = LoadTexture( "resources/images/madeira.png" );
    start = LoadTexture( "resources/images/start_button.png" );
    tituloFont = LoadFont("resources/font/Asimovian-Regular.ttf");
    papelLixo = LoadTexture("resources/images/paperGarbage.png");
    vidroLixo = LoadTexture("resources/images/glassGarbage.png");
    plasticoLixo = LoadTexture("resources/images/plasticGarbage.png");
    metalLixo = LoadTexture("resources/images/metalGarbage.png");
    jogador.sprite = LoadTexture("resources/images/player_spritesheet.png");
    lixeiraPlastico = LoadTexture("resources/images/lixeira_plastico.png");
    lixeiraVidro = LoadTexture("resources/images/lixeira_vidro.png");
    lixeiraMetal = LoadTexture("resources/images/lixeira_metal.png");
    lixeiraPapel = LoadTexture("resources/images/lixeira_papel.png");
    frame = LoadTexture("resources/images/frame.png");
    hand = LoadTexture("resources/images/hand.png");

    musica = LoadMusicStream("resources/sounds/fundo.wav");
    somDescarteCerto = LoadSound("resources/sounds/acerto.mp3");
    somDescarteErrado = LoadSound("resources/sounds/erro.wav");
    // configura o volume da musica de fundo
    SetMusicVolume(musica, 0.2f);

    // Inicia a reprodução da música de fundo
    PlayMusicStream(musica);

    jogador.pos = (Vector2){ GetScreenWidth()/2- 40, GetScreenHeight()/2 - 60 };
    jogador.dim = (Vector2){ 120, 120 }; //tamanho do mergulhador
    jogador.vel = 190; // velocidade do mergulhador
    jogador.tipoLixo = NENHUM;
    jogador.pontuacao = 0;
    // Inicialização dos campos de animação
    jogador.frameWidth = 100;
    jogador.frameHeight = 100;
    jogador.currentFrame = 0;
    jogador.totalFrames = 4;
    jogador.frameTimer = 0;
    jogador.frameSpeed = 0.15f;
    jogador.isMoving = false;
    jogador.isFlipped = false;


    spritesLixo[PLASTICO] = plasticoLixo;
    spritesLixo[VIDRO] = vidroLixo;
    spritesLixo[PAPEL] = papelLixo;
    spritesLixo[METAL] = metalLixo;

    //loop array lixo
    for (int i = 0; i < MAX_LIXOS; i++) {
       itensLixo[i].active = false;
    }

    float startY = GetScreenHeight() - LIXEIRA_HEIGHT - 20; // 20 pixels de margem do fundo

    // Configura cada lixeira (tipo e sprite)
    lixeiras[PLASTICO] = (Lixeira){ .type = PLASTICO, .sprite = lixeiraPlastico };
    lixeiras[VIDRO] = (Lixeira){ .type = VIDRO, .sprite = lixeiraVidro };
    lixeiras[METAL] = (Lixeira){ .type = METAL, .sprite = lixeiraMetal };
    lixeiras[PAPEL] = (Lixeira){ .type = PAPEL, .sprite = lixeiraPapel };

    // Configura a posição e tamanho de cada lixeira
    for (int i = 0; i < NUM_LIXEIRAS; i++) {
        lixeiras[i].rect.x = 70 + i * (LIXEIRA_WIDTH + 100); // Posição X com espaçamento
        lixeiras[i].rect.y = startY;
        lixeiras[i].rect.width = LIXEIRA_WIDTH;
        lixeiras[i].rect.height = LIXEIRA_HEIGHT;
    }

    // game loop
    while ( !WindowShouldClose() ) {
        update( GetFrameTime() );
        UpdateMusicStream(musica);
        draw();
    }

    // Unload resources before closing
    UnloadTexture(background);
    UnloadFont(tituloFont);
    UnloadTexture(fire);
    UnloadTexture(legenda);
    UnloadTexture(placaLegenda);
    UnloadTexture(madeira);
    UnloadTexture(start);
    UnloadTexture(papelLixo);
    UnloadTexture(vidroLixo);
    UnloadTexture(plasticoLixo);
    UnloadTexture(metalLixo);
    UnloadTexture(lixeiraPlastico);
    UnloadTexture(lixeiraVidro);
    UnloadTexture(lixeiraMetal);
    UnloadTexture(lixeiraPapel);
    UnloadTexture(frame);
    UnloadTexture(hand);

    UnloadMusicStream(musica);
    UnloadSound(somDescarteCerto);
    UnloadSound(somDescarteErrado);

    //Liberação da textura do mergulhador
    UnloadTexture(jogador.sprite);
    // close audio device only if your game uses sounds
    CloseAudioDevice();
    CloseWindow();

    return 0;

}

void update( float delta ) {
    if (ESTADO == PARADO) {

        // Botao iniciar
        Rectangle iniciar = { GetScreenWidth()/2 - 90, 260, 180, 55 };
        bool isCollision = CheckCollisionPointRec( GetMousePosition(), iniciar );
        if( isCollision ){
            if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
                ESTADO = RODANDO;
                // Spawn do primeiro lixo do jogo
                itensLixo[0].active = true;
                itensLixo[0].pos.x = GetRandomValue(30, GetScreenWidth() - 30);
                itensLixo[0].pos.y = GetRandomValue(60, GetScreenHeight() - 150);
                int tipoAleatorio = GetRandomValue(0, 3);
                itensLixo[0].type = (TipoDoLixo)tipoAleatorio;
                itensLixo[0].sprite = spritesLixo[tipoAleatorio];
            }
        }

    } else if (ESTADO == RODANDO) {

        // cronometro
        if( tempoRestante > 0 ) {
            tempoRestante -= GetFrameTime();
        } else if ( tempoRestante <= 0 && jogador.pontuacao < 2000 ) { // Sistema de derrota
            if( jogador.melhorPontuacao < jogador.pontuacao ){
                jogador.melhorPontuacao = jogador.pontuacao;
            }
            tempoRestante = 0;
            ESTADO = GAME_LOSE;
            jogador.pos = (Vector2){ GetScreenWidth()/2- 40, GetScreenHeight()/2 - 60 };
        }

        // movimentacao do jogador
        AtualizarJogador(&jogador, KEY_A, KEY_D, KEY_W, KEY_S, delta);

        // Lógica de animação do sprite do jogador
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
            jogador.isMoving = true;
            jogador.frameTimer += GetFrameTime();
            if (jogador.frameTimer >= jogador.frameSpeed) {
                jogador.frameTimer = 0;
                jogador.currentFrame++;
                if (jogador.currentFrame >= jogador.totalFrames) {
                    jogador.currentFrame = 0; // Reinicia a animação
                }
            }
        } else {
            jogador.isMoving = false;
            jogador.currentFrame = 0; // Volta para a primeira frame quando o jogador para
        }


        // Colisao do jogador
        Rectangle jogadorRec = { jogador.pos.x, jogador.pos.y, jogador.dim.x, jogador.dim.y };

        // Aperte E para pegar o lixo
        if( IsKeyPressed(KEY_E) ){
            for( int i = 0; i < MAX_LIXOS; i++ ){
                if( itensLixo[i].active ){
                    Rectangle lixoRec = { itensLixo[i].pos.x, itensLixo[i].pos.y, LIXO_WIDTH, LIXO_HEIGHT };
                    if( CheckCollisionRecs(jogadorRec, lixoRec) ){
                        jogador.tipoLixo = itensLixo[i].type;
                        itensLixo[i].active = false;
                        break;
                    }
                }
            }
        }

        // Aperte Q para descartar o lixo
        if( IsKeyPressed(KEY_Q) && jogador.tipoLixo != NENHUM ){
            for( int i = 0; i < NUM_LIXEIRAS; i++ ){
                Rectangle lixeiraRec = lixeiras[i].rect;
                if (CheckCollisionRecs(jogadorRec, lixeiraRec)) {
                    if (jogador.tipoLixo == lixeiras[i].type) {
                        printf("Lixo descartado corretamente na lixeira %d!\n", i);
                        PlaySound(somDescarteCerto);
                        jogador.pontuacao += 100;
                    } else {
                        printf("Tipo de lixo incorreto. Tente outra lixeira.\n");
                        PlaySound(somDescarteErrado);
                        jogador.pontuacao -= 50;
                    }
                    // Spawn do lixo
                    for(int i = 0; i < MAX_LIXOS; i++){
                        if(!itensLixo[i].active){
                            itensLixo[i].active = true;
                            itensLixo[i].pos.x = GetRandomValue(30, GetScreenWidth() - 30);
                            itensLixo[i].pos.y = GetRandomValue(60, GetScreenHeight() - 150);

                            int tipoAleatorio = GetRandomValue(0, 3);
                            itensLixo[i].type = (TipoDoLixo)tipoAleatorio;
                            itensLixo[i].sprite = spritesLixo[tipoAleatorio];

                            break;
                        }
                    }
                    // Limpa o lixo da mão do jogador
                    jogador.tipoLixo = NENHUM;
                    break;
                }
            }
        }

        // Sistema de vitoria
        if ( jogador.pontuacao >= 2000 ){
            jogador.pontuacao = 2000;
            if( jogador.melhorPontuacao < jogador.pontuacao ){
                jogador.melhorPontuacao = jogador.pontuacao;
            }
            ESTADO = GAME_WIN;
            jogador.pos = (Vector2){ GetScreenWidth()/2- 40, GetScreenHeight()/2 - 60 };
        }

        // Botao "G" para ganhar automaticamente
        if( IsKeyPressed(KEY_G) ){
            jogador.pontuacao = 2000;
        }

        // Botao "P" para perder automaticamente
        if( IsKeyPressed(KEY_P) ){
            tempoRestante = 0;
        }

    } else if (ESTADO == GAME_WIN){
        // Botao menu
        Rectangle menu = { 310, 267, 180, 50 };
        bool isCollision = CheckCollisionPointRec( GetMousePosition(), menu );
        if( isCollision ){
            if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
                ESTADO = PARADO;
                jogador.pontuacao = 0;
                tempoRestante = 180.0f;
                jogador.tipoLixo = NENHUM;
                for (int i = 0; i < MAX_LIXOS; i++) {
                    itensLixo[i].active = false;
                }
            }
        }
    } else if (ESTADO == GAME_LOSE){
        // Botao menu
        Rectangle menu = { 310, 267, 180, 50 };
        bool isCollision = CheckCollisionPointRec( GetMousePosition(), menu );
        if( isCollision ){
            if( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ){
                ESTADO = PARADO;
                jogador.pontuacao = 0;
                tempoRestante = 180.0f;
                jogador.tipoLixo = NENHUM;
                for (int i = 0; i < MAX_LIXOS; i++) {
                    itensLixo[i].active = false;
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
    } else if (ESTADO == GAME_WIN){
        draw_win();
    } else if (ESTADO == GAME_LOSE){
        draw_lose();
    }

    EndDrawing();
}

void draw_menu( void ){
    // Fundo
    Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRec = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    Color transparentWhite = ColorAlpha(WHITE, 0.9f);
    DrawTexturePro(background, sourceRec, destRec, origin, 0, transparentWhite);

    // Titulo
    Vector2 tituloPos = { GetScreenWidth() / 2 - MeasureTextEx(tituloFont, "Ocean Guardians", 100, 1).x / 2, 100 };
    DrawTextEx(tituloFont, "Ocean Guardians", tituloPos, 100, 1, WHITE);
    // Botao start
    Rectangle startSourceRec = { 0, 0, (float)start.width, (float)start.height};
    Rectangle startDestRec = { GetScreenWidth()/2 - 150, 163.5, 300, 250 };
    Vector2 startOrigin = { 0 , 0 };
    DrawTexturePro(start, startSourceRec, startDestRec, startOrigin, 0, WHITE);
    

    // Legenda
    Rectangle placaLegendaSourceRec = { 0, 0, (float)placaLegenda.width, (float)placaLegenda.height};
    Rectangle placaLegendaDestRec = { GetScreenWidth() - 200, GetScreenHeight() - 360, 200, 280 };
    Vector2 placaLegendaOrigin = { 0 , 0 };
    DrawTexturePro(placaLegenda, placaLegendaSourceRec, placaLegendaDestRec, placaLegendaOrigin, 0, WHITE);
    // Figura legenda
    Rectangle legendaSourceRec = { 0, 0, (float)legenda.width, (float)legenda.height};
    Rectangle legendaDestRec = { GetScreenWidth() - 325, GetScreenHeight() - 335, 450, 200 };
    Vector2 legendaOrigin = { 0 , 0 };
    DrawTexturePro(legenda, legendaSourceRec, legendaDestRec, legendaOrigin, 0, WHITE);

    // Textos secundarios
    Rectangle madeiraSourceRec = { 0, 0, (float)madeira.width, (float)madeira.height};
    Rectangle madeiraDestRec = { GetScreenWidth() - 630, GetScreenHeight() / 2 + 70 , 450, 200 };
    Vector2 madeiraOrigin = { 0 , 0 };
    DrawTexturePro(madeira, madeiraSourceRec, madeiraDestRec, madeiraOrigin, 0, WHITE);
    DrawText("WASD para movimentacao", GetScreenWidth()/2 - 150 , 435, 20, WHITE);
    DrawText("Aperte E para pegar o lixo", GetScreenWidth()/2 - 150 , 455, 20, WHITE);
    DrawText("Aperte Q para descartar o lixo", GetScreenWidth()/2 - 150 , 475, 20, WHITE);

    Rectangle fireSourceRec = { 0, 0, (float)fire.width, (float)fire.height};
    Rectangle fireDestRec = { 45, GetScreenHeight() / 2 - 105, 160, 160 };
    Vector2 fireOrigin = { 0 , 0 };
    DrawTexturePro(fire, fireSourceRec, fireDestRec, fireOrigin, 0, WHITE);
    DrawText("Melhor", 75, GetScreenHeight()/2 - 20, 20, WHITE);
    DrawText("Pontuacao:", 75, GetScreenHeight()/2 - 5, 20, WHITE);
    DrawText( TextFormat( "%03d", jogador.melhorPontuacao ) , 110, GetScreenHeight()/2 + 20, 20, WHITE);

    DrawText("Desenvolvido por estudantes do segundo semestre de ciencia da computacao", 10, 580, 19, BLACK);
}

void draw_gameplay( void ){
    // background
    Rectangle sourceRecBackground = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRecBackground = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 originBackground = { 0, 0 };
    DrawTexturePro(background, sourceRecBackground, destRecBackground, originBackground, 0, WHITE);

    // desenho das lixeiras
    for (int i = 0; i < NUM_LIXEIRAS; i++) {
        Rectangle source = { 0, 0, (float)lixeiras[i].sprite.width, (float)lixeiras[i].sprite.height };
        DrawTexturePro(lixeiras[i].sprite, source, lixeiras[i].rect, (Vector2){0,0}, 0, WHITE);
    }

    // geracao do lixo na tela
    for (int i = 0; i < MAX_LIXOS; i++) {
        if (itensLixo[i].active) {
            Rectangle source = {0, 0, (float)itensLixo[i].sprite.width,
            (float)itensLixo[i].sprite.height };

            Rectangle dest = {itensLixo[i].pos.x, itensLixo[i].pos.y, LIXO_WIDTH, LIXO_HEIGHT};

            Vector2 origin = {0, 0};

            DrawTexturePro(itensLixo[i].sprite, source, dest, origin, 0, WHITE);
        }
    }

    // pontuacao
    DrawText( TextFormat( "%d", jogador.pontuacao ), 20, 17.5, 30, BLACK );

    // cronometro
    int minutos = (int)(tempoRestante / 60);
    int segundos = (int)(tempoRestante) % 60;
    DrawText( TextFormat( "%02d:%02d", minutos, segundos ), GetScreenWidth()/2 - 30, 15, 40, BLACK );

    // item na mao (frame)
    Rectangle frameSourceRec = { 0, 0, (float)frame.width, (float)frame.height };
    Rectangle frameDestRec = { GetScreenWidth() - 75, 10, 55, 55 };
    Vector2 frameOrigin = { 0, 0 };
    DrawTexturePro(frame, frameSourceRec, frameDestRec, frameOrigin, 0, WHITE);

    // item na mao (lixo)
    if (jogador.tipoLixo != NENHUM) {
        Texture2D itemSprite = spritesLixo[jogador.tipoLixo];
        Rectangle itemSourceRec = { 0, 0, (float)itemSprite.width, (float)itemSprite.height };
        Rectangle itemDestRec = { GetScreenWidth() - 60, 22, 25, 30 };
        Vector2 itemOrigin = { 0, 0 };
        DrawTexturePro(itemSprite, itemSourceRec, itemDestRec, itemOrigin, 0, WHITE);
    } else {
        Rectangle handSourceRec = { 0, 0, (float)hand.width, (float)hand.height };
        Rectangle handDestRec = { GetScreenWidth() - 63, 23, 32, 27 };
        Vector2 handOrigin = { 0, 0 };
        DrawTexturePro(hand, handSourceRec, handDestRec, handOrigin, 0, WHITE);
    }

    // mergulhador(player)
    // inverter o sprite caso o jogador esteja se movendo para a direita
    float frameWidth = (float)jogador.frameWidth;
    if (jogador.isFlipped) {
        frameWidth = -frameWidth; // vira o sprite horizontalmente
    }
    // source desenha a parte da imagem do arquivo spritesheet
    Rectangle source = { (float)jogador.currentFrame * jogador.frameWidth, 0, frameWidth, (float)jogador.frameHeight };
    Rectangle dest = { jogador.pos.x, jogador.pos.y, jogador.dim.x, jogador.dim.y };
    Vector2 origin = { 0, 0 };
    DrawTexturePro(jogador.sprite, source, dest, origin, 0, WHITE);
}

void draw_win( void ){
    // Fundo
    Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRec = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    Color transparentWhite = ColorAlpha(WHITE, 0.9f);
    DrawTexturePro(background, sourceRec, destRec, origin, 0, transparentWhite);

    // Mensagem de win e botao
    Vector2 tituloPos = { GetScreenWidth() / 2 - MeasureTextEx(tituloFont, "Vitória", 100, 1).x / 2, 100 };
    DrawTextEx(tituloFont, "Vitoria", tituloPos, 100, 1, WHITE);
    DrawRectangle(310, 267, 180, 50, BLUE);
    int iniciarTextWidth = MeasureText("MENU", 30);
    DrawText("MENU", GetScreenWidth()/2 - iniciarTextWidth/2, 280, 30, WHITE);

    // Textos secundarios
    DrawRectangle( (GetScreenWidth() / 2 - 250), (GetScreenHeight() / 2 + 75), 480, 70, BLUE );
    DrawText("Sua ajuda virou o jogo contra a poluição!", GetScreenWidth()/2 - 225 , 390, 20, WHITE);
    DrawText("Seja a mudança que você quer ver no mar!", GetScreenWidth()/2 - 225 , 410, 20, WHITE);
}

void draw_lose( void ){
    // Fundo
    Rectangle sourceRec = { 0, 0, (float)background.width, (float)background.height };
    Rectangle destRec = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0, 0 };
    Color transparentWhite = ColorAlpha(WHITE, 0.9f);
    DrawTexturePro(background, sourceRec, destRec, origin, 0, transparentWhite);

    // Mensagem de win e botao
    Vector2 tituloPos = { GetScreenWidth() / 2 - MeasureTextEx(tituloFont, "Vitória", 100, 1).x / 2 - 30, 100 };
    DrawTextEx(tituloFont, "Derrota", tituloPos, 100, 1, WHITE);
    DrawRectangle(310, 267, 180, 50, BLUE);
    int iniciarTextWidth = MeasureText("MENU", 30);
    DrawText("MENU", GetScreenWidth()/2 - iniciarTextWidth/2, 280, 30, WHITE);

    // Textos secundarios
    DrawRectangle( (GetScreenWidth() / 2 - 225), (GetScreenHeight() / 2 + 75), 420, 70, BLUE );
    DrawText("A poluição tomou conta desta vez...", GetScreenWidth()/2 - 200 , 390, 20, WHITE);
    DrawText("Mas você ainda pode lutar pelo mar!", GetScreenWidth()/2 - 200 , 410, 20, WHITE);
}

// Função para movimento do jogador
void AtualizarJogador(Jogador *jogador, int teclaEsquerda, int teclaDireita, int teclaCima, int teclaBaixo, float delta){

    // Movimento do jogador
    if ( IsKeyDown( teclaEsquerda ) ) {
        jogador->pos.x -= jogador->vel * delta;
        jogador->isFlipped = false; // Vira para a esquerda (padrão)
    }

    if ( IsKeyDown( teclaDireita ) ) {
        jogador->pos.x += jogador->vel * delta;
        jogador->isFlipped = true;  // Vira para a direita
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