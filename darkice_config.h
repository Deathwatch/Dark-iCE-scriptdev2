/*
 * Copyright (C) 2009-2010 Project Dark-iCE <http://projectdarkice.clanice.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef DARKICE_CONFIG_H
#define DARKICE_CONFIG_H

#include "Platform/CompilerDefs.h"
#include "revision.h"

// Format is YYYYMMDDRR where RR is the change in the conf file
// for that day.
#define DARKICE_CONF_VERSION    2010060701

#ifdef WIN32
  #define MANGOS_DLL_EXPORT extern "C" __declspec(dllexport)
#elif defined( __GNUC__ )
  #define MANGOS_DLL_EXPORT extern "C"
#else
  #define MANGOS_DLL_EXPORT extern "C" export
#endif

#ifndef _VERSION
  #define _VERSION "Revision [" REVISION_ID "] " REVISION_DATE " " REVISION_TIME
#endif

// The path to config files
#ifndef SYSCONFDIR
  #define SYSCONFDIR ""
#endif

#if PLATFORM == PLATFORM_WINDOWS
  #ifdef _WIN64
    #define _FULLVERSION _VERSION " (Win64)"
  #else
    #define _FULLVERSION _VERSION " (Win32)"
  #endif
  #define _DARKICE_CONFIG  "darkice.conf"
#else
  #define _FULLVERSION _VERSION " (Unix)"
  #define _DARKICE_CONFIG  SYSCONFDIR "darkice.conf"
#endif

#endif
