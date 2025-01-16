#include "game/game.hpp"

#include <chrono>
#include <thread>


int main()
{
  Game game;

  std::chrono::time_point<std::chrono::high_resolution_clock> timer1;
  std::chrono::time_point<std::chrono::high_resolution_clock> timer2;

  while (game.IsRunning()) {
    timer2 = std::chrono::high_resolution_clock::now();

    game.Update((timer2 - timer1).count());
    timer1 = timer2;

    game.Render();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  
  return 0;
}
