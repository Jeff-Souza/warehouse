/* Jeff Souza and Caius Worthen 
 * 2/7/15
 * Header file for simulator.cpp
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <map>

namespace cs3505 {
  struct Request;
  
  class warehouse;  // A forward declaration of the class.  We can now
					// create pointer variables 
  class simulator {
    public:
    simulator(std::string filename);     // Constructor.  Notice the default parameter value.
  	~simulator();

	private:
    void process_requests();
  	void parse_log(std::string filename);
    void add_warehouse(std::string city);
    void add_food_item(std::string name, std::string code, int shelfLife);
    void receive_food(std::string location, std::string code, int quantity);
    void request_food(Request& r);
    void next_day(); 
  	void print_report();

	std::map<std::string, warehouse> warehouses; // map of warehouses, key is string city
	std::map<std::string, int> upcToShelfLife; //Maps UPC code (string) to item shelf life in days (int)
	std::map<std::string, std::string> upcToName; //Maps UPC code (string) to english name (string)
  };
}

#endif
