#include "UIHelp.h"

#include "UIText.h" // UIText
#include "Noggit.h" // app.getArial14()
#include "Video.h" // video

const int winWidth = 775;
const int winHeight = 800;

UIHelp::UIHelp( )
: UICloseWindow( video.xres() / 2.0f - winWidth / 2.0f, video.yres() / 2.0f - winHeight / 2.0f, winWidth, winHeight, "Keybindings", true )
{
  addChild( new UIText( 20.0f, 30.0f,
    "Basic controls:\n"
    "  Left mouse dragged - Rotate camera\n"
    "  Left mouse click - Select chunk or object\n"
    "  Both mouse buttons - Move forward\n"
    "  I - Invert mouse up and down\n"
    "  Q, E - Move vertically up, down\n"
    "  A, D, W, S - Move left, right, forward, backward\n"
	"  C - Switch cursor type\n"
    "  M - Show map\n"
    "  U - 2D texture editor\n"
    //"C - chunk settings\n" //! \todo: C chunk settings must get fixed first. Then turn on this again
    "  H - This help\n"
	"  CTRL + ALT + C - Toggle cursor options\n"
    "  SHIFT + R - Turn camera 180 degres\n"
    "  SHIFT + F4 - Change to auto select mode\n"
	"  SHIFT + 1, 2, 3 or 4 - Set a predefined speed.\n"
    "  ESC - exit to main menu\n"
    "\n"
    "Toggles:\n"
    "  F1 - Toggle M2s\n"
    "  F2 - Toggle WMO doodads set\n"
    "  F3 - Toggle ground\n"
    "  F4 - Toggle water\n"
    "  F6 - Toggle WMOs\n"
    "  F7 - Toggle chunk (red) and ADT (green) lines\n"
    "  F8 - Toggle detailed window\n"
    "  F9 - Toggle map contour\n"
    "  F - Toggle fog\n"
    "  TAB - toggle UI view\n"
	"  X - Toggle tool settings\n"
    "  CTRL + X - detail window\n"
    "  1,2,3,4,5 and 6 - Select the editing modes\n"
    "\n"
    "Files:\n"
    "  F5 - save bookmark\n"
    "  F10 - reload BLP\n"
    "  F11 - reload M2s\n"
    "  F12 - reload wmo\n"
    "  SHIFT + J - reload ADT tile\n"
    "  CTRL + S -  Save all changed ADT tiles\n"
    "  CTRL + SHIFT + S - Save ADT tile at camera position\n"
    "  g - Save port commands to ports.txt\n"
	"\n"
	"Adjust:\n"
    "  O / P - slower/faster movement\n"
    "  B / N - slower/faster time\n"
	"  SHIFT + +/-: fog distance when no model is selected\n"
	"\n"
    , app.getArial14(), eJustifyLeft )
  );

  addChild( new UIText( 380.0f, 30.0f,
    "Edit ground:\n"
    "  SHIFT + F1 - toggle ground edit mode\n"
    "  ALT + left mouse + mouse move - change brush size\n"
	"  Z - Change the mode in option window\n"
    "Terrain mode \"raise / lower\":\n"
    "  SHIFT + Left mouse - raise terrain\n"
    "  CTRL + Left mouse - lower terrain\n"
    "Terrain mode \"flatten / blur\"\n"
    "  SHIFT + Left mouse click - flatten terrain\n"
    "  CTRL + Left mouse  click - blur terrain\n"
    "\n"
    "Edit objects if a model is selected with left click:\n"
    "  Hold middle mouse - move object\n"
    "  ALT + Hold middle mouse - scale M2\n"
    "  SHIFT / CTRL / ALT + Hold left mouse - rotate object\n"
    "  0 - 9 - change doodads set of selected WMO\n"
    "  CTRL + R - Reset rotation\n"
    "  PageDown - Set object to Groundlevel\n"
    "  CTRL + C - Copy object to clipboard\n"
    "  CTRL + V - Paste object on mouse position\n"
	"  SHIFT + C - Insert last M2 from MV\n"
	"  ALT + C - Insert last WMO from MV\n"
    "  - / + - scale M2\n"
    "  Numpad 7 / 9 - rotate object\n"
    "  Numpad 4 / 8 / 6 / 2 - vertical position\n"
    "  Numpad 1 / 3 -  move up/dow\n"
    "    holding SHIFT: double speed \n"
    "    holding CTRL: triple speed \n"
    "    holding SHIFT and CTRL together: half speed \n"
    "\n"
    "Edit texture:\n"
    "  CTRL + SHIFT + left mouse - clear all textures on chunk\n"
    "  CTRL + left mouse - draw texture or fills if chunk is empty\n"
    "\n"
	"Holes:\n"
	"  SHIFT + Left mouse - Add texture\n"
	"  CTRL + Left mouse  - Remove texture\n"
	"\n"
	"AreaID:\n"
	"  X - Show browser with existing AreaID in AreaTable.dbc\n"
	"      Click show sub-zones\n"
	"  CTRL + Left mouse  - Pick existing AreaID\n"
	"  SHIFT + Left mouse - Paint selected AreaID\n"
	"\n"
	"Impassible Flags:\n"
	"  SHIFT + Left mouse - Paint flag\n"
	"  CTRL + Left mouse  - Clear flag\n"
    , app.getArial14(), eJustifyLeft )
  );
}
