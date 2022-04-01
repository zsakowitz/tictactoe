#define EMPTY 0
#define X 1
#define O -1

#define IS_AI_FIRST true

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int board[3][3] = {{EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}, {EMPTY, EMPTY, EMPTY}};
int bi, bj;

bool has_player_won(int player)
{
  return board[0][0] == player && board[0][1] == player && board[0][2] == player ||
         board[1][0] == player && board[1][1] == player && board[1][2] == player ||
         board[2][0] == player && board[2][1] == player && board[2][2] == player ||
         board[0][0] == player && board[1][0] == player && board[2][0] == player ||
         board[0][1] == player && board[1][1] == player && board[2][1] == player ||
         board[0][2] == player && board[1][2] == player && board[2][2] == player ||
         board[0][0] == player && board[1][1] == player && board[2][2] == player ||
         board[0][2] == player && board[1][1] == player && board[2][0] == player;
}

bool is_board_full()
{
  int i, j;

  for (i = 0; i < 3; i++)
  {
    for (j = 0; j < 3; j++)
    {
      if (board[i][j] == EMPTY)
      {
        return false;
      }
    }
  }

  return true;
}

int max(int a, int b)
{
  return a > b ? a : b;
}

int min(int a, int b)
{
  return a < b ? a : b;
}

int score(int player, bool set_move)
{
  if (has_player_won(player))
    return 1;
  else if (has_player_won(-player))
    return -1;
  else if (is_board_full())
    return 0;

  int i, j, best = -1;
  for (i = 2; i >= 0; i--)
  {
    for (j = 2; j >= 0; j--)
    {
      if (board[i][j] == EMPTY)
      {
        board[i][j] = player;
        int amount = -score(-player, false);
        best = max(best, amount);
        board[i][j] = EMPTY;

        if (set_move && amount == best)
        {
          bi = i;
          bj = j;
        }
      }
    }
  }

  return best;
}

bool play_ai_move(int player)
{
  bi = -2;
  bj = -2;
  score(player, true);

  if (bi != -2 && bj != -2)
  {
    board[bi][bj] = player;
    return true;
  }

  return false;
}

void output_board()
{
  int i, j;
  system("clear");

  for (i = 0; i < 3; i++)
  {
    printf(" ");

    for (j = 0; j < 3; j++)
    {
      if (board[i][j] == X)
        printf("\033[91;1mX\033[0m");
      else if (board[i][j] == O)
        printf("\033[32;1mO\033[0m");
      else
        printf("\033[90;1m%d\033[0m", 3 * i + j);

      if (j != 2)
        printf(" | ");
    }

    if (i != 2)
      printf("\n-----------\n");
    else
    {
      int k = score(IS_AI_FIRST ? O : X, false);

      if (k == 1)
        printf("\n\nYou are currently winning.\n");
      if (k == -1)
        printf("\n\nYou are currently losing.\n");
      if (k == 0)
        printf("\n\nThe outcome is set to be a draw.\n");
    }
  }
}

void prompt_user(int player)
{
  int loc;
  printf("Where do you want to move? ");
  scanf("%d", &loc);

  int col = loc % 3;
  int row = loc / 3;

  if (board[row][col] != EMPTY)
  {
    printf("Invalid move.\n");
    prompt_user(player);
  }
  else
  {
    board[row][col] = player;
  }
}

bool next_move(int player, bool is_ai)
{
  output_board();

  if (has_player_won(X))
  {
    printf("X wins!\n");
    return false;
  }

  if (has_player_won(O))
  {
    printf("O wins!\n");
    return false;
  }

  if (is_board_full())
  {
    printf("Draw!\n");
    return false;
  }

  if (is_ai)
    play_ai_move(player);
  else
    prompt_user(player);

  return true;
}

int main()
{
  while (1)
  {
    if (!next_move(X, IS_AI_FIRST))
      break;

    if (!next_move(O, !IS_AI_FIRST))
      break;
  }

  return 0;
}
