#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum Menu
{
    jatekinditas,
    beallitasok,
    dics_tabl,
    kilepes,
} Menu;

typedef enum elem_meret
{
    kicsi,kozepes,nagy
} elem_meret;

typedef struct Beallitasok
{
    int szelesseg;
    int magassag;
    elem_meret e_meret;
} Beallitasok;

typedef enum beall_menupont
{
    magas,szeles,merete
} beall_menupont;


void fomenu(Menu *menupont,SDL_Renderer *renderer,bool *kilep,bool *ujra, Beallitasok *akt_beallitas);
void beallitas_megjelenit(Beallitasok *akt_beallitas,beall_menupont *most, SDL_Renderer *renderer);
void dics_t_menu(SDL_Renderer *renderer);
