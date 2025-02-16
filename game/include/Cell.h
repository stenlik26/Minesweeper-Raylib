#ifndef _CELL_INFO
struct Cell {
	bool is_mine;
	bool is_revealed;
	bool is_flaged;
	int adj_mines;
};

#endif // !_CELL
