#pragma once
#include "Cell.h"
#include "GameState.h"
class GameUI {
public:
	virtual ~GameUI() = default;

	virtual int get_cell_rows() const = 0;
	virtual int get_cell_cols() const = 0;
	virtual int get_bombs_left() const = 0;

	virtual void mouse_click(int r, int c, int mouse_click) = 0; // 1 - left; 2 - right; 3 - middle

	virtual const Cell* const* get_board() const = 0;

	virtual void restart_ui_game() = 0;

	virtual void new_ui_game(int x, int y) = 0;

	virtual GameState get_game_state() const = 0;
};