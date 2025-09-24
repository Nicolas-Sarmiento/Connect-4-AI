#include <connect4/game.hpp>
#include <string> 

int main(int argc, char* argv[]) {
  bool enableGui = true;

  
  for (int i = 1; i < argc; ++i) {
      
    std::string arg = argv[i];

    if (arg == "--nogui") {
      enableGui = false;
      break; 
    }
  }


  if (enableGui) {
    GUI();
  } else {
    terminal_simulation();
  }

  return 0; 
}