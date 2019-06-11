/*
 * debug.h
 *  Sebastien Couret <sebastien.couret@elios-informatique.fr> November 2003
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEBUG_DHCP_H
#define _DEBUG_DHCP_H


#include <stdio.h>

//#define DEBUGGING
#define LOGACTIVE

#ifdef DEBUGGING
#define LOG_WARNING "WARN"
#define LOG_ERR     "ERR"
#define LOG_INFO    "INFO"
#else
#define LOG_WARNING "WARN"
#define LOG_ERR     "ERR"
#define LOG_INFO    ""
#endif

#ifdef LOGACTIVE
#define LOG(level, function, str, args...) do { if(strlen(level)) { printf("[DHCPELIOS]%s:%s: ",level, function); \
                printf(str, ## args); \
                printf("\n");} } while(0)
#else
#define LOG(level,fnction, str, args...) do {;} while(0)
#endif


#endif
