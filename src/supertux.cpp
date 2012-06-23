//  $Id: supertux.cpp 997 2004-05-05 16:07:20Z rmcruz $
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

#include <sys/types.h>
#include <ctype.h>

#include "defines.h"
#include "globals.h"
#include "setup.h"
#include "intro.h"
#include "title.h"
#include "gameloop.h"
#include "leveleditor.h"
#include "screen.h"
#include "worldmap.h"
#include "resources.h"
#include "texture.h"
#include "tile.h"

int main(int argc, char * argv[])
{
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  st_directory_setup();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  parseargs(argc, argv);
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  
  st_audio_setup();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  st_video_setup();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  st_joystick_setup();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  st_general_setup();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  st_menu();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  loadshared();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);

  //pxx
  level_startup_file = "/home/emscripten/share/supertux/levels/world1/level1.stl"; 
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);

//  if (launch_leveleditor_mode && level_startup_file)
//    {
//    leveleditor(level_startup_file);
//    }
//  else if (level_startup_file)
//    {
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
      GameSession session(level_startup_file, 1, ST_GL_LOAD_LEVEL_FILE);
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
      session.run();
//  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//    }
//  else
//    {  
//      title();
//    }
  
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//  clearscreen(0, 0, 0);
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//  updatescreen();
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//
//  unloadshared();
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//  st_general_free();
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//  TileManager::destroy_instance();
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
//#ifdef DEBUG
//  Surface::debug_check();
//#endif
//  st_shutdown();
////  printf("pxx: %s, %d\n", __FUNCTION__, __LINE__);
  //printf("pxx: %s, %s, %d, %s\n", __FILE__, __FUNCTION__, __LINE__, datadir.c_str());
  
  return 0;
}
