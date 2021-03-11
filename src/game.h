#ifndef GAME_H
#define GAME_H

#include "SDL.h"

#include "tetrimino.h"
#include "renderer.h"

#include <stdlib.h>
#include <time.h>

#define TETRIMINO_BLOCKS 5
#define TETRIMINO_NEXT_X 15
#define TETRIMINO_NEXT_Y 4

#define GAME_LEVEL 5

class Game
{
    public:
        Game();
        void Run(Renderer &renderer, std::size_t target_frame_duration);
        void Test(Renderer &renderer, std::size_t target_frame_duration);
        int Score() const { return currentScore; }

    private:
        void InitBoard();
        void InitGame();
        void DrawBoard(Renderer &renderer);
        void DrawTetrimino(Renderer &renderer, Tetrimino& tetrimino);
        void DrawScene(Renderer &renderer);

	    void DeleteLine(int y);
        void DeleteFilledLines(Renderer &renderer);
        void UpdateScoreLevel(int line_cleared);
	    
	    bool IsGameOver() const;
        bool IsPosFree(int pX, int pY) const;
        
        void PlaceTetrimino(Renderer &renderer);
	    void GenerateNewTetrimino();
        
        bool IsMovable(int pX, int pY, int tetriminoId, int rotationId) const;
        void HandleInput(Renderer &renderer);

        Board mBoard[BOARD_WIDTH][BOARD_HEIGHT];

        Tetrimino mCurrentTetrimino;
        Tetrimino mNextTetrimino;

        int mFallWaitMs[GAME_LEVEL];
        int gameLevel;
        int currentScore;

        bool mRunning;
        bool gameOver;
        bool showGameOverBanner;
};

#endif // GAME_H