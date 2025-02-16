#ifndef _RENDER_H
#define _RENDER_H4
#include <raylib.h>
#include "Utils.h"
#include "GameUI.h"


class Renderer
{
private:
	GameUI* minesweeper_ui;
	int start_x, start_y, window_size, cell_size;
	Texture2D texture_mine, texture_flag;
	Texture2D texture_numbers[8];
	Rectangle texture_rect;
	Vector2 texture_origin;
	
	Vector2 title_pos;
	Vector2 bomb_text_pos;
	
	Rectangle gamex5_button;
	Rectangle gamex9_button;
	Rectangle gamex15_button;

	Font custom_font;

	Rectangle calculate_button_size(const char* text, int x_offset);
	
	void load_textures();
	void unload_textures();

	void setup_font();

	void update_ui_positions();

	void handle_mouse();

	void draw_board();
	void draw_cell(int r, int c, int screen_x, int screen_y);
	void draw_header();
	void draw_buttons();

public:
	Renderer(GameUI* minesweeperUI);
	~Renderer();

	bool render();
};

#endif