#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdbool.h>
#include "jatek.h"
void letorol_aktelem(const Jatek *j,SDL_Renderer *renderer);
void kirajzol_aktelem(const Jatek *j,SDL_Renderer *renderer);
void kirajzol_elemek(const Jatek *j,SDL_Renderer *renderer);
void letorol_pluszelemek(const Jatek *j,SDL_Renderer *renderer);
void kiir_pontszam(const Jatek *j,SDL_Renderer *renderer);
void letorol_pontszam(const Jatek *j,SDL_Renderer *renderer);
void kirajzol_egesz(const Jatek *j,SDL_Renderer *renderer);
void kiir_ujra_e(SDL_Renderer *renderer,bool *ujra);
void kiir_mentes_e(SDL_Renderer *renderer,bool *mentes, Jatek *j);
void verem_letorol(Jatek *j,SDL_Renderer *renderer);
void verem_kirajzol(Jatek *j,SDL_Renderer *renderer);
bool input_text(char *dest, size_t hossz, SDL_Rect teglalap, SDL_Color hatter, SDL_Color szoveg, TTF_Font *font, SDL_Renderer *renderer);
void elmentes_kirajzol( SDL_Renderer *renderer, TTF_Font *font, char* nev);
void beallitas_megjelenit(Beallitasok *akt_beallitas,beall_menupont *most, SDL_Renderer *renderer);
void kirajzol_fomenu(Menu *menupont,SDL_Renderer *renderer);
void kirajzol_dics_t(Dicsoseg_tabla *dics_t,int honnantol,SDL_Renderer *renderer);
