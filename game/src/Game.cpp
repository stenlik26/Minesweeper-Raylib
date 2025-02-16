#include "Game.h"

int Game::get_cell_rows() const
{
	return board_r;
}

int Game::get_cell_cols() const
{
	return board_c;
}

int Game::get_bombs_left() const
{
	return bombs_left - planted_flags;
}

void Game::reveal_board()
{
	for (size_t r = 0; r < board_r; r++)
	{
		for (size_t c = 0; c < board_c; c++)
		{
			board[r][c].is_revealed = true;
		}
	}
}

bool Game::check_board_won()
{
	if (get_bombs_left() != 0)
		return false; 

	for (size_t r = 0; r < board_r; r++)
	{
		for (size_t c = 0; c < board_c; c++)
		{
			if (board[r][c].is_revealed)
				continue;

			if (board[r][c].is_flaged && board[r][c].is_mine)
				continue;

			return false;
		}
	}
	return true;
}

void Game::mouse_click(int r, int c, int mouse_click) // 1 - left; 2 - right; 3 - middle
{
	if (!is_valid_field(r, c))
		return;

	if (mouse_click == 1 && !board[r][c].is_flaged)
	{
		if (board[r][c].is_mine)
		{
			game_state = GAME_OVER;
			reveal_board();
		}

		flood_open(r, c);
	}
	else if (mouse_click == 2 && !board[r][c].is_revealed)
	{
		board[r][c].is_flaged = !board[r][c].is_flaged;

		if (board[r][c].is_flaged)
			planted_flags++;
		else
			planted_flags--;
	}
	else
		//TODO: implement safe reveal? Scroll click
		return;

	if (check_board_won())
	{
		game_state = GAME_WON;
		reveal_board();
	}
}

const Cell* const* Game::get_board() const
{
	return board;
}

GameState Game::get_game_state() const
{
	return game_state;
}

void Game::restart_ui_game() 
{
	restart_game();
}


void Game::generate_mines_board()
{
	bombs_left = board_r * board_c * density_factor;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> distrib_rows(0, board_r - 1);
	std::uniform_int_distribution<> distrib_cols(0, board_c - 1);

	for (size_t i = 0; i < bombs_left; i++)
	{

		int r = distrib_rows(gen);
		int c = distrib_cols(gen);

		if (board[r][c].is_mine)
		{
			i--;
			continue;
		}

		board[r][c].is_mine = true;
	}
}

void Game::free_board()
{
	for (size_t i = 0; i < board_r; i++)
	{
		delete[] board[i];
	}
	delete[] board;
}

bool Game::is_valid_field(int r, int c) const
{
	return r >= 0 && r < board_r && c >= 0 && c < board_c;
}

void Game::flood_open(int r, int c)
{
	if (!is_valid_field(r, c))
		return;

	if (board[r][c].is_mine || board[r][c].is_flaged || board[r][c].is_revealed)
		return;

	board[r][c].is_revealed = true;

	if (board[r][c].adj_mines != 0)
		return;

	flood_open(r + 1, c);
	flood_open(r - 1, c);
	flood_open(r, c + 1);
	flood_open(r, c - 1);
	flood_open(r + 1, c + 1);
	flood_open(r + 1, c - 1);
	flood_open(r - 1, c + 1);
	flood_open(r - 1, c - 1);
}

void Game::calculate_board()
{
	for (int r = 0; r < board_r; r++)
	{
		for (int c = 0; c < board_c; c++)
		{
			if (board[r][c].is_mine)
				continue;

			for (int off_c = -1; off_c <= 1; off_c++)
			{
				for (int off_r = -1; off_r <= 1; off_r++)
				{
					if (is_valid_field(r + off_r, c + off_c) &&
						board[r + off_r][c + off_c].is_mine)
						board[r][c].adj_mines++;
				}
			}
		}
	}
}

void Game::init_board()
{
	board = new Cell * [board_r];
	for (int i = 0; i < board_r; i++)
	{
		board[i] = new Cell[board_c];

		for (int j = 0; j < board_c; j++)
			board[i][j] = { false, false, false, 0 };
	}
}

void Game::new_ui_game(int x, int y)
{
	init_game(x, y);
}

Game::Game()
{
	init_game(9,9);
}

Game::~Game()
{
	free_board();
}

void Game::init_game(int r, int c)
{
	game_state = GAME_IN_PROGRESS;
	board_r = r;
	board_c = c;
	planted_flags = 0;

	init_board();
	generate_mines_board();
	calculate_board();
}

void Game::restart_game()
{
	planted_flags = 0;
	game_state = GAME_IN_PROGRESS;
	for (size_t r = 0; r < board_r; r++)
	{
		for (size_t c = 0; c < board_c; c++)
		{
			board[r][c] = { false, false, false, 0 };
		}
	}
	generate_mines_board();
	calculate_board();
}
