#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct markaz
{
    double x;
    double y;
    int soldiers;
    uint32_t color;
    int rate;
    int speed;
    int max;
    int r;
    bool majoon;
    int type;
    bool on;
};

struct sarbaz
{
    double x;
    double y;
    struct sarbaz *next;
};

struct fight
{
    struct markaz start, end;
    int s, e;
    double shib;
    int nsarbaz;
    int counter;
    struct sarbaz *head;
};

struct potion
{
    double x;
    double y;
    uint32_t color;
    int type;
    int whichone;
};

struct active
{
    int type;
    int color;
    int time;
};

const int FPS = 60;
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const int s=50;
int check[50]={0};
uint32_t colors[100];
uint32_t color[9]={0xffffc75f ,0xffaaaa00  , 0xff5fc7ff , 0xffff00ff,0xffccb5fc , 0xffff6f91, 0xffff9671, 0xff916fff, 0xff9b9d9e};
int numberofareas, numberofplayers, number=0;
struct markaz *center;
struct fight *bazi;
struct sarbaz *soldier;
struct potion majoon;
struct active potion1, potion2;
bool nist=true;
int mokhtasat[SCREEN_WIDTH][SCREEN_HEIGHT]={0};
bool running=true;
bool edame=false;

#ifdef main
#undef main
#endif
void init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER |TTF_Init()) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
}

void generate_random(SDL_Renderer *renderer, double x, double y);

void txtRGBA(SDL_Renderer *renderer,int x,int y,const char * text,int font_size,int R,int G,int B, int A)
{
    TTF_Init();
    SDL_Color text_color = { 0, 0, 0, 255};
    const char * font_address_01="font.ttf";
    const char * Font =NULL;
    Font =font_address_01;
    int mWidth=0;
    int mHeight=0;
    SDL_Rect *clip = NULL;
    TTF_Font *gFont = NULL;
    double angle = 0.0;
    SDL_Point *c = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture* mTexture = NULL;
    gFont = TTF_OpenFont(Font, font_size );
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont,text, text_color );
    mWidth = textSurface->w;
    mHeight = textSurface->h;
    mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, c, flip );

    SDL_DestroyTexture( mTexture );
    TTF_CloseFont(gFont);
}

void makecolors ()
{
    for (int i=0; i<numberofareas; i++)
    {
        if (i<numberofplayers)
        {
            colors[i]=color[i];
        }
        else
            colors[i]=color[8];
//        printf("%x\n", colors[i]);
    }
}

void find_center( double x, double y, uint32_t target)
{
    center[number].x=x+0.5*s* sqrt(3);
    center[number].y=y+0.5*s;
    center[number].color=target;
    mokhtasat[(int)x][(int)y]++;
}

void m0(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
            center[i].speed=8;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].speed=8;
    if (center[i].color!=potion1.color && center[i].color!=potion2.color)
        center[i].speed=4;
//    printf ("speed:%d %d\n", center[i].speed, i);
}

void m00(int i, int k)
{
//    if (k==1)
//        if (potion1.color==center[i].color)
//             center[i].speed=4;
//    if (k==2)
//        if (potion2.color==center[i].color)
            center[i].speed=4;
//    printf ("speed*:%d %d\n", center[i].speed, i);
}

void m1(int i, int k)
{
    if (k==1)
        if (potion1.color!=center[i].color)
            center[i].speed=2;
    if (k==2)
        if (potion2.color!=center[i].color)
            center[i].speed=2;
    if (center[i].color==potion1.color || center[i].color==potion2.color)
        center[i].speed=4;
//    printf ("speed2:%d %d\n", center[i].speed, i);
}

void m11(int i, int k)
{
//     if (k==1)
//        if (potion1.color!=center[i].color)
//            center[i].speed=4;
//    if (k==2)
//        if (potion2.color!=center[i].color)
            center[i].speed=4;
//    printf ("speed2*:%d %d\n", center[i].speed, i);
}

void m2(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
            center[i].max=10000;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].max=10000;
//    printf ("max:%d %d\n", center[i].max, i);
}

void m22(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
            center[i].max=80;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].max=80;
//    printf ("max*:%d %d\n", center[i].max, i);
}

void m3(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
        center[i].rate=5;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].rate=5;
//    printf ("rate:%d %d\n", center[i].rate, i);
}

void m33(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
        center[i].rate=15;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].rate=15;
//    printf ("rate*:%d %d\n", center[i].rate, i);
}

void qheyrfaal()
{
    if (potion1.time>=300 && potion1.color!=0)
    {
        for (int i=0; i<numberofareas; i++)
        {
            if (potion1.type==0)
                m00(i, 1);
            if (potion1.type==1)
                m11(i, 1);
            if (potion1.type==2)
                m22(i, 1);
            if (potion1.type==3)
                m33(i, 1);
        }
//        printf("potion*: %d %d\n", potion1.time, potion2.time);
        potion1.color=0;
        potion1.type=20;
        potion1.time=0;
    }
    if (potion2.time>=300 && potion2.color!=0)
    {
        for (int i=0; i<numberofareas; i++)
        {
            if (potion2.type==0)
                m00(i, 2);
            if (potion2.type==1)
                m11(i, 2);
            if (potion2.type==2)
                m22(i, 2);
            if (potion2.type==3)
                m33(i, 2);
        }
        potion2.color = 0;
        potion2.type=20;
        potion2.time=0;
    }
}

void active_potion()
{
    qheyrfaal();
    for (int i=0; i<numberofareas; i++)
    {
        if (potion1.color==center[i].color)
        {
            center[i].majoon = true;
            center[i].type = potion1.type;
        }
        else if (potion2.color==center[i].color)
        {
            center[i].majoon = true;
            center[i].type = potion2.type;
        }
        else
            center[i].majoon = false;
        if (potion1.type==0 && potion1.color!=0)
        {
            m0(i, 1);
//            printf("heyyyyy\n");
        }
        if (potion1.type==1 && potion1.color!=0)
        {
            m1(i, 1);
        }
        if (potion1.type==2 && potion1.color!=0)
        {
            m2(i, 1);
        }
        if (potion1.type==3 && potion1.color!=0)
        {
            m3(i, 1);
        }
        if (potion2.type==0 && potion2.color!=0)
        {
            m0(i, 2);
        }
        if (potion2.type==1 && potion2.color!=0)
        {
            m1(i, 2);
        }
        if (potion2.type==2 && potion2.color!=0)
        {
            m2(i, 2);
        }
        if (potion2.type==3 && potion2.color!=0)
        {
            m3(i, 2);
        }
    }
    potion1.time++;
    potion2.time++;
//    printf("potion:%d %d\n", potion1.time, potion2.time);
}

void bekesh (SDL_Renderer *renderer, struct markaz center[])
{
    for (int i=0; i<numberofareas; i++)
    {
        double x=center[i].x-0.5*s*sqrt(3), y=center[i].y-0.5*s;
        char numberofs[1000];
        sprintf(numberofs, "%d", center[i].soldiers);
        SDL_Rect test = {center[i].x-10 , center[i].y-10 , 20 , 20};
        SDL_Color col = {0,0,0,255};
        TTF_Font * font = TTF_OpenFont("font.ttf" , 26);
        if (!font)
            printf("Failed to load image at%s", SDL_GetError());
        SDL_Surface *adads= TTF_RenderText_Solid(font , numberofs , col);
        SDL_Texture *tex= SDL_CreateTextureFromSurface(renderer , adads);
        uint32_t target=center[i].color;
        if (target!=color[0] && target!=color[8])
            edame=true;
        boxColor(renderer, x, y, x+s* sqrt(3), y+s, target);
        filledTrigonColor(renderer, x, y, x+s/2* sqrt(3), y-s/2, x+s* sqrt(3), y, target);
        filledTrigonColor(renderer, x, y+s, x+s/2* sqrt(3), y+s+s/2, x+s* sqrt(3), y+s, target);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(renderer, x, y, x+s*0.5* sqrt(3), y-s/2);
        SDL_RenderDrawLine(renderer, x+s* sqrt(3), y, x+s*0.5* sqrt(3), y-s/2);
        SDL_RenderDrawLine(renderer, x, y, x, y+s);
        SDL_RenderDrawLine(renderer, x+s* sqrt(3), y, x+s* sqrt(3), y+s);
        SDL_RenderDrawLine(renderer, x, y+s, x+s/2* sqrt(3), y+1.5*s);
        SDL_RenderDrawLine(renderer, x+s* sqrt(3), y+s, x+s*0.5* sqrt(3), y+s*1.5);
        aaellipseRGBA(renderer, center[i].x, center[i].y, center[i].r, center[i].r, 255, 255, 255, 255);
        aaellipseRGBA(renderer, center[i].x, center[i].y, center[i].r-3, center[i].r-3, 255, 255, 255, 255);
        SDL_RenderCopy(renderer , tex, NULL, &test);
        SDL_FreeSurface(adads);
        SDL_DestroyTexture(tex);
        TTF_CloseFont(font);
//        majoonfaal(i);
        if (center[i].majoon)
        {
            SDL_Rect size = {center[i].x, center[i].y + 15, 20, 20};
            SDL_Surface *m;
            if (center[i].type==0)
                m = SDL_LoadBMP("m0.bmp");
            if (center[i].type==1)
                m = SDL_LoadBMP("m1.bmp");
            if (center[i].type==2)
                m = SDL_LoadBMP("m2.bmp");
            if (center[i].type==3)
                m = SDL_LoadBMP("m3.bmp");
            if (!m)
                printf("Failed to load image at%s", SDL_GetError());
            SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, m);
            SDL_RenderCopy(renderer, img, NULL, &size);
            SDL_FreeSurface(m);
            SDL_DestroyTexture(img);
        }
    }
//    qheyrfaal();
//    potion1.time++;
//    potion2.time++;
}

void draw (SDL_Renderer *renderer, double x, double y)
{
    if (number>=numberofareas)
        return;
    srand(time(0));
    int r=rand()%numberofareas;
    while(check[r]!=0)
        r = rand() % numberofareas;
    printf("%d %d\n", check[r], r);
    check[r]=1;
    uint32_t target;
    target=colors[r];
    find_center(x,y, target);
    number++;
    printf("%lf %lf %d\n", x, y, number);
    if(number<numberofareas)
        generate_random(renderer, x, y);
    else
        return;
}

void generate_random(SDL_Renderer *renderer, double x, double y)
{
    printf("%lf %lf %d\n", x, y, number);
    for (int i=1; i<=6; i++)
    {
        bool halat=rand()%2;
//        printf("i%d halat%d\n", i, halat);
        if (halat)
        {
            if (i==1 && number<numberofareas && x <SCREEN_WIDTH- 1.5*s*sqrt(3) && y>2*s*sqrt(3) && mokhtasat[(int)(x+0.5 *s* sqrt(3))][(int)(y- 1.5*s)]==0 )
                draw(renderer, x+0.5 *s* sqrt(3), y- 1.5*s);
            else if (i==2 && number<numberofareas && x>0.5*s*sqrt(3) && y>2*s*sqrt(3) && mokhtasat[(int)(x-0.5 *s* sqrt(3))][(int)(y- 1.5*s)]==0)
                draw(renderer, x-0.5 *s* sqrt(3), y- 1.5*s);
            else if (i==3 && number<numberofareas && x<SCREEN_WIDTH- 2*s*sqrt(3) && mokhtasat[(int)(x+s* sqrt(3))][(int)y]==0 )
                draw(renderer, x+s* sqrt(3), y);
            else if (i==4 && number<numberofareas && x>s* sqrt(3) && mokhtasat[(int)(x-s* sqrt(3))][(int)y]==0 )
                draw(renderer, x - s * sqrt(3), y);
            else if (i==5 && number<numberofareas && x <SCREEN_WIDTH- 1.5*s*sqrt(3) && y<SCREEN_HEIGHT-3.5*s && mokhtasat[(int)(x+0.5 *s* sqrt(3))][(int)(y+ 1.5*s)]==0 )
                draw(renderer, x+0.5 *s* sqrt(3), y+ 1.5*s);
            else if (i==6 && number<numberofareas  && x>0.5*s*sqrt(3) && y<SCREEN_HEIGHT-3*s && mokhtasat[(int)(x-0.5 *s* sqrt(3))][(int)(y+ 1.5*s)]==0)
                draw(renderer, x-0.5 *s* sqrt(3), y+ 1.5*s);
        }
        if (number>=numberofareas)
            return;
    }
}

void saveclick(double x, double y , int *start, int *end)
{
    for (int i=0; i<numberofareas; i++)
    {
        if (pow(x-center[i].x, 2)+ pow(y-center[i].y, 2)<=400)
        {
//            int k=1;
//            for (int j=0; j<100 && *start==-1; j++)
//            {
//                if ( bazi[j].nsarbaz!=0  && (bazi[j].start.x== center[i].x && bazi[j].start.y== center[i].y))
//                    k=0;
//            }
            if (*start==-1 && center[i].color ==color[0] && !center[i].on)
            {
//                printf("hi%d\n", bazi[j].nsarbaz);
                *start = i;
//                return;
            }
            else if (*start!=-1 && i!=*start)
                *end = i;
            else
            {
                *start = -1;
                *end= -1;
            }
//            printf("%d %d\n", *start, *end);
            return;
        }
    }
    *start = -1;
    *end = -1;
}

void faal(int i)
{
    struct sarbaz *head=bazi[i].head;
    while (head!=NULL)
    {
//        printf("hello : %lf %lf %lf %lf\n",head->x, majoon.x, head->y, majoon.y);
        if (pow(head->x-majoon.x, 2) + pow(head->y-majoon.y, 2)<=64 && center[bazi[i].s].majoon==false)
        {
            nist=false;
//            majoonfaal(i);
            if (majoon.whichone==1)
            {
                potion1.type=majoon.type;
                potion1.color=center[bazi[i].s].color;
                potion1.time=0;
                majoon.whichone=2;
            }
            else if (majoon.whichone==2)
            {
                potion2.type=majoon.type;
                potion2.color=center[bazi[i].s].color;
                potion2.time=0;
                majoon.whichone=1;
            }
            majoon.x=0;
            majoon.y=0;
            return;
        }
        head=head->next;
    }
}

void generate_soldier(int c)
{
    for (int i=0; i<numberofareas; i++)
    {
//        if (center[i].color==color[8])
//            center[i].max=20;
//        else
//            center[i].max=80;
        if (center[i].soldiers<center[i].max && center[i].rate!=0 && c%center[i].rate==0)
            center[i].soldiers++;
    }
}

void addattail(struct sarbaz *head, double x0, double y0, int i)
{
    if (bazi[i].counter==bazi[i].nsarbaz)
        return;
    while(head->next != NULL)
    {
        head = head->next;
//        printf ("1 you\n");
    }
    head->next = (struct sarbaz*)(malloc(sizeof(struct sarbaz)));
    head->next->x = bazi[i].start.x;
    head->next->y = bazi[i].start.y;
//    printf("%lf %lf\n", head-> x, head->y );
    head->next->next = NULL;
    bazi[i].counter++;
    if (center[bazi[i].s].soldiers>0 && center[bazi[i].s].color==bazi[i].start.color)
        center[bazi[i].s].soldiers--;
}

struct sarbaz* delete_from_head(struct sarbaz* head, int i)
{
    if(head != NULL )
    {
        struct sarbaz* new_head = head->next;
        free(head);
        if (center[bazi[i].e].color==bazi[i].start.color)
        {
            center[bazi[i].e].soldiers++;
        }
        else
        {
            if (center[bazi[i].e].soldiers>0)
                center[bazi[i].e].soldiers--;
            center[bazi[i].e].rate=0;
            if (center[bazi[i].e].soldiers<=0)
            {
                center[bazi[i].e].color = bazi[i].start.color;
                if(bazi[i].end.color==color[8])
                    center[bazi[i].e].max=80;
            }
        }
        return new_head;
    }
}

void ziadkon(struct sarbaz *head, int i)
{
    while(head != NULL )
    {
//        printf("%lf %lf %lf %lf\n", bazi[i].start.x, bazi[i].end.x, bazi[i].start.y, bazi[i].end.y );
        if (bazi[i].start.x-bazi[i].end.x<-1)
        {
            head->x += center[bazi[i].s].speed * 1/sqrt(pow(bazi[i].shib,2)+1);
            head->y += center[bazi[i].s].speed * 1/sqrt(pow(bazi[i].shib,2)+1) * bazi[i].shib;
        }
        else if (bazi[i].start.x-bazi[i].end.x>1)
        {
            head->x -= center[bazi[i].s].speed * 1/sqrt(pow(bazi[i].shib,2)+1);
            head->y -= center[bazi[i].s].speed * 1/sqrt(pow(bazi[i].shib,2)+1) * bazi[i].shib;
        }
        else
        {
            if (bazi[i].start.y<bazi[i].end.y)
                head->y += center[bazi[i].s].speed;
            if (bazi[i].start.y>bazi[i].end.y)
                head->y -= center[bazi[i].s].speed;
        }
//        printf ("mmmm%lf %lf\n", head->x, head->y);
        head = head->next;
    }
}

int checkkon(struct sarbaz *head, int i)
{
//    printf("c%lf %lf %lf %lf\n",bazi[i].start.x, bazi[i].start.y, head->x, head->y);
    while (head->next !=NULL)
        head=head->next;
    if (pow(head->x-bazi[i].start.x,2)+pow(head->y-bazi[i].start.y,2)<=200)
        return 0;
//    if (bazi[i].current.x-bazi[i].start.x <10 || bazi[i].current.y-bazi[i].start.y <10)
//        return 0;
//    printf("abc\n");
    return 1;
}

int checknakon(struct sarbaz *head, int i)
{
//    printf("opopop%lf %lf\n", head->x, head->y);
    if (pow(head->x-bazi[i].end.x,2)+pow(head->y-bazi[i].end.y,2)<=64)
    {
//        printf("hello\n");
        return 0;
    }
    return 1;
}

void hamle(SDL_Renderer *renderer, int c, int i)
{
//    printf("a%d b%d\n", bazi[i].nsarbaz, bazi[i].counter);
//    if (bazi[i].head==NULL && bazi[i].counter!=0)
    if (bazi[i].counter==bazi[i].nsarbaz && bazi[i].head==NULL)
    {
        bazi[i].nsarbaz=0;
        center[bazi[i].s].rate=15;
        center[bazi[i].e].rate=15;
        center[bazi[i].s].on=false;
        return;
    }
    if (bazi[i].counter==0 || bazi[i].head==NULL)
    {
        bazi[i].head = (struct sarbaz*)(malloc(sizeof(struct sarbaz)));
        bazi[i].head->x = bazi[i].start.x;
        bazi[i].head->y = bazi[i].start.y;
//        printf("%lf %lf\n", bazi[i].head-> x, bazi[i].head->y );
        bazi[i].head->next = NULL;
        bazi[i].counter++;
//        bazi[i].current=*bazi[i].head;
        center[bazi[i].s].soldiers--;
    }
    if (checkkon(bazi[i].head, i))
    {
        addattail(bazi[i].head, bazi[i].start.x, bazi[i].start.y, i);
    }
    if (!checknakon(bazi[i].head, i))
    {
        bazi[i].head= delete_from_head(bazi[i].head, i);
    }
    ziadkon(bazi[i].head, i);
    faal(i);
    struct sarbaz *h=bazi[i].head;
    while (h != NULL)
    {
        aaellipseRGBA(renderer, h->x, h->y, 5, 5, 0,0,0,255);
        filledEllipseColor(renderer, h->x, h->y, 4, 4, bazi[i].start.color);
        h=h->next;
    }
}

void is_same(int i, int j, struct sarbaz *headi, struct sarbaz *headj)
{
    while (headj->next!=NULL && bazi[i].head !=NULL)
    {
        if (pow(bazi[i].head->x-headj->next->x, 2)+pow(bazi[i].head->y-headj->next->y, 2)<100 && bazi[i].start.color != bazi[j].start.color)
        {
            free(headj->next);
            headj->next=headj->next->next;
            free(bazi[i].head);
            bazi[i].head=bazi[i].head->next;
            return;
        }
        headj=headj->next;
    }
}

void randomareas (SDL_Renderer *renderer)
{
    nist=true;
    srand(time(NULL));
    int one, two;
    one = rand()%numberofareas;
    two =rand()%numberofareas;
    while (two==one || (center[one].color==center[two].color && center[one].color==color[8]))
        two =rand()%numberofareas;
    majoon.type=rand()%4;
    majoon.x= (center[one].x+center[two].x)/2;
    majoon.y= (center[one].y+center[two].y)/2;
    printf ("%majoon %d %lf %lf\n", majoon.type, majoon.x, majoon.y);
}

void makemajoon(SDL_Renderer *renderer)
{
    if (nist)
    {
        SDL_Rect size = {majoon.x - 25, majoon.y - 25, 50, 50};
        SDL_Surface *m;
        if (majoon.type==0)
            m = SDL_LoadBMP("m0.bmp");
        if (majoon.type==1)
            m = SDL_LoadBMP("m1.bmp");
        if (majoon.type==2)
            m = SDL_LoadBMP("m2.bmp");
        if (majoon.type==3)
            m = SDL_LoadBMP("m3.bmp");
        if (!m)
            printf("Failed to load image at%s", SDL_GetError());
        SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, m);
        SDL_RenderCopy(renderer, img, NULL, &size);
        SDL_FreeSurface(m);
        SDL_DestroyTexture(img);
    }
}

void startfight(int *start, int *end)
{
    printf("start:%d end:%d\n", *start, *end);
    int i=0;
    while (bazi[i].nsarbaz!=0)
        i++;
    bazi[i].start=center[*start];
    bazi[i].end=center[*end];
    bazi[i].s=*start;
    bazi[i].e=*end;
    if (!center[*end].x-center[*start].x)
        bazi[i].shib=(center[*end].y-center[*start].y)/(center[*end].x-center[*start].x);
    else
        bazi[i].shib=0;
    bazi[i].nsarbaz=center[*start].soldiers;
    bazi[i].counter=0;
//            bazi[i].rate=4;
    center[*start].rate=0;
    center[bazi[i].s].on= true;
    printf("%lf %lf %lf %lf\n",center[*start].x, center[*start].y, bazi[i].start.x, bazi[i].start.y);
//            if (bazi[i].nsarbaz==0)
    *start = -1;
    *end = -1;
    printf("1\n");
}

void barkhord(SDL_Renderer *renderer, int c)
{
    for (int i=0; i<100; i++)
    {
        if (bazi[i].nsarbaz!=0)
        {
            hamle(renderer, c, i);

            for (int j=0; j<100; j++)
            {
                if (i!=j)
                {
                    if (bazi[j].nsarbaz != 0)
                    {
                        struct sarbaz *h= (struct sarbaz*)malloc(sizeof (struct sarbaz)), *m=(struct sarbaz*)malloc(sizeof (struct sarbaz));
                        h->next = bazi[j].head;
                        m->next  =bazi[i].head;
                        is_same(i, j, m, h);
                        bazi[j].head = h->next;
                    }
                }
            }
        }
    }
}

int halat0(int aval)
{
    srand(time(NULL));
    int akhar=rand()%numberofareas;
    while (center[akhar].color==center[aval].color || akhar==aval)
        akhar=rand()%numberofareas;
    return akhar;
}

int halat1(int aval)
{
    srand(time(NULL));
    int akhar=rand()%numberofareas, k=0;
//    center[akhar].soldiers>center[aval].soldiers || center[akhar].color!=center[aval].color ||
    while (akhar==aval)
    {
        akhar=rand()%numberofareas;
        k++;
        if (k>numberofareas)
        {
            akhar= halat0(aval);
            break;
        }
    }
    return akhar;
}

int halat2(int aval)
{
    srand(time(NULL));
    int akhar=rand()%numberofareas, k=0;
    while (center[akhar].color!=color[0])
    {
        akhar=rand()%numberofareas;
        k++;
        if (k>=numberofareas)
        {
            akhar= halat1(aval);
            break;
        }
    }
    return akhar;
}

int halat3(int aval)
{
    int min=1000000, akhar=-1;
    for (int i=0; i<numberofareas; i++)
    {
        if (center[i].soldiers<min && i!=aval && center[i].color!=center[aval].color)
        {
            min=center[i].soldiers;
            akhar=i;
        }
    }
    return akhar;
}

int halat4(int aval)
{
    srand(time(NULL));
    int akhar=rand()%numberofareas;
    for (int i=0; i<numberofareas; i++)
    {
        if (center[i].color==color[8])
        {
            akhar=i;
            break;
        }
//        if (majoon.x!=0 && majoon.y!=0 && (int)(center[aval].x+center[i].x) % (int)majoon.x==0 && (int)(center[aval].y+center[i].y) % (int)majoon.y==0)
//        {
//            akhar=i;
//            break;
//        }
    }
    return akhar;
}

void enemy(int *start, int *end)
{
    int ehtemal=rand()%100, halat=rand()%5;
    printf("%d %d\n", halat, ehtemal);
    int m=*start;
    if(ehtemal<80 && halat==1)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0] || center[aval].on)
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat1(*start);
        startfight(start, end);
    }
    if(ehtemal<65 && halat==2)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0] || center[aval].on)
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat2(*start);
        startfight(start, end);
    }
    if(ehtemal<80 && halat==3)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0] || center[aval].on)
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat3(*start);
        startfight(start, end);
    }
    if(ehtemal<95 && halat==4)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0] || center[aval].on)
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat4(*start);
        startfight(start, end);
    }
    *start=m;
}

int map(SDL_Window *window)
{
    srand(time(NULL));
    init();
    //window
//    SDL_Window *window = SDL_CreateWindow("Maps",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    //map
    int a = rand() % (SCREEN_WIDTH - s * 2), b = rand() % (SCREEN_HEIGHT-200 - 2 * s) + s * 0.5, c=0, start=-1, end=-1;
    double x = a * 1.0, y = b * 1.0;
    scanf("%d %d", &numberofareas, &numberofplayers);
    center = malloc(sizeof(struct markaz) * numberofareas);
    makecolors(color);
    draw(renderer, x, y);
    while (number<numberofareas)
        generate_random(renderer, x, y);
    for (int i=0; i<numberofareas; i++)
    {
        center[i].soldiers = 20;
        center[i].rate=15;
        center[i].r=20;
        center[i].speed = 4;
        if (center[i].color==color[8])
            center[i].max=20;
        else
            center[i].max=80;
        center[i].majoon=false;
        center[i].on=false;
    }
    //background
    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("background.bmp");
    if (!background)
        printf("Failed to load image at%s", SDL_GetError());
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);
    bazi = malloc(sizeof (struct fight)*100);
    for (int i=0; i<100; i++)
    {
        bazi[i].head = NULL;
        bazi[i].nsarbaz=0;
    }
    SDL_Event event;
    majoon.whichone=1;
    potion1.color=0;
    potion2.color=0;
    //running
    while (running)
    {
        edame=false;
        c++;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        bekesh(renderer, center);
        if (!edame)
            return 0;
        SDL_PollEvent(&event);
        if (event.type == SDL_MOUSEBUTTONDOWN)
            saveclick(event.motion.x, event.motion.y, &start, &end);
        if (start!=-1 && end!=-1)
            startfight(&start, &end);
        barkhord(renderer, c);
        if (c%400==0)
            randomareas(renderer);
        if (c%400>=0 && c%400<=300 && c>=400)
            makemajoon(renderer);
        active_potion();
        if (c%200==100)
            enemy(&start, &end);
        SDL_RenderPresent(renderer);
        if (event.type == SDL_QUIT)
            running=false;
        generate_soldier(c);
    }
    SDL_DestroyTexture(img);
    SDL_FreeSurface(background);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

void menu(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("background.bmp");
    if (!background)
        printf("Failed to load image at%s", SDL_GetError());
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);
    SDL_Event event;
    while (running)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        SDL_PollEvent(&event);
        SDL_RenderPresent(renderer);
        if (event.type == SDL_QUIT)
            running=false;
    }
}

int main()
{
    srand(time(NULL));
    init();
    //window
    SDL_Window *window = SDL_CreateWindow("Maps",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    map(window);
//    menu(window);
    SDL_DestroyWindow(window);
}
