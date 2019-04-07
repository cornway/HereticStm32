//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
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
// DoomData.h

// all external data is defined here
// most of the data is loaded into different structures at run time

#ifndef __DOOMDATA__
#define __DOOMDATA__

#include "doomtype.h"
#include "arch.h"

/*
===============================================================================

						map level types

===============================================================================
*/

// lump order in a map wad
enum
{
    ML_LABEL,
    ML_THINGS,
    ML_LINEDEFS,
    ML_SIDEDEFS,
    ML_VERTEXES,
    ML_SEGS,
    ML_SSECTORS,
    ML_NODES,
    ML_SECTORS,
    ML_REJECT,
    ML_BLOCKMAP
};


typedef V_PREPACK struct {
    short x, y;
} V_POSTPACK mapvertex_t;

typedef V_PREPACK struct {
    short textureoffset;
    short rowoffset;
    char toptexture[8], bottomtexture[8], midtexture[8];
    short sector;               // on viewer's side
} V_POSTPACK mapsidedef_t;

typedef V_PREPACK struct {
    short v1, v2;
    short flags;
    short special, tag;
    short sidenum[2];           // sidenum[1] will be -1 if one sided
} V_POSTPACK maplinedef_t;

#define	ML_BLOCKING			1
#define	ML_BLOCKMONSTERS	2
#define	ML_TWOSIDED			4       // backside will not be present at all
                                                                        // if not two sided

// if a texture is pegged, the texture will have the end exposed to air held
// constant at the top or bottom of the texture (stairs or pulled down things)
// and will move with a height change of one of the neighbor sectors
// Unpegged textures allways have the first row of the texture at the top
// pixel of the line for both top and bottom textures (windows)
#define	ML_DONTPEGTOP		8
#define	ML_DONTPEGBOTTOM	16

#define ML_SECRET			32      // don't map as two sided: IT'S A SECRET!
#define ML_SOUNDBLOCK		64      // don't let sound cross two of these
#define	ML_DONTDRAW			128     // don't draw on the automap
#define	ML_MAPPED			256     // set if allready drawn in automap


typedef V_PREPACK struct {
    short floorheight, ceilingheight;
    char floorpic[8], ceilingpic[8];
    short lightlevel;
    short special, tag;
} V_POSTPACK mapsector_t;

typedef V_PREPACK struct {
    short numsegs;
    short firstseg;             // segs are stored sequentially
} V_POSTPACK mapsubsector_t;

typedef V_PREPACK struct {
    short v1, v2;
    short angle;
    short linedef, side;
    short offset;
} V_POSTPACK mapseg_t;

#define	NF_SUBSECTOR	0x8000
typedef V_PREPACK struct {
    short x, y, dx, dy;         // partition line
    short bbox[2][4];           // bounding box for each child
    unsigned short children[2]; // if NF_SUBSECTOR its a subsector
} V_POSTPACK mapnode_t;

typedef V_PREPACK struct {
    short x, y;
    short angle;
    short type;
    short options;
} V_POSTPACK mapthing_t;

#define	MTF_EASY		1
#define	MTF_NORMAL		2
#define	MTF_HARD		4
#define	MTF_AMBUSH		8

/*
===============================================================================

						texture definition

===============================================================================
*/

typedef V_PREPACK struct {
    short originx;
    short originy;
    short patch;
    short stepdir;
    short colormap;
} V_POSTPACK mappatch_t;

typedef V_PREPACK struct {
    char name[8];
    boolean masked;
    short width;
    short height;
    int obsolete;
    short patchcount;
    mappatch_t patches[1];
} V_POSTPACK maptexture_t;


/*
===============================================================================

							graphics

===============================================================================
*/

// a pic is an unmasked block of pixels
typedef struct
{
    byte width, height;
    byte data;
} pic_t;




/*
===============================================================================

							status

===============================================================================
*/




#endif // __DOOMDATA__
