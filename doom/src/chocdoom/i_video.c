// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for X11, UNIX.
//
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <input_main.h>
#include <lcd_main.h>
#include <bsp_sys.h>
#include <misc_utils.h>
#include <dev_io.h>
#include <bsp_cmd.h>


#include "v_video.h"
#include "m_argv.h"
#include "d_event.h"
#include "d_main.h"
#include "i_video.h"
#include "z_zone.h"

#include "tables.h"
#include "doomkeys.h"

#include "g_game.h"
#include "D_player.h"

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define IVID_IRAM 0

// The screen buffer; this is modified to draw things to the screen

#if IVID_IRAM
pix_t I_VideoBuffer_static[SCREENHEIGHT * SCREENWIDTH * sizeof(pix_t)];
#endif
extern pix_t *I_VideoBuffer;

// If true, game is running as a screensaver

boolean screensaver_mode = false;

// Flag indicating whether the screen is currently visible:
// when the screen isnt visible, don't render the screen

boolean screenvisible;

// Gamma correction level to use

int usegamma = 0;

// If true, keyboard mapping is ignored, like in Vanilla Doom.
// The sensible thing to do is to disable this if you have a non-US
// keyboard.

int vanilla_keyboard_mapping = true;


typedef struct
{
    byte r;
    byte g;
    byte b;
} PACKEDATTR rgb_t;

// Palette converted to RGB565

static void *rgb_palette;

void *p_palette;

void I_StartFrame (void)
{

}
void I_UpdateNoBlit (void)
{
}

void I_FinishUpdate (void)
{
    screen_t scr;
    scr.buf = &I_VideoBuffer[0];
    scr.width = SCREENWIDTH;
    scr.height = SCREENHEIGHT;
    vid_update(&scr);
}


//
// I_ReadScreen
//
void I_ReadScreen (pix_t* scr)
{
    d_memcpy (scr, I_VideoBuffer, SCREENHEIGHT * SCREENWIDTH * sizeof(pix_t));
}

//
// I_SetPalette
//

static void *palettes[16] = {NULL};
static const uint32_t clut_num_entries = (256);
static const uint32_t clut_num_bytes = (clut_num_entries * sizeof(uint32_t));
static void *prev_clut = NULL;
static byte *aclut = NULL;
static byte *aclut_map = NULL;

/*FIXME : !!!*/
#if 0/*(GFX_COLOR_MODE == GFX_COLOR_MODE_RGB565)*/

static const uint16_t aclut_entry_cnt = 0xffff;

pix_t I_BlendPix (pix_t fg, pix_t bg, byte a)
{
#define __blend(f, b, a) (byte)(((uint16_t)(f * a) + (uint16_t)(b * (255 - a))) / 255)
    pix_t ret;

    byte fg_r = GFX_ARGB_R(fg);
    byte fg_g = GFX_ARGB_G(fg);
    byte fg_b = GFX_ARGB_B(fg);

    byte bg_r = GFX_ARGB_R(bg);
    byte bg_g = GFX_ARGB_G(bg);
    byte bg_b = GFX_ARGB_B(bg);

    byte r = __blend(fg_r, bg_r, a) << 3;
    byte g = __blend(fg_g, bg_g, a) << 2;
    byte b = __blend(fg_b, bg_b, a) << 3;

    ret = GFX_RGB(GFX_OPAQUE, r, g, b);
    return ret;
}

byte I_BlendPalEntry (byte _fg, byte _bg, byte a)
{
    pix_t fg = rgb_palette[_fg];
    pix_t bg = rgb_palette[_bg];
    pix_t pix = I_BlendPix(fg, bg, a);


    return I_GetPaletteIndex(GFX_ARGB_R(pix), GFX_ARGB_G(pix), GFX_ARGB_B(pix));
}

static void I_GenAclut (void)
{
    int i, j;
    byte *map;

    if (aclut)
        Z_Free(aclut);
    if (aclut_map)
        Z_Free(aclut_map);

    aclut = (byte *)Z_Malloc(aclut_entry_cnt * sizeof(*aclut), PU_STATIC, 0);
    aclut_map = (byte *)Z_Malloc(clut_num_entries * clut_num_entries * sizeof(*aclut), PU_STATIC, 0);

    for (i = 0; i < aclut_entry_cnt; i++) {
        aclut[i] = I_GetPaletteIndex(GFX_ARGB_R(i), GFX_ARGB_G(i), GFX_ARGB_B(i));
    }

    for (i = 0; i < clut_num_entries; i++) {
        map = aclut_map + (i * clut_num_entries);
        for (j = 0; j < clut_num_entries; j++) {
            map[j] = I_BlendPalEntry(rgb_palette[i], rgb_palette[j], 128);
        }
    }
}

void I_CacheAclut (void)
{
    if (aclut && aclut_map)
        return;

    I_GenAclut();
}

#endif /*(GFX_COLOR_MODE == GFX_COLOR_MODE_RGB565)*/

pix_t I_BlendPixMap (pix_t fg, pix_t bg)
{
    byte *map = aclut_map + (aclut[bg] * clut_num_entries);
    return map[aclut[fg]];
}

void I_SetPlayPal (void)
{
    if (!rgb_palette) {
        fatal_error("");
    }
    prev_clut = p_palette;
    p_palette = rgb_palette;
}

void I_RestorePal (void)
{
    if (!prev_clut) {
        fatal_error("");
    }
    p_palette = prev_clut;
}

void I_SetPalette (byte* palette, int idx)
{
    unsigned int i;
    rgb_t* color;
    uint32_t *ppal = (uint32_t *)p_palette;

    if (!p_palette)
        p_palette = Z_Malloc(clut_num_bytes, PU_STATIC, 0);

    palettes[idx] = p_palette;
    rgb_palette = p_palette;

    for (i = 0; i < clut_num_entries; i++)
    {
        color = (rgb_t*)palette;
        ppal[i] = GFX_RGBA8888(gammatable[usegamma][color->r],
                        gammatable[usegamma][color->g],
                        gammatable[usegamma][color->b],
                        0xff);
        palette += 3;
    }
    vid_set_clut(p_palette, clut_num_entries);
    return;
}

static void I_RefreshPalette (int pal_idx)
{
    uint32_t *pal;
    int i;
    byte r, g, b;
    if (pal_idx >= arrlen(palettes)) {
        fatal_error("");
    }
    pal = palettes[pal_idx];

    if (pal == rgb_palette) {
        fatal_error("");
    }

    for (i = 0; i < clut_num_entries; i++) {
        r = GFX_ARGB8888_R(pal[i]);
        g = GFX_ARGB8888_G(pal[i]);
        b = GFX_ARGB8888_B(pal[i]);

        pal[i] = GFX_RGBA8888(gammatable[usegamma][r],
                        gammatable[usegamma][g],
                        gammatable[usegamma][b],
                        0xff);
    }
}


void I_RefreshClutsButPlaypal (void)
{
    int i;
    for (i = 1; i < arrlen(palettes); i++) {
        if (palettes[i]) {
            I_RefreshPalette(i);
        }
    }
}

/*FIXME : !!!*/
#if 0/*(GFX_COLOR_MODE != GFX_COLOR_MODE_CLUT)*/

static int _I_GetClutIndex (uint32_t *pal, pix_t pix)
{
    int i = 0;
    for (i = 0; i < clut_num_entries; ++i) {
        if (pal[i] == pix) {
            return i;
        }
    }
    return -1;
}

int I_GetClutIndex (pix_t pix)
{
    int i, pal_idx = -1;
    if (!rgb_palette || !p_palette) {
        fatal_error("");
    }
    for (i = 0; i < arrlen(palettes); i++) {
        if (palettes[i]) {
            pal_idx = _I_GetClutIndex(palettes[i], pix);
            if (pal_idx >= 0) {
                return pal_idx;
            }
        }
    }
    pal_idx = I_GetPaletteIndex(GFX_ARGB_R(pix), GFX_ARGB_G(pix), GFX_ARGB_B(pix));
    return pal_idx;
}

#endif /*(GFX_COLOR_MODE == GFX_COLOR_MODE_CLUT)*/

// Given an RGB value, find the closest matching palette index.

int I_GetPaletteIndex (int r, int g, int b)
{
    int best, best_diff, diff;
    int i;
    rgb_t color;
    uint32_t *pal = (uint32_t *)rgb_palette;

    best = 0;
    best_diff = INT_MAX;

    for (i = 0; i < clut_num_entries; ++i)
    {
        color.r = GFX_ARGB8888_R(pal[i]);
        color.g = GFX_ARGB8888_G(pal[i]);
        color.b = GFX_ARGB8888_B(pal[i]);
        diff = (r - color.r) * (r - color.r)
             + (g - color.g) * (g - color.g)
             + (b - color.b) * (b - color.b);

        if (diff < best_diff)
        {
            best = i;
            best_diff = diff;
        }

        if (diff == 0)
        {
            break;
        }
    }

    return best;
}

void I_BeginRead (void)
{
}

void I_EndRead (void)
{
}

void I_SetWindowTitle (char *title)
{
}

void I_GraphicsCheckCommandLine (void)
{
}

void I_SetGrabMouseCallback (grabmouse_callback_t func)
{
}

void I_EnableLoadingDisk (void)
{
}

void I_BindVideoVariables (void)
{
}

void I_DisplayFPSDots (boolean dots_on)
{
}

void I_CheckIsScreensaver (void)
{
}


float mouse_acceleration = 1.0f;
int usemouse = 0;
int mouse_threshold;
int *joy_extrafreeze = NULL;
uint32_t joy_act_timestamp = 0;
int32_t joy_freeze_per = 200;

const kbdmap_t gamepad_to_kbd_map[JOY_STD_MAX] =
{
    [JOY_UPARROW]       = {KEY_UPARROW, 0},
    [JOY_DOWNARROW]     = {KEY_DOWNARROW, 0},
    [JOY_LEFTARROW]     = {KEY_LEFTARROW ,0},
    [JOY_RIGHTARROW]    = {KEY_RIGHTARROW, 0},
    [JOY_K1]            = {KEY_USE, PAD_FREQ_LOW},
    [JOY_K4]            = {KEY_END,  0},
    [JOY_K3]            = {KEY_FIRE, 0},
    [JOY_K2]            = {KEY_TAB,    PAD_FREQ_LOW},
    [JOY_K5]            = {KEY_STRAFE_L,    0},
    [JOY_K6]            = {KEY_STRAFE_R,    0},
    [JOY_K7]            = {KEY_DEL,  0},
    [JOY_K8]            = {KEY_PGDN, 0},
    [JOY_K9]            = {KEY_ENTER, 0},
    [JOY_K10]           = {KEY_ESCAPE, PAD_FREQ_LOW},
};

extern int d_rlimit_wrap (uint32_t *tsf, uint32_t period);

static i_event_t *__post_key (i_event_t *events, i_event_t *e)
{
    event_t event = {e->state == keyup ? ev_keyup : ev_keydown, e->sym, -1, -1, -1};

    if (joy_extrafreeze && *joy_extrafreeze && e->state == keydown) {
        if (0 == d_rlimit_wrap(&joy_act_timestamp, joy_freeze_per)) {
            return NULL;
        }
    }
    D_PostEvent(&event);
    return events;
}

void I_GetEvent (void)
{
    input_proc_keys(NULL);
}

void I_InitGraphics (void)
{
#if !IVID_IRAM
    I_VideoBuffer = (pix_t*)Z_Malloc (SCREENWIDTH * SCREENHEIGHT * sizeof(pix_t), PU_STATIC, NULL);
#else
    I_VideoBuffer = I_VideoBuffer_static;
#endif
	screenvisible = true;
    p_palette = rgb_palette;

    cmd_register_i32(&joy_freeze_per, "joyfreeze");
    input_soft_init(__post_key, (void *)gamepad_to_kbd_map);
}

void I_ShutdownGraphics (void)
{
#if !IVID_IRAM
	Z_Free (I_VideoBuffer);
#endif
    I_VideoBuffer = NULL;
}

