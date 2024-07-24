//Stubbed Video.c

// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------

//static const char
//rcsid[] = "$Id: i_x.c,v 1.6 1997/02/03 22:45:10 b1 Exp $";

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

#define POINTER_WARP_COUNTDOWN	1

//#include "DrawFunctions.c"

#include "vmio.h"
#include "vmgraph.h"

void HandleKey(int keycode, int bDown) {

    int rc;

    switch(keycode)
    {
      case VM_KEY_UP:		rc = KEY_LEFTARROW;	break;
      case VM_KEY_DOWN:		rc = KEY_RIGHTARROW;	break;
      case VM_KEY_LEFT:		rc = KEY_DOWNARROW;	break;
      case VM_KEY_RIGHT:	rc = KEY_UPARROW;	break;
      /*case XK_Escape:	rc = KEY_ESCAPE;	break;
      case XK_Return:	rc = KEY_ENTER;		break;
      case XK_Tab:	rc = KEY_TAB;		break;
      case XK_F1:	rc = KEY_F1;		break;
      case XK_F2:	rc = KEY_F2;		break;
      case XK_F3:	rc = KEY_F3;		break;
      case XK_F4:	rc = KEY_F4;		break;
      case XK_F5:	rc = KEY_F5;		break;
      case XK_F6:	rc = KEY_F6;		break;
      case XK_F7:	rc = KEY_F7;		break;
      case XK_F8:	rc = KEY_F8;		break;
      case XK_F9:	rc = KEY_F9;		break;
      case XK_F10:	rc = KEY_F10;		break;
      case XK_F11:	rc = KEY_F11;		break;
      case XK_F12:	rc = KEY_F12;		break;
	
      case XK_BackSpace:
      case XK_Delete:	rc = KEY_BACKSPACE;	break;

      case XK_Pause:	rc = KEY_PAUSE;		break;

      case XK_KP_Equal:
      case XK_equal:	rc = KEY_EQUALS;	break;

      case XK_KP_Subtract:
      case XK_minus:	rc = KEY_MINUS;		break;

      case XK_Shift_L:
      case XK_Shift_R:
	rc = KEY_RSHIFT;
	break;
	
      case XK_Control_L:
      case XK_Control_R:
	rc = KEY_RCTRL;
	break;
	
      case XK_Alt_L:
      case XK_Meta_L:
      case XK_Alt_R:
      case XK_Meta_R:
	rc = KEY_RALT;
	break;
	
      default:
	if (rc >= XK_space && rc <= XK_asciitilde)
	    rc = rc - XK_space + ' ';
	if (rc >= 'A' && rc <= 'Z')
	    rc = rc - 'A' + 'a';
	break;*/
    }

    event_t event;
	event.type = bDown;//?ev_keydown:ev_keyup; 0 = down, 1 = up
	event.data1 = rc;
	D_PostEvent(&event);
}

static int	lastmousex = 0;
static int	lastmousey = 0;
boolean		mousemoved = false;


int mousemask = 0;

void HandleButton( int x, int y, int button, int bDown )
{
	int dx = x - lastmousex;
	int dy = y - lastmousey;

	if( bDown )
	{
		mousemask |= 1<<button;
	}
	else
	{
		mousemask &= ~(1<<button);
	}

    event_t event;
	event.type = ev_mouse;
	event.data1 = mousemask;
	event.data2 = dx*10;
	event.data3 = dy*10;
	D_PostEvent(&event);

	lastmousex = x;
	lastmousey = y;
	
}

void HandleMotion( int x, int y, int mask )
{
	int dx = x - lastmousex;
	int dy = y - lastmousey;

	mousemask = mask;


    event_t event;
	event.type = ev_mouse;
	event.data1 = mousemask;
	event.data2 = dx*10;
	event.data3 = dy*10;
	D_PostEvent(&event);


	lastmousex = x;
	lastmousey = y;

}



//
// I_SetPalette
//

static byte lpalette[256*3];

void I_SetPalette (byte* palette)
{
	memcpy(lpalette, palette, sizeof( lpalette ));
    //UploadNewPalette(X_cmap, palette);
	
}

#define OUTSCALE 1

//
// I_UpdateNoBlit
//
void I_UpdateNoBlit (void)
{
    // what is this?
}

void I_InitGraphics(void)
{
	//CNFGSetup( "EmbeddedDoom", SCREENWIDTH*OUTSCALE, SCREENHEIGHT*OUTSCALE );
}

void I_StartTic (void)
{
	//CNFGHandleInput();
}
void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH*SCREENHEIGHT);
}
void I_StartFrame()
{
}

void I_ShutdownGraphics(void)
{
	exit(0);
}

extern VMUINT8 *layer_bufs[1];
const int MRE_SCREEN_WIDTH = 240;
const int MRE_SCREEN_HEIGHT = 320;

void I_FinishUpdate (void)
{

    static int	lasttic;
    int		tics;
    int		i;
    // UNUSED static unsigned char *bigscreen=0;

#if 0
    // draws little dots on the bottom of the screen
    if (devparm)
    {
		i = I_GetTime();
		tics = i - lasttic;
		lasttic = i;
		if (tics > 20) tics = 20;

		for (i=0 ; i<tics*2 ; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0xff;
		for ( ; i<20*2 ; i+=2)
			screens[0][ (SCREENHEIGHT-1)*SCREENWIDTH + i] = 0x0;	
    }
#endif

	// This is for display on PC only.  Don't worry about the output staging buffer
	// being big!

	int y, x;
	for( y = 0; y < SCREENHEIGHT; y++ )
	{
		const uint8_t * screenline = &screens[0][y*SCREENWIDTH];
		for( x = 0; x < SCREENWIDTH; x++ )
		{
			//lpalette
			int col = screenline[x];
			uint32_t rgb = (lpalette[col*3+0]<<16)|(lpalette[col*3+1]<<8)|(lpalette[col*3+2]<<0) | 0xff000000;
			uint16_t rgb565 = (((rgb >> 19) & 0x3F) << 11) | (((rgb >> 10) & 0x7C) << 5) | ((rgb & 0x1F));
			
			// Write to screen
			layer_bufs[(MRE_SCREEN_WIDTH * (x + 1) - y) * 2] = rgb565;
		}
	}

	vm_graphic_flush_layer(layer_bufs, 1);
}


