#include <iostream>
#include "simulator.h"

using namespace std;

int main (int argc, char* argv[])
{
  if(argc != 2)
  {
    cout << "No filename given" << endl;
    return 1;
  }

  std::string filename(argv[1]);
  cs3505::simulator simulation(filename);

  return 0;
}
