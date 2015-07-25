/*

Elder Trip - A fuga do asilo
UFRRJ - Instituto Multidisciplinar
Jogo elaborado para a matéria de Computação 1
Baseado no jogo Donkey Kong 1981.

Integrantes:
- Vinicius Ayres     - viniciusAyres@live.it
- Marcos Vinicius    - marcoscribeiro1@gmail.com
- Gabriel Costa      - gabrielcs23@hotmail.com

Versão 1.0
* 1 Fase
* Aberturas personalizadas

*/

#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define MAX 8
#define CAI 5

//-----------------------------------Globais --------------------------------------------------------------//
    SDL_Surface *screen;
    SDL_Surface *henry;
    SDL_Surface *background;
    SDL_Surface *backmove;
    SDL_Surface *hud;
    SDL_Surface *barril;
    SDL_Surface *backdie;
    SDL_Surface *fundo_menu;
    SDL_Surface *jovem;
    SDL_Surface *explosao;
    SDL_Surface *boss;
    SDL_Surface *premio;
    SDL_Surface *gameover;
    SDL_Event event;
    Mix_Music *music;
    Mix_Chunk* jump;
    Mix_Chunk* smash;
    Mix_Chunk* somPonto;
    Mix_Chunk* scream;
    Mix_Chunk* pof;
    TTF_Font* font;
    Uint32 start;
    int VIDAS, SCORE, TEMPO, INCREMENTO_ANDA = 0, INCREMENTO_ESCADA = 0, INCREMENTO_PULO = 0;
    char str_score[100], str_vidas[2];

    int btnSom=0, btnSFx=0, repetirPartida=0, first=0;

    int cont = 0, velX = 0, velY = 0, velG = 1, velJ, velE = -5, done = 0; // Variável de controle do loop

    int TeclaEspace   = 0,
        TeclaCima     = 0,
        TeclaBaixo    = 0,
        TeclaEsquerda = 0,
        TeclaDireita  = 0;

    int PULO_ON         =  0,
        PULO_INCREMENTO = -5,
        PULO_TAMANHO    =  0;
    int ESCADA_ON       =  0,
        TEMPO;

    typedef struct  // Levar la pra cima
    {
        int velx_barril, PONTO, ANDAR_ATUAL, flag, queda, ENEMY_ON, ATIVO, timer, sx, sy, INC_ANI_ANDA; // Nao preciso mais declarar
        SDL_Rect fonte_barril, dest_barril, score_barril, colide_barril; // essas variaveis globalmente
    } INIMIGO;

    typedef struct
    {
        char nome[15];
        int pontos;
    } JOGADOR;

//----------------------------------------------------------------------------------------------------------------
//                                                VARIAVEIS POSIÇOES e RECTS
// Para saber a posição real ainda tem que somar com a altura do henry

    int ANDAR_Y[6]; //Essas são as posi
    SDL_Rect fonte_henry, dest_henry, ceu, plataforma_1,plataforma_2,plataforma_3,plataforma_4,plataforma_5,plataforma_6,
             escada_b1,escada_b2,escada_b3,escada_b4,escada_b5,escada_b6,escada_r1,escada_r2,escada_r3, boss_fonte, boss_dest, premio_dest, explosao_fonte, explosao_dest;

    INIMIGO cadeirante[MAX];

    void carregaAndarRects()
    {
        henry        = IMG_Load ("imagens/henry.png");      if (henry == NULL) puts("Não foi possível carregar henry.png");

        ANDAR_Y[1] = 561;
        ANDAR_Y[2] = 451;
        ANDAR_Y[3] = 340;
        ANDAR_Y[4] = 228;
        ANDAR_Y[5] = 117;
        ANDAR_Y[0] = 83;// Andar do boss

        TeclaBaixo = TeclaCima = TeclaDireita = TeclaEsquerda = TeclaEspace = 0;
        velX = 0;
        velY = 0;

        fonte_henry.x = 12 * 196/3;
        fonte_henry.y = 0;
        fonte_henry.w = 1564/24;
        fonte_henry.h = 328/8;

        dest_henry.x = 0;
        dest_henry.y = ANDAR_Y[1] - fonte_henry.h;
        dest_henry.w = fonte_henry.w;
        dest_henry.h = fonte_henry.h;

        ceu.x = 0;
        ceu.y = 0;
        ceu.w = 800; // Ajustar dimensoes de largura e altura
        ceu.h = 200;

        escada_b1.x = 468;
        escada_b1.y = 451;
        escada_b1.w = 17;
        escada_b1.h = 111;

      	escada_b2.x = 185;
        escada_b2.y = 340;
        escada_b2.w = 17;
        escada_b2.h = 112;

      	escada_b3.x = 650;
        escada_b3.y = 228;
        escada_b3.w = 17;
        escada_b3.h = 115;

      	escada_b4.x = 158;
        escada_b4.y = 115;
        escada_b4.w = 17;
        escada_b4.h = 114;

      	escada_b5.x = 765;
        escada_b5.y = 0;
        escada_b5.w = 17;
        escada_b5.h = 80;

      	escada_r1.x = 368;
        escada_r1.y = 340;
        escada_r1.w = 17;
        escada_r1.h = 112;

      	escada_r2.x = 520;
        escada_r2.y = 228;
        escada_r2.w = 17;
        escada_r2.h = 115;

      	escada_r3.x = 333;
        escada_r3.y = 115;
        escada_r3.w = 17;
        escada_r3.h = 114;

      	plataforma_1.x = 0; //plataforma do chão
      	plataforma_1.y = 561;
      	plataforma_1.h = 33;
      	plataforma_1.w = 800;

      	plataforma_2.x = 0;
        plataforma_2.y = 451;
        plataforma_2.h = 29;
        plataforma_2.w = 690;

        plataforma_3.x = 77;
        plataforma_3.y = 340;
        plataforma_3.h = 29;
        plataforma_3.w = 723;

        plataforma_4.x = 0;
        plataforma_4.y = 228;
        plataforma_4.h = 29;
        plataforma_4.w = 704;

        plataforma_5.x = 81; //plataforma antes do boss
        plataforma_5.y = 117;
        plataforma_5.h = 29;
        plataforma_5.w = 719;

        plataforma_6.x = 666;//plataforma do boss
        plataforma_6.y = 82;
        plataforma_6.h = 36;
        plataforma_6.w = 134;

        boss_fonte.x = 9 * 2604/20;
        boss_fonte.y = 1392/12;
        boss_fonte.w = 2604/20;
        boss_fonte.h = 1392/12;

        boss_dest.x = 650;
        boss_dest.y = ANDAR_Y[5] - boss_fonte.h;
        boss_dest.w = 2604/20;
        boss_dest.h = 1392/12;

        premio_dest.x = 394;
        premio_dest.y = ANDAR_Y[5] - 50;
        premio_dest.w = 50;
        premio_dest.h = 50;

        explosao_fonte.x = 0;
        explosao_fonte.y = 0;
        explosao_fonte.w = 526/8;
        explosao_fonte.h = 64;

        explosao_dest.x = 0;
        explosao_dest.y = ANDAR_Y[5] - boss_fonte.h;
        explosao_dest.w = 526/8;
        explosao_dest.h = 64;

        TEMPO = 0;

        int i;//LOCAL
        for(i = 0; i < MAX; i++)
        {

            cadeirante[i].velx_barril = -10; cadeirante[i].PONTO = 0;
            cadeirante[i].ANDAR_ATUAL = 5; cadeirante[i].flag = 1;
            cadeirante[i].queda = rand()%3; cadeirante[i].ENEMY_ON = 1;
            cadeirante[i].ATIVO = 0; cadeirante[i].timer = 0;
            cadeirante[i].INC_ANI_ANDA = 0;


            cadeirante[i].fonte_barril.x = 6 * 930/15;
            cadeirante[i].fonte_barril.y = 0;
            cadeirante[i].fonte_barril.w = 930/15;
            cadeirante[i].fonte_barril.h = 350/7;

            cadeirante[i].dest_barril.x = 500;
            cadeirante[i].dest_barril.y = ANDAR_Y[5] - cadeirante[i].fonte_barril.h;
            cadeirante[i].dest_barril.w = cadeirante[i].fonte_barril.w;
            cadeirante[i].dest_barril.h = cadeirante[i].fonte_barril.h;

            cadeirante[i].score_barril.x = cadeirante[i].dest_barril.x;
            cadeirante[i].score_barril.y = cadeirante[i].dest_barril.y - 30;
            cadeirante[i].score_barril.w = cadeirante[i].dest_barril.w - 40;
            cadeirante[i].score_barril.h = 30;

            cadeirante[i].colide_barril.x = cadeirante[i].dest_barril.x;
            cadeirante[i].colide_barril.y = cadeirante[i].dest_barril.y + 5;
            cadeirante[i].colide_barril.w = 930/15 - 40;
            cadeirante[i].colide_barril.h = cadeirante[i].dest_barril.w - 20;
        }
    }
//---------------------------------------------------------------------------------------------------------------------

void Inicio()
{
  	SDL_Init(SDL_INIT_VIDEO);

  	TTF_Init();

  	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

  	//esta função aui carrega a músic
  	if(first!=0)
  	{
        music = Mix_LoadMUS("sons/LUTA_110-highway-in-the-sky.mp3");             if (music == NULL) printf("não carregou musica_abertura.\n");
  	}else
        music = Mix_LoadMUS("sons/musica_abertura.mp3");             if (music == NULL) printf("não carregou musica_abertura.\n");
  	jump = Mix_LoadWAV("sons/jump.wav");                         if (jump == NULL) printf("não carregou jump.\n");
  	smash = Mix_LoadWAV("sons/smash.wav");                       if (smash == NULL) printf("não carregou smash.\n");
  	somPonto = Mix_LoadWAV("sons/ponto.wav");                    if (somPonto == NULL) printf("não carregou somPonto.\n");
  	scream = Mix_LoadWAV("sons/MonsterScream.wav");              if (scream == NULL) printf("Nao carregou scream.\n");
  	pof = Mix_LoadWAV("sons/POF.wav");                           if (pof == NULL) printf("Nao carregou POF.\n");
  	Mix_AllocateChannels(5);

    SDL_WM_SetCaption("Elder Trip - A fuga do asilo", NULL);
  	screen       = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE); // Cria a janela;

  	henry        = IMG_Load ("imagens/henry.png");      if (henry == NULL) puts("Não foi possível carregar henry.png");
    background   = IMG_Load ("imagens/background.png"); if (background == NULL) puts("Não foi possível carregar background.png");
    backmove     = IMG_Load ("imagens/nuvem.png");      if (backmove == NULL) puts("Não foi possível carregar nuvem.png");
    hud   = IMG_Load ("imagens/hud.png");               if (hud == NULL) puts("Não foi possível carregar hud.png");
    barril       = IMG_Load ("imagens/barril.png");     if (barril == NULL) puts("Não foi possível carregar barril.png");
    backdie      = IMG_Load ("imagens/backdie.png");    if (backdie == NULL) puts("Não foi possível carregar backdie.png");
    boss         = IMG_Load ("imagens/boss.png");       if (boss == NULL) puts("Não foi possível carregar boss.png");
    premio       = IMG_Load ("imagens/premio.png");     if (premio == NULL) puts("Não foi possível carregar premio.png");
    //jovem        = IMG_Load ("imagens/jovem.png");      if(jovem = NULL) puts("Não foi possivel carregar jovem.png");
    gameover     = IMG_Load ("imagens/gameover.png");    if (gameover == NULL) puts("Não foi possível carregar gameover.png");
    explosao     = IMG_Load ("imagens/explosao.png");   if(jovem = NULL) puts("Não foi possivel carregar explosao.png");
    VIDAS        = 3;
    SCORE        = 0;
    str_score[0] ='0';
    str_vidas[0] ='3';
    srand(time(NULL));
}

void ControlFPS(int FPS)
{
    if(1000/FPS >(SDL_GetTicks()-start))
        SDL_Delay(1000/FPS-(SDL_GetTicks()-start));
    SDL_Flip( screen);
}

int Colisao(SDL_Rect a, SDL_Rect b)
{
    if(b.x + b.w < a.x) //- b.direita < a.esquerda
        return 0;
    if(b.x > a.x + a.w/2) //- b.esquerda > a.direita
        return 0;
    if(b.y + b.h < a.y) //- b.baixo < a.cima
        return 0;
    if(b.y > a.y + a.h) //- b.cima > a.baixo
        return 0;

    return 1; //- retângulos se colidem
}


int ColisaoPlataforma(SDL_Rect player, SDL_Rect plataforma) // Função pra colidir com as plataformas ( assim ele n cai)
{
    if((player.y + player.h >= plataforma.y) && (player.y + player.h <= plataforma.y + plataforma.h)
            &&  ((player.x + player.w/2) >=plataforma.x) && ((player.x + player.w/2)<= plataforma.x + plataforma.w))
                        return 1;
    else
        return 0;
}
int ChecarPlataforma(SDL_Rect *objeto)
{
    if(ColisaoPlataforma(*objeto,plataforma_1) == 1 && ESCADA_ON == 0 )
    {
        objeto->y = ANDAR_Y[1] - objeto->h;
        return 1;
    }
  	if(ColisaoPlataforma(*objeto,plataforma_2) == 1 && ESCADA_ON == 0)
  	{
        objeto->y = ANDAR_Y[2] - objeto->h;
  		return 2;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_3) == 1 && ESCADA_ON == 0)
  	{
        objeto->y = ANDAR_Y[3] - objeto->h;
  		return 3;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_4) == 1 && ESCADA_ON == 0)
  	{
        objeto->y = ANDAR_Y[4] - objeto->h;
  		return 4;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_5) == 1 && ESCADA_ON == 0)
  	{
        objeto->y = ANDAR_Y[5] - objeto->h;
  		return 5;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_6) == 1 && ESCADA_ON == 0)
    {
        objeto->y = ANDAR_Y[0] - objeto->h;
  		return 6;
  	}

    return 0;
}

int ChecarPlataformaBarril(SDL_Rect *objeto, int acerta) // ESCADA ON N INTERFERE
{
    if(ColisaoPlataforma(*objeto,plataforma_1) == 1 )
    {
        if(acerta)
             objeto->y = ANDAR_Y[1] - objeto->h;

        return 1;
    }
  	if(ColisaoPlataforma(*objeto,plataforma_2) == 1 )
  	{
        if(acerta)
            objeto->y = ANDAR_Y[2] - objeto->h;

  		return 2;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_3) == 1 )
  	{
        if(acerta)
            objeto->y = ANDAR_Y[3] - objeto->h;

  		return 3;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_4) == 1 )
  	{
        if(acerta)
            objeto->y = ANDAR_Y[4] - objeto->h;

  		return 4;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_5) == 1 )
  	{
        if(acerta)
            objeto->y = ANDAR_Y[5] - objeto->h;

  		return 5;
  	}
  	if(ColisaoPlataforma(*objeto,plataforma_6) == 1)
    {
        if(acerta)
            objeto->y = ANDAR_Y[0] - objeto->h;

  		return 6;
  	}

    return 0;
}



int ChecarEscada(SDL_Rect objeto) // Valores pares escadas ruins, valores impares escadas boas.
{
    if(ColisaoPlataforma(objeto, escada_b1) == 1 )
        return 1;
    if(ColisaoPlataforma(objeto, escada_b2) == 1)
        return 3;
    if(ColisaoPlataforma(objeto, escada_b3) == 1)
        return 5;
    if(ColisaoPlataforma(objeto, escada_b4) == 1)
        return 7;
    if(ColisaoPlataforma(objeto, escada_b5) == 1) // Ultima escada, mas acho que ele n sobe
        return 9;
    if(ColisaoPlataforma(objeto, escada_r1) == 1)
        return 2;
    if(ColisaoPlataforma(objeto, escada_r2) == 1)
        return 4;
    if(ColisaoPlataforma(objeto, escada_r3) == 1)
        return 6;

    return 0;
}

//void AJUSTE_QUEDA (SDL_Rect *objeto)

//-----------------------------------Pulo-------------------------------------------------------------------------------
void pulo()
{
    if(TeclaEspace == 1 && PULO_ON == 0 && ESCADA_ON == 0)
    {
        PULO_ON = 1;
        velJ = -11;
    }

    if(PULO_ON == 1)
    {
        if(ChecarPlataforma(&dest_henry)>0)
        {
            if(btnSFx==0)//verifica se ta na plataforma, se está então sai som pulo
            {
                Mix_PlayChannel(0,jump,0);
                Mix_VolumeChunk(jump,60);
            }
        }

        dest_henry.y += velJ;
        velJ +=    velG;


    /*            if(ChecarPlataforma(&henry) == 0)
            {
                fonte_henry.y =  3 * fonte_henry.h;
                if(fonte_henry.x > 11 * fonte_henry.w)
                    fonte_henry.x = 18 * fonte_henry.w;
                else
                    fonte_henry.x = 5 * fonte_henry.w;
            }*/
    }

    if(ChecarPlataforma(&dest_henry) > 0 && ESCADA_ON == 0 ) //Botei henry, pq só ele pula ne. CP > 0 pq ele tem que estar no chao
    {
        PULO_ON = 0;
        velJ = 0;
    }

}
//----------------------------ESCADA----------------------------------------------------------------------//
void escada(SDL_Rect *objeto)
{
    if(ChecarEscada(dest_henry)%2 == 1 && velJ == 0) // movimento da escada
    {   //printf("escada  %d\n =", ChecarEscada(dest_henry));
        if(TeclaCima == 1)
        {
            objeto->y+= velE;//Velocidade da escada
            ESCADA_ON = 1;
        }
        if(TeclaBaixo == 1)
        {
            objeto->y+= -velE;
            ESCADA_ON = 1;
        }
    }
    else
    {
        objeto->y += velJ;
        velJ+= velG;
        ESCADA_ON = 0;

    }
    if(ESCADA_ON == 1 && ChecarPlataforma(&objeto) == 0 && ChecarEscada(*objeto) == 1) //Coloquei só a plataforma 2, mas no jogo vai precisar
    {                                                     // ser todas com exceção da primeira
        velX = 0;
        objeto->x = escada_b1.x - objeto->w/2; //Centraliza na escada p subir
    }
    else if(ESCADA_ON == 1 && ChecarPlataforma(&objeto) == 0 && ChecarEscada(*objeto) == 3) //Coloquei só a plataforma 2, mas no jogo vai precisar
    {                                                     // ser todas com exceção da primeira
        velX = 0;
        objeto->x = escada_b2.x - objeto->w/2; //Centraliza na escada p subir
    }
    else if(ESCADA_ON == 1 && ChecarPlataforma(&objeto) == 0 && ChecarEscada(*objeto) == 5) //Coloquei só a plataforma 2, mas no jogo vai precisar
    {                                                     // ser todas com exceção da primeira
        velX = 0;
        objeto->x = escada_b3.x - objeto->w/2; //Centraliza na escada p subir
    }
    else if(ESCADA_ON == 1 && ChecarPlataforma(&objeto) == 0 && ChecarEscada(*objeto) == 7) //Coloquei só a plataforma 2, mas no jogo vai precisar
    {                                                     // ser todas com exceção da primeira
        velX = 0;
        objeto->x = escada_b4.x - objeto->w/2; //Centraliza na escada p subir
    }
}
void BossAnima()
{
    if(TEMPO%72 < 20)//ANDA P ESQUERDA
    {
        boss_fonte.y = boss_fonte.h;

        if(TEMPO%72 == 0)
        {
            boss_fonte.x = 9 * boss_fonte.w;
            boss_dest.x = 650;

        }
        boss_fonte.x -= boss_fonte.w;
        boss_dest.x -= 6;
        if (boss_fonte.x < 3 * boss_fonte.w)
            boss_fonte.x = 9 * boss_fonte.w;
    }
    else if(TEMPO%72 < 28)//ESPANCA
    {
        boss_fonte.y = 3 * boss_fonte.h;

        if(TEMPO%72 == 20)
            boss_fonte.x = 9 * boss_fonte.w;

        boss_fonte.x -= boss_fonte.w;
    }
    else if(TEMPO%72 < 37)//LANÇA
    {
        boss_fonte.y = 6 * boss_fonte.h;

        if(TEMPO%72 == 28)
            boss_fonte.x = 9 * boss_fonte.w;

        boss_fonte.x -= boss_fonte.w;

        if (boss_fonte.x < 0)
           boss_fonte.x = 9 * boss_fonte.w; // 10 p ja diminuir

    }else if(TEMPO%72 < 46)//BAFADA
    {
        boss_fonte.y = 7 * boss_fonte.h;

        if(TEMPO%72 == 37)
            boss_fonte.x = 9 * boss_fonte.w;

        if(boss_fonte.x > 5 * boss_fonte.w && boss_fonte.x <= 9 * boss_fonte.w)
            boss_fonte.x -= boss_fonte.w;

        if(TEMPO%72 == 40 && !btnSFx)
        {
            Mix_PlayChannel(3,scream,0);
            Mix_VolumeChunk(scream,60);
        }

        if(TEMPO%72 == 45)
            boss_fonte.x = 4 * boss_fonte.w;
    }
    else if(TEMPO%72 < 71) //VOLTA
    {
        boss_fonte.y = boss_fonte.h;

        if(TEMPO%72 == 46)
        {
            boss_fonte.x = 10 * boss_fonte.w;
            boss_dest.x += 30;
        }
        boss_fonte.x += boss_fonte.w;
        boss_dest.x += 6;

        if (boss_fonte.x > 16 * boss_fonte.w)
            boss_fonte.x = 10 * boss_fonte.w;
    }
}
void HenryAnima()
{
    if(ESCADA_ON == 1)//Subir escada
    {
        fonte_henry.y = 7 * fonte_henry.h;

       // if(fonte_henry.x > 11 * fonte_henry.w) // p direita
        //{
            fonte_henry.x = 14 * fonte_henry.w + INCREMENTO_ESCADA;

            if(fonte_henry.x > 15 * fonte_henry.w) // Voltar sprite
            {
                fonte_henry.x = 14 * fonte_henry.w;
                INCREMENTO_ESCADA = 0;
            }
        //}
       // else
       // {
           // fonte_henry.x = 9 * fonte_henry.w - INCREMENTO_ESCADA;

            //if(fonte_henry.x < 8 * fonte_henry.w) // Voltar sprite
           // {
             //   fonte_henry.x = 9 * fonte_henry.w;
            //    INCREMENTO_ESCADA = 0;
          //  }

       // }

    }
    else if (PULO_ON == 1) // Pular
    {
        fonte_henry.y =  3 * fonte_henry.h;

        if(fonte_henry.x > 11 * fonte_henry.w)
        {
            if(velJ < 0)
                fonte_henry.x = 14 * fonte_henry.w;
            else
                fonte_henry.x = 18 * fonte_henry.w;
        }
        else
        {
            if(velJ < 0)
                fonte_henry.x =  9 * fonte_henry.w;
            else
                fonte_henry.x = 5 * fonte_henry.w;
        }
    }
    else if(TeclaDireita == 1) // Andar
    {
        //fonte_henry.y = 2 * fonte_henry.h;
        fonte_henry.x = 12 * fonte_henry.w + INCREMENTO_ANDA;

        if(fonte_henry.x > 19*fonte_henry.w) // Volta pro inicio
        {
            fonte_henry.x = 12 * fonte_henry.w;
            INCREMENTO_ANDA = fonte_henry.w;
        }
    }
    else if(TeclaEsquerda == 1) // Andar
    {
        //fonte_henry.y = 2 * fonte_henry.h;
        fonte_henry.x = 11 * fonte_henry.w + INCREMENTO_ANDA;

        if(fonte_henry.x < 5 * fonte_henry.w) // Volta pro inicio
        {
            fonte_henry.x = 11 * fonte_henry.w;
            INCREMENTO_ANDA = -fonte_henry.w;
        }
    }
    else
    {
        if(fonte_henry.x > 11 * fonte_henry.w) // Direita
        {
            fonte_henry.x = 12 * fonte_henry.w;
            fonte_henry.y = 0;
        }
        else // Esquerda
        {
            fonte_henry.x = 11 * fonte_henry.w;
            fonte_henry.y = 0;
        }

        INCREMENTO_ANDA = 0;
    }








    if(TeclaDireita)
    {
        velX = 15;
        INCREMENTO_ANDA += fonte_henry.w;
        //fonte_henry.y = 2*fonte_henry.h;
    }

    if(TeclaEsquerda)
    {
        velX = -15;
        INCREMENTO_ANDA -= fonte_henry.w;
        //fonte_henry.y = 2*fonte_henry.h;
    }

    if ( (TeclaCima || TeclaBaixo) && ESCADA_ON)
    {

        INCREMENTO_ESCADA += fonte_henry.w;
        //fonte_henry.y = 2*fonte_henry.h;
    }

    if(ESCADA_ON == 0)
    {
        INCREMENTO_ESCADA = 0;
    }
}

void Final()
{
   if(Colisao(dest_henry, premio_dest))
    {
        //puts("ganhou");
        fonte_henry.y = 7 * fonte_henry.h;
        dest_henry.y = ANDAR_Y[5] - dest_henry.h;
        boss_fonte.x = 9 * 2604/20;
        boss_fonte.y = 0;

        explosao_dest.x = dest_henry.x;
        explosao_dest.y = dest_henry.y;


        if(fonte_henry.x > 11 * fonte_henry.w)
            fonte_henry.x = 19 * fonte_henry.w;
        else
            fonte_henry.x = 5 * fonte_henry.w;


        if(btnSFx==0)//verifica se ta na plataforma, se está então sai som pulo
        {
            Mix_PlayChannel(1,somPonto,0);
            Mix_VolumeChunk(somPonto,60);
        }

        SCORE += 1000;
        sprintf(str_score, "%d", SCORE);

        int conta = 0;
        int x_ini = dest_henry.x + 20,
            y_ini = dest_henry.y - 15;

        do
        {
            start = SDL_GetTicks();

            moveCeu();

            SDL_BlitSurface(backmove, &ceu, screen, 0);
            SDL_BlitSurface(background, 0, screen, 0);
            SDL_BlitSurface(boss, &boss_fonte, screen, &boss_dest);

            if(conta < 3)
                SDL_BlitSurface(premio, 0, screen, &premio_dest);

            SDL_Color ouro = {255,215,0};

            if(conta < 6)
            {
                font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);
                desenhaTexto("+ 1000",screen, x_ini, y_ini - 3*conta, ouro);
                TTF_CloseFont(font);
            }

            SDL_BlitSurface(henry, &fonte_henry, screen, &dest_henry);

            if(conta < 3)
            {
                if(fonte_henry.x > 11 * fonte_henry.w)
                    fonte_henry.x += fonte_henry.w;
                else
                    fonte_henry.x -= fonte_henry.w;
            }
            else if(conta == 3)
            {
                if(btnSFx==0)
                {
                    Mix_PlayChannel(4,pof,0);
                    Mix_VolumeChunk(pof,60);
                }

                henry = IMG_Load("imagens/jovem.png"); if(henry == NULL) puts("Não foi possivel abrir jovem.png");
                fonte_henry.w = 266/8;
                fonte_henry.h = 69/2;
                fonte_henry.x = 0;
                fonte_henry.y = 0;

                dest_henry.y = ANDAR_Y[5] - fonte_henry.h;
                dest_henry.x += 15;
            }
            if(explosao_fonte.x < 7 * explosao_fonte.w && conta >=3)
            {
                SDL_BlitSurface(explosao, &explosao_fonte, screen, &explosao_dest);
                explosao_fonte.x += explosao_fonte.w;
            }

            //printf("fonte = %d\n", fonte_henry.x/fonte_henry.w);
            //printf("Explosa.x = %d, henry.x = %d\n", explosao_dest.x, dest_henry.x);

            ControlFPS(15);
            conta++;


        } while(conta < 20);
        VIDAS = 0;
    }
}

void IA (INIMIGO *cadeirante)
{
if(cadeirante->ATIVO == 1)
{
    cadeirante->dest_barril.x += cadeirante->velx_barril;

    if(cadeirante->dest_barril.x < 3 ) // Bate na esquerda vai p direita
    {
        cadeirante->fonte_barril.x = 8 * 930/15; // Vira pra direita
        cadeirante->velx_barril*=-1;
        if(ChecarPlataformaBarril(&cadeirante->dest_barril, 0) == 1) // Volta pro inicio
        {
            cadeirante->dest_barril.x = 500;
            cadeirante->dest_barril.y = ANDAR_Y[5] - cadeirante->fonte_barril.h;
            cadeirante->velx_barril *= -1;
            cadeirante->fonte_barril.x = 6 * 930/15;// Vira pra esquerda
            cadeirante->ANDAR_ATUAL = 5;
            cadeirante->PONTO = 0;
            cadeirante->ATIVO = 0;
        }
    }
    if(cadeirante->dest_barril.x + cadeirante->dest_barril.w >= 800) // Bate na direita vai p esquerda
    {
        cadeirante->fonte_barril.x = 6 * 930/15;// Vira pra esquerda
        cadeirante->velx_barril*=-1;
    }

    if(cadeirante->queda == 0)
    {
        if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1) == 0 && (
        (cadeirante->ANDAR_ATUAL == 5 && cadeirante->dest_barril.x <= 12)  ||
        (cadeirante->ANDAR_ATUAL == 4 && cadeirante->dest_barril.x >= 708) ||
        (cadeirante->ANDAR_ATUAL == 3 && cadeirante->dest_barril.x <= 12)  ||
        (cadeirante->ANDAR_ATUAL == 2 && cadeirante->dest_barril.x >= 695) )   )
        {
            cadeirante->dest_barril.y += CAI;
            cadeirante->dest_barril.x -= cadeirante->velx_barril;
            cadeirante->flag = 0;

        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 0 && !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 8 * 930/15; // Vira pra direita
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;
        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 1 && !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 6 * 930/15; // Vira pra esquerda
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;
        }
    }
    else if(cadeirante->queda == 1)
    {
        if (
        (cadeirante->ANDAR_ATUAL == 5 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_b4.y + escada_b4.h && cadeirante->dest_barril.x <= 120) ||
        (cadeirante->ANDAR_ATUAL == 4 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_b3.y + escada_b3.h && cadeirante->dest_barril.x >= 645) ||
        (cadeirante->ANDAR_ATUAL == 3 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_b2.y + escada_b2.h && cadeirante->dest_barril.x <= 148) ||
        (cadeirante->ANDAR_ATUAL == 2 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_b1.y + escada_b1.h && cadeirante->dest_barril.x >= 465)  )
        {
            cadeirante->dest_barril.y += CAI;
            cadeirante->dest_barril.x -= cadeirante->velx_barril;
            cadeirante->flag = 0;

            if(cadeirante->ANDAR_ATUAL == 5) //Casso precise
            {
                cadeirante->dest_barril.x = 126;
            }
            else if(cadeirante->ANDAR_ATUAL == 4)
            {
                //dest_barril.x = 625;
            }
            else if(cadeirante->ANDAR_ATUAL == 3)
            {
                //dest_barril.x = 164;
            }
            else if(cadeirante->ANDAR_ATUAL == 2)
            {
               // dest_barril.x = 444;
            }


        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 0 && cadeirante->ANDAR_ATUAL != ChecarPlataformaBarril(&cadeirante->dest_barril, 0) && !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 8 * 930/15; // Vira pra direita
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;
        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 1 && cadeirante->ANDAR_ATUAL != ChecarPlataformaBarril(&cadeirante->dest_barril, 0) && !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 6 * 930/15; // Vira pra esquerda
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;

        }
    }
    else if(cadeirante->queda == 2)
    {
        if(ChecarPlataformaBarril(&cadeirante->dest_barril, 0) == 2)
            cadeirante->queda = 1;

        if (
        (cadeirante->ANDAR_ATUAL == 5 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_r3.y + escada_r3.h && cadeirante->dest_barril.x <= 295) ||
        (cadeirante->ANDAR_ATUAL == 4 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_r2.y + escada_r2.h && cadeirante->dest_barril.x >= 513) ||
        (cadeirante->ANDAR_ATUAL == 3 && cadeirante->dest_barril.y + cadeirante->dest_barril.h <= escada_r1.y + escada_r1.h && cadeirante->dest_barril.x <= 337)   )
        {
            cadeirante->dest_barril.y += CAI;
            cadeirante->dest_barril.x -= cadeirante->velx_barril;
            cadeirante->flag = 0;

            if(cadeirante->ANDAR_ATUAL == 5) // Caso precise, se nao pode apagar
            {
                //dest_barril.x = 126;
            }
            else if(cadeirante->ANDAR_ATUAL == 4)
            {
                //dest_barril.x = 625;
            }
            else if(cadeirante->ANDAR_ATUAL == 3)
            {
                //dest_barril.x = 164;
            }

        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 0 && cadeirante->ANDAR_ATUAL != ChecarPlataformaBarril(&cadeirante->dest_barril, 0) &&  !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 8 * 930/15; // Vira pra direita
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;
        }
        else if(ChecarPlataformaBarril(&cadeirante->dest_barril, 1)%2 == 1 && cadeirante->ANDAR_ATUAL != ChecarPlataformaBarril(&cadeirante->dest_barril, 0) && !cadeirante->flag)
        {
            cadeirante->queda = rand()%3;
            cadeirante->fonte_barril.x = 6 * 930/15; // Vira pra esquerda
            cadeirante->velx_barril*=-1;
            cadeirante->ANDAR_ATUAL = ChecarPlataformaBarril(&cadeirante->dest_barril, 0);
            cadeirante->flag = 1;
        }
    }

    cadeirante->score_barril.x = cadeirante->dest_barril.x; //Rect invisivel pra marcar pontos
    cadeirante->score_barril.y = cadeirante->dest_barril.y - 30;

    cadeirante->colide_barril.x = cadeirante->dest_barril.x;
    cadeirante->colide_barril.y = cadeirante->dest_barril.y + 5;


    if( Colisao(dest_henry, cadeirante->score_barril)) // GANHA PONTO
    {
        if( cadeirante->PONTO == 0  && !cadeirante->PONTO)
        {
            if(btnSFx==0)//verifica se ta na plataforma, se está então sai som pulo
            {
                Mix_PlayChannel(1,somPonto,0);
                Mix_VolumeChunk(somPonto,60);
            }
            SCORE+=100;
            cadeirante->PONTO++;
            sprintf(str_score, "%d", SCORE);
            cadeirante->timer = 9;
            cadeirante->sx = dest_henry.x +20;
            cadeirante->sy = dest_henry.y - 10;
        }

    }
    else if(Colisao(dest_henry, cadeirante->colide_barril)) // PERDE VIDA
    {
        //SCORE-= 100;
        //printf("Morreu! Score = %d\n",SCORE); 12-16
        int i, j;

            if (fonte_henry.x < 12 * fonte_henry.w)
            {
                //printf("x = %d\n", dest_henry.x);// N precisa
                dest_henry.x += 15;
                //printf("x = %d\n", dest_henry.x); // N precisa

            }
            else;


        if(btnSFx==0)//verifica se ta na plataforma, se está então sai som pulo
        {
            Mix_PlayChannel(2,smash,0);
            Mix_VolumeChunk(smash,60);
        }

        fonte_henry.y = 5 * fonte_henry.h;
        cadeirante->fonte_barril.y = 5 * cadeirante->fonte_barril.h; //Animando velho

        for(j = 0; j <= 4; j++)
        {

            if (fonte_henry.x > 11 * fonte_henry.w)
                fonte_henry.x = (12 + j) * fonte_henry.w;
            else
                fonte_henry.x = (11 - j) * fonte_henry.w;

            if(cadeirante->fonte_barril.x > 7 * cadeirante->fonte_barril.w && j < 4)
            {
                cadeirante->fonte_barril.x = (8 + j) * cadeirante->fonte_barril.w;
                cadeirante->dest_barril.x-= 4; // P ficar parado quando morre
            }
            else if(cadeirante->fonte_barril.x < 7 * cadeirante->fonte_barril.w && j < 4)
            {
                cadeirante->fonte_barril.x = (6 - j) * cadeirante->fonte_barril.w;
                cadeirante->dest_barril.x+= 4; // P ficar parado quando morre
            }



            moveCeu();

            SDL_BlitSurface(backmove, &ceu, screen, 0);
            SDL_BlitSurface(background, 0, screen, 0);
            SDL_BlitSurface(boss, &boss_fonte, screen, &boss_dest);

            for(i = 0; i < MAX; i++)
            {
                if(cadeirante[i].ATIVO == 1)
                    SDL_BlitSurface(barril, &cadeirante[i].fonte_barril, screen, &cadeirante[i].dest_barril);
            }

            SDL_BlitSurface(henry, &fonte_henry, screen, &dest_henry);

            SDL_Flip(screen);
            SDL_Delay(100);

        }


    font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);
    for (i = 0; i < MAX; i++) // Coloca o score quando bate no boneco
    {
        if(cadeirante[i].timer > 0)
        {
            //font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);
            SDL_Color branco = {255,255,255};
            desenhaTexto("+ 100",screen,cadeirante[i].sx, cadeirante[i].sy, branco);
            cadeirante[i].sy -= 3;
            cadeirante[i].timer--;
        }
    }


        VIDAS--;
        sprintf(str_vidas, "%d", VIDAS);
        SDL_BlitSurface(backdie, 0, screen, 0);
        TeclaDireita = 0;
        TeclaEsquerda = 0;
        SDL_Color branco = {255,255,255};
        font = TTF_OpenFont("fontes/Numero10Clean.ttf",45);
        desenhaTexto("X", screen, 480, 300, branco);
        desenhaTexto(str_vidas, screen, 520, 300, branco);
        TTF_CloseFont(font);

        if(VIDAS == 0)
            SDL_BlitSurface(gameover, 0, screen, 0);

        SDL_Flip(screen);
        SDL_Delay(2000);
        carregaAndarRects();
        TEMPO--;
        //Fazer uma animação aqui de perder vida, e parar todos os rects, e recarregar tudo
    }
}
}

//-------------------------------------// FUNÇÃO DO FUNDO MOVENDO // ----------------------------------------------------
void moveCeu(){
    ceu.x++;

    if(ceu.x + ceu.w>1320) //Controla os cortes da imagem para nao ultrapassar as dimensoes definidas.
        ceu.x=0;
}
//--------------------------------------------------------------------------------------------------------------------

void desenhaTexto(char* texto, SDL_Surface* dst, int x, int y, SDL_Color cor)
{
    SDL_Surface* src = TTF_RenderText_Blended(font, texto, cor);
    SDL_Rect dst_rect = {x, y, 0, 0};

    SDL_BlitSurface(src, 0, dst, &dst_rect);

    SDL_FreeSurface(src);
}

char* escreveNome()
{
    int escrevendo=0, cont=0, i, direita=0, flag=1, conta=1, flags=1;
    char *aux = (char*)malloc(11*sizeof(char));
//------------------LIMPANDO LIXO DE MEMORIA-------------------//
    for(i=0;i<11;i++){ aux[i]='\0'; }
//-------------------------------------------------------------//
    SDL_Surface *fundo_escreve_score, *txtBox, *cole, *cole_ao_contrario;//, *hr;

    fundo_escreve_score = IMG_Load("imagens/asilo.png");
    if(fundo_escreve_score==NULL){ printf("não abriu asilo.png\n"); }

    txtBox = IMG_Load("imagens/comboBox.png");
    if(txtBox==NULL){ printf("não abriu comboBox.png\n"); }

    cole = IMG_Load("imagens/henry_pequeno.png");
    if(cole == NULL){ printf("não abriu henry_pequeno.png\n"); }

    henry        = IMG_Load ("imagens/henry.png");      if (henry == NULL) puts("Não foi possível carregar henry.png");

    cole_ao_contrario = IMG_Load("imagens/henry_pequeno2.png");
    if(cole_ao_contrario == NULL){ printf("não abriu henry_pequeno2.png\n"); }

    //hr = IMG_Load("imagens/henry_menu.png");
    //if(hr == NULL){ printf("não abriu henry_do_escreve_nome\n"); }

    SDL_Rect destTxtBox, fonte_oculos, dest_oculos, fonte_cole, dest_cole,
             fonte_cole_ac, dest_cole_ac, fonte_hr_rodando, dest_hr_rodando;

    fonte_oculos.x = 12 * 196/3;
    fonte_oculos.y = 0;//328/8;
    fonte_oculos.w = 1564/24;
    fonte_oculos.h = 328/8;

    dest_oculos.x = 156;
    dest_oculos.y = 279;
    dest_oculos.h = fonte_oculos.h;
    dest_oculos.w = fonte_oculos.w;

    destTxtBox.x = 215;
    destTxtBox.y = 168;
    destTxtBox.h = 121;
    destTxtBox.w = 355;

    fonte_cole.x = 3 * 1177/12;
    fonte_cole.y = 0;
    fonte_cole.w = 1177/12;
    fonte_cole.h = 160;

    dest_cole.x = 100;
    dest_cole.y = 100;
    dest_cole.w = fonte_cole.w;
    dest_cole.h = fonte_cole.h;

    fonte_cole_ac.x = 8 * 1177/12;
    fonte_cole_ac.y = 0;
    fonte_cole_ac.w = 1177/12;
    fonte_cole_ac.h = 160;

    dest_cole_ac.x = 600;
    dest_cole_ac.y = 100;
    dest_cole_ac.w = fonte_cole.w;
    dest_cole_ac.h = fonte_cole.h;

    fonte_hr_rodando.x = 196/3;
    fonte_hr_rodando.y = 7 *328/8;
    fonte_hr_rodando.w = 1564/24;
    fonte_hr_rodando.h = 328/8;

    dest_hr_rodando.x = 559;
    dest_hr_rodando.y = 279;
    dest_hr_rodando.h = fonte_oculos.h;
    dest_hr_rodando.w = fonte_oculos.w;

    SDL_Color corParab = {255,255,255};
    SDL_Color corNome = {30,30,30};

    SDL_BlitSurface(fundo_escreve_score, 0, screen, 0);
    SDL_BlitSurface(txtBox, 0, screen, &destTxtBox);

    font = TTF_OpenFont("fontes/Armageda.ttf",35);//para o player escrever o nome

    SDL_Flip(screen);//para que a tela apareça a 1° vez
    while(escrevendo==0)
    {
        start = SDL_GetTicks();
        while (SDL_PollEvent(&event)) // Loop de eventos
        {

            if (event.type == SDL_MOUSEBUTTONDOWN) // Se o usuário moveu o cursor sobre a tela
            {
                //printf("X = %d, Y = %d\n", event.motion.x, event.motion.y);

            }

            if(event.type == SDL_KEYDOWN)
            {
              switch(event.key.keysym.sym){

                case SDLK_RIGHT:
                    direita=1;
                    break;
                case SDLK_a:
                    if(cont<10)
                    {
                        aux[cont]='A';
                        cont++;
                    }
                    break;
                case SDLK_b:
                    if(cont<10)
                    {
                        aux[cont]='B';
                        cont++;
                    }
                    break;
                case SDLK_c:
                    if(cont<10)
                    {
                        aux[cont]='C';
                        cont++;
                    }
                    break;
                case SDLK_d:
                    if(cont<10)
                    {
                        aux[cont]='D';
                        cont++;
                    }
                    break;
                case SDLK_e:
                    if(cont<10)
                    {
                        aux[cont]='E';
                        cont++;
                    }
                    break;
                case SDLK_f:
                    if(cont<10)
                    {
                        aux[cont]='F';
                        cont++;
                    }
                    break;
                case SDLK_g:
                    if(cont<10)
                    {
                        aux[cont]='G';
                        cont++;
                    }
                    break;
                case SDLK_h:
                    if(cont<10)
                    {
                        aux[cont]='H';
                        cont++;
                    }
                    break;
                case SDLK_i:
                    if(cont<10)
                    {
                        aux[cont]='I';
                        cont++;
                    }
                    break;
                case SDLK_j:
                    if(cont<10)
                    {
                        aux[cont]='J';
                        cont++;
                    }
                    break;
                case SDLK_k:
                    if(cont<10)
                    {
                        aux[cont]='K';
                        cont++;
                    }
                    break;
                case SDLK_l:
                    if(cont<10)
                    {
                        aux[cont]='L';
                        cont++;
                    }
                    break;
                case SDLK_m:
                    if(cont<10)
                    {
                        aux[cont]='M';
                        cont++;
                    }
                    break;
                case SDLK_n:
                    if(cont<10)
                    {
                        aux[cont]='N';
                        cont++;
                    }
                    break;
                case SDLK_o:
                    if(cont<10)
                    {
                        aux[cont]='O';
                        //sprintf(aux,"%s%c",aux,'o');
                        cont++;
                        //printf("%s\n",aux);
                    }
                    break;
                case SDLK_p:
                    if(cont<10)
                    {
                        aux[cont]='P';
                        cont++;
                    }
                    break;
                case SDLK_q:
                    if(cont<10)
                    {
                        aux[cont]='Q';
                        cont++;
                    }
                    break;
                case SDLK_r:
                    if(cont<10)
                    {
                        aux[cont]='R';
                        cont++;
                    }
                    break;
                case SDLK_s:
                    if(cont<10)
                    {
                        aux[cont]='S';
                        cont++;
                    }
                    break;
                case SDLK_t:
                    if(cont<10)
                    {
                        aux[cont]='T';
                        cont++;
                    }
                    break;
                case SDLK_u:
                    if(cont<10)
                    {
                        aux[cont]='U';
                        cont++;
                    }
                    break;
                case SDLK_v:
                    if(cont<10)
                    {
                        aux[cont]='V';
                        cont++;
                    }
                    break;
                case SDLK_w:
                    if(cont<10)
                    {
                        aux[cont]='W';
                        cont++;
                    }
                    break;
                case SDLK_x:
                    if(cont<10)
                    {
                        aux[cont]='X';
                        cont++;
                    }
                    break;
                case SDLK_y:
                    if(cont<10)
                    {
                        aux[cont]='Y';
                        cont++;
                    }
                    break;
                case SDLK_z:
                    if(cont<10)
                    {
                        aux[cont]='Z';
                        cont++;
                    }
                    break;
                case SDLK_0:
                    if(cont<10)
                    {
                        aux[cont]='0';
                        cont++;
                    }
                    break;
                case SDLK_1:
                    if(cont<10)
                    {
                        aux[cont]='1';
                        cont++;
                    }
                    break;
                case SDLK_2:
                    if(cont<10)
                    {
                        aux[cont]='2';
                        cont++;
                    }
                    break;
                case SDLK_3:
                    if(cont<10)
                    {
                        aux[cont]='3';
                        cont++;
                    }
                    break;
                case SDLK_4:
                    if(cont<10)
                    {
                        aux[cont]='4';
                        cont++;
                    }
                    break;
                case SDLK_5:
                    if(cont<10)
                    {
                        aux[cont]='5';
                        cont++;
                    }
                    break;
                case SDLK_6:
                    if(cont<10)
                    {
                        aux[cont]='6';
                        cont++;
                    }
                    break;
                case SDLK_7:
                    if(cont<10)
                    {
                        aux[cont]='7';
                        cont++;
                    }
                    break;
                case SDLK_8:
                    if(cont<10)
                    {
                        aux[cont]='8';
                        cont++;
                    }
                    break;
                case SDLK_9:
                    if(cont<10)
                    {
                        aux[cont]='9';
                        cont++;
                    }
                    break;
                case SDLK_BACKSPACE:
                    if(cont>0)
                    {
                        aux[cont-1]='\0';
                        cont--;
                    }
                    break;
                case SDLK_RETURN:
                    //printf("\n%s\n","Enter");
                    return aux;
                    escrevendo=1;
                    break;
              }//switch
            }//button down
          if(event.type == SDL_KEYUP)
          {
            switch(event.key.keysym.sym){

                case SDLK_RIGHT:
                    direita=0;
                    break;

            }
          } //button up

        }//pool event

        if(fonte_oculos.x < (16 * 196/3) && flag==1)//HENRY DO OCULOS
        {
            fonte_oculos.x += 196/3;
        }else
        {
            if(fonte_oculos.x > 12 * 196/3)
            {
                flag=-1;
                fonte_oculos.x += -196/3;
            }else
                flag=1;
        }

        if(conta%3==0)//COLE
        {
            if(fonte_cole.x < (4* 1177/12))
            {
                fonte_cole.x += fonte_cole.w;
            }else
                fonte_cole.x = 3* 1177/12;

            if(fonte_cole_ac.x <= (8* 1177/12))
            {
                fonte_cole_ac.x -= fonte_cole_ac.w;
                if(fonte_cole_ac.x < (7* 1177/12))
                {
                    fonte_cole_ac.x = (8 * 1177/12);
                }
            }
        }
        conta++;

        if(fonte_hr_rodando.x < (21*196/3) && flags==1)
        {

            fonte_hr_rodando.x+=fonte_hr_rodando.w;
            if(fonte_hr_rodando.x==11*fonte_hr_rodando.w)
                fonte_hr_rodando.x = 14*fonte_hr_rodando.w;
            //printf("%d\n",fonte_hr_rodando.x);
        }else{
            if(fonte_hr_rodando.x > 196/3){
                flags=-1;
                fonte_hr_rodando.x -= fonte_hr_rodando.w;
                //printf("%d\n",fonte_hr_rodando.x);
                if(fonte_hr_rodando.x==14*fonte_hr_rodando.w)
                    fonte_hr_rodando.x = 10*fonte_hr_rodando.w;
            }else{
                flags=1;
                //printf("uma repetição\n");
            }
        }

        SDL_BlitSurface(fundo_escreve_score,0,screen,0);
        SDL_BlitSurface(txtBox, 0, screen, &destTxtBox);
        desenhaTexto(aux,screen,281,215,corNome);
        TTF_CloseFont(font);//para sempre printar as palavras na tela
            //cor.r = cor.g = cor.r = 255;
          font = TTF_OpenFont("fontes/Numero10Clean.ttf",35);
            desenhaTexto("Parabens!",screen,287,65,corParab);
            desenhaTexto("Novo High Score!",screen,227,100,corParab);
          TTF_CloseFont(font);

          font = TTF_OpenFont("fontes/Numero10Clean.ttf",20);
            desenhaTexto("Informe seu nome:", screen, 285, 149, corParab);
            desenhaTexto("Aperte enter para continuar.", screen, 213, 303, corParab);
          TTF_CloseFont(font);
        font = TTF_OpenFont("fontes/Armageda.ttf",35);
          //cor.r = cor.g = cor.b = 0;
        SDL_BlitSurface(henry, &fonte_oculos, screen, &dest_oculos);
        SDL_BlitSurface(henry, &fonte_hr_rodando, screen, &dest_hr_rodando);
        SDL_BlitSurface(cole, &fonte_cole, screen, &dest_cole);
        SDL_BlitSurface(cole_ao_contrario, &fonte_cole_ac, screen, &dest_cole_ac);

        SDL_Flip(screen);

        ControlFPS(10);
    }//principal
    free(aux);
    SDL_FreeSurface(txtBox);
    SDL_FreeSurface(fundo_escreve_score);
    SDL_FreeSurface(cole);
    SDL_FreeSurface(cole_ao_contrario);
    //SDL_FreeSurface(hr);
    TTF_CloseFont(font);
}

void abertura()
{
    char diretorio[3001];
    int na_abertura=0, posiSprite=0, numeroSprite=1;

    SDL_Color cor = {255,255,255};

    SDL_Surface *abert;

    abert = IMG_Load("imagens/animacao/intro0.jpg");

    if(abert == NULL){ printf("Erro ao carregar a intro: %s\n", IMG_GetError()); }

    SDL_Rect fonte_ab;

    fonte_ab.x = 0;
    fonte_ab.y = 0;
    fonte_ab.w = 9600/12;
    fonte_ab.h = 3600/6;

    Mix_FadeInMusic(music,-1,2000);//inicia o som gradualmente em 7seg

    while(na_abertura==0)//loop principal do menu
    {
        start = SDL_GetTicks();

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN) // Se o usuário apertou um botão do teclado
            {
                switch(event.key.keysym.sym)
                {
                    /*case SDLK_LEFT: //Movimento para Esquerda
                        /printf("apertou esquerda.\n");
                        fonte_ab.x = 0;
                        break;*/
                    case SDLK_RETURN:
                        //printf("enter.\n");
                        na_abertura=1;
                        Mix_FadeOutMusic(1000);//musica sai de execução em 1,1 segungos
                        music = Mix_LoadMUS("sons/LUTA_110-highway-in-the-sky.mp3");
                        Mix_FadeInMusic(music,-1,4000);//inicia o som gradualmente em 7seg
                        break;
                }

            }
        }

        SDL_BlitSurface(abert, &fonte_ab, screen, 0);
        font = TTF_OpenFont("fontes/Silvery Tarjey.ttf",20);
        desenhaTexto("Aperte o enter para continuar...", screen, 551, 580, cor);
        TTF_CloseFont(font);

        if(fonte_ab.x <9600)
        {
            fonte_ab.x += 800;
            numeroSprite++;
            //printf("numero: %d\n",numeroSprite);
            if(numeroSprite==720)
            {
                Mix_FadeOutMusic(1000);//musica sai de execução em 1,1 segungos
                music = Mix_LoadMUS("sons/LUTA_110-highway-in-the-sky.mp3");
                Mix_FadeInMusic(music,-1,4000);//inicia o som gradualmente em 7seg
                Mix_VolumeMusic(30);//inicia a musica com 90 de 128 do volume
            }
        }else
            if(fonte_ab.x == 9600)
            {
                fonte_ab.y += 600;
                if(fonte_ab.y == 3600)
                {
                    posiSprite++;
                    if(posiSprite==24)
                    {
                        na_abertura=1;
                        break;
                    }
                    sprintf(diretorio,"imagens/animacao/intro%d.jpg",posiSprite);
                    abert = IMG_Load(diretorio);
                    if(abert == NULL){ printf("Erro ao carregar a intro: %s\n", IMG_GetError()); }
                    fonte_ab.y=0;
                }
                fonte_ab.x=0;
            }

        if(na_abertura==1)
        {
            break;
        }

        SDL_Flip(screen);
        ControlFPS(26);
    }
    SDL_FreeSurface(abert);
}
void rodaMenu()
{
    int no_menu=0,tela=0;

    int btnIniciar=0,btnRecord=0,btnSair=0,
        btnVoltar=0,vbb=0,btnComoJogar=0, btnCreditos=0;
//-----------------------Surfaces de Botões-------------------//
    SDL_Surface *botoes,*btVoltar,*btSom,*btSFX,*bab,*drag,*cs,*henry_menu;
//------------------------------------------------------------//

//----------------------carregando imagens--------------------//
    fundo_menu = IMG_Load("imagens/menu.png");
    btVoltar = IMG_Load("imagens/voltar.png");
    botoes = IMG_Load("imagens/placas_sprite.png");
    if(btnSom==0)
        btSom = IMG_Load("imagens/volume_on.png");
    else
        btSom = IMG_Load("imagens/volume_off.png");
    btSFX = IMG_Load("imagens/sfx_on.png");
    henry_menu = IMG_Load("imagens/henry_menu.png");

    bab = IMG_Load("obj/Source/Data/bab.png");
    drag = IMG_Load("imagens/drag.jpg");
    cs = IMG_Load("imagens/cs.jpg");
//------------------------------------------------------------//

//------------------------Rects de Botões---------------------//
    SDL_Rect fonte_iniciar,dest_iniciar,
             fonte_creditos,dest_creditos,
             fonte_record,dest_record,
             fonte_comoJogar,dest_comoJogar,
             fonte_sair, dest_sair,
             fonte_voltar, dest_voltar,
             dest_som, dest_sfx, dest_henry_menu,
             dest_bab, dest_drag, dest_cs;
//------------------------------------------------------------//

//------------------Tamanho/Valores Rects Botões--------------//
        fonte_iniciar.x = 0;//208
        fonte_iniciar.y = 0;//203
        fonte_iniciar.h = 96;
        fonte_iniciar.w = 125;

        dest_iniciar.x = 64;
        dest_iniciar.y = 94;
        dest_iniciar.h = fonte_iniciar.h;
        dest_iniciar.w = fonte_iniciar.w;

        fonte_record.x = 0;
        fonte_record.y = 101;
        fonte_record.h = 96;
        fonte_record.w = 125;

        dest_record.x = 64;
        dest_record.y = 195;
        dest_record.h = fonte_record.h;
        dest_record.w = fonte_record.w;

        fonte_comoJogar.x = 0;
        fonte_comoJogar.y = 202;
        fonte_comoJogar.h = 96;
        fonte_comoJogar.w = 125;

        dest_comoJogar.x = 64;
        dest_comoJogar.y = 296;
        dest_comoJogar.h = fonte_comoJogar.h;
        dest_comoJogar.w = fonte_comoJogar.w;

        fonte_creditos.x = 0;
        fonte_creditos.y = 303;
        fonte_creditos.h = 96;
        fonte_creditos.w = 125;

        dest_creditos.x = 64;
        dest_creditos.y = 397;
        dest_creditos.h = fonte_creditos.h;
        dest_creditos.w = fonte_creditos.w;

        fonte_sair.x = 0;
        fonte_sair.y = 404;
        fonte_sair.h = 96;
        fonte_sair.w = 125;

        dest_sair.x = 64;
        dest_sair.y = 498;
        dest_sair.h = fonte_sair.h;
        dest_sair.w = fonte_sair.w;

        fonte_voltar.x = 0;
        fonte_voltar.y = 0;
        fonte_voltar.h = 62;
        fonte_voltar.w = 82;

        dest_voltar.x = 30;
        dest_voltar.y = 450;
        dest_voltar.h = fonte_voltar.h;
        dest_voltar.w = fonte_voltar.w;

        dest_som.x = 77;//70;
        dest_som.y = 42;//50;
        dest_som.h = 35;
        dest_som.w = 34;

        dest_sfx.x = 127;//125;
        dest_sfx.y = 30;//38;
        dest_sfx.h = 61;
        dest_sfx.w = 57;

        dest_bab.x = 546;
        dest_bab.y = 33;
        dest_bab.h = 270;
        dest_bab.w = 206;

        dest_drag.x = 62;
        dest_drag.y = 33;
        dest_drag.h = 270;
        dest_drag.w = 206;

        dest_cs.x = 300;
        dest_cs.y = 33;
        dest_cs.h = 270;
        dest_cs.w = 206;

        dest_henry_menu.x = 490;
        dest_henry_menu.y = 321;
        dest_henry_menu.h = 279;
        dest_henry_menu.w = 274;
//----------------------------------------------------------//

//---------------função printa record-----------------------//
    void escreveRecord()
    {
        int xA=128, yA=177, xB=580, yB=177;
        FILE* arqRecord = fopen("score","r");
        char str[50];
        int cont=1;
        SDL_Color cor = {220,220,220};
        font = TTF_OpenFont("fontes/Numero10Clean.ttf",35);
        desenhaTexto("High Score",screen,290,29,cor);
        while(fgets(str,50,arqRecord)!=NULL)
        {
            if(cont==1)
            {
                desenhaTexto(str,screen,88,100,cor);
                cont++;
            }else
                if(cont==2)
                {
                    desenhaTexto(str,screen,544,100,cor);
                    cont++;
                    TTF_CloseFont(font);
                }else{
                    cor.b = 255;
                    cor.g = 255;
                    cor.r = 255;
                    if(cont%2==0)
                    {
                        desenhaTexto(str,screen,xB,yB,cor);
                        yB += 53;
                        cont++;
                        TTF_CloseFont(font);
                    }else{
                        font = TTF_OpenFont("fontes/Armageda.ttf",25);
                        desenhaTexto(str,screen,xA,yA,cor);
                        yA += 53;
                        cont++;
                    }
                }
        }
        fclose(arqRecord);
    }
//----------------------------------------------------------//

//---------------função que desloca os rects----------------//
    void deslocaRects()
    {
        dest_iniciar.x = dest_creditos.x = dest_comoJogar.x = dest_record.x = dest_sair.x = dest_som.x = 801;
        dest_iniciar.y = dest_creditos.y = dest_comoJogar.y = dest_record.y = dest_sair.y = dest_sfx.y = 801;

        dest_voltar.x = 50;
        dest_voltar.y = 530;
    }
//----------------------------------------------------------//

//------------------------bab :) ---------------------------//
    int fcBab(int x, int y)
    {
        if(x>=62 && x<=232 && y>=118 && y<=291)
        {
            return 1;
        }
        if(x>=311 && x<=482 && y>=118 && y<=291)
        {
            return 2;
        }
        if(x>=560 && x<=729 && y>=118 && y<=291)
        {
            return 3;
        }

        return 0;
    }
//----------------------------------------------------------//

    SDL_Rect aux; //Cirando um rect auxiliar para testar colisão com botões
    aux.h=1;
    aux.w=1;

//-------------------------------------------------------------//
    if(btnSom==0 &&first==2)
    {
        Mix_FadeInMusic(music,-1,7000);//inicia o som gradualmente em 7seg
    }
    while(no_menu==0)//loop principal do menu
    {
        while (SDL_PollEvent(&event)) // Loop de eventos
        {

            if (event.type == SDL_MOUSEBUTTONDOWN) // Se o usuário moveu o cursor sobre a tela
            {
                aux.x = event.motion.x;//passando coordenadas x e y para onde ser acriado o rect auxiliar
                aux.y = event.motion.y;

                //printf("X = %d, Y = %d\n", event.motion.x, event.motion.y);

                if(Colisao(aux,dest_iniciar)==1)
                {
                    btnIniciar=1;
                    fonte_iniciar.x = 129;//botão selecionado
                    fonte_iniciar.y = 0;
                    SDL_BlitSurface(botoes,&fonte_iniciar,screen,&dest_iniciar);
                    //printf("Apertou iniciar\n");
                }

                if(Colisao(aux,dest_record)==1)
                {
                    btnRecord=1;
                    fonte_record.x = 129;//botão selecionado
                    fonte_record.y = 101;
                    SDL_BlitSurface(botoes, &fonte_record, screen, &dest_record);
                    //printf("Apertou Recordes\n");
                }

                if(Colisao(aux,dest_comoJogar)==1)
                {
                    btnComoJogar=1;
                    fonte_comoJogar.x = 129;
                    fonte_comoJogar.y = 202;
                    SDL_BlitSurface(botoes, &fonte_comoJogar, screen, &dest_comoJogar);
                    //printf("Apertou como jogar\n");
                }

                if(Colisao(aux,dest_creditos)==1)
                {
                    btnCreditos=1;
                    fonte_creditos.x = 129;
                    fonte_creditos.y = 303;
                    SDL_BlitSurface(botoes, &fonte_creditos, screen, &dest_creditos);
                    //printf("Apertou Creditos\n");
                }

                if(Colisao(aux,dest_sair)==1)
                {
                    btnSair=1;
                    fonte_sair.x = 129;
                    fonte_sair.y = 404;
                    SDL_BlitSurface(botoes, &fonte_sair, screen, &dest_sair);
                    //printf("Apertou Sair\n");
                }

                if(Colisao(aux,dest_voltar)==1)
                {
                    btnVoltar=1;
                    fonte_voltar.x = 82;
                    fonte_voltar.y = 0;
                    SDL_BlitSurface(btVoltar, &fonte_voltar, screen, &dest_voltar);
                    //printf("Apertou voltar.\n");
                }
                if(Colisao(aux,dest_som)==1)
                {
                    if(btnSom==0)
                    {
                        //printf("colidiu com volume on\n");
                        btSom = IMG_Load("imagens/volume_off.png");
                        Mix_HaltMusic();//função que aparentemente para todos os sons
                        btnSom=1;
                    }else{
                        //printf("colidiu com volume off\n");
                        btSom = IMG_Load("imagens/volume_on.png");
                        music = Mix_LoadMUS("sons/LUTA_110-highway-in-the-sky.mp3");
                        Mix_PlayMusic(music,-1);//incia o som em music gradualmente
                        btnSom=0;
                    }

                }
                if(Colisao(aux,dest_sfx)==1)
                {
                    if(btnSFx==0)
                    {
                        //printf("colidiu com sfx on\n");
                        btSFX = IMG_Load("imagens/sfx_off.png");
                        Mix_HaltChannel(1);
                        btnSFx=1;
                    }else{
                        //printf("colidiu com sfx off\n");
                        btSFX = IMG_Load("imagens/sfx_on.png");
                        Mix_Resume(1);
                        btnSFx=0;
                    }
                }

            }

            if (event.type == SDL_MOUSEBUTTONUP) // Se o usuário moveu o cursor sobre a tela
            {
                /*if( (event.motion.x>=356 && event.motion.x>=401) && (event.motion.y >= 139 && event.motion.y >= 179) )
                {
                    //printf("entrou\n");
                    escreveNome();  //TESTANDO ESCREVENOME
                }*/

                if(btnIniciar==1)
                {
                    btnIniciar=0;
                    fonte_iniciar.x = 0;
                    fonte_iniciar.y = 0;
                    no_menu=1;
                }

                if(btnRecord==1)
                {
                    btnRecord=0;
                    deslocaRects();
                    fonte_record.x = 0;//botão normal
                    fonte_record.y = 101;
                    tela=1;
                }

                if(btnComoJogar==1)
                {
                    btnComoJogar=0;
                    deslocaRects();

                    fonte_comoJogar.x = 0;
                    fonte_comoJogar.y = 202;
                    tela=2;
                }

                if(btnCreditos==1)
                {
                    btnCreditos=0;
                    deslocaRects();
                    fonte_creditos.x = 0;
                    fonte_creditos.y = 303;
                    tela=3;
                }

                if(btnSair==1)
                {
                    btnSair=0;
                    fonte_sair.x = 0;
                    fonte_sair.y = 404;
                    no_menu=1;
                    done=1;
                    repetirPartida=1;
                }

                if(btnVoltar==1)
                {
                    btnVoltar=0;

                    dest_iniciar.x = dest_comoJogar.x = dest_creditos.x = dest_record.x = dest_sair.x = 64;
                    dest_iniciar.y = 94; dest_record.y = 195; dest_comoJogar.y = 296;

                    dest_som.x = 77;    dest_som.y = 42;
                    dest_sfx.x = 127;   dest_sfx.y = 30;

                    dest_creditos.y = 397; dest_sair.y = 498;

                    dest_voltar.x = dest_voltar.y =801;

                    fonte_voltar.x = 0;
                    fonte_voltar.y = 0;
                    SDL_BlitSurface(btVoltar, 0, screen, &dest_voltar);
                    fundo_menu = IMG_Load("imagens/menu.png");
                    tela=0;
                }

            }
        }
        switch(tela){
            case 0:
                fundo_menu = IMG_Load("imagens/menu.png");
                SDL_BlitSurface(fundo_menu,0,screen,0);
                SDL_BlitSurface(btSom, 0,screen,&dest_som);
                SDL_BlitSurface(btSFX, 0,screen,&dest_sfx);
                SDL_BlitSurface(botoes, &fonte_iniciar, screen, &dest_iniciar);
                SDL_BlitSurface(botoes, &fonte_record, screen, &dest_record);
                SDL_BlitSurface(botoes, &fonte_comoJogar, screen, &dest_comoJogar);
                SDL_BlitSurface(botoes, &fonte_creditos, screen, &dest_creditos);
                SDL_BlitSurface(botoes, &fonte_sair, screen, &dest_sair);
                SDL_BlitSurface(henry_menu, 0,screen, &dest_henry_menu);
                break;
             case 1:
                fundo_menu = IMG_Load("imagens/record.png");
                SDL_BlitSurface(fundo_menu,0,screen,0);
                SDL_BlitSurface(btVoltar, &fonte_voltar, screen, &dest_voltar);
                escreveRecord();
                break;
            case 2:
                fundo_menu = IMG_Load("imagens/como_jogar.png");
                SDL_BlitSurface(fundo_menu,0,screen,0);
                SDL_BlitSurface(btVoltar, &fonte_voltar, screen, &dest_voltar);
                break;
            case 3:
                fundo_menu = IMG_Load("imagens/creditos.png");
                SDL_BlitSurface(fundo_menu,0,screen,0);
                SDL_BlitSurface(btVoltar, &fonte_voltar, screen, &dest_voltar);
                vbb=fcBab(event.motion.x,event.motion.y);
                switch(vbb)
                {
                    case 1:
                        SDL_BlitSurface(drag, 0, screen, &dest_drag);
                        break;
                    case 2:
                        SDL_BlitSurface(cs, 0, screen, &dest_cs);
                        break;
                    case 3:
                        SDL_BlitSurface(bab, 0, screen, &dest_bab);
                        break;
                }
                break;
        }
        SDL_Flip(screen);
    }
    //printf("saiu do menu\n");
    if(btnSom==0)
    {
        Mix_FadeOutMusic(1100);//musica sai de execução em 1,1 segungos
        music = Mix_LoadMUS("sons/FUNDO1.mp3");
        Mix_FadeInMusic(music,-1,4000);//inicia o som gradualmente em 7seg
        Mix_VolumeMusic(30);//inicia a musica com 90 de 128 do volume
    }
    SDL_FreeSurface(fundo_menu);
    SDL_FreeSurface(botoes);
    SDL_FreeSurface(btVoltar);
    SDL_FreeSurface(btSom);
    SDL_FreeSurface(btSFX);
    SDL_FreeSurface(bab);
    SDL_FreeSurface(henry_menu);
}



void Recorde(int ponto_atual)
{
//-------------------TROCA SONS, DAR EFEITO DELAY-------------------------//
        music = Mix_LoadMUS("sons/FUNDO2.mp3");
        Mix_FadeInMusic(music,-1,1100);//inicia o som gradualmente em 7seg
        Mix_VolumeMusic(90);//inicia a musica com 90 de 128 do volume
//------------------------------------------------------------------------//

        FILE *arq = fopen("score", "r+");
        if( arq == NULL )
        {
            puts("Não foi possivel abrir os recordes.");
            return -1;
        }

        JOGADOR player[6];
        int i , j, tam = 6;

        fscanf(arq, "%s%s", player[0].nome, player[0].nome); // To usando isso pra não ler as duas primeiras linhas

        //Lendo do arquivo e salvando
        for(i = 0; i < tam; i++ )
        {
            fscanf(arq, "%s%d", player[i].nome, &player[i].pontos);
            //printf("Nome: %s\n"
            //       "Pontuação: %d\n", player[i].nome, player[i].pontos);
        }


        JOGADOR temp, novo;

        //puts("Digite sua pontuação:");
        //scanf("%d", &novo.pontos);
        novo.pontos = ponto_atual;

        if ( novo.pontos > player[tam - 1].pontos )
        {
            strcpy(novo.nome, escreveNome());//jogador informa o nome aqui

            player[tam - 1] = novo;

            //Ordenando as pontuações
            for( j = 0; j < tam - 1; j++)
            {
                for(i = j + 1; i < tam; i++ )
                {
                    if( player[j].pontos < player[i].pontos )
                    {
                        temp = player[j];
                        player[j] = player[i];
                        player[i] = temp;
                    }
                }
            }

            rewind(arq); // Voltando ao inicio do arquivo

            //puts("--------------------NEW---------------");

            fprintf(arq, "JOGADOR\nPONTOS\n");

            //Atualizando o arquivo
            for(i = 0; i < tam; i++ )
            {
                //fscanf(arq, "%s%d", player[i].nome, &player[i].pontos);
                //printf("Nome: %s\n"
                //       "Pontuação: %d\n", player[i].nome, player[i].pontos);
                fprintf(arq, "%s\n%d\n", player[i].nome, player[i].pontos);
            }

        }
        fclose(arq);
        Mix_FadeOutMusic(1100);//musica sai de execução em 1,1 segungos

    }



int main(int argc, char **args){



  while(repetirPartida==0)
  {

    Inicio();
    carregaAndarRects();

    if(first==0)
    {
        abertura();
        first=1;
    }else
        first=2;

    rodaMenu();

    int i;

//------------------------------------------------------------------------------------------------------------------
  sprintf(str_score, "%d", 0);
  sprintf(str_vidas, "%d", 3);
  while (done == 0 && VIDAS > 0) // Enquanto não concluir
  {
  start = SDL_GetTicks();
    while (SDL_PollEvent(&event)) // Loop de eventos
    {
        // Verifica se o evento mais antigo é do tipo SDL_QUIT
        if (event.type == SDL_QUIT) // Se o usuário clicou para fechar a janela
            done = 1; // Encerre o loop
        if (event.type == SDL_MOUSEBUTTONDOWN) // Se o usuário moveu o cursor sobre a tela
        {
            //printf("X = %d, Y = %d\n", event.motion.x, event.motion.y);
        }
    }
//--------------------------------------------_Controles------------------------------------------------------------
     // Verifica se o evento mais antigo é do tipo SDL_KEYDOWN
    if (event.type == SDL_KEYDOWN) // Se o usuário apertou um botão do teclado
        {
            // Verifica qual tecla foi apertada

            switch(event.key.keysym.sym){
            case SDLK_LEFT: //Movimento para Esquerda
            {
                if(ESCADA_ON == 0)
                {
                    TeclaDireita = 0;
                    fonte_henry.y = 2 * fonte_henry.h;
                    fonte_henry.x = 11 * fonte_henry.w + INCREMENTO_ANDA;
                    TeclaEsquerda = 1;
                }
                break;
            }
            case SDLK_RIGHT: //Movimento para Direita
            {
                if(ESCADA_ON == 0)
                {
                    TeclaEsquerda = 0;
                    fonte_henry.y = 2 * fonte_henry.h;
                    fonte_henry.x = 12 * fonte_henry.w + INCREMENTO_ANDA;
                    TeclaDireita = 1;
                }
                break;
            }
            case SDLK_UP:
            {
                TeclaCima = 1;
                break;
            }
            case SDLK_DOWN:
            {
                TeclaBaixo = 1;
                break;
            }
            case SDLK_SPACE:
            {
                TeclaEspace = 1;

                //PULO_ON = 1;
               // velJ = -20;

                break;
            }
            }
        }

    if (event.type == SDL_KEYUP) // Se o usuário soltou um botão do teclado
        {
          // Verifica qual tecla foi solta
            switch(event.key.keysym.sym)
           {


            case SDLK_LEFT: //Movimento para Esquerda

            {
                TeclaEsquerda = 0;
                velX = 0;

                break;
            }
            case SDLK_RIGHT: //Movimento para Direita
            {
                TeclaDireita = 0;
                velX = 0;

                break;
            }
            case SDLK_UP:
            {
                TeclaCima = 0;
                break;
            }
            case SDLK_DOWN:
            {
                TeclaBaixo = 0;
                break;
            }
            case SDLK_SPACE:
            {
                TeclaEspace = 0;
                break;
            }

           }
        }


    BossAnima();
    HenryAnima();
    escada(&dest_henry);
    pulo();
    Final();

//----------------------------AQUI BARRIL ANDA----------------------------------------------------------------------//
    if(TEMPO == 0 + 37)
        cadeirante[0].ATIVO = 1;
    else if(TEMPO == 72 + 37)
        cadeirante[1].ATIVO = 1;
    else if(TEMPO == 2*72 + 37)
        cadeirante[2].ATIVO = 1;
    else if(TEMPO == 3*72 + 37)
        cadeirante[3].ATIVO = 1;
    else if(TEMPO == 4*72 + 37)
        cadeirante[4].ATIVO = 1;
    else if(TEMPO == 5*72 + 37)
        cadeirante[5].ATIVO = 1;
    else if(TEMPO == 6*72 + 37)
        cadeirante[6].ATIVO = 1;
    else if(TEMPO == 7*72 + 37)
        cadeirante[7].ATIVO = 1;
    else if (TEMPO == 8 * 72 + 37)
        TEMPO = 36;

    for(i = 0; i < MAX; i++)
        IA (&cadeirante[i]);

    //printf("TEMPO = %d\n", TEMPO);
//-------------------------AQUI HENRY ANDA------------------------------------------------------------------------------

    dest_henry.x+= velX;
    dest_henry.y+= velY;

//-----------------------Nao sai da tela ----------------------------------------------------------------------------
    if(dest_henry.x + dest_henry.w >= 805)
    {
        dest_henry.x -= velX;
        velX = 0;
    }
//-----------------------------------------Blitagem-----------------------------------------------------------

    moveCeu();
    if(VIDAS)
    {
        font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);

        SDL_BlitSurface(backmove, &ceu, screen, 0);
        SDL_BlitSurface(background, 0, screen, 0);
        SDL_BlitSurface(hud, 0, screen, 0);
        SDL_Color preto = {0,0,0};
        desenhaTexto("SCORE: ",screen, 0, 0, preto);
        desenhaTexto(str_score, screen, 95, 0, preto);
        desenhaTexto("VIDAS: ", screen, 200, 0, preto);
        sprintf(str_vidas, "%d", VIDAS);
        desenhaTexto(str_vidas, screen, 290, 0, preto);
        SDL_BlitSurface(boss, &boss_fonte, screen, &boss_dest);
        SDL_BlitSurface(premio, 0, screen, &premio_dest);

            for(i = 0; i < MAX; i++)
            {
                if(cadeirante[i].ATIVO == 1)
                    SDL_BlitSurface(barril, &cadeirante[i].fonte_barril, screen, &cadeirante[i].dest_barril);
            }

        SDL_BlitSurface(henry, &fonte_henry, screen, &dest_henry);

        for (i = 0; i < MAX; i++) // Coloca o score quando bate no boneco
        {
            if(cadeirante[i].timer > 0)
            {
                //font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);
                SDL_Color branco = {255,255,255};
                desenhaTexto("+ 100",screen,cadeirante[i].sx, cadeirante[i].sy, branco);
                cadeirante[i].sy -= 3;
                cadeirante[i].timer--;
            }
        }

        //font = TTF_OpenFont("fontes/Numero10Clean.ttf",23);

        TTF_CloseFont(font);

    }

    TEMPO++;
    //Atualiza a tela

    ControlFPS(20);
    //SDL_Delay(30);
    //SDL_Flip(screen);

    }

    if(!done) // Se não saiu do jogo clicando no x
    {
        Mix_FadeOutMusic(400);//musica sai de execução em 1,1 segungos
        Recorde(SCORE);
    }

  	//TTF_CloseFont(font);

    Mix_FreeMusic(music);
    Mix_FreeChunk(jump);
    Mix_FreeChunk(smash);
    Mix_FreeChunk(somPonto);
    Mix_FreeChunk(scream);
    Mix_FreeChunk(pof);
    Mix_CloseAudio();

    SDL_FreeSurface(henry); //Libera o espaço de memoria
    SDL_FreeSurface(screen);
    SDL_FreeSurface(background);
    SDL_FreeSurface(backmove);
    SDL_FreeSurface(backdie);
    SDL_FreeSurface(boss);
    SDL_FreeSurface(premio);
    SDL_FreeSurface(hud);
    SDL_FreeSurface(gameover);
    SDL_Quit();
  }

    return 0;
}
