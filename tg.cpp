#include <allegro.h>

// C library definitions
#include <cstdio> 
#include <cstdlib>
#include <ctime> 
#include <cstring>

// C++ library definitions
#include <map> 
#include <string>
#include <sstream>

// Import into global namespace
using namespace std;

// SDL library includes.
//#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"

FILE *logFile = NULL;

const int TILE_SIZE = 32;
const int SCREEN_BORDER = 80;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// NEW: Not sure about these.
#define WPN_DAGGER 1
#define WPN_SHORT 2
#define WPN_LONG 4
#define WPN_GREAT 8
#define WPN_MITHRIL 16

// NEW: Not sure about these.
#define AMR_CLOTHES 1
#define AMR_LEATHER 2
#define AMR_CHAIN 4
#define AMR_PLATE 8
#define AMR_DRAGON 16

// NEW: Not sure about these.
#define MAG_FIRE 1
#define MAG_STUN 2
#define MAG_CURE 4
#define MAG_WARP 8
#define MAG_MANA 16

// NEW: Not sure about these.
#define KEY_SILVER 1
#define KEY_GOLD 2
#define KEY_EBONY 4
#define KEY_RUBY 8
#define KEY_EMERALD 16
#define KEY_SAPPHIRE 32

// NEW: Not sure about these.
#define CRYSTAL_HEART_A 1
#define CRYSTAL_HEART_B 2
#define CRYSTAL_HEART_C 4
#define CRYSTAL_HEART_D 8

// NEW: Not sure about these.
#define CRYSTAL_STAR_A 16
#define CRYSTAL_STAR_B 32
#define CRYSTAL_STAR_C 64
#define CRYSTAL_STAR_D 128

// NEW: Not sure about these.
#define ITEM_SOUL_GEM 1
#define ITEM_MERMAID_SCALE 2
#define ITEM_MAGMA_STONE 4
#define ITEM_ANGEL_WING 8
#define ITEM_SHIELD_RING 16
#define ITEM_ELVEN_BOOTS 32
#define ITEM_PHOENIX_FEATHER 64
#define ITEM_CRYSTAL_ORB 128

#define CMD_DOWN 1
#define CMD_UP 2
#define CMD_LEFT 3
#define CMD_RIGHT 4
#define CMD_ATTACK 5
#define CMD_MAGIC 6
#define CMD_SHOOT 7

// NEW
#define CMD_MTAUR_ATTACK 8

#define FACE_FRONT 0
#define FACE_BACK 3
#define FACE_LEFT 6
#define FACE_RIGHT 9

// NEW
#define FACE_DEAD 16

#define PFRAME 12

#define PAIN_FRONT 10
#define PAIN_BACK 20
#define PAIN_LEFT 30
#define PAIN_RIGHT 40

#define OBJ_DEAD -1
#define OBJ_NULL 0
#define OBJ_PLAYER 1
#define OBJ_SLIME 2
#define OBJ_DARKELF 3

// NEW
#define OBJ_BAT 4
#define OBJ_GOBLIN 5
#define OBJ_ORC 6
#define OBJ_OOZE 7
#define OBJ_KNIGHT 8

// NEW
#define OBJ_GHOST 14
#define OBJ_DOUBLE 61
#define OBJ_MTAUR_TOP 63
#define OBJ_MINOTAUR 62
#define OBJ_V 66

#define OBJ_LIFE 128
#define OBJ_MANA 129
#define OBJ_HEART 130
#define OBJ_STAR 131

#define OBJ_DAGGER 132
#define OBJ_SHORTSWD 133
#define OBJ_LONGSWD 134
#define OBJ_GREATSWD 135
#define OBJ_MITHRILSWD 136

#define OBJ_CLOTHES 137
#define OBJ_LEATHER 138
#define OBJ_CHAIN 139
#define OBJ_PLATE 140
#define OBJ_DRAGON 141

#define OBJ_FIRESPL 143
#define OBJ_STUNSPL 144
#define OBJ_WARPSPL 145
#define OBJ_MANASPL 146
#define OBJ_CURESPL 147

#define OBJ_GOLDKEY 148
#define OBJ_SILVERKEY 149 

// NEW
#define OBJ_EBONYKEY 150
#define OBJ_RUBYKEY 151
#define OBJ_EMERALDKEY 152
#define OBJ_SAPPHIREKEY 153

// NEW
#define OBJ_SOULGEM 154
#define OBJ_MERMAIDSCALE 155
#define OBJ_MAGMASTONE 156
#define OBJ_ANGELWING 157
#define OBJ_SHIELDRING 158
#define OBJ_ELVENBOOTS 159
#define OBJ_PHOENIXFEATHER 160
#define OBJ_CRYSTALORB 161

// NEW
#define OBJ_DOGBERT 241

#define MAXOBJ 21

#define OBJ_TYPE 1
#define OBJ_X 2
#define OBJ_Y 3

string consoletext = "";
int console = 0;

int messagetime = 0;
string messagetext = "";

int cheat = 0;
int tg_pause = 0; 

void Screen_ClearBuffer(void);
void Sys_LoadGame(const string& fname, int snum);
void Think_MissileThink(void);
void Sys_SaveGame(const string& fname, int snum);
void Sys_NewGame();
void Screen_CreditsScreen();
void Map_LoadMap(const string& fname);
void Obj_LoadObjects();
BITMAP *Data_GetGfxFile(const string& fname);
SAMPLE *Data_GetSfxFile(const string& fname);
void Screen_CopyBuffer(void) ;


#define GFX(x) Data_GetGfxFile(x)
#define SFX(x) Data_GetSfxFile(x)

unsigned long screenshot = 0; 
 
/* 
int stage[] = { 16, 32, 34, 37, 40, 46 }; 
int level = 0; 
*/ 
 
char J[] = "TARD"; 

/* 
PALETTE pal; 
PALETTE rpal, fpal; 
PALETTE bpal, spal; 
PALETTE wpal, cpal; 
PALETTE gpal, mpal; 
PALETTE kpal, ppal; 
*/
 
//DATAFILE *idat; 
//DATAFILE *objfile, *ts_base, *ts_occlude, *ts_fringe; 
//BITMAP *objfile, *ts_base, *ts_occlude, *ts_fringe;
  
void Sys_Shutdown(void); 
void Sys_Initialize(void); 
 
int gameover = 0; 
int gamestart = 0; 
 
int pflag = FALSE; 
 
int prt = 0;

typedef struct {
    int area;
    int hearts;
    int stars;
} save_t;
  
save_t save[7]; 
save_t tmpsav; 

typedef struct {
    char mapfile[30];
    int map_x, map_y;
    int pos_x, pos_y;
    int facing;
} portal_t;
 
portal_t portal[30]; 
 
typedef struct {
    string sprite;
    string weapon;
    string missile;
    string snd_pain;
    string snd_death;
    string snd_attack;
    int cmd;
    int cast;
    int keys;
    int items;
    int type;
    int oldtype;
    int dumb;
    int frame;
    int pos_x, pos_y;
    int offset;
    int facing;
    int life, max_life;
    int mana, max_mana;
    int att, def;
    int msl_at;
    int msl_pos_x, msl_pos_y;
    int msl_offset;
    int wpn, amr, mag;
    int trans;
    int spell;  
    int msl;
    int msl_trans;
    int msl_facing;
    int speed, msl_speed;
    int gold;
    int crystals;
    int attack;
    int pain;
} obj_t;


typedef struct map_s {
   unsigned char base[20][10];
   unsigned char fringe[20][10];
   unsigned char occlude[20][10];
   unsigned char obj[MAXOBJ][3];
} map_t;

typedef struct world_s {
   map_t map[10][10];
   unsigned char map_x, map_y;
} world_t;

obj_t obj[MAXOBJ]; 
world_t world; 
 
obj_t *self; 

int minimap[10][10];
 
int nextobj; 
 
int white, black, red, green, blue, ltblue, dkblue, ltgray, gray, dkgray; 
 
char *desc[] =
{
    "World of Life",
    "Goblin Caves",
    "Ship of Fools",
    "Dark Pyramid",
    "Tower of the Wind",
    "World of Mana",
    "Warlock's Crypt",
    "Undersea Shrine",
    "Volcano Cave",
    "Sky Palace",
    "World of Chaos",
    "Castle of Despair",
    "Dragon's Lair",
    "Tower of the Magi",
    "The Nexus"
};

void Con_Command(const string& cmdtext);
 
void Tile_LoadSets(void); 


/*
===============
System Code
===============
*/

void Game_Init();
void Game_Frame();
void Game_Shutdown();

map<string,BITMAP*> gfx;
map<string,SAMPLE*> sfx;
 
BITMAP *buffer; 
 
MIDI *bgm; 
string music;
 
FONT *mfont; 

void Sound_StopMidi() 
{   
   stop_midi(); 
   if (bgm != NULL)
    destroy_midi(bgm);
   bgm = NULL;
   music = "";
} 
 
void Sound_PlayMidi(const string& fname) 
{ 
    Sound_StopMidi();       
    bgm = load_midi((string("music/") + fname).c_str());    
    if (bgm != NULL)
    {
    play_midi(bgm, 1); 
    music = fname;
   }
} 
 
 
void Sound_PlaySound(const string& file) 
{ 
    SAMPLE *snd = SFX(file);
    
    if (snd != NULL)
    play_sample(snd, 255, 127, 1000, 0); 
} 

void Screen_FadeOut()
{
    Screen_ClearBuffer();
    rest(100);  
}

void Screen_FadeIn()
{
    rest(100);
    Screen_CopyBuffer();
}
 
void Screen_GetColors(void) 
{ 
   white = makecol(255, 255, 255); 
   black = makecol(0, 0, 0); 
   red = makecol(255, 0, 0); 
   green = makecol(0, 255, 0); 
   ltgray = makecol(192, 192, 192);
   gray = makecol(128, 128, 128); 
   dkgray = makecol(64, 64, 64);
   ltblue = makecol(0, 0, 192); 
   blue = makecol(0, 0, 127); 
   dkblue = makecol(0, 0, 63);
} 
 
void Screen_SaveShot(void) 
{ 
   BITMAP *bmp; 
   PALETTE plt; 
   char fname[20]; 
 
   mkdir("shots"); 
 
   sprintf(fname, "shots/tg%i.tga", screenshot++); 
 
   while (exists(fname)) 
   { 
      sprintf(fname, "shots/tg%i.tga", screenshot++); 
      if (screenshot >= 1000000) 
         return; 
   } 
 
   //get_palette(plt); 
   //bmp = create_sub_bitmap(screen, 0, 0, SCREEN_W, SCREEN_H); 
   //save_bitmap(fname, bmp, NULL); 
   //destroy_bitmap(bmp); 
   
   save_bitmap(fname, buffer, NULL);
} 

void Data_Free()
{
    map<string,BITMAP*>::iterator i;
    map<string,SAMPLE*>::iterator j;
    
    for (i = gfx.begin(); i != gfx.end(); i++)
        destroy_bitmap((*i).second);
    
    for (j = sfx.begin(); j != sfx.end(); j++)
        destroy_sample((*j).second);
}

void Data_LoadFiles()
{
   mfont = font;
}

void Tile_DrawTile(const string& file, int tile, int x, int y)
{
    BITMAP *bmp = GFX(file);
    
    if (bmp != NULL)
    {
        int numTilesAcross = bmp->w / TILE_SIZE;
        
        int tilePosX = TILE_SIZE * (tile % numTilesAcross);
        int tilePosY = TILE_SIZE * (tile / numTilesAcross);
        
        BITMAP *bmp2 = create_sub_bitmap(bmp, tilePosX, tilePosY, TILE_SIZE, TILE_SIZE);
        if (bmp2 != NULL)
        {
            draw_sprite(buffer, bmp2, x, y);
            destroy_bitmap(bmp2);
        }
    }
}

void Tile_DrawTransTile(const string& file, int tile, int x, int y)
{
    BITMAP *bmp = GFX(file);
    
    if (bmp != NULL)
    {
        int numTilesAcross = bmp->w / TILE_SIZE;
        
        int tilePosX = TILE_SIZE * (tile % numTilesAcross);
        int tilePosY = TILE_SIZE * (tile / numTilesAcross);
        
        BITMAP *bmp2 = create_sub_bitmap(bmp, tilePosX, tilePosY, TILE_SIZE, TILE_SIZE);
        if (bmp2 != NULL)
        {
            draw_trans_sprite(buffer, bmp2, x, y);
            destroy_bitmap(bmp2);
        }
    }
}

BITMAP *Data_GetGfxFile(const string& fname)
{
    if (gfx.find(fname) == gfx.end())
    {
        PALETTE pal;        
        
        gfx[fname] = load_bitmap((string("gfx/") + fname).c_str(), pal);            
        if (gfx[fname] == NULL)
            fprintf(logFile, ((string("WARNING: File gfx/") + fname + string(" not found.\n")).c_str()));                       
    }       
    
    return gfx[fname];
}

SAMPLE *Data_GetSfxFile(const string& fname)
{
    if (sfx.find(fname) == sfx.end())
    {
        sfx[fname] = load_sample((string("sfx/") + fname).c_str());
        if (sfx[fname] == NULL)
            fprintf(logFile, ((string("WARNING: File sfx/") + fname + string(" not found.\n")).c_str()));   
    }
    
    return sfx[fname];
}   

void Sys_Initialize(void) 
{   
   int gfxdrv = GFX_AUTODETECT;
   
   logFile = fopen("log.txt", "wt");
   
   allegro_init(); 

   set_window_title("Tilegame: The Adventures of Mike");

   install_timer(); 
   initialise_joystick(); 

   set_color_depth(32);
   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0) != 0)
   //if (set_gfx_mode(gfxdrv, 320, 200, 0, 0) != 0)
   {
      allegro_message ("Couldn't set graphics mode: %s", allegro_error);
      exit (1);
   }

   install_keyboard(); 
 
   install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL); 
 
   buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT); 
 
   set_trans_blender(0, 0, 0, 128);
   
   text_mode(-1);    
   
    Screen_GetColors(); 
    Data_LoadFiles(); 
    
    srand((unsigned)time(NULL)); 
    
    Game_Init();      
} 
 
void Sys_Shutdown(void) 
{ 
    Game_Shutdown();
    
   set_gfx_mode(GFX_TEXT, 0, 0, 0, 0); 
   remove_keyboard(); 
 
   Data_Free(); 
   destroy_bitmap(buffer); 
 
   fclose(logFile);
} 
 
void Screen_CopyBuffer(void) 
{
   acquire_screen();
   blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   release_screen();
} 
 
void Screen_ClearBuffer(void) 
{ 
    /*
   if (obj[0].pain) 
      clear_to_color(buffer, red); 
   else 
   */
   clear(buffer); 
} 

void Screen_DrawRect(int x1, int y1, int x2, int y2, int color)
{
    rect(buffer, x1, y1, x2, y2, color);
}

void Screen_FillRect(int x1, int y1, int x2, int y2, int color)
{
    rectfill(buffer, x1, y1, x2, y2, color);
}

void Screen_DrawText(const string& txt, int x, int y, int color)
{
    textout(buffer, mfont, txt.c_str(), x-1, y-1, black);
    textout(buffer, mfont, txt.c_str(), x+0, y-1, black);
    textout(buffer, mfont, txt.c_str(), x+1, y-1, black);
    textout(buffer, mfont, txt.c_str(), x-1, y+0, black);
    textout(buffer, mfont, txt.c_str(), x+1, y+0, black);
    textout(buffer, mfont, txt.c_str(), x-1, y+1, black);
    textout(buffer, mfont, txt.c_str(), x+0, y+1, black);
    textout(buffer, mfont, txt.c_str(), x+1, y+1, black);
    textout(buffer, mfont, txt.c_str(), x, y, color);
}

void Screen_DrawTextCenter(const string& txt, int x, int y, int color)
{
    textout_centre(buffer, mfont, txt.c_str(), x-1, y-1, black);
    textout_centre(buffer, mfont, txt.c_str(), x+0, y-1, black);
    textout_centre(buffer, mfont, txt.c_str(), x+1, y-1, black);
    textout_centre(buffer, mfont, txt.c_str(), x-1, y+0, black);
    textout_centre(buffer, mfont, txt.c_str(), x+1, y+0, black);
    textout_centre(buffer, mfont, txt.c_str(), x-1, y+1, black);
    textout_centre(buffer, mfont, txt.c_str(), x+0, y+1, black);
    textout_centre(buffer, mfont, txt.c_str(), x+1, y+1, black);
    textout_centre(buffer, mfont, txt.c_str(), x, y, color);
}

void Screen_DrawImage(const string& file, int x, int y)
{
    BITMAP *bmp = GFX(file);
    blit(bmp, buffer, 0, 0, x, y, bmp->w, bmp->h);
}

int Screen_TextWidth(const string& txt)
{
    return text_length(mfont, txt.c_str());
}

int Screen_TextHeight()
{
    return text_height(mfont);
}

void Sys_KeyClear(void) 
{ 
   key[KEY_UP] = key[KEY_DOWN] = key[KEY_LEFT] = key[KEY_RIGHT] = key[KEY_ENTER] = key[KEY_Y] = key[KEY_N] = key[KEY_ESC] = key[KEY_F12] = 0; 
   joy_up = joy_down = joy_left = joy_right = joy_b1 = joy_b2 = 0; 
} 

int Sys_Confirm(const string& txt, int t2, int h, int s) 
{ 
   int sel = 1; 
   int booboo = 0; 
   char st; 
 
   while (TRUE) 
   { 
      Sys_KeyClear(); 
       
      if (booboo == 10) 
      { 
         booboo = 0; 
         poll_joystick(); 
      } 
      else 
         booboo++; 
 
      //Screen_FillRect(100, 50, 221, 151, white);
      //Screen_FillRect(101, 51, 220, 150, blue);
      Screen_FillRect(200, 100, 442, 302, white);
      Screen_FillRect(201, 101, 440, 300, blue);
 
      //Screen_DrawTextCenter(txt, 160, 55, white); 
      Screen_DrawTextCenter(txt, 320, 110, white); 
 
      if (t2 > -1) 
         //Screen_DrawTextCenter(desc[t2], 160, 105, white); 
         Screen_DrawTextCenter(desc[t2], 320, 210, white); 
         
      if (h >= 0 && s >= 0) 
      { 
 
      //Tile_DrawTile("objects.pcx", OBJ_HEART, 125, 80); 
      //Tile_DrawTile("objects.pcx", OBJ_STAR, 175, 80);       
      Tile_DrawTile("objects.pcx", OBJ_HEART, 250, 160); 
      Tile_DrawTile("objects.pcx", OBJ_STAR, 350, 160); 
 
      //Screen_DrawRect(125, 80, 125+TILE_SIZE, 80+TILE_SIZE, white); 
      //Screen_DrawRect(175, 80, 175+TILE_SIZE, 80+TILE_SIZE, white); 
      Screen_DrawRect(250, 160, 250+TILE_SIZE, 160+TILE_SIZE, white); 
      Screen_DrawRect(350, 160, 350+TILE_SIZE, 160+TILE_SIZE, white); 

      
      string st;
      
      st = string(1, '0'+h);      
      //Screen_DrawText(st, 134, 87, white); 
      Screen_DrawText(st, 268, 174, white); 
 
      st = string(1, '0'+s);
      //Screen_DrawText(st, 184, 87, white); 
      Screen_DrawText(st, 368, 174, white); 
 
      } 
      else 
      { 
      //Tile_DrawTile("objects.pcx", OBJ_CLOTHES, 150, 80); 
      Tile_DrawTile("objects.pcx", OBJ_CLOTHES, 300, 160); 
      } 
    
      //Screen_DrawTextCenter("(Y)es", 130, 120, gray); 
      //Screen_DrawTextCenter("(N)o", 190, 120, gray); 
      Screen_DrawTextCenter("(Y)es", 260, 240, gray); 
      Screen_DrawTextCenter("(N)o", 380, 240, gray); 

 
      if (sel == 1) 
         //Screen_DrawTextCenter("(Y)es", 130, 120, white); 
         Screen_DrawTextCenter("(Y)es", 260, 240, white); 
      else if (sel == 0) 
         //Screen_DrawTextCenter("(N)o", 190, 120, white); 
         Screen_DrawTextCenter("(N)o", 380, 240, white); 
 
      Screen_CopyBuffer(); 
 
      if (key[KEY_LEFT] || key[KEY_RIGHT] || joy_left || joy_right) 
         sel = !sel; 
 
      if (key[KEY_Y]) 
         return 1; 
      if (key[KEY_N]) 
         return 0; 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      if (key[KEY_ENTER] || joy_b1) 
      {
         Sys_KeyClear();
         return sel; 
      } 
   } 
} 
 
int Sys_StatusLoadGame(void) 
{ 
   int sel = 1; 
   char fname[32]; 
   int booboo = 0; 
 
   FILE *sfile;
   int i; 
 
   for (i = 1; i <= 6; i++) 
   { 
      sprintf(fname, "save%i.sav", i); 
      sfile = fopen(fname, "rb");
      if (!sfile) 
      { 
         save[i].area = -1; 
         save[i].hearts = 0; 
         save[i].stars = 0; 
      } 
      else 
      { 
          fread(&save[i], sizeof(save_t), 1, sfile);
      } 
      
      fclose(sfile);
   } 
 
   while (TRUE) 
   { 
      Sys_KeyClear(); 
 
      if (booboo == 10) 
      { 
         booboo = 0; 
         poll_joystick(); 
      } 
      else 
         booboo++; 
 
      Screen_FillRect(100, 50, 221, 151, white); 
      Screen_FillRect(101, 51, 220, 150, blue); 
    
      Screen_DrawTextCenter("Load Game", 160, 55, white); 
 
      Screen_DrawText("Save 1", 111, 70, gray); 
      Screen_DrawText("Save 2", 111, 95, gray); 
      Screen_DrawText("Save 3", 111, 120, gray); 
    
      Screen_DrawText("Save 4", 161, 70, gray); 
      Screen_DrawText("Save 5", 161, 95, gray); 
      Screen_DrawText("Save 6", 161, 120, gray); 
    
      switch (sel) 
      { 
         case 1 : Screen_DrawText("Save 1", 111, 70, white); break; 
         case 2 : Screen_DrawText("Save 2", 111, 95, white); break; 
         case 3 : Screen_DrawText("Save 3", 111, 120, white); break; 
         case 4 : Screen_DrawText("Save 4", 161, 70, white); break; 
         case 5 : Screen_DrawText("Save 5", 161, 95, white); break; 
         case 6 : Screen_DrawText("Save 6", 161, 120, white); break; 
      } 
 
      Screen_CopyBuffer(); 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      if (key[KEY_UP] || joy_up) 
      { 
         sel--; 
         if (sel < 1) 
            sel = 6; 
      } 
      if (key[KEY_DOWN] || joy_down) 
      { 
         sel++; 
         if (sel > 6) 
            sel = 1; 
      } 
      if (key[KEY_RIGHT]) 
      { 
         if (sel <= 3) 
            sel = sel + 3; 
         else 
            sel = sel - 3; 
      } 
      if (key[KEY_LEFT]) 
      { 
         if (sel >= 4) 
            sel = sel - 3; 
         else 
            sel = sel + 3; 
      } 
      if (key[KEY_ENTER] || joy_b1) 
      { 
         if (Sys_Confirm("Reload game?", save[sel].area, save[sel].hearts, save[sel].stars)) 
         { 
            switch (sel) 
            { 
               case 1 : Sys_LoadGame("save1.sav", 1); break; 
               case 2 : Sys_LoadGame("save2.sav", 2); break; 
               case 3 : Sys_LoadGame("save3.sav", 3); break; 
               case 4 : Sys_LoadGame("save4.sav", 4); break; 
               case 5 : Sys_LoadGame("save5.sav", 5); break; 
               case 6 : Sys_LoadGame("save6.sav", 6); break; 
            } 
 
            return 1; 
         } 
      } 
      if (key[KEY_ESC] || joy_b2) 
      { 
         return 0; 
      } 
   } 
} 
 
void Sys_GameMenu(void) 
{ 
   int sel = 1; 
   int booboo = 0; 
 
   int i; 
 
//  fade_in(pal, 32); 
 
   while (TRUE) 
   { 
      Sys_KeyClear(); 
 
      if (booboo == 10) 
      { 
         booboo = 0; 
         poll_joystick(); 
      } 
      else 
         booboo++; 
 
      Screen_FillRect(100, 50, 221, 151, white); 
      Screen_FillRect(101, 51, 220, 150, blue); 
    
      Screen_DrawTextCenter("Game Menu", 160, 55, white); 
 
      Screen_DrawText("New Game", 130, 70, gray); 
      Screen_DrawText("Load Game", 130, 95, gray); 
      Screen_DrawText("Exit Game", 130, 120, gray); 
    
      switch (sel) 
      { 
         case 1 : Screen_DrawText("New Game", 130, 70, white); break; 
         case 2 : Screen_DrawText("Load Game", 130, 95, white); break; 
         case 3 : Screen_DrawText("Exit Game", 130, 120, white); break; 
      } 
 
      Screen_CopyBuffer(); 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      if (key[KEY_UP] || joy_up) 
      { 
         sel--; 
         if (sel < 1) 
            sel = 3; 
      } 
      if (key[KEY_DOWN] || joy_down) 
      { 
         sel++; 
         if (sel > 3) 
            sel = 1; 
      } 
      if (key[KEY_ENTER] || joy_b1) 
      { 
         switch (sel) 
         { 
            case 1 : if (Sys_Confirm("Start anew?", -1, -1, -1)) { Sys_NewGame(); return; } break; 
            case 2 : if (Sys_StatusLoadGame()) { return; } break; 
            case 3 : if (Sys_Confirm("Really quit?", -1, -1, -1)) { Sys_Shutdown(); exit(0); } break; 
         } 
      } 
   } 
} 
 
 
int Sys_StatusSaveGame(void) 
{ 
   int sel = 1; 
   char fname[32]; 
   int booboo = 0; 
 
   FILE *sfile;
   int i; 
 
   for (i = 1; i <= 6; i++) 
   { 
      sprintf(fname, "save%i.sav", i); 
      sfile = fopen(fname, "rb");
      if (!sfile) 
      { 
         save[i].area = -1; 
         save[i].hearts = 0; 
         save[i].stars = 0; 
      } 
      else 
      { 
          fread(&save[i], sizeof(save_t), 1, sfile);
      } 
      fclose(sfile);
   } 
 
   while (TRUE) 
   { 
      Sys_KeyClear(); 
 
      if (booboo == 10) 
      { 
         booboo = 0; 
         poll_joystick(); 
      } 
      else 
         booboo++; 
 
      Screen_FillRect(100, 50, 221, 151, white); 
      Screen_FillRect(101, 51, 220, 150, blue); 
 
      Screen_DrawTextCenter("Save Game", 160, 55, white); 
    
      Screen_DrawText("Save 1", 111, 70, gray); 
      Screen_DrawText("Save 2", 111, 95, gray); 
      Screen_DrawText("Save 3", 111, 120, gray); 
    
      Screen_DrawText("Save 4", 161, 70, gray); 
      Screen_DrawText("Save 5", 161, 95, gray); 
      Screen_DrawText("Save 6", 161, 120, gray); 
 
      switch (sel) 
      { 
         case 1 : Screen_DrawText("Save 1", 111, 70, white); break; 
         case 2 : Screen_DrawText("Save 2", 111, 95, white); break; 
         case 3 : Screen_DrawText("Save 3", 111, 120, white); break; 
         case 4 : Screen_DrawText("Save 4", 161, 70, white); break; 
         case 5 : Screen_DrawText("Save 5", 161, 95, white); break; 
         case 6 : Screen_DrawText("Save 6", 161, 120, white); break; 
      } 
 
      Screen_CopyBuffer(); 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      if (key[KEY_UP] || joy_up) 
      { 
         sel--; 
         if (sel < 1) 
            sel = 6; 
      } 
      if (key[KEY_DOWN] || joy_down) 
      { 
         sel++; 
         if (sel > 6) 
            sel = 1; 
      } 
      if (key[KEY_RIGHT]) 
      { 
         if (sel <= 3) 
            sel = sel + 3; 
         else 
            sel = sel - 3; 
      } 
      if (key[KEY_LEFT]) 
      { 
         if (sel >= 4) 
            sel = sel - 3; 
         else 
            sel = sel + 3; 
      } 
      if (key[KEY_ENTER] || joy_b1) 
      { 
         if (Sys_Confirm("Overwrite slot?", save[sel].area, save[sel].hearts, save[sel].stars)) 
         { 
            switch (sel) 
            { 
               case 1 : Sys_SaveGame("save1.sav", 1); break; 
               case 2 : Sys_SaveGame("save2.sav", 2); break; 
               case 3 : Sys_SaveGame("save3.sav", 3); break; 
               case 4 : Sys_SaveGame("save4.sav", 4); break; 
               case 5 : Sys_SaveGame("save5.sav", 5); break; 
               case 6 : Sys_SaveGame("save6.sav", 6); break; 
            } 
 
            return 1; 
         } 
      } 
      if (key[KEY_ESC] || joy_b2) 
      { 
         return 0; 
      } 
 
   } 
} 

void Txt_Credit(const string& bleah) { 
   Screen_DrawTextCenter(bleah, 160*2, 96*2, white); 
} 
 
void Txt_Credit2(const string& bleah) { 
   Screen_DrawTextCenter(bleah, 160*2, 80*2, white); 
} 
 
void Txt_Credit3(const string& bleah) { 
   Screen_DrawTextCenter(bleah, 160*2, 120*2, white); 
} 

/*
    OLD VERSION
    
void Screen_CreditsScreen() 
{ 
   int xpos, ypos; 
   int tpos; 
   int fpos; 
   int spos; 
   //BITMAP *disco; 
 
   //disco = GFX("disco1.pcx"); 
   //clear(buffer); 
   //clear(screen);   
    Screen_ClearBuffer();
    Screen_CopyBuffer();
   //set_palette(pal); 
   xpos = -32; 
   ypos = 92; 
   fpos = 0; 
   tpos = 0; 
   spos = 0; 
 
   Sound_StopMidi();    
   Sound_PlayMidi("credits.mid");
   
   clear_keybuf(); 
   while (!keypressed()) 
   { 
       Screen_ClearBuffer();
      //clear(buffer); 
 
      if (tpos < 401) 
      { 
      xpos++; 
      tpos++; 
      fpos++; 
      if (fpos == 32) 
         fpos = 0; 
         
      Tile_DrawTile("disco1.pcx", fpos/8, xpos, ypos);
      //draw_sprite(buffer, (BITMAP *)disco[(int)fpos/8].dat, xpos, ypos); 
      } 
 
      if (tpos > 20 && tpos < 40) 
         Txt_Credit("Tilegame: The Adventures of Mike"); 
      if (tpos > 50 && tpos < 70) 
         Txt_Credit("A Magist Software Concoction"); 
 
      if (tpos > 90 && tpos < 110) 
         Txt_Credit("- Design - "); 
      if (tpos > 120 && tpos < 160) 
         Txt_Credit("WF & Vry"); 
 
      if (tpos > 180 && tpos < 200) 
         Txt_Credit("- Art Pimping -"); 
      if (tpos > 210 && tpos < 250) 
         Txt_Credit("Vrykolakas"); 
 
      if (tpos > 270 && tpos < 290) 
         Txt_Credit("- Code Monkeying -"); 
      if (tpos > 300 && tpos < 340) 
         Txt_Credit("Wildfire"); 
 
      if (tpos > 350 && tpos < 400) 
         Txt_Credit2("- And -"); 
      if (tpos > 360 && tpos < 400) 
         Txt_Credit3("MIKE"); 
 
      if (tpos > 370 && tpos < 400) 
         Tile_DrawTile("objects.pcx", OBJ_CLOTHES, 152, 100); 
 
      if (tpos > 400) 
         Txt_Credit("Thank you for playing Tilegame Test I."); 
 
      Screen_CopyBuffer();      
      rest(100); 
   }    
 
   Sound_StopMidi(); 
   Sys_Shutdown(); 
   exit(0); 
} 
*/

void Screen_CreditsScreen() 
{ 
   int xpos, ypos; 
   int tpos; 
   int fpos; 
   int spos; 
   //BITMAP *disco; 
 
   //disco = GFX("disco1.pcx"); 
   //clear(buffer); 
   //clear(screen);   
    Screen_ClearBuffer();
    Screen_CopyBuffer();
   //set_palette(pal); 
   xpos = -32*2; 
   ypos = 92*2; 
   fpos = 0; 
   tpos = 0; 
   spos = 0; 
 
   Sound_StopMidi();    
   Sound_PlayMidi("credits.mid");
   
   clear_keybuf(); 
   while (!keypressed()) 
   { 
       Screen_ClearBuffer();
      //clear(buffer); 
 
      if (tpos < 401*2) 
      { 
      xpos += 2; 
      tpos += 2; 
      fpos += 2; 
      if (fpos == 32*2) 
         fpos = 0; 
         
      Tile_DrawTile("disco1.pcx", fpos/16, xpos, ypos);
      //draw_sprite(buffer, (BITMAP *)disco[(int)fpos/8].dat, xpos, ypos); 
      } 
 
      if (tpos > 20*2 && tpos < 40*2) 
         Txt_Credit("Tilegame: The Adventures of Mike"); 
      if (tpos > 50*2 && tpos < 70*2) 
         Txt_Credit("A Magist Software Concoction"); 
 
      if (tpos > 90*2 && tpos < 110*2) 
         Txt_Credit("- Design - "); 
      if (tpos > 120*2 && tpos < 160*2) 
         Txt_Credit("WF & Vry"); 
 
      if (tpos > 180*2 && tpos < 200*2) 
         Txt_Credit("- Art Pimping -"); 
      if (tpos > 210*2 && tpos < 250*2) 
         Txt_Credit("Vrykolakas"); 
 
      if (tpos > 270*2 && tpos < 290*2) 
         Txt_Credit("- Code Monkeying -"); 
      if (tpos > 300*2 && tpos < 340*2) 
         Txt_Credit("Wildfire"); 
 
      if (tpos > 350*2 && tpos < 400*2) 
         Txt_Credit2("- And -"); 
      if (tpos > 360*2 && tpos < 400*2) 
         Txt_Credit3("MIKE"); 
 
      if (tpos > 370*2 && tpos < 400*2) 
         Tile_DrawTile("objects.pcx", OBJ_CLOTHES, 152*2, 100*2); 
 
      if (tpos > 400*2) 
         Txt_Credit("Thank you for playing Tilegame: The Adventures of Mike."); 
 
      Screen_CopyBuffer();      
      rest(100); 
   }    
 
   Sound_StopMidi(); 
   Sys_Shutdown(); 
   exit(0); 
} 

 
void Screen_TitleScreen(void) 
{ 
   //BITMAP *title; 
   //RGB *tpal; 
 
   //title = GFX("title.pcx"); 
   Sound_PlayMidi("title.mid");
   
   Screen_FadeIn();            
   
   Screen_ClearBuffer();

   Screen_DrawImage("title.pcx", 0, 0);
   //blit(title, buffer, 0, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
   
   Screen_DrawTextCenter("The Adventures of MIKE!", 320, 160, white);
      
   Screen_DrawTextCenter("(c) 1997, 2002 Magist Software", 320, 380, white);
   Screen_DrawTextCenter("Press Any Key", 320, 380+Screen_TextHeight()+5, white);
   
   //Screen_DrawTextCenter("(c) 1997, 2002 Magist Software", 160, 155, white); 
   //Screen_DrawTextCenter("Press Any Key", 160, 165, white); 
      
   Screen_CopyBuffer();
 
   clear_keybuf();    
   readkey(); 
   
   Screen_ClearBuffer();
   Screen_CopyBuffer();
      
    Screen_FadeOut();
          
   Sound_StopMidi(); 
} 

void Sys_CheckGameOver(void) 
{ 
   if (obj[0].type == -1) 
   { 
      //Screen_DrawFrame(); 
      //fade_out(1); 
      Screen_FadeOut();
      //fade_in(pal, 64); 
      
      Screen_ClearBuffer();
      Screen_DrawTextCenter("Alas, our hero Mike has failed", 160, 100, white); 
      Screen_DrawTextCenter("in his quest. All is lost.", 160, 110, white); 
      Screen_CopyBuffer();
      
      Screen_FadeIn();      
      
      while (!key[KEY_ESC]) 
      {  
      } 
      gameover = TRUE; 
   } 
} 

void Think_RunThinks();
void Think_RunActions();
void Screen_DrawFrame();
void Sys_CheckGameOver();
 
int main(int argc, char *argv[]) 
{ 
   Sys_Initialize(); 
 
   Screen_TitleScreen(); 
   //Sys_GameMenu(); 
   Sys_NewGame();
 
   Screen_DrawFrame(); 
 
   gameover = 0; 
 
   while (!gameover) {
      Game_Frame();      
   } 
 
   Sys_Shutdown();

   return 0;
} 
END_OF_MAIN();

/*
===============
Game Code
===============
*/ 

void Map_LoadPortals(void) 
{ 
   FILE *pfile; 
   int i; 
 
   pfile = fopen("portals.tgm", "rb"); 
 
   for (i = 0; i < 6; i++) 
   { 
      fscanf(pfile, "%s", &portal[i].mapfile); 
      fscanf(pfile, "%i", &portal[i].map_x); 
      fscanf(pfile, "%i", &portal[i].map_y); 
      fscanf(pfile, "%i", &portal[i].pos_x); 
      fscanf(pfile, "%i", &portal[i].pos_y); 
      fscanf(pfile, "%i", &portal[i].facing); 
   } 
 
 
   fclose(pfile); 
} 
 
void Map_Teleport(portal_t *dest) 
{ 
   Sound_StopMidi(); 
 
   Map_LoadMap(dest->mapfile); 
   world.map_x = dest->map_x; 
   world.map_y = dest->map_y; 
   obj[0].pos_x = dest->pos_x; 
   obj[0].pos_y = dest->pos_y; 
   obj[0].facing = dest->facing; 
 
   nextobj = 1; 
   Obj_LoadObjects(); 
 
    Sound_PlayMidi("map_l1.mid");   
} 

void Obj_SetupPlayer(obj_t *ent) 
{ 
   ent->type = 1; 
   ent->sprite = "mike4.pcx"; 
   ent->weapon = "weapon1.pcx"; 
   ent->missile = ""; 
   ent->speed = 2; 
   ent->life = 8; 
   ent->max_life = 8; 
   ent->mana = 8; 
   ent->max_mana = 8; 
   ent->att = 1; 
   ent->def = 1; 
   ent->wpn = WPN_DAGGER; 
   ent->amr = AMR_CLOTHES; 
   ent->mag = MAG_FIRE; 
   ent->spell = MAG_FIRE; 
   ent->gold = 0;
   ent->snd_death = "plyrdeth.wav";
   ent->snd_pain = "";
   ent->snd_attack = "weapon1.wav";
} 
 
void Obj_SetupDouble(obj_t *ent) 
{ 
   ent->sprite = "mike4.pcx"; 
   ent->missile = ""; 
   ent->speed = 2; 
   ent->life = 4; 
   ent->att = 2; 
   ent->def = 1;
   ent->snd_death = "plyrdeth.wav";
   ent->snd_pain = "";
   ent->snd_attack = "weapon1.wav";
} 
 
void Obj_SetupBat(obj_t *ent) 
{ 
   ent->sprite = "bat.pcx"; 
   ent->weapon = ""; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 1; 
   ent->att = 1; 
   ent->def = 1; 
   ent->trans = 0;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "";
} 
 
void Obj_SetupOrc(obj_t *ent) 
{ 
   ent->sprite = "orc2.pcx"; 
   ent->weapon = "spear.pcx"; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 2; 
   ent->att = 1; 
   ent->def = 1; 
   ent->trans = 0;
   ent->snd_death = "gbordeth.wav";
   ent->snd_pain = "gborpain.wav";
   ent->snd_attack = "gborwpn2.wav";
} 
 
void Obj_SetupGoblin(obj_t *ent) 
{ 
   ent->sprite = "goblin2.pcx"; 
   ent->weapon = ""; 
   ent->missile = "spear.pcx"; 
   ent->speed = 1; 
   ent->life = 1; 
   ent->att = 1; 
   ent->def = 1; 
   ent->msl_speed = 2; 
   ent->trans = 0;
   ent->snd_death = "gbordeth.wav";
   ent->snd_pain = "gborpain.wav";
   ent->snd_attack = "gborwpn1.wav";
}
 
void Obj_SetupKnight(obj_t *ent) 
{ 
   ent->sprite = "knight.pcx"; 
   ent->weapon = "weapon3.pcx"; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 4; 
   ent->att = 3; 
   ent->def = 2; 
   ent->trans = 0;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "";
} 
 
void Obj_SetupSlime(obj_t *ent) 
{ 
   ent->sprite = "crazeeblobbie.pcx"; 
   ent->weapon = ""; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 1; 
   ent->att = 1; 
   ent->def = 1; 
   ent->trans = 1;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "";
} 
 
void Obj_SetupOoze(obj_t *ent) 
{ 
   ent->sprite = "crazeeblobbie2.pcx"; 
   ent->weapon = ""; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 4; 
   ent->att = 2; 
   ent->def = 2; 
   ent->trans = 1;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "";
} 
 
void Obj_SetupGhost(obj_t *ent) 
{ 
   ent->sprite = "ghost.pcx"; 
   ent->weapon = ""; 
   ent->missile = ""; 
   ent->speed = 2; 
   ent->life = 2; 
   ent->att = 1; 
   ent->def = 1; 
   ent->trans = 1;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "";
} 
 
void Obj_Erase(int objnum) 
{ 
   memset(&obj[objnum], 0, sizeof(obj_t)); 
} 
 
void Obj_SetupDarkElf(obj_t *ent) 
{ 
   ent->sprite = "archer2.pcx"; 
   ent->weapon = ""; 
   ent->missile = "arrow.pcx"; 
   ent->speed = 1; 
   ent->life = 1; 
   ent->att = 1; 
   ent->def = 1; 
   ent->msl_speed = 4;
   ent->snd_death = "";
   ent->snd_pain = "";
   ent->snd_attack = "gborwpn2.wav";
}
 
void Obj_SetupMinotaur(obj_t *ent) 
{ 
   int i, tgt = 0; 
   int num = 0; 
 
   if (obj[0].crystals & CRYSTAL_HEART_A) 
   { 
      ent->type = 0; 
      return; 
   } 
 
   ent->sprite = "minib.pcx"; 
   ent->weapon = ""; 
   ent->missile = ""; 
   ent->speed = 1; 
   ent->life = 6; 
   ent->att = 2; 
   ent->def = 2;
   ent->snd_death = "mntrdeth.wav";
   ent->snd_pain = "mntrpain.wav";
   ent->snd_attack = "mntrclub.wav";
 
   for (i = 0; i < MAXOBJ; i++) 
   { 
      if (obj[i].type == OBJ_MTAUR_TOP) 
         tgt = i; 
      if (&obj[i] == ent) 
         num = i; 
   } 
 
   if (tgt != 0) 
   { 
      obj[tgt].sprite = "minit.pcx"; 
      obj[tgt].weapon = "club.pcx"; 
      obj[tgt].missile = ""; 
      obj[tgt].pos_x = ent->pos_x; 
      obj[tgt].pos_y = ent->pos_y - 1; 
      obj[tgt].dumb = num; 
   } 
} 
 
void Obj_SetupV(obj_t *ent) 
{ 
   ent->sprite = "vrypire.pcx"; 
   ent->weapon = ""; 
   ent->missile = "bleah.pcx";
   ent->msl_speed = 4;
   ent->speed = 2; 
   ent->life = 6; 
   ent->att = 2; 
   ent->def = 2;
   ent->snd_death = "";
} 
 
void Obj_SetupObject(int objnum) 
{ 
   obj_t *ent; 
 
   ent = &obj[objnum]; 
 
   switch (ent->type) 
   { 
      case (OBJ_PLAYER): Obj_SetupPlayer(ent); break; 
      case (OBJ_SLIME): Obj_SetupSlime(ent); break; 
      case (OBJ_DARKELF): Obj_SetupDarkElf(ent); break; 
      case (OBJ_BAT): Obj_SetupBat(ent); break; 
      case (OBJ_GOBLIN): Obj_SetupGoblin(ent); break; 
      case (OBJ_ORC): Obj_SetupOrc(ent); break; 
      case (OBJ_OOZE): Obj_SetupOoze(ent); break; 
      case (OBJ_KNIGHT): Obj_SetupKnight(ent); break; 
      case (OBJ_GHOST): Obj_SetupGhost(ent); break; 
 
      case (OBJ_DOUBLE): Obj_SetupDouble(ent); break; 
      case (OBJ_MINOTAUR): Obj_SetupMinotaur(ent); break; 
 
      case (OBJ_V): Obj_SetupV(ent); break; 
 
      default: break; 
   } 
} 
 
void Obj_AddObject(int objtype, int x, int y) 
{ 
   if (nextobj == MAXOBJ) 
   { 
      Sys_Shutdown(); 
      allegro_message("Error: No free edicts\n"); 
      exit(0); 
   } 
 
   Obj_Erase(nextobj); 
    
   obj[nextobj].type = objtype; 
   obj[nextobj].pos_x = x; 
   obj[nextobj].pos_y = y; 
 
   Obj_SetupObject(nextobj); 
 
   nextobj++; 
} 

void Tile_TileCoords(int *x, int *y)
{
    if (!x || !y)
        return;

    *x = *x * TILE_SIZE;
    *y = (*y * TILE_SIZE) + SCREEN_BORDER;
}

void Tile_SpriteCoords(int *x, int *y, int face, int offset)
{
    int painDir = 0;
    
    if (!x || !y)
        return;
        
    Tile_TileCoords(x, y);
    
    if (face < 10) 
   { 
      if (face == FACE_BACK) 
         *y += offset; 
      else if (face == FACE_FRONT) 
         *y -= offset; 
      else if (face == FACE_LEFT) 
         *x += offset; 
      else if (face == FACE_RIGHT) 
         *x -= offset; 
   } 
   else if (face < 20) 
      painDir = 10; 
   else if (face < 30) 
      painDir = 20; 
   else if (face < 40) 
      painDir = 30; 
   else if (face < 50) 
      painDir = 40; 
 
   if (painDir != 0) 
   { 
      if (painDir == PAIN_BACK) 
         *y += offset; 
      if (painDir == PAIN_FRONT) 
         *y -= offset; 
      if (painDir == PAIN_LEFT) 
         *x += offset; 
      if (painDir == PAIN_RIGHT) 
         *x -= offset; 
   } 
}   

void Tile_DrawBase(void) 
{ 
   int x, y, c; 
 
   for (x = 0; x < 20; x++) 
   { 
      for (y = 0; y < 10; y++) 
      { 
          int u = x;
          int v = y;
          
         c = world.map[world.map_x][world.map_y].base[x][y];
         
         Tile_TileCoords(&u, &v);
         Tile_DrawTile("ts_base.pcx", c, u, v); 
      } 
   } 
} 
 
 
void Tile_DrawFringe(void) 
{ 
   int x, y, c; 
 
   for (x = 0; x < 20; x++) 
   { 
      for (y = 0; y < 10; y++) 
      { 
          int u = x;
          int v = y;
          
         c = world.map[world.map_x][world.map_y].fringe[x][y]; 
         
         Tile_TileCoords(&u, &v);
         if (c > 0 && c < 224) 
            Tile_DrawTile("ts_frng.pcx", c, u, v); 
         else if (c >= 224) 
            Tile_DrawTransTile("ts_frng.pcx", c, u, v); 
      } 
   } 
} 
 
void Tile_DrawOcclude(void) 
{ 
   int x, y, c; 
 
   for (x = 0; x < 20; x++) 
   { 
      for (y = 0; y < 10; y++) 
      { 
          int u = x;
          int v = y;
          
         c = world.map[world.map_x][world.map_y].occlude[x][y]; 
         
         Tile_TileCoords(&u, &v);
         if (c > 0 && c < 192) 
            Tile_DrawTile("ts_occl.pcx", c, u, v); 
         else if (c >= 192) 
            Tile_DrawTransTile("ts_occl.pcx", c, u, v); 
      } 
   } 
} 
 
int Map_RoomColor(int wx, int wy)
{
    int r = 0, g = 0, b = 0;
    
    for (int tx = 0; tx < 20; tx++)
    {
        for (int ty = 0; ty < 10; ty++)
        {
            //int color;
            
            if (world.map[wx][wy].base[tx][ty] != 0)
                return gray;
        }
    }
    
    return dkgray;
}       

void Map_CreateMiniMap()
{
    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
            minimap[x][y] = Map_RoomColor(x, y);
}       
 
void Map_LoadMap(const string& fname) 
{ 
   int x, y; 
 
   FILE *mfile;
 
   mfile = fopen(fname.c_str(), "rb");
    
   if (!mfile) 
   { 
      Sys_Shutdown(); 
      allegro_message("Unable to open file %s for reading\n", fname.c_str()); 
      exit(1); 
   } 
 
   fread(&world, sizeof(world), 1, mfile);
   fclose(mfile);
   
   Map_CreateMiniMap();
} 

void Obj_LoadObjects(void) 
{ 
   int i; 
 
   obj[0].msl = 0; 
 
   nextobj = 1; 
 
   for (i = nextobj; i < MAXOBJ; i++) 
   { 
      obj[i].msl = 0; 
      Obj_AddObject(world.map[world.map_x][world.map_y].obj[i][1], world.map[world.map_x][world.map_y].obj[i][2], world.map[world.map_x][world.map_y].obj[i][3]); 
   } 
} 
 
void Sprite_DrawObjs(void) 
{ 
   int i, x, y, f; 
    
   for (i = 0; i < nextobj; i++) 
   { 
      if (obj[i].type < 0) 
      {
         x = obj[i].pos_x; 
         y = obj[i].pos_y;

         Tile_SpriteCoords(&x, &y, 0, 0);
         Tile_DrawTile(obj[i].sprite, FACE_DEAD, x, y); 
      } 
      if (obj[i].type >= 100 && (obj[i].type < 200 || obj[i].type > 240))  
      {
          x = obj[i].pos_x; 
         y = obj[i].pos_y; 
 
         Tile_SpriteCoords(&x, &y, 0, 0);
         Tile_DrawTile("objects.pcx", obj[i].type, x, y);        
      } 
   } 
 
   for (i = 0; i < nextobj; i++) 
   {
      if (obj[i].type < 128 && obj[i].type > 0) 
      { 
         x = obj[i].pos_x; 
         y = obj[i].pos_y; 
 
            f = obj[i].facing; 
 
            if (f >= 40) 
               f -= 40; 
            else if (f >= 30) 
               f -= 30; 
            else if (f >= 20) 
               f -= 20; 
            else if (f >= 10) 
               f -= 10; 

            Tile_SpriteCoords(&x, &y, obj[i].facing, obj[i].offset);

            if (obj[i].trans) 
                Tile_DrawTransTile(obj[i].sprite, obj[i].frame, x, y); 
            else 
                Tile_DrawTile(obj[i].sprite, obj[i].frame, x, y);
      } 
   } 
 
   for (i = 0; i < nextobj; i++) 
   { 
      if (obj[i].msl) 
      {
         x = obj[i].msl_pos_x;
         y = obj[i].msl_pos_y;
                 
            Tile_SpriteCoords(&x, &y, obj[i].msl_facing, obj[i].msl_offset);
         if (obj[i].msl_trans) 
            Tile_DrawTransTile(obj[i].missile, obj[i].msl_facing/3, x, y); 
            else 
            Tile_DrawTile(obj[i].missile, obj[i].msl_facing/3, x, y); 
      } 
 
      if (obj[i].attack > 0 && obj[i].attack < 16) 
      { 
         if (obj[i].weapon != "") 
         {
            f = obj[i].facing; 
 
            if (f >= 40) 
               f -= 40; 
            else if (f >= 30) 
               f -= 30; 
            else if (f >= 20) 
               f -= 20; 
            else if (f >= 10) 
               f -= 10; 
 
            x = obj[i].pos_x; 
            y = obj[i].pos_y; 
 
            if (f == FACE_FRONT) 
               y++; 
            if (f == FACE_BACK) 
               y--; 
            if (f == FACE_LEFT) 
               x--; 
            if (f == FACE_RIGHT) 
               x++; 
            
            if (y >= 0 && y < 10) 
            {
                Tile_SpriteCoords(&x, &y, 0, 0);
               Tile_DrawTile(obj[i].weapon, f/3, x, y); 
            }
         } 
      } 
   } 
}

void Txt_Message(const string& msg)
{
    messagetext = msg;
   messagetime = 100;
}

void Screen_HLine(int x1, int x2, int y, int color)
{
    Screen_FillRect(x1, y, x2, y, color);
}

void Screen_VLine(int x, int y1, int y2, int color)
{
    Screen_FillRect(x, y1, x, y2, color);
}

void Screen_DrawWindow(int x1, int y1, int x2, int y2, int color)
{
    // dkgray border (outer)
    Screen_HLine(x1+2, x2-2, y1+0, dkgray);
    Screen_HLine(x1+2, x2-2, y2-0, dkgray);
    Screen_VLine(x1+0, y1+2, y2-2, dkgray);
    Screen_VLine(x2-0, y1+2, y2-2, dkgray);
    
    // gray border (outer)
    Screen_HLine(x1+2, x2-2, y1+1, gray);
    Screen_HLine(x1+2, x2-2, y2-1, gray);
    Screen_VLine(x1+1, y1+2, y2-2, gray);
    Screen_VLine(x2-1, y1+2, y2-2, gray);
    
    // ltgray border (outer)
    Screen_DrawRect(x1+2, y1+2, x2-2, y2-2, ltgray);
    
    // white borders
    Screen_DrawRect(x1+3, y1+3, x2-3, y2-3, white);
    Screen_DrawRect(x1+4, y1+4, x2-4, y2-4, white);
    
    // ltgray border (inner)
    Screen_DrawRect(x1+5, y1+5, x2-5, y2-5, ltgray);
    
    // gray border (inner)
    Screen_DrawRect(x1+6, y1+6, x2-6, y2-6, gray);
    
    // dkgray border (inner)
    Screen_DrawRect(x1+7, y1+7, x2-7, y2-7, dkgray);
    
    // window contents
    Screen_FillRect(x1+8, y1+8, x2-8, y2-8, color);
}

void Screen_DrawConsole(void)
{
   if (messagetime > 0)
   {
       Screen_DrawText(messagetext, 4, 4, white);      
      messagetime--;
   }

   if (console)
   {
      int y = (SCREEN_BORDER - 4 - Screen_TextHeight());
      Screen_DrawText(consoletext, 4, y, white);      
   }
}

void Screen_DrawLifeAndManaBars()
{    
   int lifeTextStartX = 16;
   int manaTextStartX = 16;
   
   int lifeBarStartX = lifeTextStartX + Screen_TextWidth("LIFE") + 8;
   int manaBarStartX = manaTextStartX + Screen_TextWidth("MANA") + 8;
   
   #if 0
   int lifeBarMidPointY = (SCREEN_BORDER/3)*1;
   int manaBarMidPointY = (SCREEN_BORDER/3)*2;
   #endif
   
   #if 1
   int lifeBarMidPointY = SCREEN_HEIGHT - (SCREEN_BORDER/3)*2;
   int manaBarMidPointY = SCREEN_HEIGHT - (SCREEN_BORDER/3)*1;
   #endif
   
   int lifeTextStartY = lifeBarMidPointY - Screen_TextHeight()/2;
   int manaTextStartY = manaBarMidPointY - Screen_TextHeight()/2;   
   
   int lifeBarWidth = obj[0].life * 10; 
   int maxLifeBarWidth = obj[0].max_life * 10; 

   int manaBarWidth = obj[0].mana * 10; 
   int maxManaBarWidth = obj[0].max_mana * 10; 
   
   int lifeBarHeight = 12;
   int manaBarHeight = 12;      
   
   int lifeBarStartY = lifeBarMidPointY - (lifeBarHeight/2);
   int manaBarStartY = manaBarMidPointY - (manaBarHeight/2);   
   
   Screen_DrawText("LIFE", lifeTextStartX, lifeTextStartY, white);      
   Screen_FillRect(lifeBarStartX, lifeBarStartY, lifeBarStartX + maxLifeBarWidth, lifeBarStartY + lifeBarHeight, white);

   if (obj[0].max_life > 0)
        Screen_FillRect(lifeBarStartX+2, lifeBarStartY+2, lifeBarStartX + maxLifeBarWidth - 2, lifeBarStartY + lifeBarHeight - 2, black);
    if (obj[0].life > 0)
        Screen_FillRect(lifeBarStartX+2, lifeBarStartY+2, lifeBarStartX + lifeBarWidth - 2, lifeBarStartY + lifeBarHeight - 2, red);  
    
    for (int lb = lifeBarStartX+20; lb < lifeBarStartX+maxLifeBarWidth; lb += 20)
        Screen_FillRect(lb-1, lifeBarStartY+2, lb-1, lifeBarStartY+lifeBarHeight-2, gray);
   
    Screen_DrawText("MANA", manaTextStartX, manaTextStartY, white); 
   Screen_FillRect(manaBarStartX, manaBarStartY, manaBarStartX + maxManaBarWidth, manaBarStartY + manaBarHeight, white);
   
   if (obj[0].max_mana > 0)
        Screen_FillRect(manaBarStartX+2, manaBarStartY+2, manaBarStartX + maxManaBarWidth - 2, manaBarStartY + manaBarHeight - 2, black);
    if (obj[0].mana > 0)
        Screen_FillRect(manaBarStartX+2, manaBarStartY+2, manaBarStartX + manaBarWidth - 2, manaBarStartY + manaBarHeight - 2, ltblue);  
    
    for (int mb = manaBarStartX+20; mb < manaBarStartX+maxManaBarWidth; mb += 20)
        Screen_FillRect(mb-1, manaBarStartY+2, mb-1, manaBarStartY+manaBarHeight-2, gray);   
}

void Screen_DrawWeaponArmorMagicIcons()
{
    int weaponIconStartX = (576-Screen_TextWidth("Gold")) - 48 - 48 - 48 - 4; // - 390;
    int armorIconStartX = (576-Screen_TextWidth("Gold")) - 48 - 48 - 4; // 440;
    int spellIconStartX = (576-Screen_TextWidth("Gold")) - 48 - 4; // 488;
        
    int weaponIconEndX = weaponIconStartX + TILE_SIZE;
    int armorIconEndX = armorIconStartX + TILE_SIZE;
    int spellIconEndX = spellIconStartX + TILE_SIZE;
    
    int midPointY = SCREEN_HEIGHT - SCREEN_BORDER / 2;
    
    int weaponTextStartX = weaponIconStartX + TILE_SIZE + 12;
    int armorTextStartX = armorIconStartX + TILE_SIZE + 12;
    int spellTextStartX = spellIconStartX + TILE_SIZE + 12;
    
    int weaponIconStartY = midPointY - TILE_SIZE/2;
    int armorIconStartY = midPointY - TILE_SIZE/2;
    int spellIconStartY = midPointY - TILE_SIZE/2;
    
    int weaponTextStartY = midPointY - Screen_TextHeight()/2;
    int armorTextStartY = midPointY - Screen_TextHeight()/2;
    int spellTextStartY = midPointY - Screen_TextHeight()/2;

    int weaponIconEndY = weaponIconStartY + TILE_SIZE;      
    int armorIconEndY = armorIconStartY + TILE_SIZE;        
    int spellIconEndY = spellIconStartY + TILE_SIZE;        
        
    Screen_DrawRect(weaponIconStartX-4, weaponIconStartY-4, weaponIconEndX+4, weaponIconEndY+4, white);    
 
   switch (obj[0].att) 
   { 
      case 1 : Tile_DrawTile("objects.pcx", OBJ_DAGGER, weaponIconStartX, weaponIconStartY); /*textout(buffer, mfont, "Dagger",  weaponTextStartX, weaponTextStartY, white);*/ break; 
      case 2 : Tile_DrawTile("objects.pcx", OBJ_SHORTSWD, weaponIconStartX, weaponIconStartY); /*textout(buffer, mfont, "Short Sword",  weaponTextStartX, weaponTextStartY, white);*/ break;
      case 3 : Tile_DrawTile("objects.pcx", OBJ_LONGSWD, weaponIconStartX, weaponIconStartY); /*textout(buffer, mfont,  "Long Sword", weaponTextStartX, weaponTextStartY, white);*/ break;
      case 4 : Tile_DrawTile("objects.pcx", OBJ_GREATSWD, weaponIconStartX, weaponIconStartY); /*textout(buffer, mfont, "Great Sword", weaponTextStartX, weaponTextStartY, white);*/ break;
      case 5 : Tile_DrawTile("objects.pcx", OBJ_MITHRILSWD, weaponIconStartX, weaponIconStartY); /*textout(buffer, mfont, "Mithril Sword", weaponTextStartX, weaponTextStartY, white);*/ break;
   } 
   
    Screen_DrawRect(armorIconStartX-4, armorIconStartY-4, armorIconEndX+4, armorIconEndY+4, white);    
 
   switch (obj[0].def) 
   { 
      case 1 : Tile_DrawTile("objects.pcx", OBJ_CLOTHES, armorIconStartX, armorIconStartY); /*textout(buffer, mfont, "Clothes",  armorTextStartX, armorTextStartY, white);*/ break; 
      case 2 : Tile_DrawTile("objects.pcx", OBJ_LEATHER, armorIconStartX, armorIconStartY); /*textout(buffer, mfont, "Leather Armor",  armorTextStartX, armorTextStartY, white);*/ break;
      case 3 : Tile_DrawTile("objects.pcx", OBJ_CHAIN, armorIconStartX, armorIconStartY); /*textout(buffer, mfont,  "Chain Mail", armorTextStartX, armorTextStartY, white);*/ break;
      case 4 : Tile_DrawTile("objects.pcx", OBJ_PLATE, armorIconStartX, armorIconStartY); /*textout(buffer, mfont, "Plate Armor", armorTextStartX, armorTextStartY, white);*/ break;
      case 5 : Tile_DrawTile("objects.pcx", OBJ_DRAGON, armorIconStartX, armorIconStartY); /*textout(buffer, mfont, "Dragon Scale", armorTextStartX, armorTextStartY, white);*/ break;
   } 
   
   Screen_DrawRect(spellIconStartX-4, spellIconStartY-4, spellIconEndX+4, spellIconEndY+4, white);    
 
   switch (obj[0].spell) 
   { 
      case 1 : Tile_DrawTile("objects.pcx", OBJ_FIRESPL, spellIconStartX, spellIconStartY); /*textout(buffer, mfont, "Fire", spellTextStartX, spellTextStartY, white);*/ break; 
      case 2 : Tile_DrawTile("objects.pcx", OBJ_STUNSPL, spellIconStartX, spellIconStartY); /*textout(buffer, mfont, "Stun", spellTextStartX, spellTextStartY, white);*/ break;
      case 3 : Tile_DrawTile("objects.pcx", OBJ_CURESPL, spellIconStartX, spellIconStartY); /*textout(buffer, mfont, "Cure", spellTextStartX, spellTextStartY, white);*/ break;
      case 4 : Tile_DrawTile("objects.pcx", OBJ_WARPSPL, spellIconStartX, spellIconStartY); /*textout(buffer, mfont, "Warp", spellTextStartX, spellTextStartY, white);*/ break;
      case 5 : Tile_DrawTile("objects.pcx", OBJ_MANASPL, spellIconStartX, spellIconStartY); /*textout(buffer, mfont, "Mana", spellTextStartX, spellTextStartY, white);*/ break;
   }    
}

void Screen_DrawGoldCount()
{
    
    int midPointY = SCREEN_HEIGHT - (SCREEN_BORDER/2);
    int textStartX = 576 - Screen_TextWidth("Gold") / 2;
    
    int labelTextStartY = midPointY - 4 - Screen_TextHeight();
    int countTextStartY = midPointY + 4;
    
    ostringstream out;
    out << obj[0].gold;   
   
    Screen_DrawTextCenter("Gold", textStartX, labelTextStartY, white);
    Screen_DrawTextCenter(out.str(), textStartX, countTextStartY, white);   
}

void Screen_DrawStatus(void) 
{ 
   int h, t, o; 
   char gpstr[255]; 
   
   Screen_DrawWindow(0, 0, SCREEN_WIDTH, SCREEN_BORDER-1, blue);
   Screen_DrawWindow(0, SCREEN_HEIGHT-SCREEN_BORDER+1, SCREEN_WIDTH, SCREEN_HEIGHT, blue);

   Screen_DrawLifeAndManaBars();       
   Screen_DrawWeaponArmorMagicIcons();   
   Screen_DrawGoldCount();   
} 
 
void Screen_DrawWorld(void) 
{ 
   int x, y; 
 
   int midPointY = SCREEN_HEIGHT - (SCREEN_BORDER/2);
   
   int startX = SCREEN_WIDTH - (10*4) - 8 - 4;
   int startY = midPointY - (10*4)/2-1;
   
   for (x = 0; x < 10; x++) 
   {
      for (y = 0; y < 10; y++) 
      { 
          int x1 = startX + (x*4);
          int x2 = x1 + 4;
          int y1 = startY + (y*4);
          int y2 = y1 + 4;
          
         if (world.map_x == x && world.map_y == y) 
            Screen_FillRect(x1, y1, x2, y2, red);          
         else
            Screen_FillRect(x1, y1, x2, y2, minimap[x][y]);
             
         Screen_DrawRect(x1, y1, x2, y2, dkgray); 
      } 
    }
} 
 
void Screen_DrawFrame (void) 
{ 
   Screen_ClearBuffer(); 
    
   Tile_DrawBase(); 
   
   Sprite_DrawObjs(); 

   Tile_DrawFringe(); 
   
   Tile_DrawOcclude(); 
   
   Screen_DrawStatus(); 
   
   Screen_DrawWorld();
   
   Screen_DrawConsole();   

   // fixme: remove?   
   vsync(); 
   
   Screen_CopyBuffer(); 
} 
 
void Txt_WriteLevelTitle(void) 
{ 
   int x = 5; 
 
   Screen_DrawText("Mike - ", x, 10, white);
   x += Screen_TextWidth("Mike - "); 
 
   if (obj[0].max_life >= 16 || obj[0].max_mana >= 16) 
      Screen_DrawText("5th Level ", x, 10, white); 
   else if (obj[0].max_life >= 14 || obj[0].max_mana >= 14) 
      Screen_DrawText("4th Level ", x, 10, white); 
   else if (obj[0].max_life >= 12 || obj[0].max_mana >= 12) 
      Screen_DrawText("3rd Level ", x, 10, white); 
   else if (obj[0].max_life >= 10 || obj[0].max_mana >= 10) 
      Screen_DrawText("2nd Level ", x, 10, white); 
   else 
      Screen_DrawText("1st Level ", x, 10, white); 
 
   x += Screen_TextWidth("1st Level "); 
 
 
   switch(obj[0].max_life) 
   { 
      case 8: 
         Screen_DrawText("Warrior ", x, 10, white); 
         x += Screen_TextWidth("Warrior "); 
         break; 
      case 10: 
         Screen_DrawText("Champion ", x, 10, white); 
         x += Screen_TextWidth("Champion "); 
         break; 
      case 12: 
         Screen_DrawText("Knight ", x, 10, white); 
         x += Screen_TextWidth("Knight "); 
         break; 
      case 14: 
         Screen_DrawText("Lord ", x, 10, white); 
         x += Screen_TextWidth("Lord "); 
         break; 
      case 16: 
         Screen_DrawText("Hero ", x, 10, white); 
         x += Screen_TextWidth("Hero "); 
         break; 
      default: 
         return; 
   } 
 
   switch(obj[0].max_mana) 
   { 
      case 8: 
         Screen_DrawText("Magician", x, 10, white); 
         break; 
      case 10: 
         Screen_DrawText("Sorcerer", x, 10, white); 
         break; 
      case 12: 
         Screen_DrawText("Conjurer", x, 10, white); 
         break; 
      case 14: 
         Screen_DrawText("Wizard", x, 10, white); 
         break; 
      case 16: 
         Screen_DrawText("Archmage", x, 10, white); 
         break; 
      default: 
         return; 
   } 
} 
 
void Screen_DrawWeapons(int func) 
{ 
   int col; 
 
   /*
   if (obj[0].wpn & WPN_DAGGER) 
      Tile_DrawTile("objects.pcx", OBJ_DAGGER, 6, 52); 
   if (obj[0].wpn & WPN_SHORT) 
      Tile_DrawTile("objects.pcx", OBJ_SHORTSWD, 26, 52); 
   if (obj[0].wpn & WPN_LONG) 
      Tile_DrawTile("objects.pcx", OBJ_LONGSWD, 46, 52); 
   if (obj[0].wpn & WPN_GREAT) 
      Tile_DrawTile("objects.pcx", OBJ_GREATSWD, 66, 52); 
   if (obj[0].wpn & WPN_MITHRIL) 
      Tile_DrawTile("objects.pcx", OBJ_MITHRILSWD, 86, 52); 
   */
   
   int ox = 32 + (570 - 320) / 2;
   int oy = 32 + (416 - 200) / 2;
   
   if (obj[0].wpn & WPN_DAGGER) 
      Tile_DrawTile("objects.pcx", OBJ_DAGGER, 6+ox, 52+oy); 
   if (obj[0].wpn & WPN_SHORT) 
      Tile_DrawTile("objects.pcx", OBJ_SHORTSWD, 26+ox, 52+oy); 
   if (obj[0].wpn & WPN_LONG) 
      Tile_DrawTile("objects.pcx", OBJ_LONGSWD, 46+ox, 52+oy); 
   if (obj[0].wpn & WPN_GREAT) 
      Tile_DrawTile("objects.pcx", OBJ_GREATSWD, 66+ox, 52+oy); 
   if (obj[0].wpn & WPN_MITHRIL) 
      Tile_DrawTile("objects.pcx", OBJ_MITHRILSWD, 86+ox, 52+oy); 

 
   if (func == 1) 
      col = white; 
   else 
      col = gray; 
 
   switch(obj[0].att) 
   { 
      /*
      case 1 : Screen_DrawRect(5, 51, 5+TILE_SIZE, 51+TILE_SIZE, col); Screen_DrawText("Dagger", 105, 58, white); break; 
      case 2 : Screen_DrawRect(25, 51, 25+TILE_SIZE, 51+TILE_SIZE, col); Screen_DrawText("Short", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 3 : Screen_DrawRect(45, 51, 45+TILE_SIZE, 51+TILE_SIZE, col); Screen_DrawText("Long", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 4 : Screen_DrawRect(65, 51, 65+TILE_SIZE, 51+TILE_SIZE, col); Screen_DrawText("Great", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 5 : Screen_DrawRect(85, 51, 85+TILE_SIZE, 51+TILE_SIZE, col); Screen_DrawText("Mithril", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      */
      case 1 : Screen_DrawRect(5+ox, 51+oy, 5+ox+TILE_SIZE, 51+oy+TILE_SIZE, col); Screen_DrawText("Dagger", 105, 58, white); break; 
      case 2 : Screen_DrawRect(25+ox, 51+oy, 25+ox+TILE_SIZE, 51+oy+TILE_SIZE, col); Screen_DrawText("Short", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 3 : Screen_DrawRect(45+ox, 51+oy, 45+ox+TILE_SIZE, 51+oy+TILE_SIZE, col); Screen_DrawText("Long", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 4 : Screen_DrawRect(65+ox, 51+oy, 65+ox+TILE_SIZE, 51+oy+TILE_SIZE, col); Screen_DrawText("Great", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 
      case 5 : Screen_DrawRect(85+ox, 51+oy, 85+ox+TILE_SIZE, 51+oy+TILE_SIZE, col); Screen_DrawText("Mithril", 105, 52, white); Screen_DrawText("Sword", 105, 60, white); break; 

   } 
} 
 
void Screen_DrawArmors(int func) 
{ 
   int col; 
 
   if (obj[0].amr & AMR_CLOTHES) 
      Tile_DrawTile("objects.pcx", OBJ_CLOTHES, 6, 72); 
   if (obj[0].amr & AMR_LEATHER) 
      Tile_DrawTile("objects.pcx", OBJ_LEATHER, 26, 72); 
   if (obj[0].amr & AMR_CHAIN) 
      Tile_DrawTile("objects.pcx", OBJ_CHAIN, 46, 72); 
   if (obj[0].amr & AMR_PLATE) 
      Tile_DrawTile("objects.pcx", OBJ_PLATE, 66, 72); 
   if (obj[0].amr & AMR_DRAGON) 
      Tile_DrawTile("objects.pcx", OBJ_DRAGON, 86, 72); 
 
   if (func == 2) 
      col = white; 
   else 
      col = gray; 
 
   switch(obj[0].def) 
   { 
      case 1 : Screen_DrawRect(5, 71, 5+TILE_SIZE, 71+TILE_SIZE, col); Screen_DrawText("Clothes", 105, 78, white); break; 
      case 2 : Screen_DrawRect(25, 71, 25+TILE_SIZE, 71+TILE_SIZE, col); Screen_DrawText("Leather", 105, 72, white); Screen_DrawText("Armor", 105, 80, white); break; 
      case 3 : Screen_DrawRect(45, 71, 45+TILE_SIZE, 71+TILE_SIZE, col); Screen_DrawText("Chain", 105, 72, white); Screen_DrawText("Mail", 105, 80, white); break; 
      case 4 : Screen_DrawRect(65, 71, 65+TILE_SIZE, 71+TILE_SIZE, col); Screen_DrawText("Plate", 105, 72, white); Screen_DrawText("Armor", 105, 80, white); break; 
      case 5 : Screen_DrawRect(85, 71, 85+TILE_SIZE, 71+TILE_SIZE, col); Screen_DrawText("Dragon", 105, 72, white); Screen_DrawText("Mail", 105, 80, white); break; 
   } 
 
} 
 
void Screen_DrawSpells(int func) 
{ 
   int col; 
 
   if (obj[0].mag & MAG_FIRE) 
      Tile_DrawTile("objects.pcx", OBJ_FIRESPL, 6, 92); 
   if (obj[0].mag & MAG_STUN) 
      Tile_DrawTile("objects.pcx", OBJ_STUNSPL, 26, 92); 
   if (obj[0].mag & MAG_CURE) 
      Tile_DrawTile("objects.pcx", OBJ_CURESPL, 46, 92); 
   if (obj[0].mag & MAG_WARP) 
      Tile_DrawTile("objects.pcx", OBJ_WARPSPL, 66, 92); 
   if (obj[0].mag & MAG_MANA) 
      Tile_DrawTile("objects.pcx", OBJ_MANASPL, 86, 92); 
 
   if (func == 3) 
      col = white; 
   else 
      col = gray; 
 
   switch(obj[0].spell) 
   { 
      case 1 : Screen_DrawRect(5, 91, 5+TILE_SIZE, 91+TILE_SIZE, col); Screen_DrawText("Fire", 105, 98, white); break; 
      case 2 : Screen_DrawRect(25, 91, 25+TILE_SIZE, 91+TILE_SIZE, col); Screen_DrawText("Stun", 105, 98, white); break; 
      case 3 : Screen_DrawRect(45, 91, 45+TILE_SIZE, 91+TILE_SIZE, col); Screen_DrawText("Cure", 105, 98, white); break; 
      case 4 : Screen_DrawRect(65, 91, 65+TILE_SIZE, 91+TILE_SIZE, col); Screen_DrawText("Warp", 105, 98, white); break; 
      case 5 : Screen_DrawRect(85, 91, 85+TILE_SIZE, 91+TILE_SIZE, col); Screen_DrawText("Mana", 105, 98, white); break; 
   } 
 
} 
 
void Screen_DrawFuncs(int func) 
{ 
   int col; 
 
   Screen_DrawText("Save Game", 200, 58, gray); 
   Screen_DrawText("Load Game", 200, 78, gray); 
   Screen_DrawText("Exit Game", 200, 98, gray); 
 
   switch(func) 
   { 
      case 4 : Screen_DrawText("Save Game", 200, 58, white); break; 
      case 5 : Screen_DrawText("Load Game", 200, 78, white); break; 
      case 6 : Screen_DrawText("Exit Game", 200, 98, white); break; 
   }  
} 
 
void Screen_DrawCrystals() 
{ 
   Screen_DrawRect(5, 134, 6+81, 134+50, white); 
   Screen_DrawTextCenter("CRYSTALS", 46, 130, white); 
   
   if (obj[0].crystals & CRYSTAL_HEART_A) 
      Tile_DrawTile("objects.pcx", OBJ_HEART, 6, 145); 
   if (obj[0].crystals & CRYSTAL_HEART_B) 
      Tile_DrawTile("objects.pcx", OBJ_HEART, 26, 145); 
   if (obj[0].crystals & CRYSTAL_HEART_C) 
      Tile_DrawTile("objects.pcx", OBJ_HEART, 46, 145); 
   if (obj[0].crystals & CRYSTAL_HEART_D) 
      Tile_DrawTile("objects.pcx", OBJ_HEART, 66, 145); 
 
   if (obj[0].crystals & CRYSTAL_STAR_A) 
      Tile_DrawTile("objects.pcx", OBJ_STAR, 6, 165); 
   if (obj[0].crystals & CRYSTAL_STAR_B) 
      Tile_DrawTile("objects.pcx", OBJ_STAR, 26, 165); 
   if (obj[0].crystals & CRYSTAL_STAR_C) 
      Tile_DrawTile("objects.pcx", OBJ_STAR, 46, 165); 
   if (obj[0].crystals & CRYSTAL_STAR_D) 
      Tile_DrawTile("objects.pcx", OBJ_STAR, 66, 165); 
 
} 
 
void Screen_DrawItems() 
{ 
   Screen_DrawRect(229, 134, 230+81, 134+50, white); 
   Screen_DrawTextCenter("ITEMS", 270, 130, white); 
   
   if (obj[0].items & ITEM_SOUL_GEM) 
      Tile_DrawTile("objects.pcx", OBJ_SOULGEM, 235, 145); 
   if (obj[0].items & ITEM_MERMAID_SCALE) 
      Tile_DrawTile("objects.pcx", OBJ_MERMAIDSCALE, 255, 145); 
   if (obj[0].items & ITEM_MAGMA_STONE) 
      Tile_DrawTile("objects.pcx", OBJ_MAGMASTONE, 275, 145); 
   if (obj[0].items & ITEM_ANGEL_WING) 
      Tile_DrawTile("objects.pcx", OBJ_ANGELWING, 295, 145); 
 
   if (obj[0].items & ITEM_SHIELD_RING) 
      Tile_DrawTile("objects.pcx", OBJ_SHIELDRING, 235, 165); 
   if (obj[0].items & ITEM_ELVEN_BOOTS) 
      Tile_DrawTile("objects.pcx", OBJ_ELVENBOOTS, 255, 165); 
   if (obj[0].items & ITEM_PHOENIX_FEATHER) 
      Tile_DrawTile("objects.pcx", OBJ_PHOENIXFEATHER, 275, 165); 
   if (obj[0].items & ITEM_CRYSTAL_ORB) 
      Tile_DrawTile("objects.pcx", OBJ_CRYSTALORB, 295, 165); 
} 
 
void Screen_DrawKeys() 
{ 
   Screen_DrawRect(119, 134, 120+81, 134+50, white); 
   Screen_DrawTextCenter("KEYS", 160, 130, white); 
 
   if (obj[0].keys & KEY_SILVER) 
      Tile_DrawTile("objects.pcx", OBJ_SILVERKEY, 135, 145); 
   if (obj[0].keys & KEY_GOLD) 
      Tile_DrawTile("objects.pcx", OBJ_GOLDKEY, 155, 145); 
   if (obj[0].keys & KEY_EBONY) 
      Tile_DrawTile("objects.pcx", OBJ_EBONYKEY, 175, 145); 
 
   if (obj[0].keys & KEY_RUBY) 
      Tile_DrawTile("objects.pcx", OBJ_RUBYKEY, 135, 165); 
   if (obj[0].keys & KEY_EMERALD) 
      Tile_DrawTile("objects.pcx", OBJ_EMERALDKEY, 155, 165); 
   if (obj[0].keys & KEY_SAPPHIRE) 
      Tile_DrawTile("objects.pcx", OBJ_SAPPHIREKEY, 175, 165); 
} 
 
void Obj_ChangeWeapon(int opt) 
{ 
   while (TRUE) 
   { 
      obj[0].att+=opt; 
 
      if (obj[0].att > 5) 
      { 
         obj[0].att = 1; 
         obj[0].weapon = "weapon1.pcx"; 
         return; 
      } 
      if (obj[0].att < 1) 
      { 
         obj[0].att = 5; 
      } 
 
      switch (obj[0].att) 
      { 
         case 1 : if (obj[0].wpn & WPN_DAGGER) { obj[0].weapon =  "weapon1.pcx"; return; } break; 
         case 2 : if (obj[0].wpn & WPN_SHORT) { obj[0].weapon = "weapon2.pcx"; return; } break; 
         case 3 : if (obj[0].wpn & WPN_LONG) { obj[0].weapon = "weapon3.pcx"; return; } break; 
         case 4 : if (obj[0].wpn & WPN_GREAT) { obj[0].weapon = "weapon4.pcx"; return; } break; 
         case 5 : if (obj[0].wpn & WPN_MITHRIL) { obj[0].weapon = "weapon5.pcx"; return; } break; 
      } 
 
   } 
} 
 
void Obj_ChangeArmor(int opt) 
{ 
   while (TRUE) 
   { 
 
      obj[0].def+=opt; 
 
      if (obj[0].def > 5) 
      { 
         obj[0].def = 1; 
         return; 
      } 
      if (obj[0].def < 1) 
      { 
         obj[0].def = 5; 
      } 
 
      switch (obj[0].def) 
      { 
         case 1 : if (obj[0].amr & AMR_CLOTHES) return; break; 
         case 2 : if (obj[0].amr & AMR_LEATHER) return; break; 
         case 3 : if (obj[0].amr & AMR_CHAIN) return; break; 
         case 4 : if (obj[0].amr & AMR_PLATE) return; break; 
         case 5 : if (obj[0].amr & AMR_DRAGON) return; break; 
      } 
 
   } 
} 
 
void Obj_ChangeSpell(int opt) 
{ 
   while (TRUE) 
   { 
 
      obj[0].spell+=opt; 
 
      if (obj[0].spell > 5) 
      { 
         obj[0].spell = 1; 
         return; 
      } 
      if (obj[0].spell < 1) 
      { 
         obj[0].spell = 5; 
      } 
 
      switch (obj[0].spell) 
      { 
         case 1 : if (obj[0].mag & MAG_FIRE) return; break; 
         case 2 : if (obj[0].mag & MAG_STUN) return; break; 
         case 3 : if (obj[0].mag & MAG_CURE) return; break; 
         case 4 : if (obj[0].mag & MAG_WARP) return; break;  
         case 5 : if (obj[0].mag & MAG_MANA) return; break; 
      } 
 
   } 
} 
 
void Sys_NewGame() 
{ 
    #if 0
    fprintf(logFile, "Start of Sys_NewGame()\n");
    fflush(logFile);
    #endif
    
   #ifndef DEBUG 
   clear(buffer); 
   clear(screen); 
   #endif 
   
   #if 0
   fprintf(logFile, "Before Obj_SetupPlayer()\n");
    fflush(logFile);
    #endif
 
   Obj_SetupPlayer(&obj[0]); 
   
   #if 0
   fprintf(logFile, "After Obj_SetupPlayer()\n");
    fflush(logFile);
    #endif
 
   gameover = 0; 
   gamestart = 1; 
 
   #if 0
   fprintf(logFile, "Before Map_Teleport()\n");
    fflush(logFile);
    #endif
    
   prt = 0; 
   Map_Teleport(&portal[0]); 
   
   #if 0
   fprintf(logFile, "After Map_Teleport()\n");
    fflush(logFile);
    #endif
    
//  fade_out(8); 
    Screen_FadeOut();
   
    #if 0
   fprintf(logFile, "Before Screen_DrawFrame()\n");
    fflush(logFile);
    #endif
 
    Screen_DrawFrame(); 
    
    #if 0
   fprintf(logFile, "After Screen_DrawFrame()\n");
    fflush(logFile);
    #endif
   
//  fade_in(pal, 8); 
    Screen_FadeIn();
      
    #if 0
   fprintf(logFile, "End of Sys_NewGame()\n");
   fflush(logFile);
   #endif
} 
 
 
void Sys_StatusScreen() 
{ 
   int x, mx; 
   int i; 
   int func = 1; 
   int booboo = 0; 
   int poopie = 10; 
    
// fade_out(4); 
    //Screen_FadeOut();
 
   while (TRUE) 
   { 
 
   Sys_KeyClear(); 
 
   if (poopie == 10) 
   { 
      poopie = 0; 
      poll_joystick(); 
   } 
   else 
      poopie++; 
 
 
   //Screen_FillRect(0, 0, 319, 199, white); 
   //Screen_FillRect(1, 1, 318, 198, blue); 
   
   // 570x416, starting at (32, 32)   
   Screen_DrawWindow(32, 32, 640-32-1, 480-32-1, blue);
   Screen_DrawTextCenter("STATUS", 640/2, 32 + Screen_TextHeight()*3/2, white);
   
   int paddingSize = 8;
   
   int topLeft_x1 = 32+paddingSize;
   int topLeft_y1 = 32+paddingSize;
   int topLeft_x2 = topLeft_x1 + ((640-64-paddingSize)*2/3) - 1;
   int topLeft_y2 = topLeft_y1 + ((480-64-paddingSize)*2/3) - 1;
  
   Screen_DrawWindow(topLeft_x1, topLeft_y1, topLeft_x2, topLeft_y2, blue);
   
   int topRight_x1 = topLeft_x2 + paddingSize;
   int topRight_y1 = topLeft_y1;
   int topRight_x2 = (640-32-paddingSize) - 1;
   int topRight_y2 = topLeft_y2;
   
   Screen_DrawWindow(topRight_x1, topRight_y1, topRight_x2, topRight_y2, blue);
   
   int bottomLeft_x1 = topLeft_x1;
   int bottomLeft_y1 = topLeft_y2 + paddingSize;
   int bottomLeft_x2 = bottomLeft_x1 + ((640-64)/3) - (paddingSize/2) - 1;
   int bottomLeft_y2 = (480-32-paddingSize) - 1;
   
   Screen_DrawWindow(bottomLeft_x1, bottomLeft_y1, bottomLeft_x2, bottomLeft_y2, blue);
   
   int bottomCenter_x1 = bottomLeft_x2 + paddingSize;
   int bottomCenter_y1 = bottomLeft_y1;
   int bottomCenter_x2 = bottomCenter_x1 + ((640-64)/3) - (paddingSize/2) - 1;
   int bottomCenter_y2 = bottomLeft_y2;
   
   Screen_DrawWindow(bottomCenter_x1, bottomCenter_y1, bottomCenter_x2, bottomCenter_y2, blue);
   
   int bottomRight_x1 = bottomCenter_x2 + paddingSize;
   int bottomRight_y1 = bottomCenter_y1;
   int bottomRight_x2 = bottomRight_x1 + ((640-64)/3) - 1;
   int bottomRight_y2 = bottomCenter_y2;
   
   Screen_DrawWindow(bottomRight_x1, bottomRight_y1, bottomRight_x2, bottomRight_y2, blue);      
 
   //text_mode(-1); 
 
   //Txt_WriteLevelTitle(); 
   
   
   int lifeBarWidth = obj[0].life * 10;
   int maxLifeBarWidth = obj[0].max_life * 10;
   
   int lifeBarHeight = 12;         
   
   int lifeTextStartX = 32+32;  
   int lifeBarStartX = lifeTextStartX + Screen_TextWidth("LIFE") + 8;
      
   int lifeBarMidPointY = 32+32 + Screen_TextHeight()*2;
   
   int lifeTextStartY = lifeBarMidPointY - Screen_TextHeight()/2;
   int lifeBarStartY = lifeBarMidPointY - lifeBarHeight/2;    
   
   Screen_DrawText("LIFE", lifeTextStartX, lifeTextStartY, white);      
   Screen_FillRect(lifeBarStartX, lifeBarStartY, lifeBarStartX + maxLifeBarWidth, lifeBarStartY + lifeBarHeight, white);

   if (obj[0].max_life > 0)
        Screen_FillRect(lifeBarStartX+2, lifeBarStartY+2, lifeBarStartX + maxLifeBarWidth - 2, lifeBarStartY + lifeBarHeight - 2, black);
    if (obj[0].life > 0)
        Screen_FillRect(lifeBarStartX+2, lifeBarStartY+2, lifeBarStartX + lifeBarWidth - 2, lifeBarStartY + lifeBarHeight - 2, red);  
    
    for (int lb = lifeBarStartX+20; lb < lifeBarStartX+maxLifeBarWidth; lb += 20)
        Screen_FillRect(lb-1, lifeBarStartY+2, lb-1, lifeBarStartY+lifeBarHeight-2, gray);
        
    int manaBarWidth = obj[0].mana * 10;
   int maxManaBarWidth = obj[0].max_mana * 10;
   
   int manaBarHeight = 12;         
   
   int manaTextStartX = 32+32;  
   int manaBarStartX = manaTextStartX + Screen_TextWidth("MANA") + 8;
      
   int manaBarMidPointY = lifeBarMidPointY + lifeBarHeight/2 + 8 + manaBarHeight/2;
   
   int manaTextStartY = manaBarMidPointY - Screen_TextHeight()/2;
   int manaBarStartY = manaBarMidPointY - manaBarHeight/2;    
   
   Screen_DrawText("MANA", manaTextStartX, manaTextStartY, white);      
   Screen_FillRect(manaBarStartX, manaBarStartY, manaBarStartX + maxManaBarWidth, manaBarStartY + manaBarHeight, white);

   if (obj[0].max_mana > 0)
        Screen_FillRect(manaBarStartX+2, manaBarStartY+2, manaBarStartX + maxManaBarWidth - 2, manaBarStartY + manaBarHeight - 2, black);
    if (obj[0].mana > 0)
        Screen_FillRect(manaBarStartX+2, manaBarStartY+2, manaBarStartX + manaBarWidth - 2, manaBarStartY + manaBarHeight - 2, ltblue);  
    
    for (int mb = manaBarStartX+20; mb < manaBarStartX+maxManaBarWidth; mb += 20)
        Screen_FillRect(mb-1, manaBarStartY+2, mb-1, manaBarStartY+manaBarHeight-2, gray);
 
    /*
   Screen_DrawText("LIFE", 32+5, 32+30, white); 
   x = obj[0].life * 10; 
   mx = obj[0].max_life * 10; 
   Screen_FillRect(44, 31, 46+mx, 39, white); 
   Screen_FillRect(45, 32, 45+mx, 38, black); 
   Screen_FillRect(45, 32, 45+x, 38, red); 
   */
 
   /*
   Screen_DrawText("MANA", 145, 30, white); 
   x = obj[0].mana * 5; 
   mx = obj[0].max_mana * 5; 
   Screen_FillRect(184, 31, 186+mx, 39, white); 
   Screen_FillRect(185, 32, 185+mx, 38, black); 
   Screen_FillRect(185, 32, 185+x, 38, ltblue); 
   */
 
   Screen_DrawWeapons(func); 
   Screen_DrawArmors(func); 
   Screen_DrawSpells(func); 
   Screen_DrawFuncs(func); 
 
   Screen_DrawCrystals(); 
   Screen_DrawKeys(); 
   Screen_DrawItems(); 
 
   Screen_CopyBuffer(); 
 
   if (!booboo) 
   { 
        //fade_in(pal, 4); 
        //Screen_FadeIn();
      booboo = !booboo; 
   } 
 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      if (key[KEY_UP] || joy_up) 
      { 
         func--; 
         if (func < 1) func = 6; 
      } 
      else if (key[KEY_DOWN] || joy_down) 
      { 
         func++; 
         if (func > 6) func = 1; 
      } 
 
      if (key[KEY_RIGHT] || joy_right) 
      { 
         switch (func) 
         { 
            case 1 : Obj_ChangeWeapon(1); break; 
            case 2 : Obj_ChangeArmor(1); break; 
            case 3 : Obj_ChangeSpell(1); break; 
         } 
      } 
      else if (key[KEY_LEFT] || joy_left) 
      { 
         switch (func) 
         { 
            case 1 : Obj_ChangeWeapon(-1); break; 
            case 2 : Obj_ChangeArmor(-1); break; 
            case 3 : Obj_ChangeSpell(-1); break; 
         } 
      } 
 
   if (key[KEY_ENTER] || joy_b1) 
   { 
      if (func == 4) 
      { 
         Sys_StatusSaveGame(); 
         key[KEY_ESC] = 0; 
      } 
      if (func == 5) 
      { 
         Sys_StatusLoadGame(); 
         key[KEY_ESC] = 0; 
      } 
      if (func == 6) 
      { 
         if (Sys_Confirm("Really exit?", -1, -1, -1)) 
         { 
            gameover = TRUE; 
            return; 
         } 
         else 
            Sys_KeyClear(); 
      } 
   } 
 
   if (key[KEY_ESC] || joy_b4) 
   { 
      //fade_out(32); 
      //Screen_FadeOut();
      Screen_DrawFrame(); 
      //Screen_FadeIn();
      //fade_in(pal, 32); 
 
      key[KEY_ESC] = joy_b4 = 0; 
      return; 
   } 
 
   key[KEY_RIGHT] = key[KEY_LEFT] = key[KEY_UP] = key[KEY_DOWN] = joy_up = joy_down = joy_left = joy_right = 0; 
 
   } 
}

void Think_RunCheats()
{
   obj[0].mana = obj[0].max_mana;
}
 
void Think_PlayerThink(void) 
{ 
   //BITMAP *bmp; 
   //PALETTE plt; 
 
   poll_joystick(); 

   if (cheat)
      Think_RunCheats();

   if (key[KEY_F12]) 
   { 
      Screen_SaveShot(); 
   }

   if (console)
   {
      int i = readkey();
      int k = (i >> 8) & 0xff;
      char c = i & 0xff;

      if (k == KEY_ENTER)
      {
         Con_Command(consoletext);
         console = 0;
      }
      else if (k == KEY_ESC)
         console = 0;
      else if (k == KEY_BACKSPACE)
         consoletext.erase( consoletext.length()-1, 1 );
         //consolepos--;
      else
        consoletext += string(1, c);
         //consoletext[consolepos++] = c;

      //if (consolepos <= 1)
      if (consoletext.length() == 0)
         console = 0;

      if (console == 0)
      {
         clear_keybuf();
         return;
      }

      //consoletext[consolepos] = 0;

      if (Screen_TextWidth(consoletext.c_str()) >= SCREEN_WIDTH /* || consolepos == 256 */)
        consoletext.erase( consoletext.length()-1, 1 );
         //consolepos--;

      clear_keybuf();               
      return;
   }

   if (key[KEY_SLASH])
   {
      consoletext = "/";
      //consolepos = 1;
      console = 1;
      clear_keybuf();
      return;
   }
   
   if (key[KEY_ESC] || joy_b4) 
   { 
      if (Sys_Confirm("Really exit?", -1, -1, -1)) 
      { 
         gameover = TRUE; 
         return; 
      } 
       
      //Sys_StatusScreen(); 
      //self->cmd = 0; 
   } 
   else if (key[KEY_UP]) 
      self->cmd = CMD_UP; 
   else if (key[KEY_DOWN]) 
      self->cmd = CMD_DOWN; 
   else if (key[KEY_LEFT]) 
      self->cmd = CMD_LEFT; 
   else if (key[KEY_RIGHT]) 
      self->cmd = CMD_RIGHT; 
   else if (key_shifts & KB_CTRL_FLAG || key[KEY_SPACE]) 
   { 
      self->cmd = CMD_ATTACK; 
   } 
   else if (key[KEY_ENTER]) 
      self->cmd = CMD_MAGIC; 
   else if (joy_up) 
      self->cmd = CMD_UP; 
   else if (joy_down) 
      self->cmd = CMD_DOWN; 
   else if (joy_left) 
      self->cmd = CMD_LEFT; 
   else if (joy_right) 
      self->cmd = CMD_RIGHT; 
   else if (joy_b1) 
   { 
      self->cmd = CMD_ATTACK; 
   } 
   else if (joy_b2) 
      self->cmd = CMD_MAGIC; 
 
   key[KEY_OPENBRACE] = key[KEY_CLOSEBRACE] = 0;

}

void Map_ScrollWorld(int x, int y)
{
   BITMAP *largebuffer;
   int siz_x = 20*TILE_SIZE, siz_y = 10*TILE_SIZE;
   int org_x = 0, org_y = 0;
   int dst_x = 0, dst_y = 0;
   int pos_x, pos_y;

   // scroll horizontally
   if (x != 0)
      siz_x *= 2;
   // scroll vertically
   if (y != 0)
      siz_y *= 2;

   largebuffer = create_bitmap(siz_x, siz_y);
   
   if (x == -1) org_x = 20*TILE_SIZE;
   if (x == 1) dst_x = 20*TILE_SIZE;
   if (y == -1) org_y = 10*TILE_SIZE;
   if (y == 1) dst_y = 10*TILE_SIZE;

   blit(buffer, largebuffer, 0, SCREEN_BORDER, org_x, org_y, 20*TILE_SIZE, 10*TILE_SIZE);

   Tile_DrawBase();
   Tile_DrawFringe(); 
   Tile_DrawOcclude();

   blit(buffer, largebuffer, 0, SCREEN_BORDER, dst_x, dst_y, 20*TILE_SIZE, 10*TILE_SIZE);
   
   //save_bitmap("buffer.tga", largebuffer, NULL);

   pos_x = org_x;
   pos_y = org_y;

   while (pos_x != dst_x || pos_y != dst_y)
   {
      acquire_screen();
      blit(largebuffer, screen, pos_x, pos_y, 0, SCREEN_BORDER, 20*TILE_SIZE, 10*TILE_SIZE);
      release_screen();

      pos_x += x;
      pos_y += y;
   }

   destroy_bitmap(largebuffer);
}
 
void Map_MoveWorld(int x, int y) 
{ 
   int msg = messagetime;
   int con = console;

   messagetime = 0;
   console = 0;

   Screen_DrawFrame();

   world.map_x += x;
   world.map_y += y; 
 
   obj[0].cmd = 0; 

   if (x == -1) 
      obj[0].pos_x = 19; 
   else if (x == 1) 
      obj[0].pos_x = 0; 
   else if (y == -1) 
      obj[0].pos_y = 9; 
   else if (y == 1) 
      obj[0].pos_y = 0; 
 
   obj[0].cmd = 0; 

   Map_ScrollWorld(x, y);

   nextobj = 1; 
   Obj_LoadObjects(); 
   Screen_DrawFrame();

   messagetime = msg;
   console = con;
} 
 
obj_t *other; 
 
void Screen_DialogBoxOld(const string& s1, const string& s2, 
    const string& s3, const string& s4, const string& s5) 
{ 
   int booboo = 0; 
    
   //text_mode(-1); 
 
   Screen_FillRect(4, 4, 316, 66, white); 
   Screen_FillRect(5, 5, 315, 65, blue); 
 
   Screen_DrawText(s1, 15, 10, white); 
   Screen_DrawText(s2, 15, 20, white); 
   Screen_DrawText(s3, 15, 30, white); 
   Screen_DrawText(s4, 15, 40, white); 
   Screen_DrawText(s5, 15, 50, white); 
 
   Screen_CopyBuffer(); 
 
   Sys_KeyClear(); 
 
   while (!(key[KEY_SPACE] || joy_b1 || key[KEY_ESC])) 
   { 
      if (booboo == 10) 
      { 
         poll_joystick(); 
         booboo = 0; 
      } 
      else 
         booboo++; 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      key[KEY_F12] = 0; 
   } 
 
   key[KEY_SPACE] = 0; 
   joy_b1 = 0; 
 
   return; 
}

static const char *Screen_BoundsLine (int *x1, int *y1, int *x2, int *y2, int *x, int *y, int rgb, const char *msg)
{
   const char *txt = msg;
   char *eol;
   int len;

   while (*txt == ' ')
      txt++;

   // split on newlines
   eol = strchr (txt, '\n');
   if (eol)
   {
      const char *ret;

      *eol = 0;
      if (ret = Screen_BoundsLine (x1, y1, x2, y2, x, y, rgb, txt))
         return ret;

      return Screen_BoundsLine (x1, y1, x2, y2, x, y, rgb, eol+1);
   }

   len = *x2 - *x;
   if (Screen_TextWidth (txt) > len)
   {
      int i = strlen (txt);

      while (i--)
      {
         char buf[1024];
         const char *ret;
         int j;

         strncpy (buf, txt, i);
         buf[i] = 0;

         if (Screen_TextWidth(buf) > len)
            continue;

         j = i;
         if (txt[j-1] != ' ' && txt[j] != ' ')
         {
            while (j && txt[j-1] != ' ')
               j--;
         }

         if (j != 0)
         {
            buf[j] = 0;
            i = j;
         }

         if (ret = Screen_BoundsLine (x1, y1, x2, y2, x, y, rgb, buf))
            return ret;

         return Screen_BoundsLine (x1, y1, x2, y2, x, y, rgb, txt+i);
      }

      // bounds are too small...
      return NULL;
   }

   if (*y + Screen_TextHeight () > *y2)
      return txt;

   Screen_DrawText (txt, *x, *y, rgb);

   *x = *x1;
   *y += Screen_TextHeight() + 2;
   return NULL;
}

void Screen_BoundsPrint (int x1, int y1, int x2, int y2, int rgb, const char *msg)
{
   Screen_BoundsLine (&x1, &y1, &x2, &y2, &x1, &y1, rgb, msg);
}

void Screen_DialogBox(const string& txt)
{ 
   int booboo = 0; 
    
   //text_mode(-1); 
 
   //Screen_FillRect(4, 4, 316, 66, white); 
   //Screen_FillRect(5, 5, 315, 65, blue); 
     
    int x = 10, y = 10;   
    
    // ???
    Screen_BoundsPrint(x, y, 640-x, SCREEN_BORDER-y, white, txt.c_str());
    
   Screen_CopyBuffer(); 
 
   Sys_KeyClear(); 
 
   while (!(key[KEY_SPACE] || joy_b1 || key[KEY_ESC])) 
   { 
      if (booboo == 10) 
      { 
         poll_joystick(); 
         booboo = 0; 
      } 
      else 
         booboo++; 
 
      if (key[KEY_F12]) 
         Screen_SaveShot(); 
 
      key[KEY_F12] = 0; 
   } 
 
   key[KEY_SPACE] = 0; 
   joy_b1 = 0; 
 
   return; 
} 
  
void Obj_TouchLife(void) 
{ 
   other->type = 0; 
   obj[0].life+=2; 
   if (obj[0].life > obj[0].max_life) 
      obj[0].life = obj[0].max_life; 
} 
 
void Obj_TouchMana(void) 
{ 
   other->type = 0; 
   obj[0].mana+=2; 
   if (obj[0].mana > obj[0].max_mana) 
      obj[0].mana = obj[0].max_mana; 
}    
 
void Obj_TouchHeart(void) 
{ 
   other->type = 0; 
   obj[0].max_life+=2; 
 
   if (obj[0].max_life > 16) 
      obj[0].max_life = 16; 
   obj[0].life = obj[0].max_life; 

   Sound_PlaySound("victory.wav");

   if (other->crystals & CRYSTAL_HEART_A) 
   { 
      Screen_DialogBox("You found a Heart Crystal! This magical artifact will amplify your life force, allowing you to survive longer in combat and use heavier swords and suits of armor."); 
      world.map[world.map_x][world.map_y].fringe[9][0] = 0; 
      world.map[world.map_x][world.map_y].fringe[10][0] = 0; 
   } 
 
   obj[0].crystals |= other->crystals; 
} 
 
void Obj_TouchStar(void) 
{ 
   other->type = 0; 
   obj[0].max_mana+=2; 
   if (obj[0].max_mana > 16) 
      obj[0].max_mana = 16; 
 
   obj[0].mana = obj[0].max_mana; 
} 
 
void Obj_TouchWeapon(int num) 
{ 
   other->type = 0; 
 
   if (num == 1) 
   { 
      obj[0].att = 1; 
      obj[0].wpn |= WPN_DAGGER; 
      Screen_DialogBox ("You got a Dagger! While a small weapon, the Dagger can be deadly in the hands of a trained thief, assassin, or would-be hero."); 
      obj[0].weapon = "weapon1.pcx";
   }
   else if (num == 2) 
   { 
      obj[0].att = 2; 
      obj[0].wpn |= WPN_SHORT; 
      Screen_DialogBox ("You got a Short Sword! Longer than a Dagger, the Short Sword is designed for a more trained fighter. Its greatest virtue is its speed."); 
      obj[0].weapon = "weapon2.pcx"; 
   } 
   else if (num == 3) 
   { 
      obj[0].att = 3; 
      obj[0].wpn |= WPN_LONG; 
      Screen_DialogBox ("You got a Long Sword! Being stronger and having a longer reach than lesser blades, the Long Sword is a formidable weapon to wield."); 
      obj[0].weapon = "weapon3.pcx"; 
   } 
   else if (num == 4) 
   { 
      obj[0].att = 4; 
      obj[0].wpn |= WPN_GREAT; 
      Screen_DialogBox ("You got a Great Sword! The strongest sword made of raw steel, only the strongest brutes can wield it without the strength of the Crystals."); 
      obj[0].weapon = "weapon4.pcx"; 
   } 
   else if (num == 5 && (!(obj[0].amr & AMR_DRAGON) || !(obj[0].mag & MAG_MANA))) 
   { 
      obj[0].att = 5; 
      obj[0].wpn |= WPN_MITHRIL; 
      Screen_DialogBox ("You found the Mithril Sword! At last! With the power of the Nexus unleashed, this eldritch blade may be the world's last hope for survival."); 
      obj[0].weapon = "weapon5.pcx"; 
   } 
   else if (num == 5 && obj[0].amr & AMR_DRAGON && obj[0].mag & MAG_MANA) 
   { 
      obj[0].att = 5; 
      obj[0].wpn |= WPN_MITHRIL; 
      Screen_DialogBox ("You found the Mithril Sword! With the power of all three artifacts combined, you are at last ready to challenge your foe. To the Nexus."); 
      obj[0].weapon = "weapon5.pcx"; 
   } 
} 
 
void Obj_TouchArmor(int num) 
{ 
   other->type = 0; 
 
   if (num == 1) 
   { 
      obj[0].def = 1; 
      obj[0].amr |= AMR_CLOTHES; 
      Screen_DialogBox ("You got Clothes! Lightweight and thin, your suit of travelling Clothes makes poor armor, but it very flexible and comfortable."); 
   } 
   else if (num == 2) 
   { 
      obj[0].def = 2; 
      obj[0].amr |= AMR_LEATHER; 
      Screen_DialogBox ("You got Leather Armor! The armor preferred by most forestmen, Leather provides adequate protection and requires minimal strength."); 
   } 
   else if (num == 3) 
   { 
      obj[0].def = 3; 
      obj[0].amr |= AMR_CHAIN; 
      Screen_DialogBox ("You got Chain Mail! These chain links are much heavier than the soft hide of Leather Armor, requiring more muscle but protecting better."); 
   } 
   else if (num == 4) 
   { 
      obj[0].def = 4; 
      obj[0].amr |= AMR_PLATE; 
      Screen_DialogBox ("You got Plate Armor! The steel armor of a knight, Plate Armor is difficult to maneuver in for all but the strongest."); 
   } 
   else if (num == 5 && (!(obj[0].amr & AMR_DRAGON) || !(obj[0].mag & MAG_MANA))) 
   { 
      obj[0].def = 5; 
      obj[0].amr |= AMR_DRAGON; 
      Screen_DialogBox ("You found Dragon Mail! The enchanted scales of this suit will be the only thing between you and certian death as you face your final foe."); 
   } 
   else if (num == 5 && obj[0].wpn & WPN_MITHRIL && obj[0].mag & MAG_MANA) 
   { 
      obj[0].def = 5; 
      obj[0].amr |= AMR_DRAGON; 
      Screen_DialogBox ("You found Dragon Mail! With the power of all three artifacts combined, you are at last ready to challenge your foe. To the Nexus."); 
   } 
} 
    
void Obj_TouchSpell(int num) 
{ 
   other->type = 0; 
 
   if (num == 1) 
   { 
      obj[0].spell = 1; 
      obj[0].mag |= MAG_FIRE; 
      Screen_DialogBox ("You got the Fire Scroll! You are unique, a boy with the gift of magic in a world where such cannot be. This weak spell is itself a miracle."); 
   } 
   else if (num == 2) 
   { 
      obj[0].spell = 2; 
      obj[0].mag |= MAG_STUN; 
      Screen_DialogBox ("You got the Stun Scroll! This magical spell will stop most monsters in their tracks, but be warned -- any physical damage will break the spell."); 
   } 
   else if (num == 3) 
   { 
      obj[0].spell = 3; 
      obj[0].mag |= MAG_CURE; 
      Screen_DialogBox ("You got the Cure Scroll! These ancient glyphs, useless now to all save the magi and you, were crafted by ancient healers, and will restore Life."); 
   } 
   else if (num == 4) 
   { 
      obj[0].spell = 4; 
      obj[0].mag |= MAG_WARP; 
      Screen_DialogBox ("You got the Warp Scroll! The magic of this scroll is connected to that of the Nexus gate itself, and casting it will transport you there."); 
   } 
   else if (num == 5 && (!(obj[0].wpn & WPN_MITHRIL) || !(obj[0].amr & AMR_DRAGON))) 
   { 
      obj[0].spell = 5; 
      obj[0].mag |= MAG_MANA; 
      Screen_DialogBox ("You got the Mana Scroll! The power of the greatest archmage fills your veins This spell, which once sealed the Nexus, is your last hope."); 
   } 
   else if (num == 5 && obj[0].amr & AMR_DRAGON && obj[0].wpn & WPN_MITHRIL) 
   { 
      obj[0].spell = 5; 
      obj[0].mag |= MAG_MANA; 
      Screen_DialogBox ("You got the Mana Scroll! With the power of all three artifacts combined, you are at last ready to challenge your foe. To the Nexus."); 
   } 
} 
 
void Obj_TouchKey(int knum) 
{ 
   switch(knum) 
   { 
      case 1: Screen_DialogBox("You found the Silver Key!"); obj[0].keys |= KEY_SILVER; break; 
      case 2: Screen_DialogBox("You found the Gold Key!"); obj[0].keys |= KEY_GOLD; break; 
      case 3: Screen_DialogBox("You found the Ebony Key!"); obj[0].keys |= KEY_EBONY; break; 
      case 4: Screen_DialogBox("You found the Ruby Key!"); obj[0].keys |= KEY_RUBY; break; 
      case 5: Screen_DialogBox("You found the Emerald Key!"); obj[0].keys |= KEY_EMERALD; break; 
      case 6: Screen_DialogBox("You found the Sapphire Key!"); obj[0].keys |= KEY_SAPPHIRE; break; 
   } 
} 
 
void Obj_TouchItem(int knum) 
{ 
   switch (knum) 
   { 
      case 1: 
         Screen_DialogBox("You found the Soul Gem! This artifact will protect your spirit from being harmed by the foul necromantic energies used to create and sustain the undead."); 
         obj[0].items |= ITEM_SOUL_GEM; 
         break; 
      case 2: 
         Screen_DialogBox("You found the Mermaid Scale! Powered by the dying wish of a pirate's sea-maiden lover, this scale gives he who holds it the power to breathe freely beneath the depths of the ocean."); 
         obj[0].items |= ITEM_MERMAID_SCALE; 
         break; 
      case 3: 
         Screen_DialogBox("You found the Magma Stone! A volcanic rock bound with sorcerous energy, the Magma Stone protects the wielder from the blazing heat of lava, allowing survival in the heart of a volcano."); 
         obj[0].items |= ITEM_MAGMA_STONE; 
         break; 
      case 4: 
         Screen_DialogBox("You found the Angel Wing! Sacrificed by a creature of purity and light to deliver the world from evil, this wing will carry you safely to the Palace of the Winds."); 
         obj[0].items |= ITEM_ANGEL_WING; 
         break; 
      case 5: 
         Screen_DialogBox("You found the Shield Ring! This piece of seemingly ordinary gold jewelry will radiate a protective aura, deflecting hostile projectiles that otherwise could lead to your demise."); 
         obj[0].items |= ITEM_SHIELD_RING; 
         break; 
      case 6: 
         Screen_DialogBox("You found the Elven Boots! Crafted of the finest enchanted leather, these boots can make even the most sluggish drone as nimble as the sylvan hunters that are their namesake."); 
         obj[0].items |= ITEM_ELVEN_BOOTS; 
         break; 
      case 7: 
         Screen_DialogBox("You found the Phoenix Feather! Like the fiery bird of legend from whence it was plucked, this feather empowers he who holds it to use his Mana to return from beyond the grave."); 
         obj[0].items |= ITEM_PHOENIX_FEATHER; 
         break; 
      case 8: 
         Screen_DialogBox("You found the Crystal Orb! Shaped long ago by the first of the sorcerers, this artifact amplifies the magical strength of the holder, requiring less energy to cast his spells."); 
         obj[0].items |= ITEM_CRYSTAL_ORB; 
         break; 
      default: 
         break; 
   } 
} 
 
void Obj_Remove(int num) 
{ 
   obj[num].type = 0; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_TYPE] = 0; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_X] = -1; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_Y] = -1; 
} 
 
void Obj_Touch(int num) 
{ 
   obj[0].cmd = 0; 
 
   other = &obj[num]; 
   switch (other->type) 
   { 
      case (OBJ_LIFE)  : Obj_TouchLife(); Obj_Remove(num); break; 
      case (OBJ_MANA)  : Obj_TouchMana(); Obj_Remove(num); break; 
      case (OBJ_HEART) : Obj_TouchHeart(); Obj_Remove(num); break; 
      case (OBJ_STAR)  : Obj_TouchStar(); Obj_Remove(num); break; 
      case (OBJ_DAGGER) : Obj_TouchWeapon(1); Obj_Remove(num); break; 
      case (OBJ_SHORTSWD) : Obj_TouchWeapon(2); Obj_Remove(num); break; 
      case (OBJ_LONGSWD) : Obj_TouchWeapon(3); Obj_Remove(num); break; 
      case (OBJ_GREATSWD) : Obj_TouchWeapon(4); Obj_Remove(num); break; 
      case (OBJ_MITHRILSWD) : Obj_TouchWeapon(5); Obj_Remove(num); break; 
      case (OBJ_CLOTHES) : Obj_TouchArmor(1); Obj_Remove(num); break; 
      case (OBJ_LEATHER) : Obj_TouchArmor(2); Obj_Remove(num); break; 
      case (OBJ_CHAIN) : Obj_TouchArmor(3); Obj_Remove(num); break; 
      case (OBJ_PLATE) : Obj_TouchArmor(4); Obj_Remove(num); break; 
      case (OBJ_DRAGON) : Obj_TouchArmor(5); Obj_Remove(num); break; 
      case (OBJ_FIRESPL) : Obj_TouchSpell(1); Obj_Remove(num); break; 
      case (OBJ_STUNSPL) : Obj_TouchSpell(2); Obj_Remove(num); break; 
      case (OBJ_WARPSPL) : Obj_TouchSpell(4); Obj_Remove(num); break; 
      case (OBJ_MANASPL) : Obj_TouchSpell(5); Obj_Remove(num); break; 
      case (OBJ_CURESPL) : Obj_TouchSpell(3); Obj_Remove(num); break; 
      case (OBJ_SILVERKEY): Obj_TouchKey(1); Obj_Remove(num); break; 
      case (OBJ_GOLDKEY): Obj_TouchKey(2); Obj_Remove(num); break; 
      case (OBJ_EBONYKEY): Obj_TouchKey(3); Obj_Remove(num); break; 
      case (OBJ_RUBYKEY): Obj_TouchKey(4); Obj_Remove(num); break; 
      case (OBJ_EMERALDKEY): Obj_TouchKey(5); Obj_Remove(num); break; 
      case (OBJ_SAPPHIREKEY): Obj_TouchKey(6); Obj_Remove(num); break; 
      case (OBJ_SOULGEM) : Obj_TouchItem(1); Obj_Remove(num); break; 
      case (OBJ_MERMAIDSCALE): Obj_TouchItem(2); Obj_Remove(num); break; 
      case (OBJ_MAGMASTONE): Obj_TouchItem(3); Obj_Remove(num); break; 
      case (OBJ_ANGELWING): Obj_TouchItem(4); Obj_Remove(num); break; 
      case (OBJ_SHIELDRING): Obj_TouchItem(5); Obj_Remove(num); break; 
      case (OBJ_ELVENBOOTS): Obj_TouchItem(6); Obj_Remove(num); break; 
      case (OBJ_PHOENIXFEATHER): Obj_TouchItem(7); Obj_Remove(num); break; 
      case (OBJ_CRYSTALORB): Obj_TouchItem(8); Obj_Remove(num); break; 
 
      default           : break; 
   } 
   
   #if 0
   fprintf(logFile, "Touched object! (Type %i)\n", other->type);
   fflush(logFile);
   #endif
 
   if (other->type >= 200 && other->type <= 205) 
   { 
      prt = other->type - 200; 
      Map_Teleport(&portal[other->type - 200]); 
 
      //fade_out(8); 
      Screen_FadeOut();
      Screen_DrawFrame(); 
      Screen_FadeIn();
      //fade_in(pal, 8); 
 
      return; 
   } 
} 
 
int Obj_CheckSolid(int x, int y, int n) 
{ 
   int i; 
 
   if (x < 0) 
   { 
      if (pflag && !obj[0].pain && world.map_x > 0) 
         Map_MoveWorld(-1, 0); 
 
      return TRUE; 
   } 
 
   if (x > 19) 
   { 
      if (pflag && !obj[0].pain && world.map_x < 9) 
         Map_MoveWorld(1, 0); 
 
      return TRUE; 
   } 
 
   if (y < 0) 
   { 
      if (pflag && !obj[0].pain && world.map_y > 0) 
         Map_MoveWorld(0, -1); 
 
      return TRUE; 
   } 
 
   if (y > 9) 
   { 
      if (pflag && !obj[0].pain && world.map_y < 9) 
         Map_MoveWorld(0, 1); 
 
      return TRUE; 
   } 
 
   if (!world.map[world.map_x][world.map_y].base[x][y]) 
      return TRUE; 
 
   if (world.map[world.map_x][world.map_y].fringe[x][y]) 
      return TRUE; 
 
   for (i = 0; i < nextobj; i++) 
   { 
      if (obj[i].pos_x == x && obj[i].pos_y == y) 
      { 
         if (n != 0 && obj[i].dumb == n) 
            return FALSE; 
         if (obj[i].type < 100 && obj[i].type > 0) 
            return TRUE; 
         else if (pflag && obj[i].type >= 100) 
            Obj_Touch(i); 
      } 
   } 
 
   return FALSE; 
} 
 
void Obj_MakeLife(int num) 
{ 
   obj[num].type = OBJ_LIFE; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_TYPE] = OBJ_LIFE; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_X] = obj[num].pos_x; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_Y] = obj[num].pos_y; 
} 
 
void Obj_MakeMana(int num) 
{ 
   obj[num].type = OBJ_MANA; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_TYPE] = OBJ_MANA; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_X] = obj[num].pos_x; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_Y] = obj[num].pos_y; 
} 
 
void Obj_MakeDead(int num) 
{ 
   obj[num].type = -1; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_TYPE] = 0; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_X] = 0; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_Y] = 0; 
} 
 
void Obj_MakeHeart(int num) 
{ 
   obj[num].type = OBJ_HEART; 
   world.map[world.map_x][world.map_y].obj[num][OBJ_TYPE] = OBJ_HEART; 
} 
 
void Obj_DoubleDeath(int num) 
{ 
   obj[1].type = 202; 
   world.map[world.map_x][world.map_y].base[obj[1].pos_x][obj[1].pos_y] = 255; 
   Screen_DialogBox("Justice is not the only right in this world. Someday you will see.\n\n...\n\nI don't know what it means either, but doesn't it sound cool?");
   //Screen_DialogBox("Well done, young Mike, but this is only a demo. Only when Tilegame is complete can you truly test your abilities. Until then, accept this Heart Crystal as your reward for victory."); 
   Obj_MakeHeart(num); 
} 
 
void Obj_MinotaurDeath(int num) 
{ 
   int j, i; 
 
   for (j = 0; j < MAXOBJ; j++) 
   { 
      if (obj[j].dumb == num) 
         obj[j].type = 0; 
   } 
 
   Sound_StopMidi();    
   Sound_PlayMidi("quiet.mid");
   
   world.map[world.map_x][world.map_y].fringe[8][9] = 134; 
   world.map[world.map_x][world.map_y].fringe[9][9] = 134; 
   world.map[world.map_x][world.map_y].fringe[10][9] = 134; 
   world.map[world.map_x][world.map_y].fringe[11][9] = 134; 
 
   Obj_MakeHeart(num+1); 
   Obj_MakeDead(num); 
 
   obj[num+1].crystals = CRYSTAL_HEART_A; 
} 
 
 
void Obj_BossDeath(int num) 
{ 
   switch (obj[num].type) 
   { 
      case (OBJ_DOUBLE) : Obj_DoubleDeath(num); break; 
      case (OBJ_MINOTAUR) : Obj_MinotaurDeath(num); break; 
      default : Obj_MakeDead(num); break; 
   } 
} 
 
void Obj_Die(int dnum) 
{ 
   int r; 
 
   obj[dnum].weapon = -1;

   if (obj[dnum].snd_death != "")
      Sound_PlaySound(obj[dnum].snd_death);
 
   if (obj[dnum].type > 1 && obj[dnum].type < 61) 
   { 
      r = rand()%100; 
       
      if (r < 3) 
         Obj_MakeLife(dnum); 
      else if (r < 9) 
         Obj_MakeMana(dnum); 
      else 
         Obj_MakeDead(dnum); 
   } 
   else if (obj[dnum].type > 60 && obj[dnum].type < 99) 
   { 
      Obj_BossDeath(dnum); 
   } 
   else 
   { 
      Obj_MakeDead(dnum); 
   } 
} 
 
int Obj_Hit(obj_t *obj) 
{ 
   return FALSE; 
} 
 
void Obj_Damage(int defnum, obj_t *attacker, obj_t *defender, int amt) 
{ 
   if (amt <= 0) 
      amt = 1; 
 
   defender->life = defender->life - amt;

   if (defender->life <= 0) 
   {
       defender->life = 0;
      Obj_Die(defnum);
   }
   else
   {
      if (defender->snd_pain != "")
         Sound_PlaySound(defender->snd_pain);
   }      
 
   if (defender->type == 99) 
   { 
      defender->type = defender->oldtype; 
   } 
} 
 
void Obj_Attack(obj_t *attacker, int x, int y)    
//int Obj_Attack(obj_t *attacker, int x, int y) 
{ 
   int i; 
 
   if (attacker->snd_attack != "")
      Sound_PlaySound(attacker->snd_attack);

   for (i = 0; i < nextobj; i++)
   { 
      if (obj[i].pos_x == x && obj[i].pos_y == y && obj[i].type > 0 && obj[i].type < 128 && &obj[i] != attacker) 
      { 
         if (obj[i].type >= 100) 
         { 
             return;
            //return Obj_Hit(&obj[i]); 
         } 
         else if (obj[i].dumb) 
            Obj_Attack(attacker, obj[obj[i].dumb].pos_x, obj[obj[i].dumb].pos_y); 
         else 
         {
            obj[i].offset = 0; obj[i].attack = 0; 
            obj[i].cmd = (attacker->facing/3)+1; 
            obj[i].pain = 1; 
            obj[i].facing = (int)(((obj[i].facing/3)+1)*10) + obj[i].facing; 
            Obj_Damage(i, attacker, &obj[i], attacker->att - obj[i].def); 
            //return TRUE; 
         } 
      } 
   } 
} 
 
void Obj_MslAttack(obj_t *attacker, int x, int y) 
{ 
   int i; 
 
   for (i = 0; i < nextobj; i++) 
   { 
      if (obj[i].pos_x == x && obj[i].pos_y == y && obj[i].type > 0 && obj[i].type < 128 && &obj[i] != attacker) 
      { 
         if (obj[i].type >= 100) 
         { 
             return;
            //return Obj_Hit(&obj[i]); 
         } 
         else 
         { 
            obj[i].offset = 0; obj[i].attack = 0; 
            obj[i].cmd = (attacker->facing/3)+1; 
            obj[i].pain = 1; 
            obj[i].facing = (int)(((obj[i].facing/3)+1)*10) + obj[i].facing; 
            if (attacker->msl_at == -1) 
            { 
               if (obj[i].type > 0 && obj[i].type < 50) 
               { 
                  obj[i].offset = 0; obj[i].attack = 0; 
                  obj[i].pain = 1; 
                  obj[i].oldtype = obj[i].type; 
                  obj[i].type = 99; 
               } 
            } 
            else if (attacker->msl_at) 
               Obj_Damage(i, attacker, &obj[i], attacker->msl_at); 
            else 
               Obj_Damage(i, attacker, &obj[i], attacker->att - obj[i].def); 

            //return TRUE;
         } 
      } 
   } 
} 
 
void Mag_CastFireMagic(void) 
{ 
   int i; 
 
   if (obj[0].mana < 1) 
      return; 

   if (!obj[0].msl && !obj[0].cast) 
   {
      Sound_PlaySound("spell1.wav");
      
      obj[0].missile = "spellf1.pcx"; 
      obj[0].msl_at = 2; 
      obj[0].msl_speed = 4; 
      obj[0].mana--; 
                                    obj[0].msl = TRUE; 
                                    obj[0].cast = 48; 
                                    obj[0].msl_facing = obj[0].facing; 
                                    obj[0].msl_offset = TILE_SIZE; 
                                    obj[0].msl_pos_x = obj[0].pos_x; 
                                    obj[0].msl_pos_y = obj[0].pos_y; 
                                    if (obj[0].facing == FACE_FRONT) obj[0].msl_pos_y++; 
                                    if (obj[0].facing == FACE_BACK) obj[0].msl_pos_y--; 
                                    if (obj[0].facing == FACE_LEFT) obj[0].msl_pos_x--; 
                                    if (obj[0].facing == FACE_RIGHT) obj[0].msl_pos_x++;   
   } 
 
   Sys_KeyClear();
   //key[KEY_ENTER] = 0; 
   //joy_b2 = 0; 
} 
 
void Mag_CastStunMagic(void) 
{ 
   int i; 
 
   if (obj[0].mana < 2) 
      return; 
 
   if (!obj[0].msl && !obj[0].cast) 
   { 
      obj[0].missile = "spells1.pcx";
      obj[0].msl_at = -1; 
      obj[0].msl_speed = 4; 
      obj[0].mana-=2; 
                                    obj[0].msl = TRUE; 
                                    obj[0].cast = 64; 
                                    obj[0].msl_facing = obj[0].facing; 
                                    obj[0].msl_offset = TILE_SIZE; 
                                    obj[0].msl_pos_x = obj[0].pos_x; 
                                    obj[0].msl_pos_y = obj[0].pos_y; 
                                    if (obj[0].facing == FACE_FRONT) obj[0].msl_pos_y++; 
                                    if (obj[0].facing == FACE_BACK) obj[0].msl_pos_y--; 
                                    if (obj[0].facing == FACE_LEFT) obj[0].msl_pos_x--; 
                                    if (obj[0].facing == FACE_RIGHT) obj[0].msl_pos_x++;   
   } 
 
 
 
   Sys_KeyClear();
   //key[KEY_ENTER] = 0; 
   //joy_b2 = 0; 
} 
 
void Mag_CastWarpMagic(void) 
{ 
   int i; 

   if (obj[0].mana < 8) 
      return; 

   obj[0].mana -= 8;
 
   if (prt < 5) 
      prt = 0; 
   else if (prt < 10) 
      prt = 5; 
   else 
      prt = 10; 
 
   Map_Teleport(&portal[prt]); 
 
   Screen_DrawFrame(); 
 
   Sys_KeyClear();
//  key[KEY_ENTER] = 0; 
//  joy_b2 = 0; 
} 
 
void Mag_CastCureMagic(void) 
{ 
   int i; 
 
   if (obj[0].mana < 1) 
      return; 
 
   if (obj[0].life >= obj[0].max_life) 
      return; 
 
   obj[0].mana--; 
 
   obj[0].life++; 
 
   Sys_KeyClear();
// key[KEY_ENTER] = 0; 
// joy_b2 = 0; 
} 
 
void Mag_CastManaMagic(void) 
{ 
   int i, x, y; 
 
   if (obj[0].mana < 12) 
      return; 
 
   obj[0].mana-=12; 
 
   for (i = 1; i < MAXOBJ; i++) 
   { 
      if (obj[i].type > 0 && obj[i].type < 100) 
      { 
         obj[i].offset = 0; obj[i].attack = 0; 
         if (obj[i].pos_x > obj[i].pos_x) 
            obj[i].cmd = CMD_LEFT; 
         else if (obj[i].pos_x < obj[i].pos_x) 
            obj[i].cmd = CMD_RIGHT; 
         else if (obj[i].pos_y > obj[i].pos_y) 
            obj[i].cmd = CMD_DOWN; 
         else if (obj[i].pos_y < obj[i].pos_y) 
            obj[i].cmd = CMD_UP; 
         obj[i].pain = 1; 
         Obj_Damage(i, &obj[i], &obj[i], 6); 
      } 
   } 
 
   Sys_KeyClear();
   //key[KEY_ENTER] = 0; 
   //joy_b2 = 0; 
} 
 
void Mag_CastSpell(obj_t *ent) 
{ 
   switch (ent->spell) 
   { 
      case 1   : Mag_CastFireMagic(); break; 
      case 2   : Mag_CastStunMagic(); break; 
      case 3   : Mag_CastCureMagic(); break; 
      case 4   : Mag_CastWarpMagic(); break; 
      case 5   : Mag_CastManaMagic(); break; 
      default  : break; 
   } 
 
   ent->cmd = 0; 
} 
 
void Think_RunActions(void) 
{ 
   int i, x, y, f, g, d; 
 
   for (i = 0; i < nextobj; i++) 
   { 
      self = &obj[i]; 
 
      if (self->msl && !self->dumb) 
         Think_MissileThink(); 
   } 
 
   for (i = 0; i < nextobj; i++) 
   { 
      g = 0; 
 
      if (obj[i].dumb != 0) 
      { 
         d = obj[i].dumb; 
         obj[i].pos_x = obj[d].pos_x; 
         obj[i].pos_y = obj[d].pos_y - 1; 
         obj[i].attack = obj[d].attack; 
         obj[i].frame = obj[d].frame; 
         obj[i].facing = obj[d].facing; 
         obj[i].offset = obj[d].offset; 
      } 
      else if (obj[i].type != OBJ_NULL) 
      { 
         f = obj[i].facing; 
         if (f >= 40) 
            f -= 40; 
         else if (f >= 30) 
            f -= 30; 
         else if (f >= 20) 
            f -= 20; 
         else if (f >= 10) 
            f -= 10; 
    
         if (obj[i].offset) 
         { 
            if (obj[i].facing >= 10) 
            { 
               obj[i].offset -= (obj[i].speed) * (TILE_SIZE/16); 
               obj[i].frame = (int)(PFRAME + f/3); 
            } 
            else 
            { 
               obj[i].offset -= obj[i].speed * (TILE_SIZE/16); 
               if (obj[i].offset < TILE_SIZE/2) 
                  obj[i].frame = f + 1; 
               else 
                  obj[i].frame = f; 
            } 
            obj[i].attack = 0; 
            obj[i].cmd = 0; 
            g = 1; 
         } 
 
         if (obj[i].attack > 0) 
         { 
            obj[i].attack -= obj[i].speed; 
            if (obj[i].attack < 24 || obj[i].attack > 8) 
               obj[i].frame = f + 2; 
            else 
               obj[i].frame = f; 
            obj[i].offset = 0; 
            obj[i].cmd = 0; 
            if (obj[i].attack == 0) 
               obj[i].attack = -32; 
            g = 1; 
         } 
 
         if (obj[i].cast > 0) 
         { 
            obj[i].cast--; 
            if (obj[i].cast < 64 && obj[i].cast > 48) 
               obj[i].frame = f + 2; 
            else 
               obj[i].frame = f; 
            if (obj[i].cmd == CMD_MAGIC) 
            { 
               obj[i].cmd = 0; 
               g = 1; 
            } 
         } 
 
         if (obj[i].type == 99) 
         { 
            g = 1; 
            obj[i].offset = 0; obj[i].attack = 0; 
            obj[i].frame = (int)(PFRAME + f/3); 
         }     
 
         if (!g) 
         { 
            if (obj[i].attack < 0) 
               obj[i].attack+=obj[i].speed; 
    
            obj[i].facing = obj[i].frame = f; 
    
            if (i == 0) 
               pflag = TRUE; 
 
            switch (obj[i].cmd) 
            { 
               case (CMD_UP)     :  if (!Obj_CheckSolid(obj[i].pos_x, obj[i].pos_y-1, i)) 
                                    { 
                                       obj[i].pos_y--; 
                                       obj[i].offset = TILE_SIZE; 
                                    } 
                                    if (!obj[i].pain) 
                                       obj[i].facing = FACE_BACK; 
                                    else 
                                       obj[i].facing += PAIN_BACK; 
                                    break; 
               case (CMD_DOWN)   :  if (!Obj_CheckSolid(obj[i].pos_x, obj[i].pos_y+1, i)) 
                                    { 
                                       obj[i].pos_y++; 
                                       obj[i].offset = TILE_SIZE; 
                                    } 
                                    if (!obj[i].pain) 
                                       obj[i].facing = FACE_FRONT; 
                                    else if (obj[i].offset) 
                                       obj[i].facing += PAIN_FRONT; 
                                    break; 
               case (CMD_LEFT)   :  if (!Obj_CheckSolid(obj[i].pos_x-1, obj[i].pos_y, i)) 
                                    { 
                                       obj[i].pos_x--; 
                                       obj[i].offset = TILE_SIZE; 
                                    } 
                                    if (!obj[i].pain) 
                                       obj[i].facing = FACE_LEFT; 
                                    else if (obj[i].offset) 
                                       obj[i].facing += PAIN_LEFT; 
                                    break; 
               case (CMD_RIGHT)  :  if (!Obj_CheckSolid(obj[i].pos_x+1, obj[i].pos_y, i)) 
                                    { 
                                       obj[i].pos_x++; 
                                       obj[i].offset = TILE_SIZE; 
                                    } 
                                    if (!obj[i].pain) 
                                       obj[i].facing = FACE_RIGHT; 
                                    else if (obj[i].offset) 
                                       obj[i].facing += PAIN_RIGHT; 
                                    break; 
               case (CMD_ATTACK) :  if (obj[i].attack < 0) 
                                       break; 
                                    obj[i].attack = 32; 
                                    x = obj[i].pos_x; y = obj[i].pos_y; 
                                    if (obj[i].facing == FACE_FRONT) 
                                       y++; 
                                    if (obj[i].facing == FACE_BACK) 
                                       y--; 
                                    if (obj[i].facing == FACE_LEFT) 
                                       x--; 
                                    if (obj[i].facing == FACE_RIGHT) 
                                       x++; 
                                    Obj_Attack(&obj[i], x, y); 
                                    break; 
               case (CMD_MTAUR_ATTACK) : 
                                    if (obj[i].attack < 0 || obj[i].type != OBJ_MINOTAUR) 
                                       break; 
                                    obj[i].attack = 64; 
                                    x = obj[i].pos_x; y = obj[i].pos_y; 
                                    if (obj[i].facing == FACE_FRONT) 
                                    {  Obj_Attack(&obj[i], x, y+1); } 
                                    if (obj[i].facing == FACE_BACK) 
                                    {  Obj_Attack(&obj[i], x, y-2); } 
                                    if (obj[i].facing == FACE_LEFT) 
                                    {  Obj_Attack(&obj[i], x-1, y); Obj_Attack(&obj[i], x-1, y-1); } 
                                    if (obj[i].facing == FACE_RIGHT) 
                                    {  Obj_Attack(&obj[i], x+1, y); Obj_Attack(&obj[i], x+1, y-1); } 
                                    break;                                        
               case (CMD_MAGIC)  :  Mag_CastSpell(&obj[i]); break; 
               case (CMD_SHOOT)  :  if (!obj[i].msl && obj[i].attack >= 0) 
                                    {
                                    Sound_PlaySound(obj[i].snd_attack);
                                    obj[i].msl = TRUE; 
                                    obj[i].attack = 32; 
                                    obj[i].msl_facing = obj[i].facing; 
                                    obj[i].msl_offset = TILE_SIZE; 
                                    obj[i].msl_pos_x = obj[i].pos_x; 
                                    obj[i].msl_pos_y = obj[i].pos_y; 
                                    if (obj[i].facing == FACE_FRONT) obj[i].msl_pos_y++; 
                                    if (obj[i].facing == FACE_BACK) obj[i].msl_pos_y--; 
                                    if (obj[i].facing == FACE_LEFT) obj[i].msl_pos_x--; 
                                    if (obj[i].facing == FACE_RIGHT) obj[i].msl_pos_x++; 
                                    } 
                                    break; 
               default           : break; 
            } 
 
            obj[i].cmd = 0; 
            obj[i].pain = 0; 
    
            pflag = FALSE; 
         } 
      } 
   } 
} 
 
int Obj_MinotaurCanAttack(void) 
{ 
   switch (self->facing) 
   { 
      case FACE_FRONT   : if (obj[0].pos_y == self->pos_y + 1 && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_BACK    : if (obj[0].pos_y == self->pos_y - 2 && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_LEFT    : if ((obj[0].pos_y == self->pos_y || obj[0].pos_y == self->pos_y - 1) && obj[0].pos_x == self->pos_x - 1) return TRUE; break; 
      case FACE_RIGHT   : if ((obj[0].pos_y == self->pos_y || obj[0].pos_y == self->pos_y - 1) && obj[0].pos_x == self->pos_x + 1) return TRUE; break; 
      default           : break; 
   } 
 
   return FALSE; 
} 
 
int Obj_CanAttack(void) 
{ 
   switch (self->facing) 
   { 
      case FACE_FRONT   : if (obj[0].pos_y == self->pos_y + 1 && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_BACK    : if (obj[0].pos_y == self->pos_y - 1 && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_LEFT    : if (obj[0].pos_y == self->pos_y && obj[0].pos_x == self->pos_x - 1) return TRUE; break; 
      case FACE_RIGHT   : if (obj[0].pos_y == self->pos_y && obj[0].pos_x == self->pos_x + 1) return TRUE; break; 
      default           : break; 
   } 
 
   return FALSE; 
} 
 
void Think_DoubleThink(void) 
{ 
   int r; 
   int c[4]; 
 
   if (self->offset) 
   { 
      self->cmd = 0; 
      return; 
   } 
 
   r = rand() % 100; 
 
   if (r < 5) 
   { 
      if (Obj_CanAttack()) 
         self->cmd = CMD_MTAUR_ATTACK; 
      else 
      { 
         c[0] = c[1] = c[2] = c[3] = FALSE; 
         if (obj[0].pos_x < self->pos_x) 
            c[2] = TRUE; // CMD_LEFT 
         if (obj[0].pos_x > self->pos_x) 
            c[3] = TRUE; // CMD_RIGHT 
         if (obj[0].pos_y > self->pos_y) 
            c[0] = TRUE; // CMD_DOWN 
         if (obj[0].pos_y < self->pos_y) 
            c[1] = TRUE; // CMD_UP 
 
         self->cmd = 0; 
         while (!self->cmd) 
         { 
            r = (int)(rand()%4); 
            if (c[r]) 
               self->cmd = r+1; 
         } 
      } 
   } 
   else 
      self->cmd = 0; 
} 
 
void Think_GruntThink(void) 
{ 
   int r; 
   int c[4]; 
 
   if (self->offset) 
   { 
      self->cmd = 0; 
      return; 
   } 
 
   r = rand() % 100; 
 
   if (r < 5) 
   { 
      if (Obj_CanAttack()) 
         self->cmd = CMD_ATTACK; 
      else 
      { 
         c[0] = c[1] = c[2] = c[3] = FALSE; 
         if (obj[0].pos_x < self->pos_x || r == 1) 
            c[2] = TRUE; // CMD_LEFT 
         if (obj[0].pos_x > self->pos_x || r == 2) 
            c[3] = TRUE; // CMD_RIGHT 
         if (obj[0].pos_y > self->pos_y || r == 3) 
            c[0] = TRUE; // CMD_DOWN 
         if (obj[0].pos_y < self->pos_y || r == 4) 
            c[1] = TRUE; // CMD_UP 
 
         self->cmd = 0; 
         while (!self->cmd) 
         { 
            r = (int)(rand()%4); 
            if (c[r]) 
               self->cmd = r+1; 
         } 
      } 
   } 
   else 
      self->cmd = 0; 
} 
 
void Think_MinotaurThink(void) 
{ 
   int r, j; 
   int c[4]; 
 
   if (music != "boss_l1.mid") 
   { 
      Sound_StopMidi();       
      Sound_PlayMidi("boss_l1.mid");
   } 
 
   if (self->offset) 
   { 
      self->cmd = 0; 
      return; 
   } 
 
   r = rand() % 100; 
 
   if (r < 5) 
   { 
      if (Obj_MinotaurCanAttack()) 
         self->cmd = CMD_ATTACK; 
      else 
      { 
         c[0] = c[1] = c[2] = c[3] = FALSE; 
         if (obj[0].pos_x < self->pos_x) 
            c[2] = TRUE; // CMD_LEFT 
         if (obj[0].pos_x > self->pos_x) 
            c[3] = TRUE; // CMD_RIGHT 
         if (obj[0].pos_y > self->pos_y) 
            c[0] = TRUE; // CMD_DOWN 
         if (obj[0].pos_y < self->pos_y) 
            c[1] = TRUE; // CMD_UP 
 
         self->cmd = 0; 
         while (!self->cmd) 
         { 
            r = (int)(rand()%4); 
            if (c[r]) 
               self->cmd = r+1; 
         } 
      } 
   } 
   else 
      self->cmd = 0; 
} 
 
 
#define OBJ_STUNNED 99 
 
int Obj_CanShoot(void) 
{ 
   if (self->msl) 
      return FALSE; 
 
   switch (self->facing) 
   { 
      case FACE_FRONT   : if (obj[0].pos_y > self->pos_y && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_BACK    : if (obj[0].pos_y < self->pos_y && obj[0].pos_x == self->pos_x) return TRUE; break; 
      case FACE_LEFT    : if (obj[0].pos_y == self->pos_y && obj[0].pos_x < self->pos_x) return TRUE; break; 
      case FACE_RIGHT   : if (obj[0].pos_y == self->pos_y && obj[0].pos_x > self->pos_x) return TRUE; break; 
      default           : break; 
   } 
 
   return FALSE; 
} 
 
 
void Think_MissileThink(void) 
{ 
   self->msl_offset -= self->msl_speed * (TILE_SIZE/16); 
 
   if (Obj_CheckSolid(self->msl_pos_x, self->msl_pos_y, 0)) 
      self->msl = 0; 
 
   Obj_MslAttack(self, self->msl_pos_x, self->msl_pos_y); 
 
   if (self->msl_offset <= 0) 
   {        
      if (self->msl_facing == FACE_FRONT) 
         self->msl_pos_y++; 
      else if (self->msl_facing == FACE_BACK) 
         self->msl_pos_y--; 
      else if (self->msl_facing == FACE_LEFT) 
         self->msl_pos_x--; 
      else if (self->msl_facing == FACE_RIGHT) 
         self->msl_pos_x++; 
 
      self->msl_offset = TILE_SIZE; 
   } 
} 
 
void Think_ArcherThink(void) 
{ 
   int r; 
   int c[4]; 
 
   if (self->offset) 
   { 
      self->cmd = 0; 
      return; 
   } 
 
   r = rand() % 100; 
 
   if (r < 5) 
   { 
      if (Obj_CanShoot()) 
         self->cmd = CMD_SHOOT; 
      else 
      { 
         c[0] = c[1] = c[2] = c[3] = FALSE; 
         if (obj[0].pos_x < self->pos_x) 
            c[2] = TRUE; // CMD_LEFT 
         if (obj[0].pos_x > self->pos_x) 
            c[3] = TRUE; // CMD_RIGHT 
         if (obj[0].pos_y > self->pos_y) 
            c[0] = TRUE; // CMD_DOWN 
         if (obj[0].pos_y < self->pos_y) 
            c[1] = TRUE; // CMD_UP 
 
         self->cmd = 0; 
         while (!self->cmd) 
         { 
            r = (int)(rand()%4); 
            if (c[r]) 
               self->cmd = r+1; 
         } 
      } 
   } 
   else 
      self->cmd = 0; 
 
} 
 
void Think_DogbertThink() 
{ 
   if (obj[0].pos_y == 3 && (obj[0].pos_x == 9 || obj[0].pos_x == 10)) 
   { 
      Screen_DialogBox("Remember my wisdom, Mike...\n\nHonesty means never having to say \"Please don't flush me down the toilet.\"");
      //Screen_DialogBox("Well done, Mike, but I'm afraid your quest is over for now. This demo is only a fraction of Tilegame. Remember, honesty means never having to say \"Please don't flush me down the toilet.\""); 
      //fade_out(1); 
      Screen_FadeOut();
      Screen_CreditsScreen(); 
   } 
} 
 
void Think_VThink() 
{ 
   int r; 
   int i; 
   int b; 
   int n; 
   int d; 
 
   if (self->offset) 
   { 
      self->cmd = 0; 
      return; 
   } 
 
   r = rand() % 100; 
 
  if (r < 4)
  {

   if (Obj_CanShoot()) 
      self->cmd = CMD_SHOOT;      
   else
   { 
      n = 0; 
 
      r = rand()%5; 
 
      if (r == 0) 
      { 
         b = FALSE; 
 
         d = 0; 
 
         for (i = 0; i < MAXOBJ; i++) 
         { 
            if (obj[i].type == -1 && !Obj_CheckSolid(obj[i].pos_x, obj[i].pos_y+1, 0) && !b) 
            { 
                  // resurrect vry's bat 
                  obj[i].type = OBJ_BAT; 
                  Obj_SetupBat(&obj[i]); 
 
                  b = TRUE;     // break out of loop 
            } 
         } 
 
         if (!b) 
         { 
            for (i = 0; i < MAXOBJ; i++) 
               if (obj[i].type == OBJ_BAT) 
                  n++; 
 
            if (!n && !Obj_CheckSolid(self->pos_x, self->pos_y + 2, 0)) 
            { 
               self->facing = FACE_FRONT; 
               for (i = 0; i < MAXOBJ; i++) 
                  if (obj[i].type == 0 && !n) 
                     n = i; 
 
               obj[n].type = OBJ_BAT; 
               obj[n].pos_x = self->pos_x; 
               obj[n].pos_y = self->pos_y + 2; 
               Obj_SetupBat(&obj[n]); 
            } 
         } 
 
 
         self->cmd = CMD_ATTACK; 
      }
      else if (r == 1) 
         self->cmd = CMD_UP; 
      else if (r == 2) 
         self->cmd = CMD_DOWN; 
      else if (r == 3) 
         self->cmd = CMD_LEFT; 
      else if (r == 4) 
         self->cmd = CMD_RIGHT; 
   }
  }
} 
 
void Think_RunThinks(void) 
{
   int n;
   int i; 

   if (tg_pause)
      n = 1;
   else
      n = nextobj;

   for (i = 0; i < n; i++)
   { 
      self = &obj[i]; 
     
      switch (self->type) 
      { 
         case (OBJ_PLAYER) : Think_PlayerThink(); break; 
         case (OBJ_SLIME) : Think_GruntThink(); break; 
         case (OBJ_DARKELF) : Think_ArcherThink(); break; 
         case (OBJ_GOBLIN): Think_ArcherThink(); break; 
         case (OBJ_ORC): Think_GruntThink(); break; 
         case (OBJ_BAT): Think_GruntThink(); break; 
         case (OBJ_OOZE): Think_GruntThink(); break; 
         case (OBJ_KNIGHT): Think_GruntThink(); break; 
         case (OBJ_GHOST): Think_GruntThink(); break; 
         case (OBJ_DOUBLE) : Think_DoubleThink(); break; 
         case (OBJ_STUNNED) : self->cmd = 0; break; 
         case (OBJ_MINOTAUR) : Think_MinotaurThink(); break; 
         case (OBJ_V): Think_VThink(); break; 
         case (OBJ_DOGBERT) : Think_DogbertThink(); break; 
         default : break; 
      } 
   } 
} 
 
void Sys_SaveGame(const string& fname, int snum) 
{ 
   FILE *sfile;
   int i; 
 
   sfile = fopen(fname.c_str(), "wb");

   save[snum].area = prt; 
   save[snum].hearts = (obj[0].max_life / 2) - 4; 
   save[snum].stars = (obj[0].max_mana / 2) - 4; 
 
   fwrite(&save[snum], sizeof(save_t), 1, sfile);
   for (i = 0; i < MAXOBJ; i++) 
      fwrite(&obj[i], sizeof(obj_t), 1, sfile);
     
   fwrite(&world, sizeof(world_t), 1, sfile); 
 
   fclose(sfile);
} 
 
void Sys_LoadGame(const string& fname, int snum) 
{ 
   int i; 
 
   FILE *sfile;
 
   sfile = fopen(fname.c_str(), "rb");
 
   if (!sfile) 
   { 
      Sys_NewGame(); 
      return; 
   } 
 
   fread(&save[snum], sizeof(save_t), 1, sfile);
 
   Obj_SetupPlayer(&obj[0]); 
   prt = save[snum].area; 
   Map_Teleport(&portal[save[snum].area]); 
 
   for (i = 0; i < MAXOBJ; i++) 
      fread(&obj[i], sizeof(obj_t), 1, sfile);
     
   fread(&world, sizeof(world_t), 1, sfile); 
 
   fclose(sfile);
} 
  
void Con_Command(const string& cmdtext)
{
   if (!strcmp(cmdtext.c_str(), "/cereal"))
   {
      char txt[256];

      cheat = !cheat;
      sprintf(txt, "Mike's Lucky Chars %s.", cheat ? "enabled" : "disabled");
      Txt_Message(txt);
   }

   if (!strncmp(cmdtext.c_str(), "/say", 4))
   {
      char txt[256];

      sprintf(txt, "Mike: %s", cmdtext.c_str()+5);
      Txt_Message(txt);
   }

   if (!strncmp(cmdtext.c_str(), "/me", 3))
   {
      char txt[256];

      sprintf(txt, "* Mike %s", cmdtext.c_str()+4);
      Txt_Message(txt);
   }

   if (!strncmp(cmdtext.c_str(), "/spawn", 6))
   {
      int i = atoi(cmdtext.c_str()+6);
      int x = obj[0].pos_x;
      int y = obj[0].pos_y;

      switch (obj[0].facing)
      {
         case FACE_FRONT: y++; break;
         case FACE_BACK: y--; break;
         case FACE_LEFT: x--; break;
         case FACE_RIGHT: x++; break;
      }

      if (i < 0 || i > 255)
         Txt_Message("Invalid object type.");

      nextobj = MAXOBJ - 1;
      Obj_AddObject(i, x, y);
   }

   if (!strncmp(cmdtext.c_str(), "/poly", 5))
   {
      string fname = cmdtext.c_str()+6;
      
      if (fname == "")
         Txt_Message("File not found.");

      obj[0].sprite = fname;
      Txt_Message(fname);
   }

   if (!strncmp(cmdtext.c_str(), "/save", 5))
   {
      Sys_SaveGame(cmdtext.c_str()+6, 6);
      Txt_Message("Game saved.");
   }

   if (!strncmp(cmdtext.c_str(), "/load", 5))
   {
      Sys_LoadGame(cmdtext.c_str()+6, 6);
      Txt_Message("Game loaded.");
   }

   if (!strncmp(cmdtext.c_str(), "/gold", 5))
   {
      obj[0].gold += atoi(cmdtext.c_str()+5);
      Txt_Message("Cha-ching!");
   }

   if (!strncmp(cmdtext.c_str(), "/trans", 6))
   {
      obj[0].trans = !obj[0].trans;
      Txt_Message("OooOo ooOo OooOo OOooo");
   }

   if (!strncmp(cmdtext.c_str(), "/map", 4))
   {
      Map_LoadMap(cmdtext.c_str()+5);
      Txt_Message("Map loaded.");
   }

   if (!strncmp(cmdtext.c_str(), "/x", 2))
      obj[0].pos_x += atoi(cmdtext.c_str()+3);

   if (!strncmp(cmdtext.c_str(), "/y", 2))
      obj[0].pos_y += atoi(cmdtext.c_str()+3);

   if (!stricmp(cmdtext.c_str(), "/stop"))
   {
      tg_pause = !tg_pause;
      Txt_Message("The flow of time is disrupted...");
   }     
}

void Game_Init()
{
   Map_LoadPortals(); 
 
   Screen_ClearBuffer();
   Screen_CopyBuffer(); 
}

void Game_Frame()
{
    Think_RunThinks(); 
   Think_RunActions(); 
   Screen_DrawFrame(); 
   Sys_CheckGameOver(); 
}

void Game_Shutdown()
{
} 
 
