#ifndef _UTILS
#define _UTILS
#include <cstdint>

const int INITIAL_WINDOW_WIDTH = 600;
const int INITIAL_WINDOW_HEIGHT = 800;
const int CELL_GAP = 2;
const int BUTTON_PADDING = 2;
const int BUTTON_GAP = 5;

const Color bg_black = Color{ 24, 21, 24, 255 };
const Color cell_gray = Color{ 45, 42, 46, 255 };
const Color cell_gray_opened = Color{ 35, 32, 36, 255 };

const Color cell_red = Color{255, 97, 136, 255};
const Color cell_green = Color{ 169, 220, 118, 255 };
const Color button_color = Color{ 33, 75,110, 255 };
#endif // !_UITLS
