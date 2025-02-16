#include <raylib.h>
#include "Game.h"
#include "Renderer.h"

int main()
{
    Game minesweeper_game;
    GameUI* minesweeper_ui = &minesweeper_game;

    Renderer renderer(&minesweeper_game);


    //int r = minesweeper_ui->get_cell_rows();
    //int c = minesweeper_ui->get_cell_cols();

    //for (size_t i = 0; i < r; i++)
    //{
    //    for (size_t j = 0; j < c; j++)
    //    {
    //        if (minesweeper_ui->get_board()[i][j].is_mine)
    //            printf("M ");
    //        else
    //            printf("%d ", minesweeper_ui->get_board()[i][j].adj_mines);
    //    }
    //    printf("\n");
    //}

    //printf("Mines left: %d \n", minesweeper_ui->get_bombs_left());
    //printf("Game state: %d \n", minesweeper_ui->get_game_state());

    while (true)
    {
        if (!renderer.render())
            break;

        //......
    }
    return 0;
} 