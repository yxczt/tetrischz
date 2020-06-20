#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "megjelenit.h"

void beallitas_menu (Beallitasok *akt_beallitas, SDL_Renderer *renderer)
{
    beall_menupont most=magas;
    beallitas_megjelenit(akt_beallitas,&most,renderer);
    bool quit=false;
    while (!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type)
        {
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym==SDLK_w && most!=magas)
                {
                    most--;
                }
                if(ev.key.keysym.sym==SDLK_s && most!=merete)
                {
                    most++;
                }
                if(ev.key.keysym.sym==SDLK_a)
                {
                    switch (most)
                    {
                        case magas:
                            if(akt_beallitas->magassag>8)
                                akt_beallitas->magassag--;
                            break;
                        case szeles:
                            if(akt_beallitas->szelesseg>6)
                                akt_beallitas->szelesseg--;
                            break;
                        case merete:
                            if(akt_beallitas->e_meret>kicsi)
                                akt_beallitas->e_meret--;
                            break;
                    }
                }
                if(ev.key.keysym.sym==SDLK_d)
                {
                    switch (most)
                    {
                        case magas:
                            if(akt_beallitas->magassag<20)
                                akt_beallitas->magassag++;
                            break;
                        case szeles:
                            if(akt_beallitas->szelesseg<16)
                                akt_beallitas->szelesseg++;
                            break;
                        case merete:
                            if(akt_beallitas->e_meret<nagy)
                                akt_beallitas->e_meret++;
                            break;
                    }
                }
                if(ev.key.keysym.sym==SDLK_ESCAPE)
                {
                    quit=true;
                    FILE *fp;
                    fp=fopen("beallitasok.txt", "wt");
                    fprintf(fp,"%d ",akt_beallitas->magassag);
                    fprintf(fp,"%d ",akt_beallitas->szelesseg);
                    fprintf(fp,"%d ",akt_beallitas->e_meret);
                    fclose(fp);
                }

        }
        beallitas_megjelenit(akt_beallitas,&most,renderer);
    }
}

void dics_t_menu(SDL_Renderer *renderer)
{
    FILE *fp;
    fp=fopen("dics_tabla.txt", "rb");
    Jatekos number;
    int nagysag=0;
    size_t olv;
    while((olv=fread(&number,sizeof(Jatekos),1,fp))>0)
    {
        nagysag++;
    }
    Dicsoseg_tabla dics_t;
    dics_t.meret=nagysag;
    dics_t.adat=(Jatekos *)malloc(sizeof(Jatekos)*(dics_t.meret));
    fseek(fp,0,SEEK_SET);
    int i=0;
    while((olv=fread(&number,sizeof(Jatekos),1,fp))>0)
    {
        dics_t.adat[i]=number;
        i++;
    }
    int hol_van=0;
    kirajzol_dics_t(&dics_t,hol_van,renderer);
    bool quit=false;
    while (!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type)
        {
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym==SDLK_ESCAPE)
                    quit=true;
                if(ev.key.keysym.sym==SDLK_w && hol_van>0)
                    hol_van--;
                if(ev.key.keysym.sym==SDLK_s && hol_van<dics_t.meret-20)
                    hol_van++;
                kirajzol_dics_t(&dics_t,hol_van,renderer);
                break;
        }
    }
    free(dics_t.adat);
}

void fomenu(Menu *menupont,SDL_Renderer *renderer,bool *kilep,bool *ujra, Beallitasok *akt_beallitas)
{
    kirajzol_fomenu(menupont,renderer);
    bool quit=false;
    while (!quit)
    {
        SDL_Event ev;
        SDL_WaitEvent(&ev);
        switch(ev.type)
        {
            case SDL_KEYDOWN:
                if(ev.key.keysym.sym==SDLK_w && (*menupont)!=jatekinditas)
                {
                    (*menupont)--;
                }
                if(ev.key.keysym.sym==SDLK_s && (*menupont)!=kilepes)
                {
                    (*menupont)++;
                }
                if(ev.key.keysym.sym==SDLK_RETURN)
                {
                    switch (*menupont)
                    {
                        case jatekinditas:
                            quit=true;
                            *ujra=true;
                            *kilep=false;
                            break;
                        case kilepes:
                            quit=true;
                            *ujra=false;
                            *kilep=true;
                            break;
                        case beallitasok:
                            beallitas_menu(akt_beallitas,renderer);
                            break;
                        case dics_tabl:
                            dics_t_menu(renderer);
                            break;
                        default:
                            break;
                    }
                }
                kirajzol_fomenu(menupont,renderer);
                break;
        }
    }
}
