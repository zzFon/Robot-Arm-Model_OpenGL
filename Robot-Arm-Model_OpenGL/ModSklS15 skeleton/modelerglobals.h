#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.

enum SampleModelControls
{
	XPOS = 0,
	YPOS = 1,
	ZPOS = 2,
	HEIGHT_SUPPORT = 3,
	HEIGHT_ARMA = 4,
	HEIGHT_ARMB = 5,
	HEIGHT_PIN = 6,
	CLAW_SIZE = 7,
	ROTATE_ARMA = 8,
	ROTATE_ARMB = 9,
	ROTATE_PIN = 10,
	CLAW = 11,
	drawOBB_flag = 12,
	NUMCONTROLS = 13,
};
//bool KK_ANIMATING = false;

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif