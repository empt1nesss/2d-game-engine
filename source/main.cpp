#include "engine/engine.hpp"

#include <chrono>
#include <thread>


int main()
{
  Engine engine;

  std::chrono::time_point<std::chrono::high_resolution_clock> timer1;
  std::chrono::time_point<std::chrono::high_resolution_clock> timer2;

  timer1 = std::chrono::high_resolution_clock::now();;
  while (engine.IsRunning()) {
    timer2 = std::chrono::high_resolution_clock::now();

    engine.Update((timer2 - timer1).count());
    timer1 = timer2;

    engine.Render();

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  
  return 0;
}
