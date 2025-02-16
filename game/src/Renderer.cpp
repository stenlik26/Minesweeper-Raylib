#include "Renderer.h"
#include <algorithm> 
#include <string>
Renderer::Renderer(GameUI* minesweeperUI)
{
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);

	InitWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Minesweeper Raylib");

	InitAudioDevice();

	SetTargetFPS(120);

	this->minesweeper_ui = minesweeperUI;

	update_ui_positions();

	load_textures(); 

	setup_font();
}

Renderer::~Renderer()
{
	UnloadFont(custom_font);
	unload_textures();

	CloseAudioDevice();

	CloseWindow();
}

bool Renderer::render()
{
	if (WindowShouldClose())
		return false;

	BeginDrawing();

	ClearBackground(bg_black);

	draw_board();
	draw_header();

	draw_buttons();

	handle_mouse();

	EndDrawing();

	return true;
}

Rectangle Renderer::calculate_button_size(const char* text, int x_offset)
{
	Rectangle result;

	Vector2 text_size = MeasureTextEx(custom_font, text, 30, 0);
	text_size.x += BUTTON_PADDING;
	text_size.y += BUTTON_PADDING;

	result.x = start_x + (minesweeper_ui->get_cell_cols() * (cell_size + CELL_GAP) - CELL_GAP) - x_offset - text_size.x;
	result.y = start_y - 45;
	result.width = text_size.x;
	result.height = text_size.y;

	return result;
}

void Renderer::setup_font()
{
	this->custom_font = LoadFontEx("resources/SourceCodePro-Light.ttf", 200, NULL, 256);

	SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_POINT);

	Vector2 text_size = MeasureTextEx(custom_font, "Minesweeper Raylib", 50, 0);

	title_pos.y = 20;
	title_pos.x = (GetScreenWidth() - text_size.x) / 2;

	bomb_text_pos.x = start_x;
	bomb_text_pos.y = start_y - 45;

	gamex15_button = calculate_button_size("15x15", 0);
	gamex9_button = calculate_button_size("9x9", gamex15_button.width + BUTTON_GAP);
	gamex5_button = calculate_button_size("5x5", gamex9_button.width + 2 * BUTTON_GAP + gamex15_button.width);
}


void Renderer::load_textures()
{
	texture_mine = LoadTexture("resources/sprite_mine.png");
	texture_flag = LoadTexture("resources/sprite_flag.png");

	for (size_t i = 1; i <= 8; i++)
	{
		std::string path = "resources/" + std::to_string(i) + ".png";
		texture_numbers[i - 1] = LoadTexture(path.c_str());
	}

	texture_rect = Rectangle{ 0,0, (float)texture_numbers[0].width, (float)texture_numbers[0].height };
	texture_origin = Vector2{ 0,0 };
}

void Renderer::unload_textures()
{
	UnloadTexture(texture_mine);
	UnloadTexture(texture_flag);

	for (size_t i = 1; i <= 8; i++)
	{
		UnloadTexture(texture_numbers[i - 1]);
	}
}

void Renderer::update_ui_positions()
{
	start_x = GetScreenWidth() / 20;
	start_y = GetScreenHeight() / 4;

	window_size = GetScreenWidth() - 2 * start_x;

	int cell_cnt = std::max(minesweeper_ui->get_cell_cols(), minesweeper_ui->get_cell_rows());

	cell_size = (window_size - (cell_cnt - 1) * CELL_GAP) / cell_cnt;

}

void Renderer::handle_mouse()
{
	Vector2 mouse_pos = GetMousePosition();

	if (CheckCollisionPointRec(mouse_pos, gamex5_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		minesweeper_ui->new_ui_game(5, 5);
		update_ui_positions();
	}

	if (CheckCollisionPointRec(mouse_pos, gamex15_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		minesweeper_ui->new_ui_game(15, 15);
		update_ui_positions();
	}

	if (CheckCollisionPointRec(mouse_pos, gamex9_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
	{
		minesweeper_ui->new_ui_game(9, 9);
		update_ui_positions();
	}

	
	int mx = mouse_pos.x - start_x;
	int my = mouse_pos.y - start_y;

	if (mx <= 0 || my <= 0 || minesweeper_ui->get_game_state() != GAME_IN_PROGRESS)
		return;

	int total_cell_size = cell_size + CELL_GAP;

	int cell_x = mx / total_cell_size;
	int cell_y = my / total_cell_size;

	int local_x = mx % total_cell_size;
	int local_y = my % total_cell_size;

	if (local_x >= cell_size || local_y >= cell_size)
		return; // Click was in the gap, ignore it

	for (int button = MOUSE_BUTTON_LEFT; button <= MOUSE_BUTTON_MIDDLE; ++button) {
		if (IsMouseButtonPressed(button)) {
			minesweeper_ui->mouse_click(cell_y, cell_x, button + 1); 
			return;
		}
	}
}

void Renderer::draw_board()
{
	int x = start_x;
	int y = start_y;
	for (int r = 0; r < minesweeper_ui->get_cell_rows(); r++)
	{
		for (int c = 0; c < minesweeper_ui->get_cell_cols(); c++)
		{
			draw_cell(r, c, x, y);
			x += CELL_GAP + cell_size;
		}
		y += CELL_GAP + cell_size;
		x = start_x;
	}
}

Rectangle get_rect_by_val(int x, int y, int w, int h) {
	return Rectangle{
		(float)x,
		(float)y,
		(float)w,
		(float)h
	};
}

void Renderer::draw_cell(int r, int c, int screen_x, int screen_y)
{
	if (!minesweeper_ui->get_board()[r][c].is_revealed)
	{
		DrawRectangle(screen_x, screen_y, cell_size, cell_size, cell_gray);
		
		if(minesweeper_ui->get_board()[r][c].is_flaged)
			DrawTexturePro(texture_flag, texture_rect,
				get_rect_by_val(screen_x, screen_y, cell_size, cell_size), texture_origin, 0, WHITE);
	}
	else
	{
		if(!minesweeper_ui->get_board()[r][c].is_mine)
			DrawRectangle(screen_x, screen_y, cell_size, cell_size, cell_gray_opened);
		else if(minesweeper_ui->get_board()[r][c].is_flaged)
			DrawRectangle(screen_x, screen_y, cell_size, cell_size, cell_green);
		else
			DrawRectangle(screen_x, screen_y, cell_size, cell_size, cell_red);



		int num = minesweeper_ui->get_board()[r][c].adj_mines;

		if (num != 0) 
			DrawTexturePro(texture_numbers[num - 1], texture_rect, 
				get_rect_by_val(screen_x, screen_y, cell_size, cell_size), texture_origin, 0, WHITE);
		

		if(minesweeper_ui->get_board()[r][c].is_mine)
			DrawTexturePro(texture_mine, texture_rect,
				get_rect_by_val(screen_x, screen_y, cell_size, cell_size), texture_origin, 0, WHITE);

	}
}

void Renderer::draw_header()
{
	DrawTextEx(custom_font, "Minesweeper Raylib", title_pos, 50, 0, WHITE);


	std::string bombs_left = "Bombs left: " + std::to_string(minesweeper_ui->get_bombs_left());
	DrawTextEx(custom_font, bombs_left.c_str(), bomb_text_pos, 30, 0, WHITE);
}

void Renderer::draw_buttons()
{
	DrawRectangleRec(gamex15_button, button_color);
	DrawTextEx(custom_font, "15x15", Vector2{ gamex15_button.x, gamex15_button.y }, 30, 0, WHITE);

	DrawRectangleRec(gamex9_button, button_color);
	DrawTextEx(custom_font, "9x9", Vector2{ gamex9_button.x, gamex9_button.y }, 30, 0, WHITE);

	DrawRectangleRec(gamex5_button, button_color);
	DrawTextEx(custom_font, "5x5", Vector2{ gamex5_button.x, gamex5_button.y }, 30, 0, WHITE);
}
