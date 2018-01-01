#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__

// all variables here must be const else linker errors will occur

namespace global
{

	const double PI  = 3.141592653589793238512808959406186204433;
}

enum itemType
{
	TYPE_EMPTY = 0,
	TYPE_SLIDEITEM = 1,
	TYPE_ANIMATEDPIXMAPITEM = 2,
	TYPE_MOVINGITEM = 3,
	TYPE_VEHICLE = 4,
    TYPE_GRAPHICS_SOFT_BUTTON = 5,

    TYPE_UPRISETORPEDO = 100,
    TYPE_BALLISTICPROJECTILE = 101,
    TYPE_MINE = 102
};



#endif // __GLOBALVARIABLES_H__
