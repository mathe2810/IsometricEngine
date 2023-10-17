#include <stdio.h>
#include "allegro.h"
#include "time.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "../includes/Graphes.h"



typedef struct
{
    int x,y;
}Vector2D;

typedef struct
{
    float x,y;
}Vector2DF;

typedef struct
{
    int x,y;
    int poids;
    bool existing;

}predecesseur;

typedef struct
{
    int num;
    bool boolBatiments;
    int numCheminPossible;
    predecesseur pred[4];
}dataCase;


typedef struct
{
    dataCase *Map;
}dataMap;

///site to convert ttf to fnt and bmp: https://ttf2fnt.com/

typedef struct
{
    BITMAP *charbmp;
    int xOffset,yOffset;
    int xAdvance;
}t_font;


BITMAP * importeImage(char *nomDeFichier)
{
    BITMAP *imageARendre= load_bitmap(nomDeFichier,NULL);
    if(!imageARendre)
    {
        allegro_message("ne peut pas ouvrir %s",nomDeFichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return imageARendre;
}

t_font * createFont(const char * filenameFNT,char * filenameBMP, int* tailleChar) {
    BITMAP *bitmapAllImages= importeImage(filenameBMP);
    FILE *pfFont= fopen(filenameFNT,"r");
    char *tmp= malloc(sizeof (char)*CHAR_MAX);
    fscanf(pfFont,"%s",tmp);
    fscanf(pfFont,"%s",tmp);
    char name[100];
    fscanf(pfFont,"%s",name);
    //printf ("name : %s\n",name);
    while(strcmp(tmp,"count")!=0){
        fscanf(pfFont,"%s",tmp);
    }
    int nbrChar;
    int charid=0;
    fscanf(pfFont,"%d",&nbrChar);
    //printf ("taille : %d\n",nbrChar);
    t_font *fontToReturn= malloc(sizeof (t_font)*nbrChar);
    for(int i=0;i<nbrChar;i++)
    {
        int id;
        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&id);
        //printf ("id :%d ",id);
        int x;
        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&x);
        //printf ("x : %d ",x);
        int y;
        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&y);
        //printf ("y : %d ",y);
        int width;
        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&width);
        //printf ("width : %d ",width);
        int height;
        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&height);
        //printf ("height : %d ",height);

        ///to save:

        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&fontToReturn[charid].xOffset);
        //printf ("xOffset : %d ",fontToReturn[charid].xOffset);

        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&fontToReturn[charid].yOffset);
        //printf ("yOffset : %d ",fontToReturn[charid].yOffset);

        fscanf(pfFont,"%s",tmp);
        fscanf(pfFont,"%d",&fontToReturn[charid].xAdvance);
        //printf ("xAdvance : %d ",fontToReturn[charid].xAdvance);

        while(strcmp(tmp,"15")!=0){
            fscanf(pfFont,"%s",tmp);
        }
        //printf ("\n");
        if(height==0||width==0)
        {

        }
        else{
            fontToReturn[charid].charbmp= create_sub_bitmap(bitmapAllImages, x, y, width, height);
            charid++;
        }
        *tailleChar=charid-1;

    }
    fclose(pfFont);
    return fontToReturn;
}

void redimesionerFont(t_font*fontCustom,int tailletab, float nvlTaille)
{
    for(int i=0;i<tailletab;i++)
    {
        BITMAP *redimensionner= create_bitmap((int)(fontCustom[i].charbmp->w*nvlTaille),(int)(fontCustom[i].charbmp->h*nvlTaille));
        stretch_blit(fontCustom[i].charbmp,redimensionner,0,0,fontCustom[i].charbmp->w, fontCustom[i].charbmp->h, 0, 0,redimensionner->w, redimensionner->h);
        fontCustom[i].charbmp=redimensionner;
        fontCustom[i].xOffset=(int)(fontCustom[i].xOffset*nvlTaille);
        fontCustom[i].yOffset=(int)(fontCustom[i].yOffset*nvlTaille);
        fontCustom[i].xAdvance=(int)(fontCustom[i].xAdvance*nvlTaille);
    }
}


void createNameFILE(const char * nomFont, char *FILEFNT, char *FILEBMP)
{

    strcpy(FILEFNT,"../font/");
    strcpy(FILEBMP,"../font/");

    strcat(FILEFNT,nomFont);
    strcat(FILEBMP,nomFont);

    strcat(FILEFNT,".fnt");
    strcat(FILEBMP,".bmp");
}

void checkVoisinPossible(dataMap *map,Vector2D vWorldSize)
{
    for (int i = 0; i < vWorldSize.y; i++) {
        for (int j = 0; j < vWorldSize.x; j++) {

            if(map->Map[i*vWorldSize.x+j].num==-1){map->Map[i*vWorldSize.x+j].numCheminPossible=0;}
            if(map->Map[i*vWorldSize.x+j].num==0){map->Map[i*vWorldSize.x+j].numCheminPossible=4;}
            if(map->Map[i*vWorldSize.x+j].num==1||map->Map[i*vWorldSize.x+j].num==2){map->Map[i*vWorldSize.x+j].numCheminPossible=2;}
            if(map->Map[i*vWorldSize.x+j].num==3||map->Map[i*vWorldSize.x+j].num==4||map->Map[i*vWorldSize.x+j].num==5||map->Map[i*vWorldSize.x+j].num==6){map->Map[i*vWorldSize.x+j].numCheminPossible=3;}
            if(map->Map[i*vWorldSize.x+j].num==7||map->Map[i*vWorldSize.x+j].num==8||map->Map[i*vWorldSize.x+j].num==9||map->Map[i*vWorldSize.x+j].num==10){map->Map[i*vWorldSize.x+j].numCheminPossible=2;}
            if(map->Map[i*vWorldSize.x+j].num==11||map->Map[i*vWorldSize.x+j].num==12||map->Map[i*vWorldSize.x+j].num==13||map->Map[i*vWorldSize.x+j].num==14){map->Map[i*vWorldSize.x+j].numCheminPossible=1;}

        }
    }
}

void init()
{
    allegro_init();
    set_color_depth(desktop_color_depth());
    if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,800,800,0,0)!=0)
    {
        allegro_message("prblm gfx");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    install_keyboard();
    install_mouse();
}

SAMPLE * importeSon(char *nomDeFichier)
{
    SAMPLE *sonARendre= load_sample(nomDeFichier);
    if(!sonARendre)
    {
        allegro_message("ne peut pas ouvrir %s",nomDeFichier);
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    return sonARendre;
}


void handleMalloc( void *var)
{
    if(var==NULL)
    {
        allegro_message("probleme d'allocation de mémoire");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}


void button(Vector2D *vOrigin)
{
    if(key[KEY_UP])
    {
        vOrigin->y++;
    }
    if(key[KEY_DOWN])
    {
        vOrigin->y--;
    }
    if(key[KEY_LEFT])
    {
        vOrigin->x++;
    }
    if(key[KEY_RIGHT])
    {
        vOrigin->x--;
    }
}

int myClock(int diffAfterReset)
{
    return clock() - diffAfterReset; // Retourne le temps depuis le dernier reset
}

void myResetClock( int * diffAfterReset)
{
    *diffAfterReset = clock(); // Met le compteur au véritable temps
}


void drawFloar(Vector2D vWorldSize, BITMAP*buffer,BITMAP *tabImages[],BITMAP *tabImagesDep[],BITMAP *tabImagesFin[],BITMAP *tabImagesChmein[],Vector2D vOrigin,Vector2D vTileSize,dataMap *map, int sDep, int sFin, const int*chemin,int nbChemin)
{
    for (int y = 0; y < vWorldSize.y; y++)
    {
        for (int x = 0; x < vWorldSize.x; x++)
        {
            // Convert cell coordinate to world space
            Vector2D vWorld= {
                    (vOrigin.x * vTileSize.x) + (x - y) * (vTileSize.x / 2),
                    (vOrigin.y * vTileSize.y) + (x + y) * (vTileSize.y / 2)
            };

            if(y*vWorldSize.x+x==sDep&&sDep!=-1)
            {
                if(map->Map[y*vWorldSize.x+x].num>=0)
                {
                    draw_sprite(buffer, tabImagesDep[map->Map[y*vWorldSize.x+x].num], vWorld.x, vWorld.y);
                    continue;
                }
            }
            else if(y*vWorldSize.x+x==sFin&&sFin!=-1)
            {
                if(map->Map[y*vWorldSize.x+x].num>=0)
                {
                    draw_sprite(buffer, tabImagesFin[map->Map[y*vWorldSize.x+x].num], vWorld.x, vWorld.y);
                    continue;
                }
            }
            else
            {
                if(map->Map[y*vWorldSize.x+x].num>=0)
                {
                    draw_sprite(buffer, tabImages[map->Map[y*vWorldSize.x+x].num], vWorld.x, vWorld.y);
                }
            }
            if(nbChemin!=-1)
            {
                for(int a=0;a<nbChemin;a++)
                {
                    if(y*vWorldSize.x+x==chemin[a])
                    {
                        if(map->Map[y*vWorldSize.x+x].num>=0)
                        {
                            draw_sprite(buffer, tabImagesChmein[map->Map[y*vWorldSize.x+x].num], vWorld.x, vWorld.y);
                            continue;
                        }
                    }
                }
            }
        }
    }
}

void drawBatiments(BITMAP *buffer,BITMAP **tabImage,int numTab,int Xposition,int Yposition,const char *batiments,Vector2D vOrigin,Vector2D vTileSize)
{
    Vector2D vBatiment;
    vBatiment.x= (vOrigin.x * vTileSize.x) + (Xposition - Yposition) * (vTileSize.x / 2);
    vBatiment.y=  (vOrigin.y * vTileSize.y) + (Xposition + Yposition) * (vTileSize.y / 2);

    int OffSetX,OffSetY;
    if(strcmp(batiments,"house")==0)
    {
        OffSetX=18;
        OffSetY=-10;
    }
    else
    {
        OffSetX=0;
        OffSetY=-75;
    }

    draw_sprite(buffer,tabImage[numTab],vBatiment.x+OffSetX,vBatiment.y+OffSetY);
}

dataMap * loadMap(FILE *pf,Vector2D vWorldSize)
{
    dataMap *map= malloc(sizeof (dataMap));
    handleMalloc(map);
    map->Map= malloc(sizeof (dataCase)*vWorldSize.x*vWorldSize.y);
    handleMalloc(map->Map);


    for (int i = 0; i < vWorldSize.y; i++) {
        for (int j = 0; j < vWorldSize.x; j++) {
            fscanf(pf,"%d",&map->Map[i*vWorldSize.x+j].num);
            map->Map[i*vWorldSize.x+j].boolBatiments=false;
            for(int a=0;a<4;a++)
            {
                map->Map[i*vWorldSize.x+j].pred[a].existing=false;
            }
        }
    }

    checkVoisinPossible(map,vWorldSize);

    return map;
}


dataMap *createMap(Vector2D vWorldSize)
{
    dataMap *map= malloc(sizeof (dataMap));
    map->Map= malloc(sizeof (dataCase)*vWorldSize.x*vWorldSize.y);
    handleMalloc(map);
    handleMalloc(map->Map);
    for (int i = 0; i < vWorldSize.y; i++) {
        for (int j = 0; j < vWorldSize.x; j++) {
            map->Map[i*vWorldSize.x+j].num=rand()%15;
        }
    }
    return map;
}

bool checkYInf(dataMap *map,int i,int j,Vector2D vWorldSize)
{
    return i-1>=0&&map->Map[(i-1)*vWorldSize.x+j].num!=-1;
}

bool checkYSup(dataMap *map,int i,int j,Vector2D vWorldSize)
{
    return i+1<vWorldSize.y&&map->Map[(i+1)*vWorldSize.x+j].num!=-1;
}

bool checkXInf(dataMap *map,int i,int j,Vector2D vWorldSize)
{
    return j-1>=0&&map->Map[i*vWorldSize.x+(j-1)].num!=-1;
}

bool checkXSup(dataMap *map,int i,int j,Vector2D vWorldSize)
{
    return j+1<vWorldSize.x&&map->Map[i*vWorldSize.x+(j+1)].num!=-1;
}

void actualpred(dataMap *map,Vector2D vWorldSize)
{
    for(int i=0;i<vWorldSize.y;i++) {
        for(int j=0;j<vWorldSize.x;j++) {

                if(map->Map[i*vWorldSize.x+j].num==0){
                    if(checkYInf(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[0].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[0].y=i-1;
                        map->Map[i*vWorldSize.x+j].pred[0].x=j;

                    }
                    if(checkYSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[1].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[1].y=i+1;
                        map->Map[i*vWorldSize.x+j].pred[1].x=j;
                    }
                    if(checkXInf(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[2].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[2].y=i;
                        map->Map[i*vWorldSize.x+j].pred[2].x=j-1;
                    }
                    if(checkXSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[3].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[3].y=i;
                        map->Map[i*vWorldSize.x+j].pred[3].x=j+1;
                    }
                }

                if(map->Map[i*vWorldSize.x+j].num==1) {
                    if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j - 1;

                    }
                    if (checkXSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j + 1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==2) {
                    if (checkYInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i - 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;

                    }
                    if (checkYSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i + 1;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==3) {
                    if (checkYSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i + 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;
                    }
                    if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j - 1;
                    }
                    if (checkXSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[2].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[2].y = i;
                        map->Map[i * vWorldSize.x + j].pred[2].x = j + 1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==4){
                    if (checkYInf(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[0].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[0].y=i-1;
                        map->Map[i*vWorldSize.x+j].pred[0].x=j;

                    }
                    if (checkYSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[1].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[1].y=i+1;
                        map->Map[i*vWorldSize.x+j].pred[1].x=j;
                    }
                    if (checkXSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[2].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[2].y=i;
                        map->Map[i*vWorldSize.x+j].pred[2].x=j+1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==5) {
                    if (checkYInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i - 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;

                    }
                if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j - 1;
                    }
                   if (checkXSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[2].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[2].y = i;
                        map->Map[i * vWorldSize.x + j].pred[2].x = j + 1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==6) {
                    if (checkYInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i - 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;

                    }
                    if (checkYSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i + 1;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j;
                    }
                if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[2].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[2].y = i;
                        map->Map[i * vWorldSize.x + j].pred[2].x = j - 1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==7){
                    if (checkXInf(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[0].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[0].y=i;
                        map->Map[i*vWorldSize.x+j].pred[0].x=j-1;
                    }
                    if (checkYSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[1].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[1].y=i+1;
                        map->Map[i*vWorldSize.x+j].pred[1].x=j;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==8){
                    if (checkYInf(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[0].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[0].y=i-1;
                        map->Map[i*vWorldSize.x+j].pred[0].x=j;

                    }
                    if (checkXSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[1].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[1].y=i;
                        map->Map[i*vWorldSize.x+j].pred[1].x=j+1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==9) {
                if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j - 1;
                    }
                    if (checkYInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[1].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[1].y = i - 1;
                        map->Map[i * vWorldSize.x + j].pred[1].x = j;

                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==10){
                    if (checkYSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[0].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[0].y=i+1;
                        map->Map[i*vWorldSize.x+j].pred[0].x=j;
                    }
                    if (checkXSup(map,i,j,vWorldSize))
                    {
                        map->Map[i*vWorldSize.x+j].pred[1].existing=true;
                        map->Map[i*vWorldSize.x+j].pred[1].y=i;
                        map->Map[i*vWorldSize.x+j].pred[1].x=j+1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==11) {
                   if (checkXSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j + 1;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==12) {
                    if (checkYSup(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i + 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;
                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==13) {
                    if (checkYInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i - 1;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j;

                    }
                }
                if(map->Map[i*vWorldSize.x+j].num==14) {
                if (checkXInf(map,i,j,vWorldSize)) {
                        map->Map[i * vWorldSize.x + j].pred[0].existing = true;
                        map->Map[i * vWorldSize.x + j].pred[0].y = i;
                        map->Map[i * vWorldSize.x + j].pred[0].x = j - 1;
                    }
                }
        }
    }
}

void actualiserFichierDjikstra(dataMap *map,Vector2D vWorldSize,int taille,int ordre)
{
    FILE *pf= fopen("../djikstra.txt","w+");
    handleMalloc(pf);
    fprintf(pf,"%d\n",ordre);
    for(int i=0;i<ordre;i++)
    {
        fprintf(pf,"%d\n",i);
    }
    char tabpred[taille][50];
    int compteur=0;
    for(int i=0;i<vWorldSize.y;i++) {
        for(int j=0;j<vWorldSize.x;j++) {
            for(int a=0;a<4;a++)
            {
                if(map->Map[i*vWorldSize.x+j].pred[a].existing)
                {
                    sprintf(tabpred[compteur],"%d %d %d\n",i*vWorldSize.x+j,map->Map[i*vWorldSize.x+j].pred[a].y*vWorldSize.x+map->Map[i*vWorldSize.x+j].pred[a].x,1);
                    compteur++;
                }
            }
        }
    }
    fprintf(pf,"%d\n",compteur);
    for(int i=0;i<compteur;i++)
    {
        fprintf(pf,"%s",tabpred[i]);
    }
    fclose(pf);


}

void saveMap(dataMap *map,Vector2D vWorldSize)
{
    FILE *pf= fopen("../map.txt","w+");
    for (int i = 0; i < vWorldSize.y; i++) {
        for (int j = 0; j < vWorldSize.x; j++) {

            fprintf(pf,"%d ",map->Map[i*vWorldSize.x+j].num);
            printf("%d ",map->Map[i*vWorldSize.x+j].num);
        }
        fprintf(pf,"%s","\n");
        printf("\n");
    }
    fclose(pf);
}

int Menu(BITMAP *buffer, BITMAP **fond)
{
    int clockFrame=0;
    clock_t tempsPourFrame;
    int frame=0;
    while(1)
    {
        if(mouse_b==1)
        {
            break;
        }
        if(key[KEY_ESC])
        {
            allegro_exit();
            exit(EXIT_SUCCESS);
        }
        clear_bitmap(buffer);
        tempsPourFrame= myClock(clockFrame);
        if(tempsPourFrame>100)
        {
            frame++;
            if(frame>8)
            {
                frame=0;
            }
            myResetClock(&clockFrame);
        }
        blit(fond[frame],buffer,0,0,0,0,SCREEN_W,SCREEN_H);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    return 1;
}

void initSommet(graphe_t *graphe)
{
    for (int i = 0; i < graphe->ordre; i++) {
        graphe->sommets[i]->dist = MAX_INT;
        graphe->sommets[i]->compoConnex = 0;
        graphe->sommets[i]->couleur = 0;
        graphe->sommets[i]->pred = -1;
    }
}


int main() {
    init();


    int playerX=0;
    int playerY=0;

    int playerWorldX,playerWorldY;



    FILE *pf= fopen("../map.txt","r");
    handleMalloc(pf);
    srand(time(NULL));
    BITMAP *tabImages[114];
    BITMAP *buffer= create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *selection=create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *selectionImage= importeImage("../images/imageSelection.bmp");

    char FILEFNT[200];
    char FILEBMP[200];
    int tailleTabMax;

    createNameFILE("OldLondon",FILEFNT,FILEBMP);

    t_font *fontCustomMedieval= createFont(FILEFNT,FILEBMP,&tailleTabMax);
    printf("lom");

    int clockMooveMap=0;
    clock_t tempsPourMooveMap;

    int clockClick=0;
    clock_t tempsPourClick;

    // Number of tiles in world
    Vector2D vWorldSize = { 20, 20 };

    int ForQuestion;
    dataMap *map;

    map= loadMap(pf,vWorldSize);

    // Size of single tile graphic
    Vector2D vTileSize = { 94, 54 };

    // Where to place tile (0,0) on screen (in tile size steps)
    Vector2D vOrigin = { 5, 1 };

    char NomDeFichier[500];

    BITMAP *tabImagesSelectionsDep[15];
    BITMAP *tabImagesSelectionsFin[15];
    BITMAP *tabImagesSelectionsChemin[15];
    for(int i=0;i<15;i++)
    {
        sprintf(NomDeFichier,"../roadsSelectionDep/road%d.bmp",i);
        tabImagesSelectionsDep[i]= importeImage(NomDeFichier);
    }

    for(int i=0;i<15;i++)
    {
        sprintf(NomDeFichier,"../roadsSelectionFin/road%d.bmp",i);
        tabImagesSelectionsFin[i]= importeImage(NomDeFichier);
    }
    for(int i=0;i<15;i++)
    {
        sprintf(NomDeFichier,"../roadsParcouru/road%d.bmp",i);
        tabImagesSelectionsChemin[i]= importeImage(NomDeFichier);
    }
    for(int i=0;i<15;i++)
    {
        sprintf(NomDeFichier,"../roads/road%d.bmp",i);
        tabImages[i]= importeImage(NomDeFichier);
    }

    BITMAP *imageSelection=importeImage("../images/CheatingImage.bmp");
    BITMAP *tower[2];
    for(int i=0;i<2;i++)
    {
        sprintf(NomDeFichier,"../images/tower%d.bmp",i);
        tower[i]= importeImage(NomDeFichier);
    }
    BITMAP *house[4];
    for(int i=0;i<4;i++)
    {
        sprintf(NomDeFichier,"../house/white_green_house%d.bmp",i);
        house[i]= importeImage(NomDeFichier);
    }

    BITMAP *fondMenu[9];
    for(int i=0;i<9;i++)
    {
        sprintf(NomDeFichier,"../fond/%d.bmp",i);
        fondMenu[i]= importeImage(NomDeFichier);
    }
    int Sequence=0;
    show_mouse(screen);

    int nombreDarc=0;
    int ordre=vWorldSize.x*vWorldSize.y;

    for (int i = 0; i < vWorldSize.y; i++) {
        for (int j = 0; j < vWorldSize.x; j++) {
            nombreDarc+=map->Map[i*vWorldSize.x+j].numCheminPossible;
        }
    }
    printf("ordres:%d tailles max possible:%d\n",ordre,nombreDarc);

    actualpred(map,vWorldSize);
    actualiserFichierDjikstra(map,vWorldSize,nombreDarc,ordre);





    graphe_t *graphe;
    int sDep=-1, sFin=-1;
    int *pred;
    int *cheminParcouru=NULL;
    int nbCheminTrouve=-1;

    bool djikstraInAction=false;
    bool selectDep=false;
    bool selectFin=false;


    rewind(pf);





    Menu(buffer, fondMenu);
    while (!key[KEY_ESC])
    {
        if (key[KEY_S]) {saveMap(map, vWorldSize);}

        tempsPourClick = myClock(clockClick);
        tempsPourMooveMap = myClock(clockMooveMap);
        clear_bitmap(buffer);
        clear_bitmap(selection);

        rectfill(buffer, 0, 0, SCREEN_W, SCREEN_H, makecol(20, 180, 255));

        // Work out active cell
        Vector2DF vCell = {(float) mouse_x / (float) vTileSize.x, (float) mouse_y / (float) vTileSize.y};
        // Work out mouse offset into cell
        Vector2D vOffset = {mouse_x % vTileSize.x, mouse_y % vTileSize.y};
        Vector2D vBatiment;
        Vector2D vSelected =
        {
      ((int) vCell.y - vOrigin.y) + ((int) vCell.x - vOrigin.x),
      ((int) vCell.y - vOrigin.y) - ((int) vCell.x - vOrigin.x)
        };
        Vector2D vSelectedWorld = {
      (vOrigin.x * vTileSize.x) + (vSelected.x - vSelected.y) * (vTileSize.x / 2),
      (vOrigin.y * vTileSize.y) + (vSelected.x + vSelected.y) * (vTileSize.y / 2)
        };
        draw_sprite(selection, imageSelection, vSelectedWorld.x, vSelectedWorld.y);
        if (getpixel(selection, mouse_x, mouse_y) == makecol(255, 0, 0)) { vSelected.x += -1; }
        if (getpixel(selection, mouse_x, mouse_y) == makecol(0, 0, 255)) { vSelected.y += -1; }
        if (getpixel(selection, mouse_x, mouse_y) == makecol(255, 255, 0)) { vSelected.y += 1; }
        if (getpixel(selection, mouse_x, mouse_y) == makecol(0, 255, 0)) { vSelected.x += 1; }

        if(key[KEY_D])
        {
            djikstraInAction=true;
            selectDep=true;
            selectFin=true;
        }
        if(selectDep&&mouse_b==1)
        {
            sDep=vSelected.y*vWorldSize.x+vSelected.x;
            if((sDep >= 0 || sDep <= ordre))
            {
                selectDep=false;
            }
        }
        if(selectFin&&mouse_b==2)
        {
            sFin=vSelected.y*vWorldSize.x+vSelected.x;
            if((sFin > 0 || sFin <= ordre))
            {
                selectFin=false;
            }
        }
        if(djikstraInAction&&!selectDep&&!selectFin)
        {
            graphe = loadGraphe();
            initSommet(graphe);
            pred = malloc(sizeof(int) * graphe->ordre); // Initialisation du tableau des predecesseurs pour DSF
            handleMalloc(pred);
            for(int i = 0; i < graphe->ordre ; i++)
                pred[i] = -1;
            myDSF(graphe, sDep, pred, 1); // Marquage de la composante connexe dans lequel est le sommet initial
            myDijkstra(graphe, sDep, 1); // Dijkstra
            cheminParcouru=getWay(graphe, sDep, sFin,&nbCheminTrouve); // Affichage du chemin
            for(int a=0;a<nbCheminTrouve;a++)
            {
                printf("%d\n",cheminParcouru[a]);
            }
            free(pred);
            djikstraInAction=false;
        }
        drawFloar(vWorldSize, buffer, tabImages,tabImagesSelectionsDep,tabImagesSelectionsFin,tabImagesSelectionsChemin, vOrigin, vTileSize, map,sDep,sFin,cheminParcouru,nbCheminTrouve);

        vSelectedWorld.x = (vOrigin.x * vTileSize.x) + (vSelected.x - vSelected.y) * (vTileSize.x / 2);
        vSelectedWorld.y = (vOrigin.y * vTileSize.y) + (vSelected.x + vSelected.y) * (vTileSize.y / 2);
        draw_sprite(buffer, selectionImage, vSelectedWorld.x, vSelectedWorld.y);

        playerWorldX=(vOrigin.x * vTileSize.x) + (playerX - playerY) * (vTileSize.x / 2);
        playerWorldY=(vOrigin.y * vTileSize.y) + (playerX + playerY) * (vTileSize.y / 2);

        circlefill(buffer,playerWorldX+vTileSize.x/2,playerWorldY+vTileSize.y/2,5, makecol(255,255,0));

            /*drawWall(buffer,tabImages);*/

        /*drawBatiments(buffer, house, 0, 0, 0, "house", vOrigin, vTileSize);
        drawBatiments(buffer, tower, 0, 5, 5, "tower", vOrigin, vTileSize);
        drawBatiments(buffer, house, 0, 9, 9, "house", vOrigin, vTileSize);*/

        /*rect(buffer,(int)vCell.x*vTileSize.x,(int)vCell.y*vTileSize.y,((int)vCell.x+1)*vTileSize.x,((int)vCell.y+1)*vTileSize.y, makecol(255,0,0));*/
        /* rect(buffer,mapX*TILE_WIDTH,mapY*TILE_HEIGHT/2,(mapX+1)*TILE_WIDTH,(mapY+1)*TILE_HEIGHT/2, makecol(255,0,0));*/
        if (tempsPourMooveMap > 60) {
        button(&vOrigin);
        myResetClock(&clockMooveMap);
        }
       /* if (tempsPourClick > 50) {
            if (selectDep== false && selectFin==false&&mouse_b == 1) {
                    if (vSelected.x >= 0 && vSelected.y >= 0 && vSelected.x <= vWorldSize.x &&
                        vSelected.y <= vWorldSize.y) {
                        map->Map[vSelected.y * vWorldSize.x + vSelected.x].num += 1;
                        if (map->Map[vSelected.y * vWorldSize.x + vSelected.x].num > 14) {
                            map->Map[vSelected.y * vWorldSize.x + vSelected.x].num = -1;
                        }
                        playerX=vSelected.x;
                        playerY=vSelected.y;
                    }
            }
            myResetClock(&clockClick);
        }*/
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    fclose(pf);
    allegro_exit();
    return 0;
}
END_OF_MAIN();
