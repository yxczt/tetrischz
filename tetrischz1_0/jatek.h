#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include "menu.h"
typedef enum Elemfajta
{
    vonal,ell,ford_ell,tee,negyzet,ess,zee
} Elemfajta;

typedef struct Elem
{
    int x[4];
    int y[4];
    Elemfajta tipus;
} Elem;
typedef enum Terulet
{
    ures,
    atmeneti,
    foglalt,
} Terulet;
typedef struct Verem
{
    Terulet **adat;
    int szelesseg,magassag;
} Verem;
typedef struct Jatek
{
    Verem verem;
    Elem elemek[4];
    int elemmeret;
    bool vege_e;
    int szint;
    int sorok;
    int pontszam;
} Jatek;

typedef struct Jatekos
{
    int helyezes;
    char nev[10];
    int palyaszelesseg;
    int palyamagassag;
    int pontszam;
} Jatekos;

typedef struct Dicsoseg_tabla
{
    Jatekos *adat;
    int meret;
} Dicsoseg_tabla;
void ujjatek_kezdes(Jatek *j, Beallitasok *akt_beallitas);
bool leert_e(Jatek *j);
bool mozoghat_e(Jatek *j,int merre);
bool foroghat_e(Jatek *j,int merre);
void forog(Jatek *j,int merre);
bool vesztett_e(Jatek *j);
void pontozas(Jatek *j,SDL_Renderer *renderer);
void ujra_e(bool *ujra, SDL_Renderer *renderer);
void mentes_e(bool *mentes, SDL_Renderer *renderer,Jatek *j);
void elem_shift(Jatek *j);
void ujelem_seged(Elem *e,int hova_x,int hova_y, int i, int meddig);
void ujelem(Elem *e,Jatek *j);
void verem_torol_aktelem(Jatek *j);
void verem_beir_aktelem(Jatek *j);
void verem_frissit(Jatek *j);
void verem_igazodik(Jatek *j, int i);
void jatszas(Jatek *j,SDL_Renderer *renderer,bool *ujra);
void elmentes(Jatek *j, SDL_Renderer *renderer, TTF_Font *font);

