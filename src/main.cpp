#include <iostream>

#include "renderer.h"
#include "game.h"

int main() {
  constexpr std::size_t kFramesPerSecond{120};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{600};
  constexpr std::size_t kScreenHeight{600};
  constexpr std::size_t kGridWidth{24};
  constexpr std::size_t kGridHeight{24};
  
  Renderer renderer("SimpleTetris", kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Game game;

  game.Run(renderer, kMsPerFrame);
  //game.Test(renderer, kMsPerFrame);

  std::cout << "SimpleTetris has terminated successfully!\n";
  
  if (game.Score()<1000)
    std::cout << "Final score: "<<game.Score()<<"\n";
  else
    std::cout << "Great play! Final score: "<<game.Score()<<"\n";

  return 0;
}