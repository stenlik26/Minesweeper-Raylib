#ifndef _GAME
#include "GameUI.h"
#include <random>
class Game : public GameUI
{
private:
	Cell** board;
	GameState game_state;
	int board_r;
	int board_c;
	int bombs_left;
	int planted_flags;

	double density_factor = 0.10;

	void generate_mines_board();
	void calculate_board();
	void init_board();
	void free_board();
	void reveal_board();
	bool check_board_won();

	bool is_valid_field(int r, int c) const;
	void flood_open(int r, int c);

	//GameUI interface
	int get_cell_rows() const override;
	int get_cell_cols() const override;
	int get_bombs_left() const override;
	void mouse_click(int r, int c, int mouse_click) override; // 1 - left; 2 - right; 3 - middle
	const Cell* const* get_board() const override;
	GameState get_game_state() const override;
	void restart_ui_game() override;
	void new_ui_game(int x,int y) override;

public:
	Game();
	~Game();

	void init_game(int r, int c);

	void restart_game();
};

#endif // !_GAME
