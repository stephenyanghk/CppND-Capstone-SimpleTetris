#include "game.h"
#include <iostream>

Game::Game(): mRunning(true), gameLevel(0), gameOver(false), currentScore(0), showGameOverBanner(true)
{
	int a[GAME_LEVEL] {800, 575, 350, 125, 50};
	for (int i=0; i<GAME_LEVEL; ++i)
		mFallWaitMs[i] = a[i];

	InitBoard();
	InitGame();
}

void Game::InitBoard()
{
	for (int i=0; i<BOARD_WIDTH; ++i)
		for (int j=0; j<BOARD_HEIGHT; ++j)
		{
			mBoard[i][j].isFree(true);
			mBoard[i][j].setRGBA(75,75,75,100);
		}	
}

void Game::InitGame()
{
	srand (time(NULL));

	int tetriminoId = rand()%7;
	int rotationId = rand()%4;
	int offsetX = Tetrimino::GetInitOffsetX(tetriminoId, rotationId) + BOARD_POSITION_X + BOARD_WIDTH/2;
	int offsetY = Tetrimino::GetInitOffsetY(tetriminoId, rotationId) + BOARD_POSITION_Y;
	mCurrentTetrimino.setValues(tetriminoId, rotationId, offsetX, offsetY);

	tetriminoId = rand()%7;
	rotationId = rand()%4;
	mNextTetrimino.setValues(tetriminoId, rotationId, TETRIMINO_NEXT_X, TETRIMINO_NEXT_Y);

	currentScore = 0;
	gameLevel = 0;
	gameOver = false;
	showGameOverBanner = true;
}

void Game::GenerateNewTetrimino()
{
	int tetriminoId = mNextTetrimino.TetriminoId();
	int rotationId = mNextTetrimino.RotationId();
	int offsetX = Tetrimino::GetInitOffsetX(tetriminoId, rotationId) + BOARD_POSITION_X + BOARD_WIDTH/2;
	int offsetY = Tetrimino::GetInitOffsetY(tetriminoId, rotationId) + BOARD_POSITION_Y;
	mCurrentTetrimino.setValues(tetriminoId, rotationId, offsetX, offsetY);

	tetriminoId = rand()%7;
	rotationId = rand()%4;
	mNextTetrimino.setValues(tetriminoId, rotationId, TETRIMINO_NEXT_X, TETRIMINO_NEXT_Y);
}

void Game::DrawBoard(Renderer &renderer)
{
	renderer.ClearScreen();
	renderer.DrawRectRange(BOARD_POSITION_X, BOARD_POSITION_Y, BOARD_WIDTH, BOARD_HEIGHT, mBoard);
}

void Game::DrawTetrimino(Renderer &renderer, Tetrimino& tetrimino)
{
	int x = tetrimino.X();
	int y = tetrimino.Y();

	for (int i=0; i<TETRIMINO_BLOCKS; ++i)
		for (int j=0; j<TETRIMINO_BLOCKS; ++j)
			if (Tetrimino::GetType(j, i, tetrimino.TetriminoId(), tetrimino.RotationId())!=0)
				renderer.DrawRect(x+i, y+j, tetrimino.colorR(), tetrimino.colorG(), tetrimino.colorB(), tetrimino.colorA());

}

void Game::DrawScene(Renderer &renderer)
{
	DrawBoard(renderer);

	DrawTetrimino(renderer, mCurrentTetrimino);
	DrawTetrimino(renderer, mNextTetrimino);

	renderer.UpdateScreen();
}

void Game::PlaceTetrimino(Renderer &renderer)
{
	int x = mCurrentTetrimino.X() - BOARD_POSITION_X;
	int y = mCurrentTetrimino.Y() - BOARD_POSITION_Y;

	for (int i1=x, i2=0; i1<x+TETRIMINO_BLOCKS; ++i1, ++i2)
		for (int j1=y, j2=0; j1<y+TETRIMINO_BLOCKS; ++j1, ++j2)
			if (Tetrimino::GetType(j2, i2, mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId())!=0)
			{
				mBoard[i1][j1].isFree(false);
				mBoard[i1][j1].setRGBA(mCurrentTetrimino.colorR(), mCurrentTetrimino.colorG(), mCurrentTetrimino.colorB(), mCurrentTetrimino.colorA());
			}

	DeleteFilledLines(renderer);

	if (IsGameOver())
		gameOver = true;
	else
		GenerateNewTetrimino();
}

bool Game::IsGameOver() const
{
	for (int i=0; i<BOARD_WIDTH; ++i)
	    if (mBoard[i][0].isFree()==false)
            return true;
	return false;
}

void Game::DeleteLine(int y)
{
	for (int j=y; j>0; --j)
	{
		for (int i=0; i<BOARD_WIDTH; ++i)
		{
			int k = j-1;
			mBoard[i][j].isFree(mBoard[i][k].isFree());
			mBoard[i][j].setRGBA(mBoard[i][k].R(),mBoard[i][k].G(),mBoard[i][k].B(),mBoard[i][k].A());
		}
	}

	for (int i=0; i<BOARD_WIDTH; ++i)
	{
		mBoard[i][0].isFree(true);
		mBoard[i][0].setRGBA(75,75,75,100);
	}
}

void Game::DeleteFilledLines(Renderer &renderer)
{
	int j = BOARD_HEIGHT-1;
	int line_cleared = 0;

	while(j>=0)
	{
		bool need_delete = true;

		for (int i=0; i<BOARD_WIDTH; ++i)
		{
			if (mBoard[i][j].isFree())
			{
				need_delete = false;
				break;
			}
		}

		if (need_delete)
		{
			int delay = mFallWaitMs[gameLevel]/10;
			for (int i=0; i<BOARD_WIDTH; ++i)
			{
				mBoard[i][j].setRGBA(255,255,255,255);
				DrawScene(renderer);
				SDL_Delay(delay);
			}
	
			DeleteLine(j);
			++line_cleared;
		}
		else
			--j;
	}

	UpdateScoreLevel(line_cleared);
}

void Game::UpdateScoreLevel(int line_cleared)
{
	if (line_cleared<4)
		currentScore += line_cleared*100;
	else
		currentScore += 800 + (line_cleared-4)*1200;

	if (currentScore>=1000 && gameLevel<1)
	{
		++gameLevel;
		return;
	}

	if (currentScore>=2000 && gameLevel<2)
	{
		++gameLevel;
		return;
	}

	if (currentScore>=3000 && gameLevel<3)
	{
		++gameLevel;
		return;
	}

	if (currentScore>=4000 && gameLevel<4)
	{
		++gameLevel;
		return;
	}

	if (currentScore>=5000 && gameLevel<GAME_LEVEL)
	{
		++gameLevel;
		return;
	}
}

bool Game::IsPosFree(int pX, int pY) const
{
	return mBoard[pX-BOARD_POSITION_X][pY-BOARD_POSITION_Y].isFree();
}

bool Game::IsMovable(int pX, int pY, int tetriminoId, int rotationId) const
{
	for (int i1=pX, i2=0; i1<pX+TETRIMINO_BLOCKS; ++i1, ++i2)
	{
		for (int j1=pY, j2=0; j1<pY+TETRIMINO_BLOCKS; ++j1, ++j2)
		{	
			if ((i1<BOARD_POSITION_X||j1<BOARD_POSITION_Y||i1>BOARD_WIDTH+BOARD_POSITION_X||j1>BOARD_HEIGHT+BOARD_POSITION_Y-1)
                    && (Tetrimino::GetType(j2, i2, tetriminoId, rotationId)!=0))
			    return false;

			if ((j1>=0) && (Tetrimino::GetType(j2, i2, tetriminoId, rotationId)!=0)
                    && (!IsPosFree(i1, j1)))
			    return false;
		}
	}

	return true;
}

void Game::Run(Renderer &renderer, std::size_t target_frame_duration)
{
	Uint32 title_timestamp = SDL_GetTicks();
	Uint32 frame_start;
  	Uint32 frame_end;
  	Uint32 frame_duration;

	Uint32 current_time;
	Uint32 prev_time;

 	int frame_count = 0;

	prev_time = SDL_GetTicks();

	while (mRunning)
	{
    	frame_start = SDL_GetTicks();

		/* Input */
		HandleInput(renderer);

		/* Update */
		if (!gameOver)
		{
			current_time = SDL_GetTicks();
			if (current_time - prev_time >= mFallWaitMs[gameLevel])
			{
				if (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y()+1, mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
					mCurrentTetrimino.Y(mCurrentTetrimino.Y()+1);
				else
					PlaceTetrimino(renderer);
					
				prev_time = current_time;
			}
		}

		/* Render */
		if (!gameOver)
		{
			DrawScene(renderer);
			renderer.UpdateWindowTitle(gameLevel, currentScore);
		}

    	frame_end = SDL_GetTicks();

    	frame_count++;
    	frame_duration = frame_end - frame_start;

	    if (frame_end - title_timestamp >= 1500) {
      		frame_count = 0;
      		title_timestamp = frame_end;

			if (gameOver)
			{
				if (showGameOverBanner)
				{
					renderer.UpdateWindowTitle("~ Game Over, press [n] to start a new game ~");
					showGameOverBanner = false;
				}
				else
				{
					renderer.UpdateWindowTitle(gameLevel, currentScore);
					showGameOverBanner = true;
				}
			}
    	}
    
    	if (frame_duration < target_frame_duration)
      		SDL_Delay(target_frame_duration - frame_duration);
	}
}

void Game::HandleInput(Renderer &renderer)
{
	SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                mRunning = false;
                break;

            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_UP:
						if (gameOver)
							break;
						{
							int newRotationId = (mCurrentTetrimino.RotationId()+1)%4;
							if (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), newRotationId))
								mCurrentTetrimino.RotationId(newRotationId);
						}
                        break;

                    case SDLK_DOWN:
						if (gameOver)
							break;
						while (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y()+1, mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
							mCurrentTetrimino.Y(mCurrentTetrimino.Y()+1);
						PlaceTetrimino(renderer);
                        break;

                    case SDLK_LEFT:
						if (gameOver)
							break;
						if (IsMovable(mCurrentTetrimino.X()-1, mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
							mCurrentTetrimino.X(mCurrentTetrimino.X()-1);
                        break;

                    case SDLK_RIGHT:
						if (gameOver)
							break;
						if (IsMovable(mCurrentTetrimino.X()+1, mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
							mCurrentTetrimino.X(mCurrentTetrimino.X()+1);
                        break;

					case SDLK_SPACE:
						if (gameOver)
							break;
						{
							int newRotationId = (mCurrentTetrimino.RotationId()+1)%4;
							if (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), newRotationId))
								mCurrentTetrimino.RotationId(newRotationId);
						}
                        break;

					case SDLK_z:
						if (gameLevel>0)
							--gameLevel;
						break;

					case SDLK_x:
						if (gameLevel<GAME_LEVEL-1)
							++gameLevel;
						break;

					case SDLK_n:
						if (gameOver)
						{
							InitBoard();
							InitGame();
						}
						break;

					case SDLK_q:
						mRunning = false;
						break;
                }

                break;

            default:
                break;
        }
    }
}

void Game::Test(Renderer &renderer, std::size_t target_frame_duration)
{
	Uint32 title_timestamp = SDL_GetTicks();
	Uint32 frame_start;
  	Uint32 frame_end;
  	Uint32 frame_duration;

	Uint32 current_time;
	Uint32 prev_time;

 	int frame_count = 0;

	prev_time = SDL_GetTicks();

	while (mRunning)
	{
    	frame_start = SDL_GetTicks();

		/* Input */
		SDL_Event e;

    	while (SDL_PollEvent(&e))
    	{
        	switch(e.type)
        	{
            	case SDL_QUIT:
                	mRunning = false;
                	break;

            	case SDL_KEYDOWN:
                	switch(e.key.keysym.sym)
                	{
                    	case SDLK_UP:
							if (gameOver)
								break;
							{
								int newRotationId = (mCurrentTetrimino.RotationId()+1)%4;
								if (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), newRotationId))
									mCurrentTetrimino.RotationId(newRotationId);
							}
                        	break;

						case SDLK_DOWN:
							if (gameOver)
								break;
							if (IsMovable(mCurrentTetrimino.X(), mCurrentTetrimino.Y()+1, mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
								mCurrentTetrimino.Y(mCurrentTetrimino.Y()+1);
                        	break;

                    	case SDLK_LEFT:
							if (gameOver)
								break;
							if (IsMovable(mCurrentTetrimino.X()-1, mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
								mCurrentTetrimino.X(mCurrentTetrimino.X()-1);
        	                break;

            	        case SDLK_RIGHT:
							if (gameOver)
								break;
							if (IsMovable(mCurrentTetrimino.X()+1, mCurrentTetrimino.Y(), mCurrentTetrimino.TetriminoId(), mCurrentTetrimino.RotationId()))
								mCurrentTetrimino.X(mCurrentTetrimino.X()+1);
                        	break;

						case SDLK_n:
							{
								InitBoard();
								InitGame();
							}
							break;

						case SDLK_q:
							mRunning = false;
							break;
                	}

                	break;

            	default:
                	break;
        	}
    	}

		/* Update */
		if (!gameOver)
		{

		}

		/* Render */
		if (!gameOver)
		{
			DrawScene(renderer);
			renderer.UpdateWindowTitle(gameLevel, currentScore);
		}

    	frame_end = SDL_GetTicks();

    	frame_count++;
    	frame_duration = frame_end - frame_start;

	    if (frame_end - title_timestamp >= 1500) {
      		frame_count = 0;
      		title_timestamp = frame_end;

			if (gameOver)
			{
				if (showGameOverBanner)
				{
					renderer.UpdateWindowTitle("~ Game Over, press [n] to start a new game ~");
					showGameOverBanner = false;
				}
				else
				{
					renderer.UpdateWindowTitle(gameLevel, currentScore);
					showGameOverBanner = true;
				}
			}
    	}
    
    	if (frame_duration < target_frame_duration)
      		SDL_Delay(target_frame_duration - frame_duration);
	}
}