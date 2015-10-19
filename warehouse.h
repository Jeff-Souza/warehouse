/* Written by Caius Worthen and Jeffrey Souza
 * 2/7/15
 * Header file for warehouse.cpp
 */

#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include<string>
#include<queue>
#include<map>

namespace cs3505 {  
  class warehouse {    
    private:
	//Maps a UPC code (string) to a queue of integers representing food stores of that item. No further information
	//than an int is required to represent a food item, the int represents the days until the item expires and the
	//data structure tracks what food item it is. 
    std::map<std::string, std::queue<int> > foodStorage; 
	//This is passed to the warehouse by the simulator, maps UPC code (string) to a food item's shelf life (int)
    std::map<std::string, int> upcToShelfLife;

	public:
	warehouse(std::map<std::string, int>& upcToShelfLife_);  // Constructor
	~warehouse();
    void add_food(std::string code, int quantity);
    void remove_food(std::string code, int quantity);
    void age_food();
	bool is_stocked(std::string code); 

  };
}

#endif
