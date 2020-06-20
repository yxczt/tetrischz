#include <stdio.h>
#include <stdlib.h>
#include "megjelenit.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>
#include <time.h>
#include <string.h>

void ujelem_seged(Elem *e,int hova_x,int hova_y, int i, int meddig)
{
    for (int sz=i;sz<meddig;sz++)
    {
        e->x[sz]=hova_x+sz;
        e->y[sz]=hova_y;
    }
}


void ujelem(Elem *e, Jatek *j)
{
    int kezdo_x=(j->verem.szelesseg)/2-1;
    e->tipus=rand()%7;
    switch (e->tipus)
    {
        case vonal:
            ujelem_seged(e,kezdo_x,2,0,4);
            break;
        case ell:
            ujelem_seged(e,kezdo_x,2,0,3);
            e->x[3]=e->x[0];
            e->y[3]=e->y[0]+1;
            break;
        case ford_ell:
            ujelem_seged(e,kezdo_x,2,0,3);
            e->x[3]=e->x[2];
            e->y[3]=e->y[2]+1;
            break;
        case tee:
            ujelem_seged(e,kezdo_x,2,0,3);
            e->x[3]=e->x[1];
            e->y[3]=e->y[1]+1;
            break;
        case negyzet:
            ujelem_seged(e,kezdo_x+1,2,0,2);
            ujelem_seged(e,kezdo_x-1,3,2,4);
            break;
        case ess:
            e->x[0]=kezdo_x+2;
            e->y[0]=2;
            e->x[1]=kezdo_x+1;
            e->y[1]=2;
            ujelem_seged(e,kezdo_x-2,3,2,4);
            break;
        case zee:
            ujelem_seged(e,kezdo_x,2,0,2);
            ujelem_seged(e,kezdo_x-1,3,2,4);
            break;

    }
}


void ujjatek_kezdes(Jatek *j, Beallitasok *akt_beallitas)
{
    j->verem.magassag=akt_beallitas->magassag+2;
    j->verem.szelesseg=akt_beallitas->szelesseg;
    for (int i=0;i<4;i++)
    {
        ujelem(&(j->elemek[i]),j);
    }
    switch (akt_beallitas->e_meret)
    {
        case kicsi:
            j->elemmeret=20;
            break;
        case kozepes:
            j->elemmeret=25;
            break;
        case nagy:
            j->elemmeret=30;
            break;

    }
    j->szint=1;
    j->sorok=0;
    j->pontszam=0;
    j->vege_e=false;
    j->verem.adat=(Terulet**)malloc(j->verem.magassag*(sizeof(Terulet*)));
    j->verem.adat[0]=(Terulet*)malloc((j->verem.magassag)*(j->verem.szelesseg)*(sizeof(Terulet)));
    for (int i=1;i<j->verem.magassag;i++)
        j->verem.adat[i]=j->verem.adat[0]+i*j->verem.szelesseg;
    for(int i=0;i<j->verem.szelesseg;i++)
    {
        for(int k=0;k<j->verem.magassag;k++)
        {
            j->verem.adat[k][i]=ures;
        }
    }
    for (int i=0;i<4;i++)
    {
        j->verem.adat[j->elemek[0].y[i]][j->elemek[0].x[i]]=atmeneti;
    }

}

bool leert_e(Jatek *j)
{
    for (int i=0;i<4;i++)
    {
        if (j->elemek[0].y[i]==j->verem.magassag-1)
            return true;
        if (j->verem.adat[j->elemek[0].y[i]+1][j->elemek[0].x[i]]==foglalt)
            return true;
    }
    return false;
}
bool mozoghat_e(Jatek *j, int merre)
{
    for(int i=0;i<4;i++)
    {
        if (j->elemek[0].x[i]+merre<0 || j->elemek[0].x[i]+merre>j->verem.szelesseg-1)
            return false;
        if (j->verem.adat[j->elemek[0].y[i]][j->elemek[0].x[i]+merre]==foglalt)
            return false;
    }
    return true;
}

void verem_torol_aktelem(Jatek *j)
{
    for (int i=0;i<4;i++)
    {
        j->verem.adat[j->elemek[0].y[i]][j->elemek[0].x[i]]=ures;
    }
}

void verem_beir_aktelem(Jatek *j)
{
    for (int i=0;i<4;i++)
    {
        j->verem.adat[j->elemek[0].y[i]][j->elemek[0].x[i]]=atmeneti;
    }
}

void elem_shift(Jatek *j)
{
    for(int i=0;i<3;i++)
    {
        for(int k=0;k<4;k++)
        {
            j->elemek[i].x[k]=j->elemek[i+1].x[k];
            j->elemek[i].y[k]=j->elemek[i+1].y[k];
        }
        j->elemek[i].tipus=j->elemek[i+1].tipus;
    }
    ujelem(&(j->elemek[3]),j);
}
void verem_frissit(Jatek *j)
{
    for (int i=0;i<j->verem.szelesseg;i++)
    {
        for (int k=0;k<j->verem.magassag;k++)
        {
            if(j->verem.adat[k][i]==atmeneti)
                j->verem.adat[k][i]=foglalt;
        }
    }
}

bool vesztett_e(Jatek *j)
{
    for(int i=0;i<4;i++)
    {
        if(j->verem.adat[j->elemek[0].y[i]][j->elemek[0].x[i]]==foglalt || leert_e(j))
            return true;
    }
    return false;
}


bool foroghat_e(Jatek *j,int merre)
{
    if (j->elemek[0].tipus==negyzet) return true;
    Elem e;
    for (int i=0;i<4;i++)
    {
        e.x[i]=j->elemek[0].x[i];
        e.y[i]=j->elemek[0].y[i];
    }
    for(int i=0;i<4;i++)
    {
        if (i==1) continue;
        int s=e.y[i];
        int o=e.x[i];
        e.x[i]=e.x[1]+merre*(s-e.y[1]);
        e.y[i]=e.y[1]+merre*(o-e.x[1]);
        if (e.x[i]!=e.x[1])
        {
            e.x[i]=e.x[1]-(e.x[i]-e.x[1]);
        }
        if(e.x[i]<0 || e.x[i]>j->verem.szelesseg-1)
            return false;
        if(e.y[i]<0 || e.y[i]>j->verem.magassag-1)
            return false;
        if(j->verem.adat[e.y[i]][e.x[i]]==foglalt)
            return false;
    }
    return true;
}

void forog(Jatek *j,int merre)
{

            if (j->elemek[0].tipus!=negyzet)
            {
                for(int i=0;i<4;i++)
                {
                    if (i==1) continue;
                    int s=j->elemek[0].y[i];
                    int o=j->elemek[0].x[i];
                    j->elemek[0].x[i]=j->elemek[0].x[1]+merre*(s-j->elemek[0].y[1]);
                    j->elemek[0].y[i]=j->elemek[0].y[1]+merre*(o-j->elemek[0].x[1]);
                    if (j->elemek[0].x[i]!=j->elemek[0].x[1])
                    {
                        j->elemek[0].x[i]=j->elemek[0].x[1]-(j->elemek[0].x[i]-j->elemek[0].x[1]);
                    }
                }
            }
}

void verem_igazodik(Jatek *j, int i)
{
    for(int k=i;k>0;k--)
    {
        for(int o=0;o<j->verem.szelesseg;o++)
        {
            j->verem.adat[k][o]=j->verem.adat[k-1][o];
        }
    }
}


void pontozas(Jatek *j,SDL_Renderer *renderer)
{
    j->pontszam+=10;
    int szorzo=1;
    int i=0;
    while(i<j->verem.magassag)
    {
        bool csupa_foglalt=true;
        for(int k=0; k<j->verem.szelesseg; k++)
        {
            if(j->verem.adat[i][k]!=foglalt)
            {
                csupa_foglalt=false;
            }
        }
        if(csupa_foglalt)
        {
            j->pontszam+=((100-((j->verem.magassag*j->verem.szelesseg)/8))*szorzo);
            j->sorok++;
            szorzo*=2;
            verem_igazodik(j,i);
            i--;
        }
        i++;
    }
    verem_letorol(j,renderer);
    verem_kirajzol(j,renderer);
}
void jatszas(Jatek *j, SDL_Renderer *renderer, bool *ujra)
{
    bool quit=false;
    int szamlalo=0;
        while (!quit && !j->vege_e)
        {
            SDL_Event ev;
            SDL_WaitEvent(&ev);
            switch(ev.type)
            {
                case SDL_KEYDOWN:
                    if (ev.key.keysym.sym==SDLK_ESCAPE)
                    {
                        quit=true;
                        *ujra=false;
                    }
                    if (mozoghat_e(j,1) && ev.key.keysym.sym==SDLK_d)
                    {
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        for(int i=0;i<4;i++)
                        {
                            j->elemek[0].x[i]++;
                        }
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    if (mozoghat_e(j,-1) && ev.key.keysym.sym==SDLK_a)
                    {
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        for(int i=0;i<4;i++)
                        {
                            j->elemek[0].x[i]--;
                        }
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    if (ev.key.keysym.sym==SDLK_SPACE)
                    {
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        while(!leert_e(j))
                        {
                            for(int i=0;i<4;i++)
                            {
                                j->elemek[0].y[i]++;
                            }
                        }
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    if (foroghat_e(j,-1) && ev.key.keysym.sym==SDLK_w)
                    {
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        forog(j,-1);
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    if (foroghat_e(j,1) && ev.key.keysym.sym==SDLK_s)
                    {
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        forog(j,1);
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    break;

            case SDL_USEREVENT:
                    szamlalo+=1;
                    if(!leert_e(j) && szamlalo>=450-((j->szint-1)*50))
                    {
                        szamlalo=0;
                        letorol_aktelem(j,renderer);
                        verem_torol_aktelem(j);
                        for(int i=0;i<4;i++)
                        {
                            j->elemek[0].y[i]++;
                        }
                        verem_beir_aktelem(j);
                        kirajzol_aktelem(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    else if(szamlalo>=450-((j->szint-1)*50))
                    {
                        szamlalo=0;
                        verem_frissit(j);
                        letorol_pontszam(j,renderer);
                        pontozas(j,renderer);
                        if (j->szint<8)
                            j->szint=j->sorok/10+1;
                        kiir_pontszam(j,renderer);
                        letorol_pluszelemek(j,renderer);
                        elem_shift(j);
                        if (vesztett_e(j))
                        {
                            j->vege_e=true;
                        }
                        kirajzol_elemek(j,renderer);
                        SDL_RenderPresent(renderer);
                    }
                    break;
            }
        }
}
void ujra_e(bool *ujra, SDL_Renderer *renderer)
{
    bool quit=false;
    kiir_ujra_e(renderer,ujra);
    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                if(event.key.keysym.sym==SDLK_d)
                {

                    *ujra=false;
                    kiir_ujra_e(renderer,ujra);
                }
                if(event.key.keysym.sym==SDLK_a)
                {
                    *ujra=true;
                    kiir_ujra_e(renderer,ujra);
                }
                if(event.key.keysym.sym==SDLK_RETURN)
                    quit=true;
                break;
        }
    }
}

void mentes_e(bool *mentes, SDL_Renderer *renderer,Jatek *j)
{
    bool quit=false;
    kiir_mentes_e(renderer,mentes,j);
    while (!quit)
    {
        SDL_Event event;
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case SDL_KEYDOWN:
                if(event.key.keysym.sym==SDLK_d)
                {

                    *mentes=false;
                    kiir_mentes_e(renderer,mentes,j);
                }
                if(event.key.keysym.sym==SDLK_a)
                {
                    *mentes=true;
                    kiir_mentes_e(renderer,mentes,j);
                }
                if(event.key.keysym.sym==SDLK_RETURN)
                    quit=true;
                break;
        }
    }
}

void elmentes(Jatek *j, SDL_Renderer *renderer, TTF_Font *font)
{
    Jatekos uj;
    elmentes_kirajzol(renderer,font,uj.nev);
    uj.helyezes=0;
    uj.palyamagassag=j->verem.magassag-2;
    uj.palyaszelesseg=j->verem.szelesseg;
    uj.pontszam=j->pontszam;
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
    dics_t.meret=nagysag+1;
    dics_t.adat=(Jatekos *)malloc(sizeof(Jatekos)*(dics_t.meret));
    fseek(fp,0,SEEK_SET);
    int i=0;
    if (dics_t.meret!=1)
    {
        bool bent_van_e=false;
        while((olv=fread(&number,sizeof(Jatekos),1,fp))>0)
        {
            if(number.pontszam<uj.pontszam && !bent_van_e)
            {
                dics_t.adat[i]=uj;
                i++;
                bent_van_e=true;
            }
            dics_t.adat[i]=number;
            i++;
        }
        if(i==nagysag)
        {
            dics_t.adat[nagysag]=uj;
        }
        int hely=1;
        dics_t.adat[0].helyezes=hely;
        for(i=1;i<dics_t.meret;i++)
        {
            if(dics_t.adat[i].pontszam<dics_t.adat[i-1].pontszam)
                hely++;
            dics_t.adat[i].helyezes=hely;
        }
    } else
    {
        uj.helyezes=1;
        dics_t.adat[0]=uj;
    }
    fclose(fp);
    fp=fopen("dics_tabla.txt", "wb");
    for(i=0;i<dics_t.meret;i++)
    {
        fwrite(&(dics_t.adat[i]),sizeof(Jatekos),1,fp);
    }
    fclose(fp);
    free(dics_t.adat);

}

























