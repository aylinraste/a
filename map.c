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
    int target;
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

struct users
{
    char name[40];
    int win;
    int lose;
    int draw;
    int score;
};

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const int s=50;
int chosenbackground=1;
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
Mix_Music *music1 = NULL;
Mix_Music *music2 = NULL;
bool win=false, lose=false;
char name[40]="";

bool hmenu=true, hmap=false, hsettings=false, hranking=false, hstart=false, hpause=false;

#ifdef main
#undef main
#endif
int pausemenu(SDL_Window *window, SDL_Texture *im, SDL_Texture *ak, SDL_Surface *back, SDL_Surface *pause);
void menu(SDL_Window *window);
void init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER |TTF_Init() | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
}
void texture(SDL_Renderer *m_renderer,int xp,int yp,char* addressOfImage,int width,int height)
{
    int n = strlen(addressOfImage);
    char char_array[n+1];

    strcpy(char_array, addressOfImage);
    SDL_Texture *myTexture;
    myTexture = IMG_LoadTexture( m_renderer, char_array);
    int w1, h1;
    SDL_QueryTexture(myTexture, NULL, NULL, &w1, &h1);
    SDL_Rect texr1;
    texr1.x = xp;
    texr1.y = yp;
    texr1.w = width;
    texr1.h = height;
    SDL_RenderCopy( m_renderer, myTexture, NULL, &texr1);
    SDL_DestroyTexture(myTexture);
}

void generate_random(SDL_Renderer *renderer, double x, double y);

void text(SDL_Renderer *renderer,int x,int y,const char * text,int font_size,int R,int G,int B, int A)

{
    TTF_Init();
    SDL_Color text_color = { R, G, B, A};
    const char * Font ="font.ttf";
    int mWidth = 0;
    int mHeight = 0;
    SDL_Rect* clip = NULL;
    TTF_Font *gFont = NULL;
    double angle = 0.0;
    SDL_Point* center = NULL;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Texture* mTexture = NULL;
    gFont = TTF_OpenFont(Font, font_size );

    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont,text, text_color );

    mWidth = textSurface->w;
    mHeight = textSurface->h;
    mTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
    SDL_FreeSurface( textSurface );
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopyEx( renderer, mTexture, clip, &renderQuad, angle, center, flip );
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
        center[i].rate=10;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].rate=10;
//    printf ("rate:%d %d\n", center[i].rate, i);
}

void m33(int i, int k)
{
    if (k==1)
        if (potion1.color==center[i].color)
        center[i].rate=20;
    if (k==2)
        if (potion2.color==center[i].color)
            center[i].rate=20;
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
    int man=0, to=0;
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
            to++;
        if (target==color[0])
            man++;
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
            SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, m);
            SDL_RenderCopy(renderer, img, NULL, &size);
            SDL_FreeSurface(m);
            SDL_DestroyTexture(img);
        }
    }
    if (man==0)
        lose=true;
    if (to==0)
        win=true;
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
    center[number].target=r;
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
            if (*start==-1 && center[i].color ==color[0])
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
//            center[bazi[i].e].rate=0;
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
    if (pow(head->x-bazi[i].start.x,2)+pow(head->y-bazi[i].start.y,2)<=220)
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
    if ((bazi[i].counter==bazi[i].nsarbaz || center[bazi[i].s].soldiers<=0) && bazi[i].head==NULL || center[bazi[i].s].color!=bazi[i].start.color)
    {
        bazi[i].nsarbaz=0;
//        center[bazi[i].s].rate=20;
//        center[bazi[i].e].rate=20;
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
//    center[*start].rate=0;
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
    int ehtemal=rand()%100, halat=rand()%4;
    printf("%d %d\n", halat, ehtemal);
    int m=*start;
    if(ehtemal<80 && halat==1)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0])
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat1(*start);
        startfight(start, end);
    }
    if(ehtemal<90 && halat==2)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0] )
            aval=rand()%numberofareas;
        *start=aval;
        *end = halat2(*start);
        startfight(start, end);
    }
    if(ehtemal<90 && halat==3)
    {
        int aval=rand()%numberofareas;
        while (center[aval].color==color[8] || center[aval].color==color[0])
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

int readfile (struct users carbar[])
{
    FILE *scores= fopen("scores.md", "r");
    char harf= fgetc(scores);
    int j=-1;
    while (harf != EOF)
    {
        j++;
        for (int m=0; m<40; m++)
            carbar[j].name[m]='\0';
        int i=0;
        while (harf!=EOF && harf!='/')
        {
            carbar[j].name[i++]=harf;
            harf= fgetc(scores);
            printf("n:%s\n", carbar[j].name);
        }
        if (harf=='/')
        {
            fscanf(scores, "%d %d %d %d ", &carbar[j].win, &carbar[j].lose, &carbar[j].draw, &carbar[j].score);
//            printf ("%d %d %d %d\n", carbar[j].win, carbar[j].lose, carbar[j].draw, carbar[j].score);
            harf= fgetc(scores);
        }
    }
//    printf("%d\n",j);
    return j;
}

void sort(FILE *scores)
{
    FILE *new= fopen("new.md", "w");
    struct users carbar[100];
    char harf= fgetc(scores);
    int j=-1;
    while (harf != EOF)
    {
        j++;
        for (int m=0; m<40; m++)
            carbar[j].name[m]='\0';
        int i=0;
        while (harf!=EOF && harf!='/')
        {
            carbar[j].name[i++]=harf;
            harf= fgetc(scores);
            printf("n:%s\n", carbar[j].name);
        }
        if (harf=='/')
        {
            fscanf(scores, "%d %d %d %d ", &carbar[j].win, &carbar[j].lose, &carbar[j].draw, &carbar[j].score);
            harf= fgetc(scores);
        }
    }
//    int j=readfile(scores);
    for (int i=1; i<=j; i++)
    {
        struct users key=carbar[i];
        int k=i-1;
        while (k>=0 && key.score>carbar[k].score)
        {
            carbar[k+1]=carbar[k];
            k--;
        }
        carbar[k+1]=key;
    }
    for (int i=0; i<=j; i++)
    {
        fprintf(new, "%s/%d %d %d %d\n", carbar[i].name, carbar[i].win, carbar[i].lose, carbar[i].draw, carbar[i].score);
//        printf("%s/%d %d %d %d\n", carbar[i].name, carbar[i].win, carbar[i].lose, carbar[i].draw, carbar[i].score);
    }
    fclose(new);
    remove("scores.md");
    rename("new.md", "scores.md");
}

int map(SDL_Window *window)
{
    srand(time(NULL));
    init();
    printf("its ok\n");
    //window
//    SDL_Window *window = SDL_CreateWindow("Maps",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    //map
    int a = rand() % (SCREEN_WIDTH-50 - s * 2), b = rand() % (SCREEN_HEIGHT-50 - 2 * s) + s * 0.5, c=0, start=-1, end=-1;
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
        center[i].rate=20;
        center[i].r=20;
        center[i].speed = 4;
        if (center[i].color==color[8])
            center[i].max=0;
        else
            center[i].max=80;
        center[i].majoon=false;
        center[i].on=false;
    }
    //background
    double xpause=5, ypause=545, w=50;
    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background;
    if (chosenbackground==1)
        background= SDL_LoadBMP("background1.bmp");
    if (chosenbackground==2)
        background= SDL_LoadBMP("background2.bmp");
    if (chosenbackground==3)
        background= SDL_LoadBMP("background3.bmp");
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {xpause, ypause , w , w};
    SDL_Surface *pause = SDL_LoadBMP("pause.bmp");
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, pause);

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
    while (running && hmap)
    {
        win=false;
        lose=false;
        c++;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        SDL_RenderCopy(renderer, aks, NULL, &size);
        bekesh(renderer, center);
        SDL_PollEvent(&event);
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
        {
            if (event.type == SDL_QUIT || win || lose)
            {
                running=false;
                FILE *scoresf=fopen("scores1.md", "w"), *scoresi= fopen("scores.md", "r");

                char harf= fgetc(scoresi);
//            fread(harf, 1, 1, scoresi);
//            fscanf(scoresi, "%c", &harf);
                int nwin, nlose, ndraw, score;
                bool nevesht=false;
                while (harf != EOF)
                {
                    int i=0;
                    char user[40]="";
                    while (harf!=EOF && harf!='/')
                    {
                        user[i++]=harf;
                        harf= fgetc(scoresi);
//                    fscanf(scoresi, "%c", &harf);
//                    printf("name:%s\n", user);
                    }
                    if (harf=='/')
                    {
                        fscanf(scoresi, "%d %d %d %d ", &nwin, &nlose, &ndraw, &score);
//                    printf("%d %d %d %d\n", nwin, nlose, ndraw, score);
                        if (!strcmp(user, name))
                        {
                            if (win)
                            {
                                nwin++;
                            }
                            if (lose)
                            {
                                nlose++;
                            }
                            if (event.type == SDL_QUIT)
                            {
                                ndraw++;
                            }
                            score=nwin*2-nlose;
                            nevesht=true;
                        }
//                    printf("%s/%d %d %d %d\n", user, nwin, nlose, ndraw, score);
                        fprintf(scoresf, "%s/%d %d %d %d\n", user, nwin, nlose, ndraw, score);
                        harf= fgetc(scoresi);
//                    printf("%c\n", harf);
                    }
                }
                if (!nevesht)
                {
                    if (win)
                    {
                        fprintf(scoresf, "%s/%d %d %d %d\n", name, 1, 0, 0, 2);
                    }
                    else if (lose)
                    {
                        fprintf(scoresf, "%s/%d %d %d %d\n", name, 0, 1, 0, -1);
                    }
//                if (event.type == SDL_QUIT)
                    else
                    {
                        fprintf(scoresf, "%s/%d %d %d %d\n", name, 0, 0, 1, 0);
                    }
                }
                fclose(scoresi);
                fclose(scoresf);
                remove("scores.md");
                rename("scores1.md", "scores.md");
                FILE *scores= fopen("scores.md", "r");
                sort(scores);
                fclose(scores);

            }
        }
        generate_soldier(c);
        if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.motion.x>=xpause && event.motion.x<=xpause+w && event.motion.y>=ypause && event.motion.y<=ypause+w)
            {
                SDL_DestroyRenderer(renderer);
                int chi=pausemenu(window, img, aks, background, pause);
                if (chi==1)
                {
                    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
                    img = SDL_CreateTextureFromSurface(renderer, background);
                    aks = SDL_CreateTextureFromSurface(renderer, pause);
                }
                if (chi==0)
                {
                    printf("running:%d\n", running);
                    hmap=false;
                    hmenu=true;
                }
            }
            else
                saveclick(event.motion.x, event.motion.y, &start, &end);
        }
    }
    printf("man\n");
    SDL_DestroyTexture(img);
//    SDL_FreeSurface(background);
//    SDL_DestroyRenderer(renderer);
    return 0;
}

void rankingmenu(SDL_Window *window)
{
    struct users carbar[40];
    int xname=100, yname=20;
    int xbord=350, ybord=20;
    int xmosavi=470, ymosavi=20;
    int xbakht=590, ybakht=20;
    int xscore=700,yscore=20;
    int x1=20, y1=90;
    int x2=20, y2=162;
    int x3=20, y3=234;
    int x4=20, y4=306;
    int x5=20, y5=378;
    int xbackicon=0, ybackicon=495, hbackicon=70, wbackicon=70;
    readfile(carbar);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("backgroundranking.bmp");
    if (!background)
        printf("Failed to load image at%s", SDL_GetError());
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {xbackicon, ybackicon , wbackicon , hbackicon};
    SDL_Surface *backicon = SDL_LoadBMP("backicon.bmp");
    if (!background)
        printf("Failed to load image at%s", SDL_GetError());
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, backicon);
    printf("hello\n");

    SDL_Event event;
    while (running && hranking)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        SDL_RenderCopy(renderer, aks, NULL, &size);
        text(renderer, xname, yname, "NAME", 48, 0, 0,0, 255);
        text(renderer, xbord, ybord, "W", 48, 0, 0,0, 255);
        text(renderer, xmosavi, ymosavi, "D", 48, 0, 0,0, 255);
        text(renderer, xbakht, ybakht, "L", 48, 0, 0,0, 255);
        text(renderer, xscore, yscore, "SCORE", 48, 0, 0,0, 255);
        text(renderer, x1, y1, "1", 48, 0, 0,0, 255);
        text(renderer, x2, y2, "2", 48, 0, 0,0, 255);
        text(renderer, x3, y3, "3", 48, 0, 0,0, 255);
        text(renderer, x4, y4, "4", 48, 0, 0,0, 255);
        text(renderer, x5, y5, "5", 48, 0, 0,0, 255);
//        printf("%s %d\n", carbar[1].name, carbar[1].score);
        for (int i=0; i<5; i++)
        {
            char win[3], lose[3], draw[3], score[7];
            sprintf(win, "%d", carbar[i].win);
            sprintf(lose, "%d", carbar[i].lose);
            sprintf(draw, "%d", carbar[i].draw);
            sprintf(score, "%d", carbar[i].score);
            text(renderer, xname, y1 + 72*i, carbar[i].name, 48, 0, 0,0, 255);
            text(renderer, xbord, y1 + 72*i, win, 48, 0, 0,0, 255);
            text(renderer, xbakht, y1 + 72*i, lose, 48, 0, 0,0, 255);
            text(renderer, xmosavi, y1 + 72*i, draw, 48, 0, 0,0, 255);
            text(renderer, xscore, y1 + 72*i, score, 48, 0, 0,0, 255);
        }
        SDL_PollEvent(&event);
        SDL_RenderPresent(renderer);
        if (event.type == SDL_QUIT)
            running=false;
        if(event.type== SDL_MOUSEBUTTONDOWN)
        {
            double x=event.motion.x;
            double y=event.motion.y;
            if (x>=xbackicon && x<=xbackicon+wbackicon && y>=ybackicon && y<= ybackicon+hbackicon)
            {
                hranking=false;
                hmenu=true;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(background);
    SDL_DestroyTexture(img);
}

void startmenu (SDL_Window *window)
{
    int xbackicon=0, ybackicon=495, hbackicon=70, wbackicon=70;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("backgroundstartmenu.bmp");
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {xbackicon, ybackicon , wbackicon , hbackicon};
    SDL_Surface *backicon = SDL_LoadBMP("backicon.bmp");
    if (!background)
        printf("Failed to load image at%s", SDL_GetError());
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, backicon);


    SDL_Event event;
    {
        while (running)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, img, NULL, &andaze);
            SDL_RenderCopy(renderer, aks, NULL, &size);
            SDL_PollEvent(&event);
            SDL_RenderPresent(renderer);
            if (event.type == SDL_QUIT)
                running=false;
            if(event.type== SDL_MOUSEBUTTONDOWN)
            {
                double x=event.motion.x;
                double y=event.motion.y;
                if (x>=xbackicon && x<=xbackicon+wbackicon && y>=ybackicon && y<= ybackicon+hbackicon)
                {
                    SDL_DestroyRenderer(renderer);
                    SDL_FreeSurface(background);
                    SDL_DestroyTexture(img);
                    menu(window);
                }
            }
        }
    }
}

void settingsmenu(SDL_Window *window)
{
    int xbackicon=0, ybackicon=495, hbackicon=70, wbackicon=70;
    double a=1, ax=0, ay=0;
    double b=1, bx=0, by=0;
    double c=1, cx=0, cy=0;
    int xback1=45, yback1=120, hback1=150, wback1=240;
    int xback2=330, yback2=120, hback2=150, wback2=240;
    int xback3=615, yback3=120, hback3=150, wback3=240;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("backgroundsettings.bmp");
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {xbackicon, ybackicon , wbackicon , hbackicon};
    SDL_Surface *backicon = SDL_LoadBMP("backicon.bmp");
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, backicon);

    SDL_Event event;
    {
        while (running && hsettings)
        {
            SDL_PollEvent(&event);
            SDL_RenderPresent(renderer);
            if (event.type == SDL_QUIT)
                running=false;
            if(event.type== SDL_MOUSEBUTTONDOWN)
            {
                double x=event.motion.x;
                double y=event.motion.y;
                if (x>=xbackicon && x<=xbackicon+wbackicon && y>=ybackicon && y<= ybackicon+hbackicon)
                {
//                    SDL_DestroyRenderer(renderer);
//                    SDL_FreeSurface(background);
//                    SDL_DestroyTexture(img);
                    hsettings=false;
                    hmenu=true;
//                    menu(window);
                }
                else if (x>=xback1 && x<=xback1+wback1 && y>=yback1 && y<= yback1+hback1)
                {
                    a=1.2;ax=24;ay=15;
                    b=1;bx=0;by=0;
                    c=1;cx=0;cy=0;
                    chosenbackground=1;
                }
                else if (x>=xback2 && x<=xback2+wback2 && y>=yback2 && y<= yback2+hback2)
                {
                    a=1;ax=0;ay=0;
                    b=1.2;bx=24;by=15;
                    c=1;cx=0;cy=0;
                    chosenbackground=2;
                }
                else if (x>=xback3 && x<=xback3+wback3 && y>=yback3 && y<= yback3+hback3)
                {
                    a=1;ax=0;ay=0;
                    b=1;bx=0;by=0;
                    c=1.2;cx=24;cy=15;
                    chosenbackground=3;
                }
            }
//            printf("%d\n", a);
            SDL_Rect size1 = {xback1-ax, yback1-ay , wback1*a , hback1*a};
            SDL_Surface *back1 = SDL_LoadBMP("background1.bmp");
            SDL_Texture *aks1 = SDL_CreateTextureFromSurface(renderer, back1);

            SDL_Rect size2 = {xback2-bx, yback2-by , wback2*b , hback2*b};
            SDL_Surface *back2 = SDL_LoadBMP("background2.bmp");
            SDL_Texture *aks2 = SDL_CreateTextureFromSurface(renderer, back2);

            SDL_Rect size3 = {xback3-cx, yback3-cy , wback3*c , hback3*c};
            SDL_Surface *back3 = SDL_LoadBMP("background3.bmp");
            SDL_Texture *aks3 = SDL_CreateTextureFromSurface(renderer, back3);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, img, NULL, &andaze);
            text(renderer, 270, 50, "CHOOSE YOUR BACKGROUND", 42, 255,255,255,255);
            SDL_RenderCopy(renderer, aks, NULL, &size);
            SDL_RenderCopy(renderer, aks1, NULL, &size1);
            SDL_RenderCopy(renderer, aks2, NULL, &size2);
            SDL_RenderCopy(renderer, aks3, NULL, &size3);
            SDL_DestroyTexture(aks1);
            SDL_FreeSurface(back1);
            SDL_DestroyTexture(aks2);
            SDL_FreeSurface(back2);
            SDL_DestroyTexture(aks3);
            SDL_FreeSurface(back3);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(background);
    SDL_DestroyTexture(img);
}

void menu(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("backgroundstart.bmp");
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {315, 70 , 250 , 250};
    SDL_Surface *start = SDL_LoadBMP("start1.bmp");
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, start);

    SDL_Rect size1 = {90, 350 , 180 , 120};
    SDL_Surface *ranking = SDL_LoadBMP("ranking.bmp");
    SDL_Texture *aks1 = SDL_CreateTextureFromSurface(renderer, ranking);

    SDL_Rect size2 = {610, 350 , 180 , 120};
    SDL_Surface *settings = SDL_LoadBMP("settings.bmp");
    SDL_Texture *aks2 = SDL_CreateTextureFromSurface(renderer, settings);

    SDL_Rect size3 = {235, 490 , 420 , 70};
    SDL_Surface *username = SDL_LoadBMP("username.bmp");
    SDL_Texture *aks3 = SDL_CreateTextureFromSurface(renderer, username);

    SDL_Event event;
    bool type=false;
    bool capslock=false;
    bool quit=false;
    int i;
    bool menuu=true;
    while (running && hmenu)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        SDL_RenderCopy(renderer, aks, NULL, &size);
        SDL_RenderCopy(renderer, aks1, NULL, &size1);
        SDL_RenderCopy(renderer, aks2, NULL, &size2);
        SDL_RenderCopy(renderer, aks3, NULL, &size3);
        SDL_PollEvent(&event);
        if ( event.type==SDL_MOUSEBUTTONDOWN )
        {
            int x=event.motion.x;
            int y=event.motion.y;
            if (pow(x-440, 2)+pow(y-195, 2)<= 125 * 125 && strlen(name)!=0 ) // start
            {
//                printf("1\n");
                hmenu=false;
                hmap=true;
//                SDL_DestroyRenderer(renderer);
//                SDL_FreeSurface(background);
//                SDL_DestroyTexture(img);
//                int k=map(window);
//                if (k==0)
//                    return;
//                startmenu(window);
            }
            else if (pow(x-445, 2)<= 210 * 210 && pow(y-525, 2)<= 35 * 35) // username
           {
//                printf("2\n");
                i=0;
                type=true;
           }
            else if (pow(x-700, 2)<=90*90 && pow(y-410, 2)<=60*60)//setting
            {
//                printf("3\n");
//                menuu=false;
                hmenu=false;
                hsettings=true;
//                SDL_DestroyRenderer(renderer);
//                SDL_FreeSurface(background);
//                SDL_DestroyTexture(img);
//                return;
//                settingsmenu(window);
            }
            else if (pow(x-180, 2)<=90*90 && pow(y-410, 2)<=60*60)//ranking
            {
//                printf("4\n");
//                menuu=false;
//                SDL_DestroyRenderer(renderer);
//                SDL_FreeSurface(background);
//                SDL_DestroyTexture(img);
                hmenu=false;
                hranking=true;
//                rankingmenu(window);
            }
        }
        if(type && !quit)
        {
            char ch='0';
            if ( event.type==SDL_KEYUP ) {
                if(event.key.keysym.sym==SDLK_CAPSLOCK)
                {
                    if (!capslock)
                        capslock=true;
                    else if (capslock)
                        capslock=false;
                }
                else if(event.key.keysym.sym==SDLK_BACKSPACE && i>0) { //irad dare!
                    name[i-1]='\0';
                    i--;
                }
                else if(event.key.keysym.sym==SDLK_RETURN)
                    quit=true;
                else
                    ch=event.key.keysym.sym;

                if ( (ch>='A' && ch <= 'Z') || (ch>='a' && ch <= 'z') || ch ==' ')
                {
                    if (capslock)
                    {
                        name[i++]=ch+'A'-'a';
//                        i++;
//                        printf("i:%d\n", i);
                    }
                    else
                    {
                        name[i++]=ch;
//                        i++;
//                        printf("i:%d\n", i);
                    }
                }
            }
        }
        if(strlen(name)!=0)
        {
            text(renderer, 330, 510, name, 56, 0, 0,0, 255);
        }
        SDL_RenderPresent(renderer);
        if (event.type == SDL_QUIT)
            running=false;
    }
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(background);
    SDL_DestroyTexture(img);
}

int pausemenu(SDL_Window *window, SDL_Texture *im, SDL_Texture *ak, SDL_Surface *back, SDL_Surface *pause)
{
    bool mutee=false;
    int xresum=135, yresum=200, w=120;
    int xrestart=390, yrestart=200;
    int xexit=645, yexit=200;
    int xmute=250, ymute=390;
    int xsave=530, ysave=390;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_Rect andaze = {0, 0 , SCREEN_WIDTH , SCREEN_HEIGHT};
    SDL_Surface *background = SDL_LoadBMP("backgroundpause.bmp");
    SDL_Texture *img = SDL_CreateTextureFromSurface(renderer, background);

    SDL_Rect size = {xmute, ymute , w , w};
    SDL_Surface *mute = SDL_LoadBMP("mute.bmp");
    SDL_Texture *aks = SDL_CreateTextureFromSurface(renderer, mute);

    SDL_Rect sizee = {xresum, yresum , w , w};
    SDL_Surface *resume = SDL_LoadBMP("resume.bmp");
    SDL_Texture *akss = SDL_CreateTextureFromSurface(renderer, resume);

    SDL_Rect sizeee = {xrestart, yrestart , w , w};
    SDL_Surface *restart = SDL_LoadBMP("restart.bmp");
    SDL_Texture *aksss = SDL_CreateTextureFromSurface(renderer, restart);

    SDL_Rect sizeeee = {xexit, yexit , w , w};
    SDL_Surface *exit = SDL_LoadBMP("exit.bmp");
    SDL_Texture *akssss = SDL_CreateTextureFromSurface(renderer, exit);

    SDL_Rect sizeeeee = {xsave, ysave , w , w};
    SDL_Surface *save = SDL_LoadBMP("save.bmp");
    SDL_Texture *aksssss = SDL_CreateTextureFromSurface(renderer, save);

    SDL_Rect sizeeeeee = {xmute, ymute, w , w};
    SDL_Surface *unmute = SDL_LoadBMP("unmute.bmp");
    SDL_Texture *akssssss = SDL_CreateTextureFromSurface(renderer, unmute);

    SDL_Event event;
    int c=0;

    while (running)
    {
        c++;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, img, NULL, &andaze);
        if (!mutee)
            SDL_RenderCopy(renderer, aks, NULL, &size);
        else
            SDL_RenderCopy(renderer, akssssss, NULL, &sizeeeeee);
        SDL_RenderCopy(renderer, akss, NULL, &sizee);
        SDL_RenderCopy(renderer, aksss, NULL, &sizeee);
        SDL_RenderCopy(renderer, akssss, NULL, &sizeeee);
        SDL_RenderCopy(renderer, aksssss, NULL, &sizeeeee);
        if (c%30<=15)
            text(renderer, 320, 70, "GAME PAUSED!", 56, 90, 100, 255, 255);
        else
            text(renderer, 320, 70, "GAME PAUSED!", 56, 230, 170, 255, 255);
        SDL_PollEvent(&event);
        SDL_RenderPresent(renderer);
        if (event.type == SDL_QUIT)
            running=false;
        if (event.type==SDL_MOUSEBUTTONDOWN)
        {
            double x=event.motion.x;
            double y=event.motion.y;
            if (x>=xresum && x<=xresum+w && y>=yresum && y<=yresum+w) //resume
            {
                SDL_DestroyRenderer(renderer);
                SDL_FreeSurface(background);
                SDL_DestroyTexture(img);
                return 1;
//                map(window);
            }
            else if (x>=xrestart && x<=xrestart+w && y>=yrestart && y<=yrestart+w)
            {
                SDL_DestroyRenderer(renderer);
                SDL_FreeSurface(background);
                SDL_DestroyTexture(img);
                SDL_FreeSurface(back);
                SDL_DestroyTexture(im);
                return 0;
            }
            else if (x>=xexit && x<=xexit+w && y>=yexit && y<=yexit+w) //quit
                running=false;
            else if (x>=xsave && x<=xsave+w && y>=ysave && y<=ysave+w) //save
            {
                FILE *planc=fopen("plansc.md", "w"), *plan= fopen("plans.md", "r");
                char harf;
                while ((harf=fgetc(plan)) != EOF)
                {
                    fprintf(planc, "%c", harf);
//                    printf("%c", harf);
                }
                char x[4], y[4], r[3], nofa[5], nofp[5];
                sprintf(nofa, "%d", numberofareas);
                sprintf(nofa, "%d", numberofplayers);
                fprintf(planc, "/%d %d\n", numberofareas, numberofplayers);
                for (int i=0; i<numberofareas; i++)
                {
                    fprintf(planc, "%lf %lf %d\n", center[i].x, center[i].y, center[i].target);
                }
                fclose(plan);
                fclose(planc);
                remove("plans.md");
                rename("plansc.md", "plans.md");
            }
            else if (x>=xmute && x<=xmute+w && y>=ymute && y<=ymute+w)//sound
            {
                if(!mutee)
                {
                    mutee=true;
                    Mix_PauseMusic();
                }
                else
                {
                    mutee=false;
                    Mix_ResumeMusic();
                }
            }
        }
    }
    return 0;
}

int main()
{
    srand(time(NULL));
    init();
    SDL_Window *window = SDL_CreateWindow("Maps",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );
    music1=Mix_LoadMUS("music0.mp3");
    music2=Mix_LoadMUS("music2.mp3");
    Mix_PlayMusic( music1, -1 );
//    map(window);
    while (running)
    {
        if (hmenu)
            menu(window);
        if (hsettings)
            settingsmenu(window);
        if (hranking)
            rankingmenu(window);
        if (hmap)
            map(window);
        printf("hello\n");
    }

//    pause(window);
//    rankingmenu(window);
    Mix_FreeMusic( music1 );
    Mix_FreeMusic( music2 );
    music1 = NULL;
    music2 = NULL;
    SDL_DestroyWindow(window);
    SDL_Quit();
}
