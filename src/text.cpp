//  $Id: text.cpp 1016 2004-05-07 00:20:29Z rmcruz $
// 
//  SuperTux
//  Copyright (C) 2004 Tobias Glaesser <tobi.web@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.

#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "defines.h"
#include "screen.h"
#include "text.h"
#include "title.h"
#include "resources.h"

Text::Text(const std::string& file, int kind_, int w_, int h_)
{
  kind = kind_;
  w = w_;
  h = h_;

  int mx, my;
  SDL_Surface *conv;
  int pixels;
  int i;
  
  if(kind == TEXT_TEXT)
    {
      mx = 26;
      my = 3;
    }
  else if(kind == TEXT_NUM)
    {
      mx = 10;
      my = 1;
    }
  else
    {
      mx = 0;
      my = 0;
    }
//  //printf("pxx: new surface from %s\n", file.c_str());
  chars = new Surface(file, USE_ALPHA);

  // Load shadow font.
  conv = SDL_DisplayFormatAlpha(chars->impl->get_sdl_surface());
  pixels = conv->w * conv->h;
  SDL_LockSurface(conv);
  for(i = 0; i < pixels; ++i)
    {
      Uint32 *p = (Uint32 *)conv->pixels + i;
      *p = *p & conv->format->Amask;
    }
  SDL_UnlockSurface(conv);
  SDL_SetAlpha(conv, SDL_SRCALPHA, 128);
  shadow_chars = new Surface(conv, USE_ALPHA);

  SDL_FreeSurface(conv);
}

Text::~Text()
{
  delete chars;
  delete shadow_chars;
}

void
Text::draw(const  char* text, int x, int y, int shadowsize, int update)
{
  if(text != NULL)
    {
      if(shadowsize != 0)
        draw_chars(shadow_chars, text,x+shadowsize,y+shadowsize, update);

      draw_chars(chars, text,x,y, update);
    }
}

void
Text::draw_chars(Surface* pchars,const  char* text, int x, int y, int update)
{
  int i,j,len;

  len = strlen(text);
  int w = this->w;
  int h = this->h;

  if(kind == TEXT_TEXT)
    {
      for( i = 0, j = 0; i < len; ++i,++j)
        {
          if( text[i] >= ' ' && text[i] <= '/')
            pchars->draw_part((int)(text[i] - ' ')*w,  0 , x+(j*w), y, w, h, 255,  update);
          else if( text[i] >= '0' && text[i] <= '?')
            pchars->draw_part((int)(text[i] - '0')*w, h*1, x+(j*w), y, w, h, 255,  update);
          else if ( text[i] >= '@' && text[i] <= 'O')
            pchars->draw_part((int)(text[i] - '@')*w, h*2, x+(j*w), y, w, h, 255,  update);
          else if ( text[i] >= 'P' && text[i] <= '_')
            pchars->draw_part((int)(text[i] - 'P')*w, h*3, x+(j*w), y, w, h, 255,  update);
          else if ( text[i] >= '`' && text[i] <= 'o')
            pchars->draw_part((int)(text[i] - '`')*w, h*4, x+(j*w), y, w, h, 255,  update);
          else if ( text[i] >= 'p' && text[i] <= '~')
            pchars->draw_part((int)(text[i] - 'p')*w, h*5, x+(j*w), y, w, h, 255,  update);
          else if ( text[i] == '\n')
            {
              y += h + 2;
              j = 0;
            }
        }
    }
  else if(kind == TEXT_NUM)
    {
      for( i = 0, j = 0; i < len; ++i, ++j)
        {
          if ( text[i] >= '0' && text[i] <= '9')
            pchars->draw_part((int)(text[i] - '0')*w, 0, x+(j*w), y, w, h, 255, update);
          else if ( text[i] == '\n')
            {
              y += h + 2;
              j = 0;
            }
        }
    }
}

void
Text::draw_align(const char* text, int x, int y,
                      TextHAlign halign, TextVAlign valign, int shadowsize, int update)
{
  if(text != NULL)
    {
      switch (halign)
        {
        case A_RIGHT:
          x += -(strlen(text)*w);
          break;
        case A_HMIDDLE:
          x += -((strlen(text)*w)/2);
          break;
        case A_LEFT:
          // default
          break;
        }

      switch (valign)
        {
        case A_BOTTOM:
          y -= h;
          break;
          
        case A_VMIDDLE:
          y -= h/2;

        case A_TOP:
          // default
          break;
        }

      draw(text, x, y, shadowsize, update);
    }
}

void
Text::drawf(const  char* text, int x, int y,
                 TextHAlign halign, TextVAlign valign, int shadowsize, int update)
{
  if(text != NULL)
    {
      if(halign == A_RIGHT)  /* FIXME: this doesn't work correctly for strings with newlines.*/
        x += screen->w - (strlen(text)*w);
      else if(halign == A_HMIDDLE)
        x += screen->w/2 - ((strlen(text)*w)/2);

      if(valign == A_BOTTOM)
        y += screen->h - h;
      else if(valign == A_VMIDDLE)
        y += screen->h/2 - h/2;

      draw(text,x,y,shadowsize, update);
    }
}

/* --- ERASE TEXT: --- */

void
Text::erasetext(const  char * text, int x, int y, Surface * ptexture, int update, int shadowsize)
{
  SDL_Rect dest;

  dest.x = x;
  dest.y = y;
  dest.w = strlen(text) * w + shadowsize;
  dest.h = h;

  if (dest.w > screen->w)
    dest.w = screen->w;

  ptexture->draw_part(dest.x,dest.y,dest.x,dest.y,dest.w,dest.h, 255, update);

  if (update == UPDATE)
    update_rect(screen, dest.x, dest.y, dest.w, dest.h);
}


/* --- ERASE CENTERED TEXT: --- */

void
Text::erasecenteredtext(const  char * text, int y, Surface * ptexture, int update, int shadowsize)
{
  erasetext(text, screen->w / 2 - (strlen(text) * 8), y, ptexture, update, shadowsize);
}


/* --- SCROLL TEXT FUNCTION --- */

#define MAX_VEL     10
#define SPEED_INC   0.01
#define SCROLL      60
#define ITEMS_SPACE 4

static int display_text_done;
static int display_text_length;
static int display_text_surface_allocated;
static Uint32 display_text_lastticks;
static float display_text_speed;
static float display_text_scroll;
static Surface* display_text_surface;
static string_list_type display_text_names;

void display_text_file(const std::string& file, const std::string& surface, float scroll_speed)
{
  Surface* sur = new Surface(datadir + surface, IGNORE_ALPHA);
  display_text_surface_allocated = 1;
  display_text_file(file, sur, scroll_speed);
  //delete sur;
}

void display_text_loop()
{
  /* in case of input, exit */
  SDL_Event event;
  while(SDL_PollEvent(&event))
    switch(event.type)
      {
      case SDL_KEYDOWN:
	switch(event.key.keysym.sym)
	  {
	  case SDLK_UP:
	    display_text_speed -= SPEED_INC;
	    break;
	  case SDLK_DOWN:
	    display_text_speed += SPEED_INC;
	    break;
	  case SDLK_SPACE:
	  case SDLK_RETURN:
	    if(display_text_speed >= 0)
	      display_text_scroll += SCROLL;
	    break;
	  case SDLK_ESCAPE:
	    display_text_done = 1;
	    break;
	  default:
	    break;
	  }
	break;
      case SDL_QUIT:
	display_text_done = 1;
	break;
      default:
	break;
      }

  if(display_text_speed > MAX_VEL)
    display_text_speed = MAX_VEL;
  else if(display_text_speed < -MAX_VEL)
    display_text_speed = -MAX_VEL;

  /* draw the credits */
  display_text_surface->draw_bg();

  int y = 0;
  for(int i = 0; i < display_text_length; i++)
    {
      switch(display_text_names.item[i][0])
	{
	case ' ':
	  white_small_text->drawf(display_text_names.item[i]+1, 0, screen->h+y-int(display_text_scroll),
				  A_HMIDDLE, A_TOP, 1);
	  y += white_small_text->h+ITEMS_SPACE;
	  break;
	case '	':
	  white_text->drawf(display_text_names.item[i]+1, 0, screen->h+y-int(display_text_scroll),
			    A_HMIDDLE, A_TOP, 1);
	  y += white_text->h+ITEMS_SPACE;
	  break;
	case '-':
	  white_big_text->drawf(display_text_names.item[i]+1, 0, screen->h+y-int(display_text_scroll),
				A_HMIDDLE, A_TOP, 3);
	  y += white_big_text->h+ITEMS_SPACE;
	  break;
	default:
	  blue_text->drawf(display_text_names.item[i], 0, screen->h+y-int(display_text_scroll),
			   A_HMIDDLE, A_TOP, 1);
	  y += blue_text->h+ITEMS_SPACE;
	  break;
	}
    }

  flipscreen();

  if(screen->h+y-display_text_scroll < 0 && 20+screen->h+y-display_text_scroll < 0)
    display_text_done = 1;

  Uint32 ticks = SDL_GetTicks();
  display_text_scroll += display_text_speed * (ticks - display_text_lastticks);
  display_text_lastticks = ticks;
  if(display_text_scroll < 0)
    display_text_scroll = 0;

  //pxx
  //SDL_Delay(10);
  if (display_text_done)
    {
      SDL_EnableKeyRepeat(0, 0);    // disables key repeating
      Menu::set_current(main_menu);
      music_manager->halt_music();
      if (display_text_surface_allocated)
	{
	  display_text_surface_allocated = 0;
	  delete display_text_surface;
	}
      title();
      emscripten_set_main_loop(title_loop, 100);
    }
}

void display_text_file(const std::string& file, Surface* surface, float scroll_speed)
{
  FILE* fi;
  char temp[1024];
  char filename[1024];
  string_list_free(&display_text_names);
  string_list_init(&display_text_names);
  sprintf(filename,"%s/%s", datadir.c_str(), file.c_str());
  if((fi = fopen(filename,"r")) != NULL)
    {
      while(fgets(temp, sizeof(temp), fi) != NULL)
        {
          temp[strlen(temp)-1]='\0';
          string_list_add_item(&display_text_names,temp);
        }
      fclose(fi);
    }
  else
    {
      string_list_add_item(&display_text_names,"File was not found!");
      string_list_add_item(&display_text_names,filename);
      string_list_add_item(&display_text_names,"Shame on the guy, who");
      string_list_add_item(&display_text_names,"forgot to include it");
      string_list_add_item(&display_text_names,"in your SuperTux distribution.");
    }


  display_text_scroll = 0;
  display_text_speed = scroll_speed / 50;
  display_text_done = 0;
  display_text_length = display_text_names.num_items;
  display_text_surface = surface;

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  display_text_lastticks = SDL_GetTicks();
  emscripten_set_main_loop(display_text_loop, 100);
//  while(done == 0)
//    {
//      /* in case of input, exit */
//      SDL_Event event;
//      while(SDL_PollEvent(&event))
//        switch(event.type)
//          {
//          case SDL_KEYDOWN:
//            switch(event.key.keysym.sym)
//              {
//              case SDLK_UP:
//                speed -= SPEED_INC;
//                break;
//              case SDLK_DOWN:
//                speed += SPEED_INC;
//                break;
//              case SDLK_SPACE:
//              case SDLK_RETURN:
//                if(speed >= 0)
//                  scroll += SCROLL;
//                break;
//              case SDLK_ESCAPE:
//                done = 1;
//                break;
//              default:
//                break;
//              }
//            break;
//          case SDL_QUIT:
//            done = 1;
//            break;
//          default:
//            break;
//          }
//
//      if(speed > MAX_VEL)
//        speed = MAX_VEL;
//      else if(speed < -MAX_VEL)
//        speed = -MAX_VEL;
//
//      /* draw the credits */
//      surface->draw_bg();
//
//      y = 0;
//      for(int i = 0; i < length; i++)
//        {
//        switch(names.item[i][0])
//          {
//          case ' ':
//            white_small_text->drawf(names.item[i]+1, 0, screen->h+y-int(scroll),
//                A_HMIDDLE, A_TOP, 1);
//            y += white_small_text->h+ITEMS_SPACE;
//            break;
//          case '	':
//            white_text->drawf(names.item[i]+1, 0, screen->h+y-int(scroll),
//                A_HMIDDLE, A_TOP, 1);
//            y += white_text->h+ITEMS_SPACE;
//            break;
//          case '-':
//            white_big_text->drawf(names.item[i]+1, 0, screen->h+y-int(scroll),
//                A_HMIDDLE, A_TOP, 3);
//            y += white_big_text->h+ITEMS_SPACE;
//            break;
//          default:
//            blue_text->drawf(names.item[i], 0, screen->h+y-int(scroll),
//                A_HMIDDLE, A_TOP, 1);
//            y += blue_text->h+ITEMS_SPACE;
//            break;
//          }
//        }
//
//      flipscreen();
//
//      if(screen->h+y-scroll < 0 && 20+screen->h+y-scroll < 0)
//        done = 1;
//
//      Uint32 ticks = SDL_GetTicks();
//      scroll += speed * (ticks - lastticks);
//      lastticks = ticks;
//      if(scroll < 0)
//        scroll = 0;
//
//      //pxx
//      //SDL_Delay(10);
//    }
//  string_list_free(&names);
//
//  SDL_EnableKeyRepeat(0, 0);    // disables key repeating
//  Menu::set_current(main_menu);
}

