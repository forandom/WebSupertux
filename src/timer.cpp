//  $Id: timer.cpp 1081 2004-05-10 20:00:22Z tobgle $
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

#include "SDL.h"
#include "defines.h"
#include "timer.h"

unsigned int st_pause_ticks, st_pause_count;

unsigned int st_get_ticks(void)
{
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
  if(st_pause_count != 0)
    return /*SDL_GetTicks()*/ - st_pause_ticks /*- SDL_GetTicks()*/ + st_pause_count;
  else
    return SDL_GetTicks() - st_pause_ticks;
}

void st_pause_ticks_init(void)
{
  st_pause_ticks = 0;
  st_pause_count = 0;
}

void st_pause_ticks_start(void)
{
  if(st_pause_count == 0)
    st_pause_count = SDL_GetTicks();
}

void st_pause_ticks_stop(void)
{
if(st_pause_count == 0)
return;

  st_pause_ticks += SDL_GetTicks() - st_pause_count;
  st_pause_count = 0;
}

bool st_pause_ticks_started(void)
{
if(st_pause_count == 0)
return false;
else
return true;
}

Timer::Timer()
{
  init(true);
}

void
Timer::init(bool st_ticks)
{
  period    = 0;
  time      = 0;
  //get_ticks = st_ticks ? st_get_ticks : SDL_GetTicks;
  use_st_ticks = st_ticks;
}

void
Timer::start(unsigned int period_)
{
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
  //pxx
  //time   = get_ticks();
  if (use_st_ticks)
    time = st_get_ticks();
  else
    time = SDL_GetTicks();
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
  period = period_;
}

void
Timer::stop()
{
  //pxx
  //if(get_ticks == st_get_ticks)
  if(use_st_ticks)
    init(true);
  else
    init(false);
}

int
Timer::check()
{
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
  unsigned int tmp;
  if (use_st_ticks)
    tmp = st_get_ticks();
  else
    tmp = SDL_GetTicks();
  if((time != 0) && (time + period > tmp))
{
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
    return true;
}
  else
    {
//  printf("pxx: %s, %s, %d\n", __FILE__, __FUNCTION__, __LINE__);
      time = 0;
      return false;
    }
}

int
Timer::started()
{
  if(time != 0)
    return true;
  else
    return false;
}

int
Timer::get_left()
{
  if(use_st_ticks)
    return (period - (st_get_ticks() - time));
  else
    return (period - (SDL_GetTicks() - time));
}

int
Timer::get_gone()
{
  if(use_st_ticks)
    return (st_get_ticks() - time);
  else
    return (SDL_GetTicks() - time);
}

//void
//Timer::fwrite(FILE* fi)
//{
//  unsigned int diff_ticks;
//  int tick_mode;
//  if(time != 0)
//    diff_ticks = get_ticks() - time;
//  else
//    diff_ticks = 0;
//
//  ::fwrite(&period,sizeof(unsigned int),1,fi);
//  ::fwrite(&diff_ticks,sizeof(unsigned int),1,fi);
//  if(get_ticks == st_get_ticks)
//      tick_mode = true;
//  else
//      tick_mode = false;
//  ::fwrite(&tick_mode,sizeof(unsigned int),1,fi);
//}
//
//void
//Timer::fread(FILE* fi)
//{
//  unsigned int diff_ticks;
//  int tick_mode;
//
//  ::fread(&period,sizeof(unsigned int),1,fi);
//  ::fread(&diff_ticks,sizeof(unsigned int),1,fi);
//  ::fread(&tick_mode,sizeof(unsigned int),1,fi);
//
//  if (tick_mode)
//    get_ticks = st_get_ticks;
//  else
//    get_ticks = SDL_GetTicks;
//
//  if (diff_ticks != 0)
//    time = get_ticks() - diff_ticks;
//  else
//    time = 0;
//
//}

/* EOF */
