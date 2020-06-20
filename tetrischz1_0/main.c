#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "megjelenit.h"
#define UNUSED(x) (void)(x)
void sdl_init(int szeles, int magas, const char* tipus, SDL_Window **pwindow, SDL_Renderer **prenderer, TTF_Font **pfont) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow("Tetrischz1_0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }

    TTF_Init();
    TTF_Font *font = TTF_OpenFont(tipus, 20);
    if (font == NULL) {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;
    *pfont = font;
}

Uint32 idozit(Uint32 ms, void *param) {
    UNUSED(param);
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

int main(int argc, char *argv[]) {

    UNUSED((void)argc);
    UNUSED((void)argv);
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    sdl_init(1024, 768,"LiberationSerif-Regular.ttf", &window, &renderer, &font);
    SDL_TimerID id = SDL_AddTimer(1, idozit, NULL);
    srand(time(NULL));
    bool kilepes=false;
    while(!kilepes)
    {
        Menu menupont=jatekinditas;
        bool ujra=true;
        Beallitasok akt_beallitas;
        FILE *fp;
        fp=fopen("beallitasok.txt", "rt");
        fscanf(fp,"%d ",&akt_beallitas.magassag);
        fscanf(fp,"%d ",&akt_beallitas.szelesseg);
        fscanf(fp,"%d ",(int *)(&akt_beallitas.e_meret));
        fclose(fp);
        fomenu(&menupont,renderer,&kilepes,&ujra,&akt_beallitas);
        Jatek j;
        while(ujra && !kilepes)
        {
            ujjatek_kezdes(&j, &akt_beallitas);
            kirajzol_egesz(&j,renderer);
            jatszas(&j,renderer,&ujra);
            if(ujra)
            {
                bool mentes=true;
                mentes_e(&mentes,renderer,&j);
                if(mentes) elmentes(&j,renderer,font);
                ujra=false;
                ujra_e(&ujra, renderer);
            }
            free(j.verem.adat[0]);
            free(j.verem.adat);
        }
    }
    SDL_RemoveTimer(id);
    SDL_Quit();

    return 0;
}
