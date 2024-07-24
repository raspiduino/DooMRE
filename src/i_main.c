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
//	Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------

//static const char
//rcsid[] = "$Id: i_main.c,v 1.4 1997/02/03 22:45:10 b1 Exp $";

// Headers
#include "doomdef.h"

#include "m_argv.h"
#include "d_main.h"

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "vmtimer.h"

// Timers
extern VMINT main_loop_timer;

// Layer handler
VMINT layer_hdl[1];
VMUINT8 *layer_bufs[1] = {0};

// Function prototypes
void handle_sysevt(VMINT message, VMINT param);
void handle_keyevt(VMINT event, VMINT keycode);
void handle_penevt(VMINT event, VMINT x, VMINT y);
extern void HandleKey(int keycode, int bDown);
extern void I_Quit (void);

/*int
main
( int		argc,
  char**	argv ) 
{ 
    myargc = argc; 
    myargv = argv; 
 
    D_DoomMain (); 

    return 0;
}*/

//char test_str[] = "Hello from DooMRE";

// MRE app entry point
void vm_main(void) {
    //printf_("Entered entrypoint. %s\n", test_str);
    
    /* initialize layer handle */
    layer_hdl[0] = -1;	
    
    /* register system events handler */
    vm_reg_sysevt_callback(handle_sysevt);
    
    /* register keyboard events handler */
    vm_reg_keyboard_callback(handle_keyevt);
    
    /* register pen events handler */
    vm_reg_pen_callback(handle_penevt);
}

void init_doom() {
    //./emdoom -warp 1 1 # where second number is the map # (1 to 9)
    myargc = 4;
    char* argv[] = {
        "./emdoom", // Program name (index 0)
        "-warp",
        "1",
        "1",        // Level (1 to 9)
        NULL        // Null terminator (essential)
    };
    myargv = argv;
    D_DoomMain();
}

void handle_sysevt(VMINT message, VMINT param) {
    switch (message) {
    case VM_MSG_CREATE:	/* the message of creation of application */
    case VM_MSG_ACTIVE: /* the message of application state from inactive to active */
        /* create base layer that has same size as the screen */
        layer_hdl[0] = vm_graphic_create_layer(0, 0, 
            vm_graphic_get_screen_width(),		/* get screen width */
            vm_graphic_get_screen_height(),		/* get screen height */
            -1);		/* -1 means layer or canvas have no transparent color */
        
        /* set clip area */
        vm_graphic_set_clip(0, 0, 
            vm_graphic_get_screen_width(), 
            vm_graphic_get_screen_height());

        /* get framebuffer */
        layer_bufs[0] = vm_graphic_get_layer_buffer(layer_hdl[0]);

        /* init doom */
        init_doom(); 
        break;
        
    case VM_MSG_PAINT:	/* the message of asking for application to repaint screen */
        // Do nothing
        break;
        
    case VM_MSG_INACTIVE:	/* the message of application state from active to inactive */
        if( layer_hdl[0] != -1 )
            vm_graphic_delete_layer(layer_hdl[0]);
        break;	
    case VM_MSG_QUIT:		/* the message of quit application */
        if( layer_hdl[0] != -1 )
            vm_graphic_delete_layer(layer_hdl[0]);
        
        if (main_loop_timer != -1) {
            vm_delete_timer(main_loop_timer);
        }

        // Quit DooM
        I_Quit();
        break;	
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {
    if (event == VM_KEY_EVENT_DOWN) {
        HandleKey(keycode, 0); // 0 = down
    } else if (event == VM_KEY_EVENT_UP) {
        HandleKey(keycode, 1); // 1 = up
    }
}

void handle_penevt(VMINT event, VMINT x, VMINT y)
{
    // Touch is not supported
}
