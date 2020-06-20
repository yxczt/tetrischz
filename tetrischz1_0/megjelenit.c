#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include "megjelenit.h"
#include <string.h>

bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer) {
    /* Ez tartalmazza az aktualis szerkesztest */
    char composition[SDL_TEXTEDITINGEVENT_TEXT_SIZE];
    composition[0] = '\0';
    /* Ezt a kirajzolas kozben hasznaljuk */
    char textandcomposition[hossz + SDL_TEXTEDITINGEVENT_TEXT_SIZE + 1];
    /* Max hasznalhato szelesseg */
    int maxw = teglalap.w - 2;
    int maxh = teglalap.h - 2;

    dest[0] = '\0';

    bool enter = false;
    bool kilep = false;

    SDL_StartTextInput();
    while (!kilep && !enter) {
        /* doboz kirajzolasa */
        boxRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, hatter.r, hatter.g, hatter.b, 255);
        rectangleRGBA(renderer, teglalap.x, teglalap.y, teglalap.x + teglalap.w - 1, teglalap.y + teglalap.h - 1, szoveg.r, szoveg.g, szoveg.b, 255);
        /* szoveg kirajzolasa */
        int w;
        strcpy(textandcomposition, dest);
        strcat(textandcomposition, composition);
        if (textandcomposition[0] != '\0') {
            SDL_Surface *felirat = TTF_RenderUTF8_Blended(font, textandcomposition, szoveg);
            SDL_Texture *felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
            SDL_Rect cel = { teglalap.x, teglalap.y, felirat->w < maxw ? felirat->w : maxw, felirat->h < maxh ? felirat->h : maxh };
            SDL_RenderCopy(renderer, felirat_t, NULL, &cel);
            SDL_FreeSurface(felirat);
            SDL_DestroyTexture(felirat_t);
            w = cel.w;
        } else {
            w = 0;
        }
        /* kurzor kirajzolasa */
        if (w < maxw) {
            vlineRGBA(renderer, teglalap.x + w + 2, teglalap.y + 2, teglalap.y + teglalap.h - 3, szoveg.r, szoveg.g, szoveg.b, 192);
        }
        /* megjeleniti a képernyon az eddig rajzoltakat */
        SDL_RenderPresent(renderer);

        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type) {
            /* Kulonleges karakter */
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    int textlen = strlen(dest);
                    do {
                        if (textlen == 0) {
                            break;
                        }
                        if ((dest[textlen-1] & 0x80) == 0x00)   {
                            /* Egy bajt */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0x80) {
                            /* Bajt, egy tobb-bajtos szekvenciabol */
                            dest[textlen-1] = 0x00;
                            textlen--;
                        }
                        if ((dest[textlen-1] & 0xC0) == 0xC0) {
                            /* Egy tobb-bajtos szekvencia elso bajtja */
                            dest[textlen-1] = 0x00;
                            break;
                        }
                    } while(true);
                }
                if (event.key.keysym.sym == SDLK_RETURN) {
                    enter = true;
                }
                break;

            /* A feldolgozott szoveg bemenete */
            case SDL_TEXTINPUT:
                if (strlen(dest) + strlen(event.text.text) < hossz) {
                    strcat(dest, event.text.text);
                }

                /* Az eddigi szerkesztes torolheto */
                composition[0] = '\0';
                break;

            /* Szoveg szerkesztese */
            case SDL_TEXTEDITING:
                strcpy(composition, event.edit.text);
                break;

            case SDL_QUIT:
                /* visszatesszuk a sorba ezt az eventet, mert
                 * sok mindent nem tudunk vele kezdeni */
                SDL_PushEvent(&event);
                kilep = true;
                break;
        }
    }

    /* igaz jelzi a helyes beolvasast; = ha enter miatt allt meg a ciklus */
    SDL_StopTextInput();
    return enter;
}

void kirajzol_fomenu(Menu *menupont,SDL_Renderer *renderer)
{
    boxRGBA(renderer,0,0,1024,768,0,0,0,255);
    stringRGBA(renderer, 500, 30, "Tetrischz", 255, 255, 255, 255);
    boxRGBA(renderer,450,200+(*menupont)*100,610,300+(*menupont)*100,0,0,255,255);
    for (int i=0; i<4;i++)
    {
        rectangleRGBA(renderer,450,200+(i*100),610,300+(i*100),255,255,255,255);
    }
    stringRGBA(renderer, 475, 250, "Jatek inditasa", 255, 255, 255, 255);
    stringRGBA(renderer, 475, 350, "Beallitasok", 255, 255, 255, 255);
    stringRGBA(renderer, 475, 450, "Dicsoseg tabla", 255, 255, 255, 255);
    stringRGBA(renderer, 475, 550, "Kilepes", 255, 255, 255, 255);
    stringRGBA(renderer, 800, 700, "Menuiranyitas: WS, Enter", 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}
void letorol_pluszelemek(const Jatek *j,SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    int x;
    int y;
    for (int i=1;i<4;i++)
    {
        for (int k=0;k<4;k++)
        {
            x=(j->elemek[i].x[k])*meret+j->verem.szelesseg*meret+100;
            y=(j->elemek[i].y[k])*meret+20+i*100;
            boxRGBA(renderer,x,y,x+meret,y+meret,0,0,0,255);
            rectangleRGBA(renderer,x,y,x+meret+1,y+meret+1,0,0,0,255);
        }
    }
}
void kirajzol_elemek(const Jatek *j,SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    int x;
    int y;
    kirajzol_aktelem(j,renderer);
    for (int i=1;i<4;i++)
    {
        for (int k=0;k<4;k++)
        {
            x=(j->elemek[i].x[k])*meret+j->verem.szelesseg*meret+100;
            y=(j->elemek[i].y[k])*meret+20+i*100;
            boxRGBA(renderer,x,y,x+meret,y+meret,255,255,255,255);
            rectangleRGBA(renderer,x,y,x+meret+1,y+meret+1,0,0,0,255);
        }
    }
}
void kirajzol_egesz(const Jatek *j,SDL_Renderer *renderer)
{

    boxRGBA(renderer,0,0,1024,768,0,0,0,255);
    int meret=j->elemmeret;
    int kezdo_x=100;
    int szelesseg=(j->verem.szelesseg)*meret;
    int kezdo_y=100;
    int magassag=(j->verem.magassag)*meret;
    kirajzol_elemek(j,renderer);
    lineRGBA(renderer,kezdo_x-1,kezdo_y+(2*meret),kezdo_x-1,kezdo_y+magassag+1,0,0,255,255);
    lineRGBA(renderer,kezdo_x-1,kezdo_y+magassag+1,kezdo_x+szelesseg+1,kezdo_y+magassag+1,0,0,255,255);
    lineRGBA(renderer,kezdo_x+szelesseg+1,kezdo_y+(2*meret),kezdo_x+szelesseg+1,kezdo_y+magassag+1,0,0,255,255);
    stringRGBA(renderer, 500, 30, "Tetrischz", 255, 255, 255, 255);
    stringRGBA(renderer, 850, 30, "Vissza a menube: ESC", 255, 255, 255, 255);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret, 50, "Pontszam: ", 255, 255, 255, 255);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret, 70, "Szint: ", 255, 255, 255, 255);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret, 90, "Sorok: ", 255, 255, 255, 255);
    stringRGBA(renderer, 800, 700, "Iranyitas: WASD, SPACE", 255, 255, 255, 255);
    kiir_pontszam(j,renderer);


    /* az elvegzett rajzolasok a kepernyore */
    SDL_RenderPresent(renderer);
}

void letorol_aktelem(const Jatek *j,SDL_Renderer *renderer)
{
        int meret=j->elemmeret;
        int kezdo_x=100;
        int kezdo_y=100;
        for (int i=0;i<4;i++)
        {
            int x=(j->elemek[0].x[i])*meret+kezdo_x;
            int y=(j->elemek[0].y[i])*meret+kezdo_y;
            boxRGBA(renderer,x,y,x+meret,y+meret,0,0,0,255);
            rectangleRGBA(renderer,x,y,x+meret+1,y+meret+1,0,0,0,255);
        }
}
void kirajzol_aktelem(const Jatek *j,SDL_Renderer *renderer)
{
        int meret=j->elemmeret;
        int kezdo_x=100;
        int kezdo_y=100;
        for (int i=0;i<4;i++)
        {
            int x=(j->elemek[0].x[i])*meret+kezdo_x;
            int y=(j->elemek[0].y[i])*meret+kezdo_y;
            boxRGBA(renderer,x,y,x+meret,y+meret,255,255,255,255);
            rectangleRGBA(renderer,x,y,x+meret+1,y+meret+1,0,0,0,255);
        }
}
void letorol_pontszam(const Jatek *j,SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    char pont[64];
    sprintf(pont,"%d",j->pontszam);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 50, pont, 0, 0, 0, 255);
    sprintf(pont,"%d",j->szint);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 70, pont, 0, 0, 0, 255);
    sprintf(pont,"%d",j->sorok);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 90, pont, 0, 0, 0, 255);

}
void kiir_pontszam(const Jatek *j,SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    char pont[64];
    sprintf(pont,"%d",j->pontszam);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 50, pont, 255, 255, 255, 255);
    sprintf(pont,"%d",j->szint);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 70, pont, 255, 255, 255, 255);
    sprintf(pont,"%d",j->sorok);
    stringRGBA(renderer, j->verem.szelesseg*meret+5*meret+100, 90, pont, 255, 255, 255, 255);

}

void verem_letorol(Jatek *j, SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    int kezdo_x=100;
    int kezdo_y=100;
    int x;
    int y;
    for(int i=0;i<j->verem.magassag;i++)
    {
        for(int k=0;k<j->verem.szelesseg;k++)
        {
            x=k*meret+kezdo_x;
            y=i*meret+kezdo_y;
            boxRGBA(renderer,x,y,x+meret,y+meret,0,0,0,255);
        }
    }
}
void verem_kirajzol(Jatek *j, SDL_Renderer *renderer)
{
    int meret=j->elemmeret;
    int kezdo_x=100;
    int kezdo_y=100;
    int x;
    int y;
    for(int i=0;i<j->verem.magassag;i++)
    {
        for(int k=0;k<j->verem.szelesseg;k++)
        {
                if(j->verem.adat[i][k]==foglalt)
                {
                    x=k*meret+kezdo_x;
                    y=i*meret+kezdo_y;
                    boxRGBA(renderer,x,y,x+meret,y+meret,255,255,255,255);
                    rectangleRGBA(renderer,x,y,x+meret+1,y+meret+1,0,0,0,255);
                }

        }
    }
}

void kiir_ujra_e(SDL_Renderer *renderer,bool *ujra)
{
    boxRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,0,0,255);
    rectangleRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,255,0,255);
    stringRGBA(renderer,1024/2-100,768/2-25, "Ujra? (Kivalasztas: ENTER)", 255,255,255,255);
    if(*ujra)
    {
        stringRGBA(renderer,1024/2-70,768/2+25, "Igen", 255,0,0,255);
        stringRGBA(renderer,1024/2+70,768/2+25, "Nem", 255,255,255,255);
    }
    else
    {
        stringRGBA(renderer,1024/2-70,768/2+25, "Igen", 255,255,255,255);
        stringRGBA(renderer,1024/2+70,768/2+25, "Nem", 255,0,0,255);
    }
    SDL_RenderPresent(renderer);
}
void kiir_mentes_e(SDL_Renderer *renderer,bool *mentes, Jatek *j)
{
    boxRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,0,0,255);
    rectangleRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,255,0,255);
    stringRGBA(renderer,1024/2-140,768/2-25, "Akarsz menteni? (Kivalasztas: ENTER)", 255,255,255,255);
    stringRGBA(renderer,1024/2-140,768/2, "Pontszamod:", 255,255,255,255);
    char pont[64];
    sprintf(pont,"%d",j->pontszam);
    stringRGBA(renderer, 1024/2,768/2, pont, 255, 255, 255, 255);
    if(*mentes)
    {
        stringRGBA(renderer,1024/2-70,768/2+25, "Igen", 255,0,0,255);
        stringRGBA(renderer,1024/2+70,768/2+25, "Nem", 255,255,255,255);
    }
    else
    {
        stringRGBA(renderer,1024/2-70,768/2+25, "Igen", 255,255,255,255);
        stringRGBA(renderer,1024/2+70,768/2+25, "Nem", 255,0,0,255);
    }
    SDL_RenderPresent(renderer);
}

void elmentes_kirajzol( SDL_Renderer *renderer, TTF_Font *font, char *nev)
{
    SDL_Color feher = {255, 255, 255, 255}, fekete = { 0, 0, 0 , 255};
    boxRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,0,0,255);
    rectangleRGBA(renderer,1024/2-150,768/2-50,1024/2+150,768/2+50,0,255,0,255);
    stringRGBA(renderer,1024/2-140,768/2, "Nev:", 255,255,255,255);
    SDL_RenderPresent(renderer);
    SDL_Rect r = { 1024/2-10,768/2-10,150,30};
    input_text(nev, 10, r, fekete, feher, font, renderer);
}

void beallitas_megjelenit(Beallitasok *akt_beallitas,beall_menupont *most, SDL_Renderer *renderer)
{
    boxRGBA(renderer,0,0,1024,768,0,0,0,255);
    stringRGBA(renderer, 500, 30, "Tetrischz", 255, 255, 255, 255);
    boxRGBA(renderer,450,200+(*most)*100,610,300+(*most)*100,0,0,255,255);
    rectangleRGBA(renderer,450,200,610,300,255,255,255,255);
    rectangleRGBA(renderer,450,300,610,400,255,255,255,255);
    rectangleRGBA(renderer,450,400,610,500,255,255,255,255);
    stringRGBA(renderer, 460, 250, "Magassag: ", 255, 255, 255, 255);
    char akt_szoveg[15];
    sprintf(akt_szoveg,"%d",akt_beallitas->magassag);
    stringRGBA(renderer, 550, 250, akt_szoveg, 255, 255, 255, 255);
    stringRGBA(renderer, 460, 350, "Szelesseg: ", 255, 255, 255, 255);
    sprintf(akt_szoveg,"%d",akt_beallitas->szelesseg);
    stringRGBA(renderer, 550, 350, akt_szoveg, 255, 255, 255, 255);
    stringRGBA(renderer, 460, 450, "Elemmeret: ", 255, 255, 255, 255);
    switch(akt_beallitas->e_meret)
    {
        case kicsi:
            stringRGBA(renderer, 550, 450, "Kicsi", 255, 255, 255, 255);
            break;
        case kozepes:
            stringRGBA(renderer, 550, 450, "Kozepes", 255, 255, 255, 255);
            break;
        case nagy:
            stringRGBA(renderer, 550, 450, "Nagy", 255, 255, 255, 255);
            break;
    }
    stringRGBA(renderer, 800, 700, "Menuiranyitas: WASD", 255, 255, 255, 255);
    stringRGBA(renderer, 850, 30, "Vissza a menube: ESC", 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}

void kirajzol_dics_t(Dicsoseg_tabla *dics_t,int honnantol,SDL_Renderer *renderer)
{
    boxRGBA(renderer,0,0,1024,768,0,0,0,255);
    stringRGBA(renderer, 500, 30, "Tetrischz", 255, 255, 255, 255);
    stringRGBA(renderer, 850, 30, "Vissza a menube: ESC", 255, 255, 255, 255);
    stringRGBA(renderer, 800, 700, "Menuiranyitas: WS", 255, 255, 255, 255);
    for(int x=honnantol; x<honnantol+20 && x<dics_t->meret; x++)
    {
        char buff[64];
        sprintf(buff,"%d %10s........%2dx%2d...%d", dics_t->adat[x].helyezes, dics_t->adat[x].nev, dics_t->adat[x].palyamagassag, dics_t->adat[x].palyaszelesseg, dics_t->adat[x].pontszam);
        stringRGBA(renderer, 370, 150+((x-honnantol)*25),buff , 255, 255, 255, 255);
    }
    SDL_RenderPresent(renderer);
}









