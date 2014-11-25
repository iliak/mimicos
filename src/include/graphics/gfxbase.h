#ifndef GRAPHICS_GFXBASE_H
#define GRAPHICS_GFXBASE_H


#include <exec/lists.h>
#include <exec/libraries.h>

#include <graphics\view.h>

#define GRAPHICSNAME	"graphics.library"

struct  GfxBase
{
	struct Library		LibNode;
	struct ViewPort *	ActiViewPort;
	struct List			TextFonts;
	//TextFont *	DefaultFont;

};








#endif	/* GRAPHICS_GFXBASE_H */
