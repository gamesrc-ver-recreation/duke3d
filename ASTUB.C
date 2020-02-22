//-------------------------------------------------------------------------
/*
Copyright (C) 1996, 2003 - 3D Realms Entertainment

This file is part of Duke Nukem 3D version 1.5 - Atomic Edition

Duke Nukem 3D is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Original Source: 1996 - Allen Blum
Prepared for public release: 05/24/2003 - Charlie Wiederhold, 3D Realms
*/
//-------------------------------------------------------------------------

/*
********************************************************

 ASTUB.C (c) 1996 Allen H. Blum III

********************************************************


Specs:

Duke Lookup Table

        1  : Blue
        2  : Red
        3  : Normal for Sky
        4  : Black Shadow
        5  :
        6  : Night Vision
        7  : Yellow
        8  : Green

        Duke Sprite

        9  : Blue
        10 : Red
        11 : Green
        12 : Grey
        13 : Ninja
        14 : G.I. Duke
        15 : Brown
    16 : Postal Duke (Dark Blue)

        21 : Blue -> Red
        22 : Blue -> Green
        23 : Blue -> Yellow

********************************************************
*/
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>

#include "gamever.h" // VERSIONS RESTORATION
//#include "dos.h"

#include "build.h"
#include "names.h"
#include "stdio.h"

#define TICSPERFRAME 3




#pragma aux setvmode =\
        "int 0x10",\
        parm [eax]\

//#include "water.c"

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
static short unusedvar; // HACK
#endif
char *Myname[1]= {"stryker@metronet.com"};

extern char keystatus[];
extern short defaultspritecstat;
extern long posx, posy, posz, horiz, qsetmode;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
extern short asksave;
#endif
extern short ang, cursectnum;
extern short ceilingheinum, floorheinum;
extern char names[MAXTILES][17];

extern long zmode, kensplayerheight, zlock;

#if (APPVER_DN3DREV < AV_DR_DN3D14)
extern short editstatus, searchit;
#else
extern short editstatus;
#endif
extern long searchx, searchy;                          //search input
extern short searchsector, searchwall, searchstat;     //search output

static short temppicnum, tempcstat;
static char tempshade, tempxrepeat, tempyrepeat, somethingintab = 255;
static long temphitag,templotag;

static long ototalclock = 0;

static long clockval[16], clockcnt = 0;

#define NUMOPTIONS 8
#define NUMKEYS 19

static long chainxres[4] = {256,320,360,400};
static long chainyres[11] = {200,240,256,270,300,350,360,400,480,512,540};
static long vesares[13][2] = {320,200,360,200,320,240,360,240,320,400,
                                                                        360,400,640,350,640,400,640,480,800,600,
                                                                        1024,768,1280,1024,1600,1200};

static char option[NUMOPTIONS] = {0,0,0,0,0,0,1,0};
static char keys[NUMKEYS] =
    {
        0xc8,0xd0,0xcb,0xcd,0x2a,0x9d,0x1d,0x39,
        0x1e,0x2c,0xd1,0xc9,0x47,0x49,
        0x9c,0x1c,0xd,0xc,0xf,
    };
extern char buildkeys[NUMKEYS];






extern void __interrupt __far timerhandler(void);

long xoldtimerhandler;

#define COKE 52

#define MAXHELP2D 9
char *Help2d[MAXHELP2D]=
        {
    {" ' M = Memory"},
    {" ' 1 = Captions "},
//    {" ' 2 = "},
    {" ' 3 = Captions Toggle"},
//    {" ' 4 = MIN FRAMES RATE"},
//    {" ' 5 = MOTORCYCLE"},
    {" ' 9 = Swap HI LO"},
//    {" ' 0 = SHRINK MAP 50"},
    {" F8  = Current Wall/Sprite"},
    {" F9  = Current Sector"},
    {" [   = Search Forward"},
    {" ]   = Search Backward"},
        {" ~   = HELP OFF"}
        };

#define MAXMODE32D 7
char *Mode32d[MAXMODE32D]=
        {
    {"NONE"},
    {"SECTORS"},
    {"WALLS"},
    {"SPRITES"},
    {"ALL"},
    {"ITEMS ONLY"},
    {"CURRENT SPRITE ONLY"}
    };

#define MAXSKILL 5
char *SKILLMODE[MAXSKILL]=
        {
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    {"BEGINNER"},
    {"EASY"},
    {"NORMAL"},
    {"NOT EASY"},
    {"ALL"},
#else
    {"1 PIECE OF CAKE"},
    {"2 LET'S ROCK"},
    {"3 COME GET SOME"},
    {"4 DAMN I'M GOOD"},
    {"ALL SKILLS"},
#endif
    };

#define MAXNOSPRITES 4
char *ALPHABEASTLOADOMAGA1[MAXNOSPRITES]=
        {
    {"DISPLAY ALL SPRITES"},
    {"NO EFFECTORS"},
    {"NO ACTORS"},
    {"NONE"},
    };

short MinRate=24, MinD=3;
// CTW - MODIFICATION
// Good to know Allen has changed in all these years. ;)
// CTW END - MODIFICATION
char *Slow[8]=
        {
        {"SALES = 0,000,000  ***********************"},
        {"100% OF NOTHING IS !! ********************"},
        {"RENDER IN PROGRESS ***********************"},
        {"YOUR MOTHER IS A WHORE *******************"},
        {"YOU SUCK DONKEY **************************"},
        {"FUCKIN PISS ANT **************************"},
        {"PISS ANT *********************************"},
    {"SLOW *************************************"}
    };

#if (APPVER_DN3DREV < AV_DR_DN3D14)
#define MAXHELP3D 15
#else
#define MAXHELP3D 20
#endif
char *Help3d[MAXHELP3D]=
        {
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    {"3D KEYS HELP"},
#else
    {"    3D KEYS HELP"},
#endif
        {" "},
    {" F1 = HELP TOGGLE"},
    {" ' R = FRAMERATE TOGGLE"},
    {" ' D = SKILL MODE"},
    {" ' W = TOGGLE SPRITE DISPLAY"},
    {" ' G = GRAPHIC TOGGLE"},
    {" ' Y = TOGGLE PURPLE BACKGROUND"},
        {" ' ENTER = COPY GRAPHIC ONLY"},
    {" ' T = CHANGE LOTAG"},
    {" ' H = CHANGE HITAG"},
    {" ' S = CHANGE SHADE"},
    {" ' V = CHANGE VISIBILITY"},
    {" ' C = CHANGE GLOBAL SHADE"},
    {" ' DEL = CSTAT=0"},
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
    {" ' - = GAMMA "},
    {" ' + = GAMMA "},
    {" ' P = GLOBAL PARALLEX PANNING OFF "},
    {" F6  = NEXT TAG "},
    {" ' / = AUTOSIZE SPRITE"},
#endif
        };


/******* VARS ********/

static char tempbuf[1024]; //1024
#if (APPVER_DN3DREV < AV_DR_DN3D14)
static int numsprite[MAXSPRITES];
static int multisprite[MAXSPRITES];
#else
static int numsprite[MAXTILES];
static int multisprite[MAXTILES];
#endif
static char lo[32];
static const char *levelname;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
static short autosizespritenum=-1;
static unsigned char debuginfotimeleft=0;
#endif
static short curwall=0,wallpicnum=0,curwallnum=0;
static short cursprite=0,curspritenum=0;
static short cursector_lotag=0,cursectornum=0;
static short search_lotag=0,search_hitag=0;
static char wallsprite=0;
static char helpon=0;
static char on2d3d=0;
//static char onwater=0;
static char onnames=4;
static char usedcount=0;
long mousxplc,mousyplc;
long ppointhighlight;
static counter=0;
char nosprites=0,purpleon=0,skill=4;
char framerateon=1,tabgraphic=0;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
static char gamma=0;
static char quickspeed=0;
static char runrate=1;
static char autosize=1;
static char autorun=0;
// VERSIONS RESTORATION - This *replaces* the function from BUILD.C
//static int getnumber256(char namestart[80], short num, long maxnumber);
#endif


static char sidemode=0;
extern long vel, svel, angvel;
long xvel, yvel, hvel, timeoff;

static char once=0;






#pragma aux mulscale =\
        "imul ebx",\
        "shrd eax, edx, cl",\
        parm [eax][ebx][ecx]\
        modify [edx]\


void ExtLoadMap(char *mapname)
{


 long i;
 long sky=0;
 int j;


 // PreCache Wall Tiles
    for(j=0;j<numwalls;j++)
        if(waloff[wall[j].picnum] == 0)
            loadtile(wall[j].picnum);


 // Presize Sprites
    for(j=0;j<MAXSPRITES;j++)
        {
        if(tilesizx[sprite[j].picnum]==0 || tilesizy[sprite[j].picnum]==0)
        sprite[j].picnum=0;

#if (APPVER_DN3DREV < AV_DR_DN3D14)
        if(sprite[j].picnum>=20 && sprite[j].picnum<=59)
        {
            if(sprite[j].picnum==26) {sprite[j].xrepeat = 8; sprite[j].yrepeat = 8;}
            else {sprite[j].xrepeat = 32; sprite[j].yrepeat = 32;}
        }
#else
        autosizesprite(j);
#endif

    }



    levelname=mapname;
    pskyoff[0]=0;
    for(i=0;i<8;i++) pskyoff[i]=0;

    for(i=0;i<numsectors;i++)
    {
        switch(sector[i].ceilingpicnum)
        {
            case MOONSKY1 :
            case BIGORBIT1 : // orbit
            case LA : // la city
                sky=sector[i].ceilingpicnum;
            break;
        }
    }

    switch(sky)
    {
        case MOONSKY1 :
    //        earth          mountian   mountain         sun
            pskyoff[6]=1; pskyoff[1]=2; pskyoff[4]=2; pskyoff[2]=3;
            break;

        case BIGORBIT1 : // orbit
    //       earth1         2           3           moon/sun
            pskyoff[5]=1; pskyoff[6]=2; pskyoff[7]=3; pskyoff[2]=4;
        break;

        case LA : // la city
    //       earth1         2           3           moon/sun
            pskyoff[0]=1; pskyoff[1]=2; pskyoff[2]=1; pskyoff[3]=3;
            pskyoff[4]=4; pskyoff[5]=0; pskyoff[6]=2; pskyoff[7]=3;
        break;
    }

    pskybits=3;
    parallaxtype=0;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
    autosizespritenum=-1;
#endif

}

void overwritesprite (long thex, long they, short tilenum,signed char shade, char stat, char dapalnum)
{
        rotatesprite(thex<<16,they<<16,65536L,(stat&8)<<7,tilenum,shade,dapalnum,
                ((stat&1^1)<<4)+(stat&2)+((stat&4)>>2)+((stat&16)>>2)^((stat&8)>>1),
                windowx1,windowy1,windowx2,windowy2);
}

void putsprite (long thex, long they, long zoom, short rot, short tilenum, signed char shade, char dapalnum)
{char stat=0;
    rotatesprite(thex<<16,they<<16,65536L-zoom,(rot+(stat&8))<<7,tilenum,shade,dapalnum,
                ((stat&1^1)<<4)+(stat&2)+((stat&4)>>2)+((stat&16)>>2)^((stat&8)>>1),
                windowx1,windowy1,windowx2,windowy2);
}


void ExtSaveMap(const char *mapname)
{
         saveboard("backup.map",&posx,&posy,&posz,&ang,&cursectnum);
}

const char *ExtGetSectorCaption(short sectnum)
{

    if(!(onnames==1 || onnames==4))
    {
        tempbuf[0] = 0;
        return(tempbuf);
    }


        if ((sector[sectnum].lotag|sector[sectnum].hitag) == 0)
        {
                  tempbuf[0] = 0;
        }
        else
        {
                switch((unsigned short)sector[sectnum].lotag)
                {
//       case 1 : sprintf(lo,"WATER"); break;
//       case 2 : sprintf(lo,"UNDERWATER"); break;
//       case 3 : sprintf(lo,"EARTHQUAKE"); break;
                 default : sprintf(lo,"%hu",(unsigned short)sector[sectnum].lotag); break;
                }
        sprintf(tempbuf,"%hu,%s",
                (unsigned short)sector[sectnum].hitag,
                lo);
        }
         return(tempbuf);
}

const char *ExtGetWallCaption(short wallnum)
{
    long i=0;

    if(!(onnames==2 || onnames==4))
    {
        tempbuf[0] = 0;
        return(tempbuf);
    }


    if(keystatus[0x57]>0) // f11   Grab pic 0x4e +
    {
        wallpicnum = wall[curwall].picnum;
        sprintf(tempbuf,"Grabed Wall Picnum %ld",wallpicnum);
        printmessage16(tempbuf);
    }


    // HERE

    if(keystatus[0x1a]>0) // [     search backward
    {
       keystatus[0x1a]=0;
        if(wallsprite==0)
        { SearchSectorsBackward();
        } else

        if(wallsprite==1)
        {
            if(curwallnum>0) curwallnum--;
            for(i=curwallnum;i>=0;i--)
            {
                if(
                    (wall[i].picnum==wall[curwall].picnum)
                    &&((search_lotag==0)||
                      (search_lotag!=0 && search_lotag==wall[i].lotag))
                    &&((search_hitag==0)||
                      (search_hitag!=0 && search_hitag==wall[i].hitag))
                  )
                {
                    posx=(wall[i].x)-(( (wall[i].x)-(wall[wall[i].point2].x) )/2);
                    posy=(wall[i].y)-(( (wall[i].y)-(wall[wall[i].point2].y) )/2);
                    printmessage16("< Wall Search : Found");
//                    curwallnum--;
                    keystatus[0x1a]=0;
                    return(tempbuf);
                }
                curwallnum--;
            }
            printmessage16("< Wall Search : none");
        } else

        if(wallsprite==2)
        {
            if(curspritenum>0) curspritenum--;
            for(i=curspritenum;i>=0;i--)
            {

                if(
                    (sprite[i].picnum==sprite[cursprite].picnum &&
                        sprite[i].statnum==0 )
                    &&((search_lotag==0)||
                      (search_lotag!=0 && search_lotag==sprite[i].lotag))
                    &&((search_hitag==0)||
                      (search_hitag!=0 && search_hitag==sprite[i].hitag))
                  )
                {
                    posx=sprite[i].x;
                    posy=sprite[i].y;
                    ang= sprite[i].ang;
                    printmessage16("< Sprite Search : Found");
//                    curspritenum--;
                    keystatus[0x1a]=0;
                    return(tempbuf);
                }
                curspritenum--;
            }
            printmessage16("< Sprite Search : none");
        }
    }


    if(keystatus[0x1b]>0) // ]     search forward
    {
       keystatus[0x1b]=0;
        if(wallsprite==0)
        { SearchSectorsForward();
        } else

        if(wallsprite==1)
        {
            if(curwallnum<MAXWALLS) curwallnum++;
            for(i=curwallnum;i<=MAXWALLS;i++)
            {
                if(
                    (wall[i].picnum==wall[curwall].picnum)
                    &&((search_lotag==0)||
                      (search_lotag!=0 && search_lotag==wall[i].lotag))
                    &&((search_hitag==0)||
                      (search_hitag!=0 && search_hitag==wall[i].hitag))
                  )
                {
                    posx=(wall[i].x)-(( (wall[i].x)-(wall[wall[i].point2].x) )/2);
                    posy=(wall[i].y)-(( (wall[i].y)-(wall[wall[i].point2].y) )/2);
                    printmessage16("> Wall Search : Found");
//                    curwallnum++;
                    keystatus[0x1b]=0;
                    return(tempbuf);
                }
                curwallnum++;
            }
            printmessage16("> Wall Search : none");
        } else

        if(wallsprite==2)
        {
            if(curspritenum<MAXSPRITES) curspritenum++;
            for(i=curspritenum;i<=MAXSPRITES;i++)
            {
                if(
                    (sprite[i].picnum==sprite[cursprite].picnum &&
                        sprite[i].statnum==0 )
                    &&((search_lotag==0)||
                      (search_lotag!=0 && search_lotag==sprite[i].lotag))
                    &&((search_hitag==0)||
                      (search_hitag!=0 && search_hitag==sprite[i].hitag))
                  )
                {
                    posx=sprite[i].x;
                    posy=sprite[i].y;
                    ang= sprite[i].ang;
                    printmessage16("> Sprite Search : Found");
//                    curspritenum++;
                    keystatus[0x1b]=0;
                    return(tempbuf);
                }
                curspritenum++;
            }
            printmessage16("> Sprite Search : none");
        }
    }


        if ((wall[wallnum].lotag|wall[wallnum].hitag) == 0)
        {
                tempbuf[0] = 0;
        }
        else
        {
                sprintf(tempbuf,"%hu,%hu",(unsigned short)wall[wallnum].hitag,
                                                                                  (unsigned short)wall[wallnum].lotag);
        }
        return(tempbuf);
} //end

const char *ExtGetSpriteCaption(short spritenum)
{


    if( onnames!=5 &&
        onnames!=6 &&
        (!(onnames==3 || onnames==4))
      )
    {
        tempbuf[0] = 0;
        return(tempbuf);
    }

    if( onnames==5 &&
        ( ((unsigned short)sprite[spritenum].picnum <= 9 ) ||
        ((unsigned short)sprite[spritenum].picnum == SEENINE )
        )
        )
    { tempbuf[0] = 0; return(tempbuf); }

    if( onnames==6 &&
        (unsigned short)sprite[spritenum].picnum != (unsigned short)sprite[cursprite].picnum
      )
    { tempbuf[0] = 0; return(tempbuf); }

    tempbuf[0] = 0;
    if ((sprite[spritenum].lotag|sprite[spritenum].hitag) == 0)
    {
        SpriteName(spritenum,lo);
        if(lo[0]!=0)
        {
            if(sprite[spritenum].pal==1) sprintf(tempbuf,"%s-M",lo);
            else sprintf(tempbuf,"%s",lo);
        }
    }
    else
        if( (unsigned short)sprite[spritenum].picnum == 175)
        {
            sprintf(lo,"%hu",(unsigned short)sprite[spritenum].lotag);
            sprintf(tempbuf,"%hu,%s",(unsigned short)sprite[spritenum].hitag,lo);
        }
        else
            {
                SpriteName(spritenum,lo);
                sprintf(tempbuf,"%hu,%hu %s",
                (unsigned short)sprite[spritenum].hitag,
                (unsigned short)sprite[spritenum].lotag,
                lo);
            }
            return(tempbuf);
} //end

//printext16 parameters:
//printext16(long xpos, long ypos, short col, short backcol,
//           char name[82], char fontsize)
//  xpos 0-639   (top left)
//  ypos 0-479   (top left)
//  col 0-15
//  backcol 0-15, -1 is transparent background
//  name
//  fontsize 0=8*8, 1=3*5

//drawline16 parameters:
// drawline16(long x1, long y1, long x2, long y2, char col)
//  x1, x2  0-639
//  y1, y2  0-143  (status bar is 144 high, origin is top-left of STATUS BAR)
//  col     0-15



void TotalMem()
{
    char incache[8192];
    int i,j,tottiles,totsprites,totactors;

    for(i=0;i<4096;i++) incache[i] = 0;

        for(i=0;i<numsectors;i++)
        {
                incache[sector[i].ceilingpicnum] = 1;
                incache[sector[i].floorpicnum] = 1;
        }
        for(i=0;i<numwalls;i++)
        {
                incache[wall[i].picnum] = 1;
                if (wall[i].overpicnum >= 0)
                        incache[wall[i].overpicnum] = 1;
        }

        tottiles = 0;
        for(i=0;i<4096;i++)
                if (incache[i] > 0)
                        tottiles += tilesizx[i]*tilesizy[i];



        for(i=0;i<4096;i++) incache[i] = 0;

        for(i=0;i<MAXSPRITES;i++)
                if (sprite[i].statnum < MAXSTATUS)
                        incache[sprite[i].picnum] = 1;
        totsprites = 0;


        for(i=0;i<4096;i++)
    {
                if (incache[i] > 0)
        {
         switch(i)
         {
            case LIZTROOP :
            case LIZTROOPRUNNING :
            case LIZTROOPSTAYPUT :
            case LIZTROOPSHOOT :
            case LIZTROOPJETPACK :
            case LIZTROOPONTOILET :
            case LIZTROOPDUCKING :
                totactors+=ActorMem(LIZTROOP);
                incache[LIZTROOP]=0;
                incache[LIZTROOPRUNNING]=0;
                incache[LIZTROOPSTAYPUT]=0;
                incache[LIZTROOPSHOOT]=0;
                incache[LIZTROOPJETPACK]=0;
                incache[LIZTROOPONTOILET]=0;
                incache[LIZTROOPDUCKING]=0;
                break;
        case OCTABRAIN :
        case OCTABRAINSTAYPUT:
        totactors+=ActorMem(OCTABRAIN);
        incache[OCTABRAIN]=0;
        incache[OCTABRAINSTAYPUT]=0;
                break;
                 case DRONE :
                totactors+=ActorMem(DRONE);
                incache[DRONE]=0;
                break;
        case COMMANDER :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case COMMANDERSTAYPUT :
#endif
        totactors+=ActorMem(COMMANDER);
        incache[COMMANDER]=0;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        incache[COMMANDERSTAYPUT]=0;
#endif
                break;
            case RECON :
                totactors+=ActorMem(RECON);
                incache[RECON]=0;
                break;
            case PIGCOP :
#if (APPVER_DN3DREV < AV_DR_DN3D14)
        totactors+=ActorMem(COMMANDER);
#else
        totactors+=ActorMem(PIGCOP);
#endif
                incache[PIGCOP]=0;
                break;
            case LIZMAN :
        case LIZMANSTAYPUT :
            case LIZMANSPITTING :
            case LIZMANFEEDING :
            case LIZMANJUMP :
                totactors+=ActorMem(LIZMAN);
                incache[LIZMAN]=0;
        incache[LIZMANSTAYPUT]=0;
                incache[LIZMANSPITTING]=0;
                incache[LIZMANFEEDING]=0;
                incache[LIZMANJUMP]=0;
                break;
            case BOSS1 :
                totactors+=ActorMem(BOSS1);
                incache[BOSS1]=0;
                break;
        case BOSS2 :
        totactors+=ActorMem(BOSS2);
        incache[BOSS2]=0;
        break;
            case BOSS3 :
                totactors+=ActorMem(BOSS3);
                incache[BOSS3]=0;
                break;

            default: totsprites += tilesizx[i]*tilesizy[i];
         }
        }
    }





    clearmidstatbar16();
    printext16(1*8,4*8,11,-1,"Memory Status",0);

    PrintStatus("Total Tiles   = ",tottiles,2,6,11);
    PrintStatus("Total Sprites = ",totsprites,2,7,11);
    PrintStatus("Total Actors  = ",totactors,2,8,11);

    PrintStatus("Total Memory  = ",(tottiles+totsprites+totactors),2,10,11);

    PrintStatus("Total W/Duke  = ",(tottiles+totsprites+totactors+ActorMem(APLAYER)),2,12,11);
        
}

void ExtShowSectorData(short sectnum)   //F5
{
    short statnum=0;
    int x,x2,y;
    int nexti;
    int i,c=0;
    int secrets=0;
    int totalactors1=0,totalactors2=0,totalactors3=0,totalactors4=0;
    int totalrespawn=0;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
    int nextfreetag=0;
    if(qsetmode==200) return;
#endif
    for(i=0;i<numsectors;i++)
    { if(sector[i].lotag==32767) secrets++;
    }

     statnum=0;
     i = headspritestat[statnum];
     while (i != -1)
     {
             nexti = nextspritestat[i];
             i = nexti;

       { switch(sprite[i].picnum)
         {
            case RECON:
            case DRONE:
            case LIZTROOPONTOILET:
            case LIZTROOPSTAYPUT:
            case LIZTROOPSHOOT:
            case LIZTROOPJETPACK:
            case LIZTROOPDUCKING:
            case LIZTROOPRUNNING:
            case LIZTROOP:
            case OCTABRAIN:
            case OCTABRAINSTAYPUT:
            case COMMANDER:
            case COMMANDERSTAYPUT:
            case EGG:
            case PIGCOP:
            case PIGCOPSTAYPUT:
            case PIGCOPDIVE:
            case LIZMAN:
            case LIZMANSTAYPUT:
            case LIZMANSPITTING:
            case LIZMANFEEDING:
            case LIZMANJUMP:
            case ORGANTIC:
            case BOSS1:
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
            case BOSS1STAYPUT:
#endif
            case BOSS2:
            case BOSS3:
            case GREENSLIME:
            case ROTATEGUN:
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
            case TANK:
            case NEWBEAST:
            case NEWBEASTSTAYPUT:
            case NEWBEASTJUMP:
            case NEWBEASTHANG:
            case NEWBEASTHANGDEAD:
            case BOSS4:
#endif
            if(sprite[i].lotag<=1) totalactors1++;
            if(sprite[i].lotag<=2) totalactors2++;
            if(sprite[i].lotag<=3) totalactors3++;
            if(sprite[i].lotag<=4) totalactors4++;
            break;

            case RESPAWN:
            totalrespawn++;

            default: break;
         }
       }
     }

#if (APPVER_DN3DREV < AV_DR_DN3D14)
     for(i=0;i<MAXSPRITES;i++) numsprite[i]=0;
     for(i=0;i<MAXSPRITES;i++) multisprite[i]=0;
#else
     for(i=0;i<MAXTILES;i++) numsprite[i]=0;
     for(i=0;i<MAXTILES;i++) multisprite[i]=0;
#endif
         for(i=0;i<MAXSPRITES;i++)
     { if(sprite[i].statnum==0)
       {
         if(sprite[i].pal!=0) multisprite[sprite[i].picnum]++;
         else numsprite[sprite[i].picnum]++;
       }
         }

        clearmidstatbar16();             //Clear middle of status bar
#if (APPVER_DN3DREV < AV_DR_DN3D14)
         sprintf(tempbuf,"Level %s",levelname);
#else
         sprintf(tempbuf,"Level %s : Next Tag %d",levelname,GetNextTag(nextfreetag));
#endif
         printmessage16(tempbuf);

     x=1; x2=14; y=4;
     printext16(x*8,y*8,11,-1,"Item Count",0);
     PrintStatus("10%health=",numsprite[COLA],x,y+2,11);
      PrintStatus("",multisprite[COLA],x2,y+2,1);
     PrintStatus("30%health=",numsprite[SIXPAK],x,y+3,11);
      PrintStatus("",multisprite[SIXPAK],x2,y+3,1);
     PrintStatus("Med-Kit  =",numsprite[FIRSTAID],x,y+4,11);
      PrintStatus("",multisprite[FIRSTAID],x2,y+4,1);
     PrintStatus("Atom     =",numsprite[ATOMICHEALTH],x,y+5,11);
      PrintStatus("",multisprite[ATOMICHEALTH],x2,y+5,1);
     PrintStatus("Shields  =",numsprite[SHIELD],x,y+6,11);
      PrintStatus("",multisprite[SHIELD],x2,y+6,1);

     x=17; x2=30; y=4;
     printext16(x*8,y*8,11,-1,"Inventory",0);
     PrintStatus("Steroids =",numsprite[STEROIDS],x,y+2,11);
      PrintStatus("",multisprite[STEROIDS],x2,y+2,1);
     PrintStatus("Airtank  =",numsprite[AIRTANK],x,y+3,11);
      PrintStatus("",multisprite[AIRTANK],x2,y+3,1);
     PrintStatus("Jetpack  =",numsprite[JETPACK],x,y+4,11);
      PrintStatus("",multisprite[JETPACK],x2,y+4,1);
     PrintStatus("Goggles  =",numsprite[HEATSENSOR],x,y+5,11);
      PrintStatus("",multisprite[HEATSENSOR],x2,y+5,1);
     PrintStatus("Boots    =",numsprite[BOOTS],x,y+6,11);
      PrintStatus("",multisprite[BOOTS],x2,y+6,1);
     PrintStatus("HoloDuke =",numsprite[HOLODUKE],x,y+7,11);
      PrintStatus("",multisprite[HOLODUKE],x2,y+7,1);
     PrintStatus("Multi D  =",numsprite[APLAYER],x,y+8,11);

     x=33; x2=46; y=4;
     printext16(x*8,y*8,11,-1,"Weapon Count",0);
     PrintStatus("Pistol   =",numsprite[FIRSTGUNSPRITE],x,y+2,11);
      PrintStatus("",multisprite[FIRSTGUNSPRITE],x2,y+2,1);
     PrintStatus("Shotgun  =",numsprite[SHOTGUNSPRITE],x,y+3,11);
      PrintStatus("",multisprite[SHOTGUNSPRITE],x2,y+3,1);
     PrintStatus("Chaingun =",numsprite[CHAINGUNSPRITE],x,y+4,11);
      PrintStatus("",multisprite[CHAINGUNSPRITE],x2,y+4,1);
     PrintStatus("RPG      =",numsprite[RPGSPRITE],x,y+5,11);
      PrintStatus("",multisprite[RPGSPRITE],x2,y+5,1);
     PrintStatus("Pipe Bomb=",numsprite[HEAVYHBOMB],x,y+6,11);
      PrintStatus("",multisprite[HEAVYHBOMB],x2,y+6,1);
     PrintStatus("Shrinker =",numsprite[SHRINKERSPRITE],x,y+7,11);
      PrintStatus("",multisprite[SHRINKERSPRITE],x2,y+7,1);
          PrintStatus("Disruptor=",numsprite[DEVISTATORSPRITE],x,y+8,11);
                PrintStatus("",multisprite[DEVISTATORSPRITE],x2,y+8,1);
     PrintStatus("Trip mine=",numsprite[TRIPBOMBSPRITE],x,y+9,11);
      PrintStatus("",multisprite[TRIPBOMBSPRITE],x2,y+9,1);
     PrintStatus("Freezeray=",numsprite[FREEZESPRITE],x,y+10,11);
      PrintStatus("",multisprite[FREEZESPRITE],x2,y+10,1);

      x=49; x2=62; y=4;
     printext16(x*8,y*8,11,-1,"Ammo Count",0);
     PrintStatus("Pistol   =",numsprite[AMMO],x,y+2,11);
      PrintStatus("",multisprite[AMMO],x2,y+2,1);
     PrintStatus("Shot     =",numsprite[SHOTGUNAMMO],x,y+3,11);
      PrintStatus("",multisprite[SHOTGUNAMMO],x2,y+3,1);
     PrintStatus("Chain    =",numsprite[BATTERYAMMO],x,y+4,11);
      PrintStatus("",multisprite[BATTERYAMMO],x2,y+4,1);
     PrintStatus("RPG Box  =",numsprite[RPGAMMO],x,y+5,11);
      PrintStatus("",multisprite[RPGAMMO],x2,y+5,1);
     PrintStatus("Pipe Bomb=",numsprite[HBOMBAMMO],x,y+6,11);
      PrintStatus("",multisprite[HBOMBAMMO],x2,y+6,1);
#if (APPVER_DN3DREV < AV_DR_DN3D14)
          PrintStatus("Disruptor=",numsprite[DEVISTATORAMMO],x,y+7,11);
                PrintStatus("",multisprite[DEVISTATORAMMO],x2,y+7,1);
     PrintStatus("Shrinker =",numsprite[CRYSTALAMMO],x,y+8,11);
      PrintStatus("",multisprite[CRYSTALAMMO],x2,y+8,1);
     PrintStatus("Freezeray=",numsprite[FREEZEAMMO],x,y+9,11);
      PrintStatus("",multisprite[FREEZEAMMO],x2,y+9,1);
#else
     PrintStatus("Shrinker =",numsprite[CRYSTALAMMO],x,y+7,11);
      PrintStatus("",multisprite[CRYSTALAMMO],x2,y+7,1);
          PrintStatus("Disruptor=",numsprite[DEVISTATORAMMO],x,y+8,11);
                PrintStatus("",multisprite[DEVISTATORAMMO],x2,y+8,1);
     PrintStatus("Enlarger =",numsprite[GROWAMMO],x,y+9,11);
      PrintStatus("",multisprite[GROWAMMO],x2,y+9,1);
     PrintStatus("Freezeray=",numsprite[FREEZEAMMO],x,y+10,11);
      PrintStatus("",multisprite[FREEZEAMMO],x2,y+10,1);
#endif

     printext16(65*8,4*8,11,-1,"MISC",0);
     PrintStatus("Secrets =",secrets,65,6,11);
#if (APPVER_DN3DREV < AV_DR_DN3D14)
     printext16(65*8,7*8,11,-1,"ACTORS",0);
#else
     printext16(65*8,7*8-2,11,-1,"ACTORS",0);
#endif
     PrintStatus("Skill 1 =",totalactors1,65,8,11);
     PrintStatus("Skill 2 =",totalactors2,65,9,11);
     PrintStatus("Skill 3 =",totalactors3,65,10,11);
     PrintStatus("Skill 4 =",totalactors4,65,11,11);
     PrintStatus("Respawn =",totalrespawn,65,12,11);


}// end ExtShowSectorData

void ExtShowWallData(short wallnum)       //F6
{
 int i,nextfreetag=0,total=0;
 char x,y;

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
 if(qsetmode==200) return;
#else // Tag code moved to separate function for most
 for(i=0;i<MAXSPRITES;i++)
 {
  if(sprite[i].statnum==0)
  switch(sprite[i].picnum)
  {
    //LOTAG
    case ACTIVATOR:
    case ACTIVATORLOCKED:
    case TOUCHPLATE:
    case MASTERSWITCH:
    case RESPAWN:
    case ACCESSSWITCH:
    case SLOTDOOR:
    case LIGHTSWITCH:
    case SPACEDOORSWITCH:
    case SPACELIGHTSWITCH:
    case FRANKENSTINESWITCH:
    case MULTISWITCH:
    case DIPSWITCH:
    case DIPSWITCH2:
    case TECHSWITCH:
    case DIPSWITCH3:
    case ACCESSSWITCH2:
    case POWERSWITCH1:
    case LOCKSWITCH1:
    case POWERSWITCH2:
    case PULLSWITCH:
    case ALIENSWITCH:
        if(sprite[i].lotag>nextfreetag) nextfreetag=1+sprite[i].lotag;
        break;

    //HITAG
    case SEENINE:
    case OOZFILTER:
        case SECTOREFFECTOR:
        if(sprite[i].lotag==10 ||
            sprite[i].lotag==27 ||
            sprite[i].lotag==28 ||
            sprite[i].lotag==29
            ) break;
        else
            if(sprite[i].hitag>nextfreetag) nextfreetag=1+sprite[i].hitag;
        break;
        default:
        break;

  }

 } // end sprite loop
#endif // APPVER_DN3DREV

 //Count Normal Actors
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    for(i=0;i<MAXSPRITES;i++) numsprite[i]=0;
    for(i=0;i<MAXSPRITES;i++) multisprite[i]=0;
#else
    for(i=0;i<MAXTILES;i++) numsprite[i]=0;
    for(i=0;i<MAXTILES;i++) multisprite[i]=0;
#endif
    for(i=0;i<MAXSPRITES;i++)
        { if(sprite[i].statnum==0)
            {
            if(sprite[i].pal!=0)
                switch(sprite[i].picnum)
                {
                    case LIZTROOP :
                    case LIZTROOPRUNNING :
                    case LIZTROOPSTAYPUT :
                    case LIZTROOPSHOOT :
                    case LIZTROOPJETPACK :
                    case LIZTROOPONTOILET :
                    case LIZTROOPDUCKING :
                        numsprite[LIZTROOP]++; break;
                    case BOSS1:
                    case BOSS1STAYPUT:
                    case BOSS1SHOOT:
                    case BOSS1LOB:
                    case BOSSTOP:
                        multisprite[BOSS1]++; break;
                    case BOSS2:
                        multisprite[BOSS2]++; break;
                    case BOSS3:
                        multisprite[BOSS3]++; break;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
                    case TANK:
                        numsprite[TANK]++; break;
                    case NEWBEAST:
                    case NEWBEASTSTAYPUT:
                    case NEWBEASTJUMP:
                    case NEWBEASTHANG:
                    case NEWBEASTHANGDEAD:
                        numsprite[NEWBEAST]++; break;
                    case BOSS4:
                        multisprite[BOSS4]++; break;
#endif

                    default:
                        break;
                }
            else
                switch(sprite[i].picnum)
                {
                    case LIZTROOP :
                    case LIZTROOPRUNNING :
                    case LIZTROOPSTAYPUT :
                    case LIZTROOPSHOOT :
                    case LIZTROOPJETPACK :
                    case LIZTROOPONTOILET :
                    case LIZTROOPDUCKING :
                        numsprite[LIZTROOP]++; break;
                    case PIGCOP:
                    case PIGCOPSTAYPUT:
                    case PIGCOPDIVE:
                        numsprite[PIGCOP]++; break;
                    case LIZMAN:
                    case LIZMANSTAYPUT:
                    case LIZMANSPITTING:
                    case LIZMANFEEDING:
                    case LIZMANJUMP:
                        numsprite[LIZMAN]++; break;
                    case BOSS1:
                    case BOSS1STAYPUT:
                    case BOSS1SHOOT:
                    case BOSS1LOB:
                    case BOSSTOP:
                        numsprite[BOSS1]++; break;
                    case COMMANDER:
                    case COMMANDERSTAYPUT:
                        numsprite[COMMANDER]++; break;
                    case OCTABRAIN:
                    case OCTABRAINSTAYPUT:
                        numsprite[OCTABRAIN]++; break;
                    case RECON:
                    case DRONE:
                    case ROTATEGUN:
                    case EGG:
                    case ORGANTIC:
                    case GREENSLIME:
                    case BOSS2:
                    case BOSS3:
                        numsprite[sprite[i].picnum]++;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
                        break;
                    case TANK:
                        numsprite[TANK]++; break;
                    case NEWBEAST:
                    case NEWBEASTSTAYPUT:
                    case NEWBEASTJUMP:
                    case NEWBEASTHANG:
                    case NEWBEASTHANGDEAD:
                        numsprite[NEWBEAST]++; break;
                    case BOSS4:
                        numsprite[sprite[i].picnum]++; break;
#endif
                    default:
                        break;

                }// end switch
            }// end if
        }//end for
    total=0;
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    for(i=0;i<MAXSPRITES;i++) if(numsprite[i]!=0) total+=numsprite[i];
    for(i=0;i<MAXSPRITES;i++) if(multisprite[i]!=0) total+=multisprite[i];
#else
    for(i=0;i<MAXTILES;i++) if(numsprite[i]!=0) total+=numsprite[i];
    for(i=0;i<MAXTILES;i++) if(multisprite[i]!=0) total+=multisprite[i];
#endif

 clearmidstatbar16();

#if (APPVER_DN3DREV < AV_DR_DN3D14)
 sprintf(tempbuf,"Level %s",levelname);
#else
 sprintf(tempbuf,"Level %s : Next Tag %d",levelname, GetNextTag(nextfreetag));
#endif
 printmessage16(tempbuf);

#if (APPVER_DN3DREV < AV_DR_DN3D14)
 sprintf(tempbuf,"Level %s Status",levelname);
 printext16(1*8,4*8,11,-1,tempbuf,0);

 PrintStatus("Next Available Tag =",nextfreetag,35,4,11);

 x=2;y=6;
#else
 x=2;y=4;
#endif
 PrintStatus("Normal Actors =",total,x,y,11);
 PrintStatus(" Liztroop  =",numsprite[LIZTROOP],x,y+1,11);
 PrintStatus(" Lizman    =",numsprite[LIZMAN],x,y+2,11);
 PrintStatus(" Commander =",numsprite[COMMANDER],x,y+3,11);
 PrintStatus(" Octabrain =",numsprite[OCTABRAIN],x,y+4,11);
 PrintStatus(" PigCop    =",numsprite[PIGCOP],x,y+5,11);
 PrintStatus(" Recon Car =",numsprite[RECON],x,y+6,11);
 PrintStatus(" Drone     =",numsprite[DRONE],x,y+7,11);
#if (APPVER_DN3DREV < AV_DR_DN3D14)
 x+=17;
 PrintStatus("Turret    =",numsprite[ROTATEGUN],x,y+1,11);
 PrintStatus("Egg       =",numsprite[EGG],x,y+2,11);
 PrintStatus("Slimer    =",numsprite[GREENSLIME],x,y+3,11);
 PrintStatus("Boss1     =",numsprite[BOSS1],x,y+4,11);
 PrintStatus("MiniBoss1 =",multisprite[BOSS1],x,y+5,11);
 PrintStatus("Boss2     =",numsprite[BOSS2],x,y+6,11);
 PrintStatus("Boss3     =",numsprite[BOSS3],x,y+7,11);
#else
 PrintStatus(" Turret    =",numsprite[ROTATEGUN],x,y+8,11);
 PrintStatus(" Egg       =",numsprite[EGG],x,y+9,11);
 x+=17;
 PrintStatus("Slimer    =",numsprite[GREENSLIME],x,y+1,11);
 PrintStatus("Boss1     =",numsprite[BOSS1],x,y+2,11);
 PrintStatus("MiniBoss1 =",multisprite[BOSS1],x,y+3,11);
 PrintStatus("Boss2     =",numsprite[BOSS2],x,y+4,11);
 PrintStatus("Boss3     =",numsprite[BOSS3],x,y+5,11);
 PrintStatus("Riot Tank =",numsprite[TANK],x,y+6,11);
 PrintStatus("New Beast =",numsprite[NEWBEAST],x,y+7,11);
 PrintStatus("Boss4     =",numsprite[BOSS4],x,y+8,11);
#endif

 //Count Respawn Actors
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    for(i=0;i<MAXSPRITES;i++) numsprite[i]=0;
    for(i=0;i<MAXSPRITES;i++) multisprite[i]=0;
#else
    for(i=0;i<MAXTILES;i++) numsprite[i]=0;
    for(i=0;i<MAXTILES;i++) multisprite[i]=0;
#endif
    for(i=0;i<MAXSPRITES;i++)
    { if(sprite[i].statnum==0 && sprite[i].picnum==RESPAWN)
      {  switch(sprite[i].hitag)
                {
                    case LIZTROOP :
                    case LIZTROOPRUNNING :
                    case LIZTROOPSTAYPUT :
                    case LIZTROOPSHOOT :
                    case LIZTROOPJETPACK :
                    case LIZTROOPONTOILET :
                    case LIZTROOPDUCKING :
                        numsprite[LIZTROOP]++; break;
                    case PIGCOP:
                    case PIGCOPSTAYPUT:
                    case PIGCOPDIVE:
                        numsprite[PIGCOP]++; break;
                    case LIZMAN:
                    case LIZMANSTAYPUT:
                    case LIZMANSPITTING:
                    case LIZMANFEEDING:
                    case LIZMANJUMP:
                        numsprite[LIZMAN]++; break;
                    case BOSS1:
                    case BOSS1STAYPUT:
                    case BOSS1SHOOT:
                    case BOSS1LOB:
                    case BOSSTOP:
                        if(sprite[i].pal!=0) multisprite[BOSS1]++;
                        else numsprite[BOSS1]++; break;
                    case COMMANDER:
                    case COMMANDERSTAYPUT:
                        numsprite[COMMANDER]++; break;
                    case OCTABRAIN:
                    case OCTABRAINSTAYPUT:
                        numsprite[OCTABRAIN]++; break;
                    case RECON:
                    case DRONE:
                    case ROTATEGUN:
                    case EGG:
                    case ORGANTIC:
                    case GREENSLIME:
                    case BOSS2:
                    case BOSS3:
                        numsprite[sprite[i].hitag]++;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
                        break;
                    case TANK:
                        numsprite[TANK]++; break;
                    case NEWBEAST:
                    case NEWBEASTSTAYPUT:
                    case NEWBEASTJUMP:
                    case NEWBEASTHANG:
                    case NEWBEASTHANGDEAD:
                        numsprite[NEWBEAST]++; break;
                    case BOSS4:
                        numsprite[sprite[i].hitag]++; break;
#endif
                    default:
                        break;
                }//end switch
      }// end if
    }// end for
    total=0;
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    for(i=0;i<MAXSPRITES;i++) if(numsprite[i]!=0) total+=numsprite[i];
    for(i=0;i<MAXSPRITES;i++) if(multisprite[i]!=0) total+=multisprite[i];
#else
    for(i=0;i<MAXTILES;i++) if(numsprite[i]!=0) total+=numsprite[i];
    for(i=0;i<MAXTILES;i++) if(multisprite[i]!=0) total+=multisprite[i];
#endif


#if (APPVER_DN3DREV < AV_DR_DN3D14)
 x=36;y=6;
 PrintStatus("Respawn",total,x,y,11);
#else
 x=36;y=4;
 PrintStatus("Respawn = ",total,x,y,11);
#endif
 PrintStatus(" Liztroop  =",numsprite[LIZTROOP],x,y+1,11);
 PrintStatus(" Lizman    =",numsprite[LIZMAN],x,y+2,11);
 PrintStatus(" Commander =",numsprite[COMMANDER],x,y+3,11);
 PrintStatus(" Octabrain =",numsprite[OCTABRAIN],x,y+4,11);
 PrintStatus(" PigCop    =",numsprite[PIGCOP],x,y+5,11);
 PrintStatus(" Recon Car =",numsprite[RECON],x,y+6,11);
 PrintStatus(" Drone     =",numsprite[DRONE],x,y+7,11);
#if (APPVER_DN3DREV < AV_DR_DN3D14)
 x+=17;
 PrintStatus("Turret    =",numsprite[ROTATEGUN],x,y+1,11);
 PrintStatus("Egg       =",numsprite[EGG],x,y+2,11);
 PrintStatus("Slimer    =",numsprite[GREENSLIME],x,y+3,11);
 PrintStatus("Boss1     =",numsprite[BOSS1],x,y+4,11);
 PrintStatus("MiniBoss1 =",multisprite[BOSS1],x,y+5,11);
 PrintStatus("Boss2     =",numsprite[BOSS2],x,y+6,11);
 PrintStatus("Boss3     =",numsprite[BOSS3],x,y+7,11);
#else
 PrintStatus(" Turret    =",numsprite[ROTATEGUN],x,y+8,11);
 PrintStatus(" Egg       =",numsprite[EGG],x,y+9,11);
 x+=17;
 PrintStatus("Slimer    =",numsprite[GREENSLIME],x,y+1,11);
 PrintStatus("Boss1     =",numsprite[BOSS1],x,y+2,11);
 PrintStatus("MiniBoss1 =",multisprite[BOSS1],x,y+3,11);
 PrintStatus("Boss2     =",numsprite[BOSS2],x,y+4,11);
 PrintStatus("Boss3     =",numsprite[BOSS3],x,y+5,11);
 PrintStatus("Riot Tank =",numsprite[BOSS3],x,y+7,11); // VERSIONS RESTORATION:
 PrintStatus("New Beast =",numsprite[TANK],x,y+6,11);  // Vanilla bug/typos
 PrintStatus("Boss4     =",numsprite[BOSS4],x,y+8,11);
#endif

}// end ExtShowWallData

void Show2dText(char *name)
{
 int i,fp;
 char t;
 char x=0,y=4,xmax=0,xx=0,col=0;
 clearmidstatbar16();
 if((fp=kopen4load(name,0)) == -1)
 {printext16(1*4,4*8,11,-1,"ERROR: file not found.",0);
  return;
 }

 t=65;
 while(t!=EOF && col<5)
 {
  kread(fp,&t,1);
  while(t!=EOF && t!='\n' && x<250)
  {
     tempbuf[x]=t;
     kread(fp,&t,1);
     x++; if(x>xmax) xmax=x;
  }
  tempbuf[x]=0;
  printext16(xx*4,(y*6)+2,11,-1,tempbuf,1);
  x=0; y++;
  if(y>18) {col++; y=6; xx+=xmax; xmax=0;}
 }
 
 kclose(fp);

}// end Show2dText

void Show3dText(char *name)
{
 int i,fp;
 char x=0,y=4,xmax=0,xx=0,col=0,t;
 if((fp=kopen4load(name,0)) == -1)
 {
    printext256(1*4,4*8,11,-1,"ERROR: file not found.",0);
    return;
 }
 t=65;
 while(t!=EOF && col<5)
 {
  kread(fp,&t,1);
  while(t!=EOF && t!='\n' && x<250)
  {
    tempbuf[x]=t;
    kread(fp,&t,1);
        x++; if(x>xmax) xmax=x;
  }
  tempbuf[x]=0;
  printext256(xx*4,(y*6)+2,11,-1,tempbuf,1);
  x=0; y++;
  if(y>18) {col++; y=6; xx+=xmax; xmax=0;}
 }

 kclose(fp);
}// end Show3dText


void ShowHelpText(char *name)
{
    FILE *fp;
    int i,t;
    char x=0,y=4,xmax=0,xx=0,col=0;
    if((fp=fopen("helpdoc.txt","rb")) == NULL)
    {
        printext256(1*4,4*8,11,-1,"ERROR: file not found.",0);
        return;
    }
/*
    fgets(tempbuf,80,fp);
    while(!feof(fp) && strcmp(tempbuf,"SectorEffector"))
    {
        fgets(tempbuf,80,fp);
    }
*/
    y=2;
    fgets(tempbuf,80,fp);
    strcat(tempbuf,"\n");
    while(!feof(fp) && !(strcmp(tempbuf,"SectorEffector")==0))
    {
        fgets(tempbuf,80,fp);
        strcat(tempbuf,"\n");
        printext256(x*4,(y*6)+2,11,-1,tempbuf,1);
        y++;
    }

    fclose(fp);
}// end ShowHelpText







void ExtShowSpriteData(short spritenum)   //F6
{
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
 if(qsetmode==200) return;
 sprintf(tempbuf,"Level %s : Next Tag %d",levelname, GetNextTag(0));
 printmessage16(tempbuf);
#endif
 Show2dText("sehelp.hlp");
}// end ExtShowSpriteData

void ExtEditSectorData(short sectnum)    //F7
{
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
 if(qsetmode==200) return;
#endif
 Show2dText("sthelp.hlp");
}// end ExtEditSectorData

void ExtEditWallData(short wallnum)       //F8
{
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
    if(qsetmode==200) return;
#endif
    if(qsetmode!=480) return;
    wallsprite=1;
    curwall = wallnum;
    curwallnum = 0;
    curspritenum = 0;
    cursectornum = 0;
    search_lotag=getnumber16("Enter Wall Search Lo-Tag : ", search_lotag, 65536L);
    search_hitag=getnumber16("Enter Wall Search Hi-Tag : ", search_hitag, 65536L);
    sprintf(tempbuf,"Current Wall %ld lo=%ld hi=%ld",
        curwall,search_lotag,search_hitag);
    printmessage16(tempbuf);

}

void ExtEditSpriteData(short spritenum)   //F8
{
    if(qsetmode!=480) return;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
    if(qsetmode==200) return;
#endif
    wallsprite=2;
    cursprite = spritenum;
    curwallnum = 0;
    curspritenum = 0;
    cursectornum = 0;
    search_lotag=getnumber16("Enter Sprite Search Lo-Tag : ", search_lotag, 65536L);
    search_hitag=getnumber16("Enter Sprite Search Hi-Tag : ", search_hitag, 65536L);
    sprintf(tempbuf,"Current Sprite %ld %s lo=%ld hi=%ld",
        cursprite,names[sprite[cursprite].picnum],search_lotag,search_hitag);
    printmessage16(tempbuf);

}


void PrintStatus(char *string,int num,char x,char y,char color)
{
     sprintf(tempbuf,"%s %d",string,num);
#if (APPVER_DN3DREV < AV_DR_DN3D14)
     printext16(x*8,y*8,color,-1,tempbuf,0);
#else
     printext16(x*8,y*8-2,color,-1,tempbuf,0);
#endif
}

#if (APPVER_DN3DREV < AV_DR_DN3D14) // VERSIONS RESTORATION - Convenience macros

#define SHOW_TEXT_COMMON(string) printext256(1*4,1*8,11,-1,string,0)
#define SHOW_TEXT_NEAR SHOW_TEXT_COMMON
#define SHOW_TEXT_MED SHOW_TEXT_COMMON
#define SHOW_TEXT_FAR SHOW_TEXT_COMMON

#else

#define SHOW_TEXT_NEAR(string) Message(string,31)
#define SHOW_TEXT_MED(string) Message(string,95)
#define SHOW_TEXT_FAR(string) Message(string,143)

static char messagecolor = 31;
static unsigned char messagedelay = 0;
static char messagebuf[1024];

void Message(char *string, char color)
{
    sprintf(messagebuf,string,0); // VERSIONS RESTORATION - THIS IS *NOT* SECURE!!
    messagedelay = 128;
    messagecolor = color;
}

void ShowMessage()
{
    if (messagedelay<1) return;
    messagedelay--;
    printext256(0,8,messagecolor,0,messagebuf,0);
}

#endif // APPVER_DN3DREV

void SpriteName(short spritenum, char *lo2)
{
    sprintf(lo2,names[sprite[spritenum].picnum]);
}// end SpriteName

char GAMEpalette[768];
char WATERpalette[768];
char SLIMEpalette[768];
char TITLEpalette[768];
char REALMSpalette[768];
char BOSS1palette[768];

ReadGamePalette()
{
 int i,fp;
 if((fp=kopen4load("palette.dat",0)) == -1) return;
 kread(fp,GAMEpalette,768);
 for(i=0;i<768;++i) GAMEpalette[i]=GAMEpalette[i];
 kclose(fp);
}


void ReadPaletteTable()
{
 int i,j,fp;
 char num_tables,lookup_num;
 if((fp=kopen4load("lookup.dat",0)) == -1) return;
 kread(fp,&num_tables,1);
 for(j=0;j<num_tables;j++)
 {
  kread(fp,&lookup_num,1);
  kread(fp,tempbuf,256);
  makepalookup(lookup_num,tempbuf,0,0,0,1);
 }
 kread(fp,WATERpalette,768);
 kread(fp,SLIMEpalette,768);
 kread(fp,TITLEpalette,768);
 kread(fp,REALMSpalette,768);
 kread(fp,BOSS1palette,768);
 kclose(fp);
 ReadGamePalette();
}// end ReadPaletteTable

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
static long count;
#endif

void Keys3d(void)
{
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    long i,count,rate,nexti;
    short statnum=0;
#else
    long i,rate,nexti;
    short statnum=0;
    int nextfreetag=0;
    ShowMessage();
    ShowDebugInfo();
#endif



//  DoWater(horiz);

        i = totalclock;
        if (i != clockval[clockcnt])
        {
     rate=(120<<4)/(i-clockval[clockcnt]);
     if(framerateon)
     {
                sprintf(tempbuf,"%ld",rate);
/*        if(rate<MinRate)
                { sprintf(tempbuf,"%ld WARNING : %s",rate,Slow[rate/MinD]);
                  printext256(0*8,0*8,255,-1,tempbuf,1);
                }
                else
*/
        {sprintf(tempbuf,"%ld",rate); printext256(0*8,0*8,15,-1,tempbuf,1);}
     }
        }
        clockval[clockcnt] = i; 
        clockcnt = ((clockcnt+1)&15);

 if(helpon==1)
 {
  for(i=0;i<MAXHELP3D;i++)
#if (APPVER_DN3DREV < AV_DR_DN3D14)
  {printext256(0*8,8+(i*8),15,-1,Help3d[i],1);
#else
  {printext256(0*8,i*8,15,-1,Help3d[i],1);
#endif
   switch(i)
   {
        case 3: sprintf(tempbuf,"%d",framerateon); break;
        case 4: sprintf(tempbuf,"%s",SKILLMODE[skill]); break;
        case 5: sprintf(tempbuf,"%s",ALPHABEASTLOADOMAGA1[nosprites]); break;
        case 6: sprintf(tempbuf,"%d",tabgraphic); break;
        case 7: sprintf(tempbuf,"%d",purpleon); break;
        default : sprintf(tempbuf," "); break;
   }
#if (APPVER_DN3DREV < AV_DR_DN3D14)
   printext256(20*8,8+(i*8),15,-1,tempbuf,1);
#else
   printext256(20*8,i*8,15,-1,tempbuf,1);
#endif
  }
  Ver();
 }

#if (APPVER_DN3DREV < AV_DR_DN3D14)
 if(purpleon) printext256(1*4,1*8,11,-1,"Purple ON",0);
#else
 if(purpleon) printext256(0,1*8,11,-1,"Purple ON",0);
#endif

 if(sector[cursectnum].lotag==2)
 { if(sector[cursectnum].floorpal==8) SetBOSS1Palette();
   else SetWATERPalette();
 }
 else SetGAMEPalette();


//Stick this in 3D part of ExtCheckKeys
//Also choose your own key scan codes



#if (APPVER_DN3DREV >= AV_DR_DN3D14)
 if(keystatus[0x28] && keystatus[0x3a]) // ' CAPS
    {
        keystatus[0x3a] = 0;
        autorun=!autorun;
        if(autorun) SHOW_TEXT_MED("Autorun On");
        else SHOW_TEXT_FAR("Autorun Off");
    }
 if(keystatus[0x28] && keystatus[0x35]) // ' .
   if (searchstat==3)
    {
        keystatus[0x35] = 0;
        if(autosize)
        {
            SHOW_TEXT_NEAR("Autosize");
            autosizesprite(searchwall);
        }
    }
 if(keystatus[0x28] && keystatus[0x1e]) // ' a
    {
        keystatus[0x1e] = 0;
        autosize=!autosize;
        if(autosize) SHOW_TEXT_MED("Autosize On");
        else SHOW_TEXT_FAR("Autosize Off");
    }
 if (autosize && autosizespritenum!=-1)
    {
        if(autosize) SHOW_TEXT_NEAR("Autosize");
        else autosizesprite(autosizespritenum);
    }
 autosizespritenum = -1;
 if (searchstat==3 && keystatus[0x52]==1) // INS
    {
        if(keystatus[0x4b]==1) // Left
        {
            sprite[searchwall].x -= 128;
            keystatus[0x4b] = 0;
        }
        if(keystatus[0x4d]==1) // Right
        {
            sprite[searchwall].x += 128;
            keystatus[0x4d] = 0;
        }
        if(keystatus[0x48]==1) // Up
        {
            sprite[searchwall].y -= 128;
            keystatus[0x48] = 0;
        }
        if(keystatus[0x50]==1) // Down
        {
            sprite[searchwall].y += 128;
            keystatus[0x50] = 0;
        }
    }
 if(keystatus[0x28]==1 && keystatus[0x10]==1) // ' q
    {
        keystatus[0x10]=0;
        quickspeed=!quickspeed;
        if(quickspeed) SHOW_TEXT_MED("Quick Speed On");
        else SHOW_TEXT_FAR("Quick Speed Off");
    }
 if(keystatus[0x2a] || autorun) // LShift
    {
        if(keystatus[0xcb] && keystatus[0x38]!=1 && quickspeed==1) // Left, no Alt
            ang -= 16;
        if(keystatus[0xcd] && keystatus[0x38]!=1 && quickspeed==1) // Right, no Alt
            ang += 16;
        runrate = 1;
        if(keystatus[0xc8] || keystatus[0xcd]) // Up/Down
            if(quickspeed==1)
                runrate = 2;
    }
 if(keystatus[0x9d]==1 && keystatus[0xc9]==1) // Ctrl PgUp
    switch (searchstat)
    {
        case 1:
            sector[searchsector].ceilingz -= 8192;
            keystatus[0xc9] = 0;
            break;
        case 2:
            sector[searchsector].floorz -= 8192;
            keystatus[0xc9] = 0;
            break;
    }
 if(keystatus[0x9d]==1 && keystatus[0xd1]==1) // Ctrl PgDn
    switch (searchstat)
    {
        case 1:
            sector[searchsector].ceilingz += 8192;
            keystatus[0xd1] = 0;
            break;
        case 2:
            sector[searchsector].floorz += 8192;
            keystatus[0xd1] = 0;
            break;
    }
 if(keystatus[0x38]!=1 && keystatus[0x28]!=1 && keystatus[0x2e]==1) // c, no Alt or '
    switch (searchstat)
    {
        case 3:
        if (sprite[searchwall].cstat&0x80) SHOW_TEXT_FAR("Sprite Center OFF");
        else SHOW_TEXT_NEAR("Sprite Center ON");
            break;
    }
 if(keystatus[0x28]!=1 && keystatus[0x13]==1) // r, no '
    switch (searchstat)
    {
        case 0:
        case 4:
            break;
        case 1:
            if (sector[searchsector].ceilingstat&0x41) SHOW_TEXT_FAR("Ceiling Relative OFF");
            else SHOW_TEXT_NEAR("Ceiling Relative ON");
            break;
        case 2:
            if (sector[searchsector].floorstat&0x41) SHOW_TEXT_FAR("Floor Relative OFF");
            else SHOW_TEXT_NEAR("Floor Relative ON");
            break;
        case 3:
            break;
    }
 if(keystatus[0x28]==1 && keystatus[0xc]==1) // ' -
    {
        keystatus[0xc] = 0;
        if(gamma<2) gamma=0;
        else gamma-=2;
        setbrightness(gamma,palette);
        sprintf(tempbuf,"Gamma Correction Value : %d",gamma);
        SHOW_TEXT_NEAR(tempbuf);
    }
 if(keystatus[0x28]==1 && keystatus[0xd]==1) // ' =
    {
        keystatus[0xd] = 0;
        if(gamma>14) gamma=16;
        else gamma+=2;
        setbrightness(gamma,palette);
        sprintf(tempbuf,"Gamma Correction Value : %d",gamma);
        SHOW_TEXT_NEAR(tempbuf);
    }
#endif // APPVER_DN3DREV >= AV_DR_DN3D14
 if(keystatus[0x28]==1 && keystatus[0x20]==1) // ' d

#if (APPVER_DN3DREV < AV_DR_DN3D14)
    {
        ShowHelpText("SectorEffector");
    }
#else // VERSIONS RESTORATION - Uncomment for 1.4 and modify print func
    {
        keystatus[0x20] = 0;
        skill++; if(skill>4) skill=0;
        sprintf(tempbuf,"%s",SKILLMODE[skill]);
        SHOW_TEXT_FAR(tempbuf);
    }
#endif
 if(keystatus[0x28]==1 && keystatus[0x22]==1) // ' g
    {
        keystatus[0x22] = 0;
        tabgraphic=!tabgraphic;
        if(tabgraphic) SHOW_TEXT_MED("Graphics ON");
        else SHOW_TEXT_FAR("Graphics OFF");
    }

 if(keystatus[0x28]==1 && keystatus[0x13]==1) // ' r
    {
        keystatus[0x13] = 0;
        framerateon=!framerateon;
        if(framerateon) SHOW_TEXT_MED("Framerate ON");
        else SHOW_TEXT_FAR("Framerate OFF");
    }

 if(keystatus[0x28]==1 && keystatus[0x11]==1) // ' w
    {
        keystatus[0x11] = 0;
        nosprites++; if(nosprites>3) nosprites=0;
        sprintf(tempbuf,"%s",ALPHABEASTLOADOMAGA1[nosprites]);
        SHOW_TEXT_NEAR(tempbuf);
    }

 if(keystatus[0x28]==1 && keystatus[0x15]==1) // ' y
    {
        keystatus[0x15] = 0;
        purpleon=!purpleon; if(nosprites>3) nosprites=0;
        if(purpleon) SHOW_TEXT_MED("Purple ON");
        else SHOW_TEXT_FAR("Purple OFF");
    }

 if(keystatus[0x28]==1 && keystatus[0x2e]==1) // ' C
          {
         keystatus[0x2e] = 0;
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
                SHOW_TEXT_MED("Global Shade changed");
#endif
                 switch (searchstat)
                 {
                        case 0: case 4:
                for(i=0;i<MAXWALLS;i++)
                {
                 if(wall[i].picnum==temppicnum) {wall[i].shade=tempshade;}
                }
                                break;
                        case 1: case 2:
                for(i=0;i<MAXSECTORS;i++)
                {
                if(searchstat==1)
                 if(sector[i].ceilingpicnum==temppicnum) {sector[i].ceilingshade=tempshade;}
                if(searchstat==2)
                 if(sector[i].floorpicnum==temppicnum) {sector[i].floorshade=tempshade;}
                }
                                break;
                        case 3:
                for(i=0;i<MAXSPRITES;i++)
                {
                 if(sprite[i].picnum==temppicnum) {sprite[i].shade=tempshade;}
                }
                                break;
                 }
      }

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
 if(keystatus[0x28]==1 && keystatus[0x19]==1) // ' P
          {
                keystatus[0x19] = 0;
                SHOW_TEXT_FAR("Global Parallax Panning Off");
                switch (searchstat)
                {
                    case 1: case 2:
                    for(i=0;i<MAXSECTORS;i++)
                    {
                        if(searchstat==1)
                         if(sector[i].ceilingstat&1) { sector[i].ceilingxpanning=0; sector[i].ceilingypanning=0; }

                        if(searchstat==2)
                         if(sector[i].floorstat&1) { sector[i].floorxpanning=0; sector[i].floorypanning=0; }
                    }
                }
          }

 if(keystatus[0x40]==1) // F6
    {
        keystatus[0x40] = 0;
        sprintf(tempbuf,"Level %s : Next Tag %d",levelname,GetNextTag(nextfreetag));
        SHOW_TEXT_NEAR(tempbuf);
    }

#endif
 if(keystatus[0x28]==1 && keystatus[0x14]==1) // ' T
          {
                 keystatus[0x14] = 0;
                 switch (searchstat)
                 {
                        case 0: case 4:
                                strcpy(tempbuf,"Wall lotag: ");
                                wall[searchwall].lotag =
                                getnumber256(tempbuf,wall[searchwall].lotag,65536L);
                                break;
                        case 1: case 2:
                                strcpy(tempbuf,"Sector lotag: ");
                                sector[searchsector].lotag =
                                getnumber256(tempbuf,sector[searchsector].lotag,65536L);
                                break;
                        case 3:
                                strcpy(tempbuf,"Sprite lotag: ");
                                sprite[searchwall].lotag =
                                getnumber256(tempbuf,sprite[searchwall].lotag,65536L);
                                break;
                 }
          }

 if(keystatus[0x28]==1 && keystatus[0x23]==1) // ' H
          {
                 keystatus[0x23] = 0;
                 switch (searchstat)
                 {
                        case 0: case 4:
                                strcpy(tempbuf,"Wall hitag: ");
                                wall[searchwall].hitag =
                                getnumber256(tempbuf,wall[searchwall].hitag,65536L);
                                break;
                        case 1: case 2:
                                strcpy(tempbuf,"Sector hitag: ");
                                sector[searchsector].hitag =
                                getnumber256(tempbuf,sector[searchsector].hitag,65536L);
                                break;
                        case 3:
                                strcpy(tempbuf,"Sprite hitag: ");
                                sprite[searchwall].hitag =
                                getnumber256(tempbuf,sprite[searchwall].hitag,65536L);
                                break;
                 }
          }

 if(keystatus[0x28]==1 && keystatus[0x1f]==1) // ' S
          {
         keystatus[0x1f] = 0;
                 switch (searchstat)
                 {
                        case 0: case 4:
                strcpy(tempbuf,"Wall shade: ");
                wall[searchwall].shade =
                getnumber256(tempbuf,wall[searchwall].shade,65536L);
                                break;
                        case 1: case 2:
                strcpy(tempbuf,"Sector shade: ");
                if(searchstat==1)
                 sector[searchsector].ceilingshade =
                 getnumber256(tempbuf,sector[searchsector].ceilingshade,65536L);
                if(searchstat==2)
                 sector[searchsector].floorshade =
                 getnumber256(tempbuf,sector[searchsector].floorshade,65536L);
                                break;
                        case 3:
                strcpy(tempbuf,"Sprite shade: ");
                sprite[searchwall].shade =
                getnumber256(tempbuf,sprite[searchwall].shade,65536L);
                                break;
                 }
      }

 if(keystatus[0x28]==1 && keystatus[0x2f]==1) // ' V
          {
         keystatus[0x2f] = 0;
                 switch (searchstat)
                 {
                        case 1: case 2:
                strcpy(tempbuf,"Sector visibility: ");
                sector[searchsector].visibility =
                 getnumber256(tempbuf,sector[searchsector].visibility,65536L);
                                break;
                 }
      }

 if(keystatus[0x28]==1 && keystatus[0x0e]==1) // ' del
          {
                 keystatus[0x23] = 0;
                 switch (searchstat)
                 {
            case 0: case 4: wall[searchwall].cstat = 0; break;
//            case 1: case 2: sector[searchsector].cstat = 0; break;
            case 3: sprite[searchwall].cstat = 0; break;
                 }
      }




 if(keystatus[0x0f]>0) // TAB : USED
 {
#if (APPVER_DN3DREV < AV_DR_DN3D14)
  usedcount=!usedcount;
#else
  usedcount=1;
  debuginfotimeleft=172;
#endif

  count=0;
  for(i=0;i<numwalls;i++)
  { if(wall[i].picnum == temppicnum) count++;
        if(wall[i].overpicnum == temppicnum) count++;
  }
  for(i=0;i<numwalls;i++)
  { if(sector[i].ceilingpicnum == temppicnum) count++;
        if(sector[i].floorpicnum == temppicnum) count++;
  }
        statnum = 0;        //status 1
        i = headspritestat[statnum];
                while (i != -1)
                {
                        nexti = nextspritestat[i];

                        //your code goes here
                        //ex: printf("Sprite %d has a status of 1 (active)\n",i,statnum);

        if(sprite[i].picnum == temppicnum) count++;
                        i = nexti;
                }

 }


 if(keystatus[0x3b]==1) // F1
 { helpon=!helpon; keystatus[0x23]=0;
   keystatus[0x3b]=0;
 }




 if(keystatus[0x28]==1 && keystatus[0x1c]==1) // ' ENTER
 {
  printext256(0,0,15,0,"Put Graphic ONLY",0);
  keystatus[0x1c]=0;
  switch(searchstat)
  {
        case 0 : wall[searchwall].picnum = temppicnum; break;
        case 1 : sector[searchsector].ceilingpicnum = temppicnum; break;
        case 2 : sector[searchsector].floorpicnum = temppicnum; break;
        case 3 : sprite[searchwall].picnum = temppicnum; break;
        case 4 : wall[searchwall].overpicnum = temppicnum; break;
  }
 }

 if(keystatus[0x0f]==1) //TAB
 {
  switch(searchstat)
  {
        case 0 :
         temppicnum = wall[searchwall].picnum;
         tempshade = wall[searchwall].shade;
         tempxrepeat = wall[searchwall].xrepeat;
         tempyrepeat = wall[searchwall].yrepeat;
         tempcstat = wall[searchwall].cstat;
         temphitag = wall[searchwall].hitag;
         templotag = wall[searchwall].lotag;
         break;
        case 1 :
         temppicnum = sector[searchsector].ceilingpicnum;
         tempshade = sector[searchsector].ceilingshade;
         tempxrepeat = sector[searchsector].ceilingxpanning;
         tempyrepeat = sector[searchsector].ceilingypanning;
         tempcstat = sector[searchsector].ceilingstat;
     temphitag = sector[searchsector].hitag; //wall
     templotag = sector[searchsector].lotag; //wall
         break;
        case 2 :
         temppicnum = sector[searchsector].floorpicnum;
         tempshade = sector[searchsector].floorshade;
         tempxrepeat = sector[searchsector].floorxpanning;
         tempyrepeat = sector[searchsector].floorypanning;
         tempcstat = sector[searchsector].floorstat;
     temphitag = sector[searchsector].hitag; //wall
     templotag = sector[searchsector].lotag; //wall
         break;
        case 3 :
         temppicnum = sprite[searchwall].picnum;
         tempshade = sprite[searchwall].shade;
         tempxrepeat = sprite[searchwall].xrepeat;
         tempyrepeat = sprite[searchwall].yrepeat;
         tempcstat = sprite[searchwall].cstat;
         temphitag = sprite[searchwall].hitag;
         templotag = sprite[searchwall].lotag;
         break;
        case 4 :
         temppicnum = wall[searchwall].overpicnum;
         tempshade = wall[searchwall].shade;
         tempxrepeat = wall[searchwall].xrepeat;
         tempyrepeat = wall[searchwall].yrepeat;
         tempcstat = wall[searchwall].cstat;
         temphitag = wall[searchwall].hitag;
         templotag = wall[searchwall].lotag;
  }// end switch
 }// end TAB

}// end 3d


void Keys2d(void)
{
    short temp=0;
    int i=0, j;
/*
   for(i=0;i<0x50;i++)
   {if(keystatus[i]==1) {sprintf(tempbuf,"key %ld",i); printmessage16(tempbuf);}}
*/

    if(keystatus[0x3b]==1 || (keystatus[0x28]==1 && keystatus[0x29]==1)) //F1 or ' ~
    {
        keystatus[0x23]=0;
        clearmidstatbar16();
        for(i=0;i<MAXHELP2D;i++) {printext16(0*8,32+(i*8),15,-1,Help2d[i],0);}
        Ver();
    }

 getpoint(searchx,searchy,&mousxplc,&mousyplc);
 ppointhighlight = getpointhighlight(mousxplc,mousyplc);

 if ((ppointhighlight&0xc000) == 16384)
 {
  // sprite[ppointhighlight&16383].cstat ^= 1;
//  cursprite=(ppointhighlight&16383);
 }

 if(keystatus[67]==1) // F9 f1=3b
 {
    keystatus[67] = 0;
    wallsprite=0;
    curwall = 0;
    curwallnum = 0;
    curspritenum = 0;
    cursectornum=0;
    cursector_lotag=getnumber16("Enter Sector Lo-Tag : ", cursector_lotag, 65536L);
    sprintf(tempbuf,"Current Sector Lo-Tag %ld",cursector_lotag);
    printmessage16(tempbuf);
 }


 if(keystatus[0x28]==1 && keystatus[0x2c]==1) // ' z
 { keystatus[0x2c]=0;
    sprite[cursprite].z=getnumber16("Sprite Zpos :    ",sprite[cursprite].z, 0x7fffffff);
 }


 if(keystatus[0x28]==1 && keystatus[0x0b]==1) // ' 0
 {
        keystatus[0x0b]=0;
        for(i=0;i<numsectors;i++)
        {
                sector[i].ceilingz >>= 1;
                sector[i].floorz >>= 1;
        }
        for(i=0;i<numwalls;i++)
        {
                wall[i].x >>= 1;
                wall[i].y >>= 1;
                wall[i].yrepeat = min(wall[i].yrepeat<<1,255); 
        }
        for(i=0;i<MAXSPRITES;i++)
        {
                sprite[i].x >>= 1;
                sprite[i].y >>= 1;
                sprite[i].z >>= 1;
                sprite[i].xrepeat = max(sprite[i].xrepeat>>1,1); 
                sprite[i].yrepeat = max(sprite[i].yrepeat>>1,1); 
        }
 }


/* shrink
  if(keystatus[0x28]==1 && keystatus[0x0b]==1) // ' 0
 {
        keystatus[0x0b]=0;
        for(i=0;i<numsectors;i++)
        {
                sector[i].ceilingz >>= 1;
                sector[i].floorz >>= 1;
        }
        for(i=0;i<numwalls;i++)
        {
                wall[i].x >>= 1;
                wall[i].y >>= 1;
                wall[i].yrepeat = min(wall[i].yrepeat<<1,255); 
        }
        for(i=0;i<MAXSPRITES;i++)
        {
                sprite[i].x >>= 1;
                sprite[i].y >>= 1;
                sprite[i].z >>= 1;
                sprite[i].xrepeat = max(sprite[i].xrepeat>>1,1); 
                sprite[i].yrepeat = max(sprite[i].yrepeat>>1,1); 
        }
 }
*/


 if(keystatus[0x28]==1 && keystatus[0x02]==1) // ' 1
 { on2d3d=!on2d3d; keystatus[0x02]=0;
 }

 if(keystatus[0x28]==1 && keystatus[0x04]==1) // ' 3
 { onnames++; if(onnames>6) onnames=0;
   keystatus[0x04]=0;
   sprintf(tempbuf,"Mode %d %s",onnames,Mode32d[onnames]);
   printmessage16(tempbuf);
//   clearmidstatbar16();
//   for(i=0;i<MAXMODE32D;i++) {printext16(0*8,32+(i*8),15,-1,Mode32d[i],0);}
//   Ver();
 }

/*
 if(keystatus[0x28]==1 && keystatus[0x05]==1) // ' 4
 {
        keystatus[0x04]=0;
    MinRate=getnumber16("Enter Min Frame Rate : ", MinRate, 65536L);
        if(MinRate==40)
        {MinRate=24; MinD=3;}
        else
        {MinRate=40; MinD=5;}
 }
*/

/*
 if(keystatus[0x28]==1 && keystatus[0x06]==1) // ' 5
 {
    keystatus[0x06]=0;
   sprintf(tempbuf,"Power-Up Ammo now equals Normal");
   printmessage16(tempbuf);
    for(i=0;i<MAXSPRITES;i++)
        {
     if(sprite[i].picnum>=20 && sprite[i].picnum<=59)
     {
        sprite[i].xrepeat = 32;
        sprite[i].yrepeat = 32;
     }
    }

 }
*/


 /* Motorcycle ha ha ha
 if(keystatus[0x28]==1 && keystatus[0x06]==1) // ' 5
 {
     keystatus[0x06]=0;
         sidemode++; if (sidemode > 2) sidemode = 0;
         if (sidemode == 1)
         {
                 editstatus = 0;
                 zmode = 2;
                 posz = ((sector[cursectnum].ceilingz+sector[cursectnum].floorz)>>1);
         }
         else
         {
                 editstatus = 1;
                 zmode = 1;
         }
 }
 */

 if(keystatus[0x28]==1 && keystatus[0x0a]==1) // ' 9 : swap hilo
 { keystatus[0x0b]=0;
        temp=sprite[cursprite].lotag;
        sprite[cursprite].lotag=sprite[cursprite].hitag;
        sprite[cursprite].hitag=temp;
 }

 if(keystatus[0x28]==1 && keystatus[0x32]==1) // ' m : Memory Usage
 { keystatus[0x32]=0;
        TotalMem();
 }


}// end key2d


void ExtInit(void)
{
    long fil;
    
#if (APPVER_DN3DREV < AV_DR_DN3D14)
    printf("------------------------------------------------------------------------------\n");
    printf("BUILD.EXE Copyright (c) 1993 - 1996 Ken Silverman, 3D Realms Entertainment.\n");
#else
    int key;

    printf("\n------------------------------------------------------------------------------\n");
    printf("BUILD.EXE for Duke Nukem 3D - Atomic Edition v1.4\n\n");
    printf("Copyright (c) 1993 - 1996 Ken Silverman, 3D Realms Entertainment.\n");
#endif
    printf("This version of BUILD was created for Duke Nukem 3D and parts were modified\n");
    printf("by Allen H. Blum III.\n");
    printf("\n");
    printf("IMPORTANT:  The Build Editor and associated tools and utilities are NOT\n");
    printf("shareware and may NOT be freely distributed to any BBS, CD, floppy, or\n");
    printf("any other media.  These tools may NOT be sold or repackaged for sale in\n");
    printf("a commercial product.\n");
    printf("\n");
    printf("Any levels created with these editors and tools may only be used with the\n");
    printf("full (registered) copy of Duke Nukem 3D, and not the shareware version.\n");
    printf("Please refer to LICENSE.DOC for further information on levels created with\n");
    printf("BUILD.EXE.\n");
    printf("\n");
#if (APPVER_DN3DREV >= AV_DR_DN3D14) // VERSIONS RESTORATION - Keep v1.3d's spacing for now
    printf("Press <Y> if you have read and accepted the terms of LICENSE.DOC,\n");
    printf("or any other key to abort the program. \n");
    printf("\n");
    key = getch();
if ((key == 'y') || (key == 'Y'))
{
#else
    printf("Please help us protect against software piracy (which drives up software\n");
    printf("prices) by following these simple rules.\n");
    printf("\n");
    printf("Thank You!\n");
    printf("------------------------------------------------------------------------------\n");
    getch();
#endif

    initgroupfile("duke3d.grp");

    if ((fil = kopen4load("setup.dat",0)) != -1)
                {
          kread(fil,option,NUMOPTIONS);
          kread(fil,keys,NUMKEYS);
                  memcpy((void *)buildkeys,(void *)keys,NUMKEYS);   //Trick to make build use setup.dat keys
          kclose(fil);
                }
                // if (option[3] != 0) moustat =
                initmouse();

        switch(option[0])
        {
                case 0: initengine(0,chainxres[option[6]&15],chainyres[(option[6]>>4)]); break;
                case 1: initengine(1,vesares[option[6]&15][0],vesares[option[6]&15][1]); break;
                default: initengine(option[0],320L,200L); break;
        }

        kensplayerheight = 40; //32
         zmode = 1;
         zlock = kensplayerheight<<8;
        defaultspritecstat = 0;

        ReadPaletteTable();
//  InitWater();
    // VERSIONS RESTORATION - Again keep v1.3d's spacing
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
}
else
{
    printf("------------------------------------------------------------------------------\n");
    exit(0);
}
#endif
}

void ExtUnInit(void)
{
// setvmode(0x03);
   uninitgroupfile();
}

static char lockbyte4094;
void ExtPreCheckKeys(void) // just before drawrooms
{
        if (qsetmode == 200)    //In 3D mode
        {
        if(purpleon) clearview(255);
                if (sidemode != 0)
                {
                        lockbyte4094 = 1;
                        if (waloff[4094] == 0)
                                allocache(&waloff[4094],320L*200L,&lockbyte4094);
                        setviewtotile(4094,320L,200L);
                        searchx ^= searchy; searchy ^= searchx; searchx ^= searchy;
                        searchx = ydim-1-searchx;
                }
        }
}

void ExtAnalyzeSprites(void)
{
        long i, j, k;
        spritetype *tspr;
    char frames=0;

        for(i=0,tspr=&tsprite[0];i<spritesortcnt;i++,tspr++)
        {
        frames=0;

        if((nosprites==1||nosprites==3)&&tspr->picnum<11) tspr->xrepeat=0;

        if(nosprites==1||nosprites==3)
         switch(tspr->picnum)
         {
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
             case OOZFILTER :
#endif
             case SEENINE :
                 tspr->xrepeat=0;
         }

        switch(tspr->picnum)
                {
// 5-frame walk
        case 1550 :             // Shark
        frames=5;


// 2-frame walk
            case 1445 :             // duke kick
            case LIZTROOPSHOOT :
            case LIZTROOPDUCKING :
            case 2030 :            // pig shot
            case PIGCOPDIVE :
            case 2190 :            // liz capt shot
            case BOSS1SHOOT :
            case BOSS1LOB :
        if(frames==0) frames=2;

// 4-frame walk
                 case 1491 :             // duke crawl
            case LIZTROOP :
        case LIZTROOPRUNNING :
            case PIGCOP :
            case LIZMAN :
            case BOSS1 :
        case BOSS2 :
            case BOSS3 :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case BOSS1STAYPUT :
        case NEWBEAST :
        case BOSS4 :
#endif
                if(frames==0) frames=4;

        case LIZTROOPJETPACK :
        case OCTABRAIN :
         case DRONE :
        case COMMANDER :
        case RECON :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case OCTABRAINSTAYPUT :
        case COMMANDERSTAYPUT :
#endif
        if(frames==0) frames=10;

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case TANK :
        if(frames==0) frames=1;
#endif
        case GREENSLIME :
        case EGG :
        case PIGCOPSTAYPUT :
        case LIZMANSTAYPUT:
            case LIZTROOPSTAYPUT :
            case LIZMANSPITTING :
            case LIZMANFEEDING :
            case LIZMANJUMP :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case NEWBEASTSTAYPUT :
        case NEWBEASTJUMP :
        case NEWBEASTHANG :
        case NEWBEASTHANGDEAD :
#endif
                if(skill!=4)
                {
                    if(tspr->lotag>skill)
                    { tspr->xrepeat=0; break; }
                }
            case APLAYER :

                if(nosprites==2||nosprites==3) 
                { tspr->xrepeat=0;
//                  tspr->cstat|=32768;
                }
//                else tspr->cstat&=32767;

        if(frames!=0)
        {
        if(frames==10) frames=0;
                k = getangle(tspr->x-posx,tspr->y-posy);
                                k = (((tspr->ang+3072+128-k)&2047)>>8)&7;
                                        //This guy has only 5 pictures for 8 angles (3 are x-flipped)
                                if (k <= 4)
                                {
                    tspr->picnum += k;
                    tspr->cstat &= 0xfb;   //clear x-flipping bit
                                }
                                else
                                {
                    tspr->picnum += 8-k;
                                        tspr->cstat |= 4;    //set x-flipping bit
                                }
        }

        if(frames==2) tspr->picnum+=((((4-totalclock>>6))&1)*5);
                if(frames==4) tspr->picnum+=((((4-totalclock>>6))&3)*5);
        if(frames==5) tspr->picnum+=(((totalclock>>6)%5))*5;

                if(tilesizx[tspr->picnum] == 0)
                    tspr->picnum -= 5;       //Hack, for actors

                                break;
              default:
                break;


                }
        }
}


    

int intro=0;

void ExtCheckKeys(void)
{
#if (APPVER_DN3DREV < AV_DR_DN3D14)
 long i,count,nexti;
#else
 long i,nexti;
#endif
 short statnum=0;
        if (qsetmode == 200)    //In 3D mode
        {
#if (APPVER_DN3DREV < AV_DR_DN3D14)
                if (sidemode != 0)
                {
                        setviewback();
                        rotatesprite(320<<15,200<<15,65536,(horiz-100)<<2,4094,0,0,2+4);
                        lockbyte4094 = 0;
                        searchx = ydim-1-searchx;
                        searchx ^= searchy; searchy ^= searchx; searchx ^= searchy;

//      overwritesprite(160L,170L,1153,0,1+2,0);
            rotatesprite(160<<16,170<<16,65536,(100-horiz+1024)<<3,1153,0,0,2);

        }
#endif

#if (APPVER_DN3DREV < AV_DR_DN3D14)
    if(intro<100)
#else
    if(intro<600)
#endif
        {
        intro++;
//        rotatesprite((160-8)<<16,(100-8)<<16,(200-intro)<<9,0,SPINNINGNUKEICON+(((4-totalclock>>3))&7),0,0,0,0,0,xdim-1,ydim-1);
        Ver();
        }

                  Keys3d();
#if (APPVER_DN3DREV < AV_DR_DN3D14)
                if (sidemode != 1) editinput();
#else
                editinput();
#endif
                if(usedcount)
        {
#if (APPVER_DN3DREV < AV_DR_DN3D14) // VERSIONS RESTORATION - Kinda moved into ShowDebugInfo
          if(tabgraphic)
           rotatesprite((320-32)<<16,(64)<<16,64<<9,0,temppicnum,0,0,0,0,0,xdim-1,ydim-1);
#endif
          if(searchstat!=3)
          {
           count=0;
           for(i=0;i<numwalls;i++)
           { if(wall[i].picnum == temppicnum) count++;
             if(wall[i].overpicnum == temppicnum) count++;
             if(sector[i].ceilingpicnum == temppicnum) count++;
             if(sector[i].floorpicnum == temppicnum) count++;
           }
          }

      if(searchstat==3)
          {
           count=0;
       statnum=0;
           i = headspritestat[statnum];
           while (i != -1)
            {
             nexti = nextspritestat[i];
             if(sprite[i].picnum == temppicnum) count++;
             i = nexti;
            }
          }

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        usedcount = 0;
#else // VERSIONS RESTORATION - Kinda moved into ShowDebugInfo as of 1.4
            printext256(70*8+1,0*8+1,0,-1,names[temppicnum],1);
            printext256(70*8+1,0*8,15,-1,names[temppicnum],1);

            sprintf(tempbuf,"lo = %ld",templotag);
            printext256(70*8+1,1*8+1,0,-1,tempbuf,1);
            printext256(70*8,1*8,15,-1,tempbuf,1);

            sprintf(tempbuf,"hi = %ld",temphitag);
        printext256(70*8+1,2*8+1,0,-1,tempbuf,1);
        printext256(70*8,2*8,15,-1,tempbuf,1);

                        sprintf(tempbuf,"USED = %ld",count);
        printext256(70*8+1,3*8+1,0,-1,tempbuf,1);
        printext256(70*8,3*8,15,-1,tempbuf,1);

        count=ActorMem(temppicnum);
                        sprintf(tempbuf,"MEM  = %ld",count);
        printext256(70*8+1,4*8+1,0,-1,tempbuf,1);
        printext256(70*8,4*8,15,-1,tempbuf,1);
#endif // APPVER_DN3DREV
                }// end if usedcount
        }
        else
        {
                  Keys2d();
        }
}
#if (APPVER_DN3DREV >= AV_DR_DN3D14) // VERSIONS RESTORATION - Mostly from ExtCheckKeys

void ShowDebugInfo(void)
{
    if (debuginfotimeleft<1) return;
    debuginfotimeleft--;
    if(tabgraphic)
        rotatesprite((320-32)<<16,(64)<<16,64<<9,0,temppicnum,0,0,0,0,0,xdim-1,ydim-1);

    printext256(70*8+1,0*8+1,0,-1,names[temppicnum],1);
    printext256(70*8+1,0*8,31,-1,names[temppicnum],1);

    sprintf(tempbuf,"Lo = %ld",templotag);
    printext256(70*8+1,1*8+1,0,-1,tempbuf,1);
    printext256(70*8,1*8,31,-1,tempbuf,1);

    sprintf(tempbuf,"Hi = %ld",temphitag);
    printext256(70*8+1,2*8+1,0,-1,tempbuf,1);
    printext256(70*8,2*8,31,-1,tempbuf,1);

    sprintf(tempbuf,"USED = %ld",count);
    printext256(70*8+1,3*8+1,0,-1,tempbuf,1);
    printext256(70*8,3*8,31,-1,tempbuf,1);

    sprintf(tempbuf,"MEM  = %ld",ActorMem(temppicnum));
    printext256(70*8+1,4*8+1,0,-1,tempbuf,1);
    printext256(70*8,4*8,31,-1,tempbuf,1);
}
#endif // APPVER_DN3DREV >= AV_DR_DN3D14

faketimerhandler()
{
  long i, j, dax, day, dist;
        long hiz, hihit, loz, lohit, oposx, oposy;
        short hitwall, daang;

    counter++; if(counter>=5) counter=0;

        if (totalclock < ototalclock+TICSPERFRAME) return;
        if (qsetmode != 200) return;
        if (sidemode != 1) return;
        ototalclock = totalclock;

        oposx = posx; oposy = posy;
#if (APPVER_DN3DREV < AV_DR_DN3D14)
        hitwall = clipmove(&posx,&posy,&posz,&cursectnum,xvel,yvel,128L,4L<<8,4L<<8,0);
#else
        hitwall = clipmove(&posx,&posy,&posz,&cursectnum,runrate*xvel,runrate*yvel,128L,4L<<8,4L<<8,0);
#endif
        xvel = ((posx-oposx)<<14); yvel = ((posy-oposy)<<14);

        yvel += 80000;
        if ((hitwall&0xc000) == 32768)
        {
                hitwall &= (MAXWALLS-1);
                i = wall[hitwall].point2;
                daang = getangle(wall[i].x-wall[hitwall].x,wall[i].y-wall[hitwall].y);

                xvel -= (xvel>>4);
                if (xvel < 0) xvel++;
                if (xvel > 0) xvel--;

                yvel -= (yvel>>4);
                if (yvel < 0) yvel++;
                if (yvel > 0) yvel--;

                i = 4-keystatus[buildkeys[4]];
                xvel += mulscale(vel,(long)sintable[(ang+512)&2047],i);
                yvel += mulscale(vel,(long)sintable[ang&2047],i);

                if (((daang-ang)&2047) < 1024)
                        ang = ((ang+((((daang-ang)&2047)+24)>>4))&2047);
                else
                        ang = ((ang-((((ang-daang)&2047)+24)>>4))&2047);

                timeoff = ototalclock;
        }
        else
        {
                if (ototalclock > timeoff+32)
                        ang = ((ang+((timeoff+32-ototalclock)>>4))&2047);
        }

        getzrange(posx,posy,posz,cursectnum,&hiz,&hihit,&loz,&lohit,128L,0);

        oposx -= posx; oposy -= posy;

  dist = ksqrt(oposx*oposx+oposy*oposy);
  if (ototalclock > timeoff+32) dist = 0;

  daang = mulscale(dist,angvel,9);
        posz += (daang<<6);
        if (posz > loz-(4<<8)) posz = loz-(4<<8), hvel = 0;
        if (posz < hiz+(4<<8)) posz = hiz+(4<<8), hvel = 0;

        horiz = ((horiz*7+(100-(daang>>1)))>>3);
        if (horiz < 100) horiz++;
        if (horiz > 100) horiz--;

        if(keystatus[0x28]==1 && keystatus[0x06]==1) // ' 5
        {
                keystatus[0x06]=0;
                editstatus = 1;
                sidemode = 2;
        }
}

void Ver()
{
#if (APPVER_DN3DREV < AV_DR_DN3D13)
 sprintf(tempbuf,"DUKE NUKEM BUILD: V032696");
#elif (APPVER_DN3DREV < AV_DR_DN3D14)
 sprintf(tempbuf,"DUKE NUKEM BUILD: V041996");
#else
 sprintf(tempbuf,"DUKE NUKEM BUILD ATOMIC EDITION : V1.4 100296");
#endif
 if (qsetmode == 200)    //In 3D mode
#if (APPVER_DN3DREV < AV_DR_DN3D14)
 { printext256(60*8,24*8,11,-1,tempbuf,1);
   rotatesprite((320-8)<<16,(200-8)<<16,64<<9,0,SPINNINGNUKEICON+(((4-totalclock>>3))&7),0,0,0,0,0,xdim-1,ydim-1);
#else
 { printext256(48*8,24*8-1,0,-1,tempbuf,1);
   printext256(48*8,24*8,31,-1,tempbuf,1);
   rotatesprite((320-8)<<16,(200-8)<<16,64<<9,0,SPINNINGNUKEICON+(((4-totalclock>>3))&7),0,0,0,0,0,xdim-1,ydim-1);
#endif
 }else
 { printext16(0,0,15,-1,tempbuf,0);
 }
}

ActorMem(int i)
{int total=0,j;
    switch(i)
    {
           case APLAYER :
                for(j=APLAYER;j<(APLAYER+131);j++) total +=tilesizx[j]*tilesizy[j];
                for(j=1780;j<(1780+32);j++) total +=tilesizx[j]*tilesizy[j];
                break;
            case LIZTROOP :
            case LIZTROOPRUNNING :
            case LIZTROOPSTAYPUT :
            case LIZTROOPSHOOT :
            case LIZTROOPJETPACK :
            case LIZTROOPONTOILET :
            case LIZTROOPDUCKING :
                for(j=LIZTROOP;j<(LIZTROOP+100);j++) total +=tilesizx[j]*tilesizy[j];
                break;
        case OCTABRAIN :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case OCTABRAINSTAYPUT :
#endif
        for(j=OCTABRAIN;j<(OCTABRAIN+40);j++) total +=tilesizx[j]*tilesizy[j];
                break;
                 case DRONE :
                for(j=DRONE;j<(DRONE+10);j++) total +=tilesizx[j]*tilesizy[j];
                break;
        case COMMANDER :
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        case COMMANDERSTAYPUT :
#endif
        for(j=COMMANDER;j<(COMMANDER+40);j++) total +=tilesizx[j]*tilesizy[j];
                break;
            case RECON :
                for(j=RECON;j<(RECON+10);j++) total +=tilesizx[j]*tilesizy[j];
                break;
            case PIGCOP :
                for(j=PIGCOP;j<(PIGCOP+61);j++) total +=tilesizx[j]*tilesizy[j];
                break;
            case LIZMAN :
        case LIZMANSTAYPUT:
            case LIZMANSPITTING :
            case LIZMANFEEDING :
            case LIZMANJUMP :
                for(j=LIZMAN;j<(LIZMAN+80);j++) total +=tilesizx[j]*tilesizy[j];
                break;
            case BOSS1 :
                for(j=BOSS1;j<(BOSS1+60);j++) total +=tilesizx[j]*tilesizy[j];
                break;
        case BOSS2 :
        for(j=BOSS2;j<(BOSS2+50);j++) total +=tilesizx[j]*tilesizy[j];
        break;
        case BOSS3 :
        for(j=BOSS3;j<(BOSS3+50);j++) total +=tilesizx[j]*tilesizy[j];
        break;

            default: total += tilesizx[i]*tilesizy[i];
    }
    return(total);
}

int curpalette=0;

SetBOSS1Palette()
{int x;
 if(curpalette==3) return;
 curpalette=3;
 kensetpalette(BOSS1palette);
}


SetSLIMEPalette()
{int x;
 if(curpalette==2) return;
 curpalette=2;
 kensetpalette(SLIMEpalette);
}

SetWATERPalette()
{int x;
 if(curpalette==1) return;
 curpalette=1;
 kensetpalette(WATERpalette);
}


SetGAMEPalette()
{int x;
 if(curpalette==0) return;
 curpalette=0;
 kensetpalette(GAMEpalette);
}

kensetpalette(char *vgapal)
{
        long i;
        char vesapal[1024];

        for(i=0;i<256;i++)
        {
                vesapal[i*4+0] = vgapal[i*3+2];
                vesapal[i*4+1] = vgapal[i*3+1];
                vesapal[i*4+2] = vgapal[i*3+0];
                vesapal[i*4+3] = 0;
        }
        VBE_setPalette(0L,256L,vesapal);
#if (APPVER_DN3DREV >= AV_DR_DN3D14)
        setbrightness(gamma,vgapal);
#endif
}

SearchSectorsForward()
{
 long ii=0;
 if(cursector_lotag!=0)
 {
     if(cursectornum<MAXSECTORS) cursectornum++;
     for(ii=cursectornum;ii<=MAXSECTORS;ii++)
     {
        if(sector[ii].lotag==cursector_lotag)
        {
            posx=wall[sector[ii].wallptr].x;
            posy=wall[sector[ii].wallptr].y;
            printmessage16("> Sector Search : Found");
//            cursectornum++;
            keystatus[0x1b]=0; // ]
            return;
        }
        cursectornum++;
     }
 }
 printmessage16("> Sector Search : none");
}

SearchSectorsBackward()
{
 long ii=0;
 if(cursector_lotag!=0)
 {
     if(cursectornum>0) cursectornum--;
     for(ii=cursectornum;ii>=0;ii--)
     {
        if(sector[ii].lotag==cursector_lotag)
        {
            posx=wall[sector[ii].wallptr].x;
            posy=wall[sector[ii].wallptr].y;
            printmessage16("< Sector Search : Found");
//            cursectornum--;
            keystatus[0x1a]=0; // [
            return;
        }
        cursectornum--;
     }
 }
 printmessage16("< Sector Search : none");
}

#if (APPVER_DN3DREV >= AV_DR_DN3D14)
// VERSIONS RESTORATION - New functions, including modified copies of BUILD.C:getnumber256 and ENGINE.C:insertsprite
static getnumber256(char namestart[80], short num, long maxnumber)
{
	char buffer[80];
	char neg = 0; // VERSIONS RESTORATION - Additional var and related code
	long j, k, n, danum, oldnum;

	danum = (long)num;
	oldnum = danum;
	if (danum < 0)
	{
		neg = 1;
		danum *= -1;
	}
	while ((keystatus[0x1c] != 2) && (keystatus[0x1] == 0))
	{
		drawrooms(posx,posy,posz,ang,horiz,cursectnum);
		//ExtAnalyzeSprites(); // VERSIONS RESTORATION - Removed
		drawmasks();

		if (!neg) sprintf(&buffer,"%s%ld_ ",namestart,danum);
		else sprintf(&buffer,"%s-%ld_ ",namestart,danum);
		printmessage256(buffer);
		nextpage();

		for(j=2;j<=11;j++)
			if (keystatus[j] > 0)
			{
				keystatus[j] = 0;
				k = j-1;
				if (k == 10) k = 0;
				n = (danum*10)+k;
				if (n < maxnumber) danum = n;
			}
		if (keystatus[0xe] > 0)    // backspace
		{
			danum /= 10;
			if (danum == 0) neg = 0;
			keystatus[0xe] = 0;
		}
		if (keystatus[0xc] > 0)    // -
		{
			neg = !neg;
			keystatus[0xc] = 0;
		}
		if (keystatus[0x1c] == 1)
		{
			oldnum = danum;
			if (neg) oldnum *= -1;
			keystatus[0x1c] = 2;
			asksave = 1;
		}
	}
	keystatus[0x1c] = 0;
	keystatus[0x1] = 0;

	// VERSIONS RESTORATION - Not in this copy of getnumber256
	//lockclock = totalclock;  //Reset timing

	return((short)oldnum);
}

insertsprite(short sectnum, short statnum)
{
	insertspritestat(statnum);
	autosizespritenum=insertspritesect(sectnum);
	return(autosizespritenum);
}

void autosizesprite (short spritenum)
{
 if(autosize==0)
     return;
 if(sprite[spritenum].picnum<11)
     return;
 if(sprite[spritenum].picnum>20 && sprite[spritenum].picnum<62) {sprite[spritenum].xrepeat = 32; sprite[spritenum].yrepeat = 32;}

 switch(sprite[spritenum].picnum)
 {
     case FEM5:
     case FEM6:
     case FEM8:
     case HOLODUKE:

         sprite[spritenum].xrepeat = 26; sprite[spritenum].yrepeat = 26;
         break;

     case FEM9:

         sprite[spritenum].xrepeat = 30; sprite[spritenum].yrepeat = 30;
         break;

     case ATOMICHEALTH:

     case NAKED1:

     case 606: // Broken space suit

     case LIGHTSWITCH2:
     case LIGHTSWITCH2+1:

     case PODFEM1:

     case FEM1:
     case FEM2:
     case FEM3:
     case FEM4:
     case FEM7:

     case PIRATE1A:
     case PIRATE4A:
     case PIRATE2A:
     case PIRATE5A:
     case PIRATE3A:
     case PIRATE6A:

     case FEM10:
     case TOUGHGAL:
     case MAN:

         sprite[spritenum].xrepeat = 32; sprite[spritenum].yrepeat = 32;
         break;

     case CHAIR1:
     case CHAIR2:

     case TOILET:

     case STALL:
     case STALLBROKE:

     case TOILETBROKE:

     case EGG:

     case CHAIR3:

     case FIREEXT:
     case HYDRENT:
     case RUBBERCAN:

     case APLAYER:

     case LIZTROOP:
     case LIZTROOPRUNNING:
     case LIZTROOPSTAYPUT:
     case LIZTROOPSHOOT:
     case LIZTROOPJETPACK:
     case LIZTROOPONTOILET:
     case LIZTROOPDUCKING:

     case OCTABRAIN:
     case OCTABRAINSTAYPUT:

     case DRONE:

     case COMMANDER:
     case COMMANDERSTAYPUT:

     case RECON:

     case PIGCOP:
     case PIGCOPSTAYPUT:
     case PIGCOPDIVE:

     case LIZMAN:
     case LIZMANSTAYPUT:
     case LIZMANSPITTING:
     case LIZMANFEEDING:
     case LIZMANJUMP:

     case ROTATEGUN:

     case GREENSLIME:

     case ORGANTIC:

     case DUKECUTOUT:

     case ROBOTPIRATE:
     case ROBOTMOUSE:

     case PIRATEHALF:

     case CANWITHSOMETHING2:
     case CANWITHSOMETHING3:
     case CANWITHSOMETHING4:

     case NEWBEAST:
     case NEWBEASTSTAYPUT:
     case NEWBEASTJUMP:
     case NEWBEASTHANG:
     case NEWBEASTHANGDEAD:

     case JURYGUY:

         sprite[spritenum].xrepeat = 40; sprite[spritenum].yrepeat = 40;
         break;

     case STATUE:
     case STATUEFLASH:
         sprite[spritenum].xrepeat = 48; sprite[spritenum].yrepeat = 40;
         break;

     case BOSS1:

     case BOSS2:

     case BOSS3:

         sprite[spritenum].xrepeat = 64; sprite[spritenum].yrepeat = 64;
         break;

     case HEAVYHBOMB:
     case AMMO:
         sprite[spritenum].xrepeat = 16; sprite[spritenum].yrepeat = 16;
         break;

     case TRIPBOMB:
         sprite[spritenum].xrepeat = 8; sprite[spritenum].yrepeat = 8;
         break;

     case TANK:
         sprite[spritenum].xrepeat = 60; sprite[spritenum].yrepeat = 60;
         break;
 }
}

int GetNextTag(int nextfreetag)
{
 int i;
 for(i=0;i<MAXSPRITES;i++)
 {
  if(sprite[i].statnum==0)
  switch(sprite[i].picnum)
  {
    //LOTAG
    case ACTIVATOR:
    case ACTIVATORLOCKED:
    case TOUCHPLATE:
    case MASTERSWITCH:
    case RESPAWN:
    case ACCESSSWITCH:
    case SLOTDOOR:
    case LIGHTSWITCH:
    case SPACEDOORSWITCH:
    case SPACELIGHTSWITCH:
    case FRANKENSTINESWITCH:
    case MULTISWITCH:
    case DIPSWITCH:
    case DIPSWITCH2:
    case TECHSWITCH:
    case DIPSWITCH3:
    case ACCESSSWITCH2:
    case POWERSWITCH1:
    case LOCKSWITCH1:
    case POWERSWITCH2:
    case PULLSWITCH:
    case ALIENSWITCH:
    case CAMERA1:
        if(sprite[i].lotag>nextfreetag) nextfreetag=1+sprite[i].lotag;
        break;

    //HITAG
    case SEENINE:
    case OOZFILTER:
        case SECTOREFFECTOR:
    case FEM1:
    case FEM2:
    case FEM3:
        if(sprite[i].lotag==10 ||
            sprite[i].lotag==27 ||
            sprite[i].lotag==28 ||
            sprite[i].lotag==29
            ) break;
        else
            if(sprite[i].hitag>nextfreetag) nextfreetag=1+sprite[i].hitag;
        break;
        default:
        break;

  }

 } // end sprite loop
 return nextfreetag;
}
#endif // APPVER_DN3DREV >= AV_DR_DN3D14
