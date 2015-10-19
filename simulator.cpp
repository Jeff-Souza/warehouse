/* This class simulates the data given by filename. It stores a map of warehouses, and calls
 * public functions on the warehouses to simulate interactions. After the simulation is complete a
 * log of unstocked and wellstocked items is printed to the console.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "warehouse.h"
#include "simulator.h"
#include <boost/algorithm/string.hpp>


namespace cs3505 {

	struct Request
	{
		std::string location;
		std::string code;
		int quantity;

		Request(std::string _location, std::string _code, int _quantity)
		{
			location = _location;
			code = _code;
			quantity = _quantity;
		}
	};

	std::vector<Request> requests;

	//Constructor, simulates the information given in filename
	simulator::simulator(std::string filename) {
		parse_log(filename);
	}
 
	simulator::~simulator() {
	}

	//This function is the primary function of this class, it parses the provided log and calls
	//the relevant functions to simulate warehouse interactions.	
	void simulator::parse_log(std::string filename) {
		std::ifstream in(filename.c_str());	//Read the file

		while(true) {
			std::vector<std::string> words; 
			char line[256];
			in.getline(line, 256);
			std::string sLine(line);

			// remove cariage return if found
			if(sLine[sLine.length()-1] == '\r') {
				sLine.erase(sLine.length()-1, 1);
			}

			boost::split(words, sLine, boost::is_any_of(" ")); //Split along spaces
			
			for(int i = 0; i < words.size(); i++) { //Erase any empty elements (accounts for multiple spaces between two words)
				if(words[i] == "") {
					words.erase(words.begin()+i);
				}
			}

			if(words[0] == "FoodItem") {
				std::vector<std::string> nameVector(words.begin()+9, words.end());
				std::string name = boost::algorithm::join(nameVector, " ");
				add_food_item(name, words[4], atoi(words[7].c_str()));
			} else if (words[0] == "Warehouse") {
				add_warehouse(words[2]);
			} else if (words[0] == "Receive:") {
				receive_food(words[3], words[1], atoi(words[2].c_str()));
			} else if (words[0] == "Request:") {
				Request* r = new Request(words[3], words[1], atoi(words[2].c_str()));
				requests.push_back(*r);
			} else if (words[0] == "Next") {
				next_day();
			} else if (words[0] == "End") {
				process_requests();
				print_report();
				break;
			}
		}
	}

	//Helper function for parse_log, adds a warehouse to the map of warehouses.
	void simulator::add_warehouse(std::string city) {
		warehouses.insert(std::pair<std::string, warehouse> (city, warehouse(upcToShelfLife)));
	}

	//Helper function for parse_log, adds a food item to both food maps, which map the UPC to
	//the item's shelf life and english name.
	void simulator::add_food_item(std::string name, std::string code, int shelfLife) {
		upcToShelfLife.insert(std::pair<std::string, int>         (code, shelfLife));
    		upcToName.insert(     std::pair<std::string, std::string> (code, name));
	}

	//Helper function for parse_log, calls "add_food()" on the relevant warehouse.
	//This function uses an iterator because the C++ map [] operator requires a default
	//constructor, because it attempts to create a new value if it does not find one.
	void simulator::receive_food(std::string location, std::string code, int quantity) {
    		std::map<std::string, warehouse>::iterator it = warehouses.find(location);
    		it->second.add_food(code, quantity);
	}

	void simulator::process_requests()
	{
    		for (int i = 0; i < requests.size(); i++)
    		{
    			request_food(requests[i]);
    		}
    		requests.clear();
   	 }

	//Helper function for parse_log, calls "remove_food" on the relevant warehouse. See
	//receive_food()'s comment for an explanation on this function.
	void simulator::request_food(Request& r) {
    		std::map<std::string, warehouse>::iterator it = warehouses.find(r.location);
    		it->second.remove_food(r.code, r.quantity);
	}

	//Calls "age_food()" on all stored warehouses.
	void simulator::next_day() {
    		process_requests();

    		for (std::map<std::string, warehouse>::iterator it = warehouses.begin(); it != warehouses.end(); ++it) { //Iterate through each warehouse
  			it->second.age_food();
  		}
  	} 

	//Prints a report of the current status of all food items in warehouses: lists food items that
	//are not present in any warehouses, and prints food items that are present in at least two 
	//warehouses. 
	void simulator::print_report() {
		std::vector<std::string> unstocked; //List of unstocked food items
		std::vector<std::string> wellstocked; //List of well stocked food items

		for (std::map<std::string, std::string>::iterator foodIt = upcToName.begin(); foodIt != upcToName.end(); ++foodIt) { //Iterate through each food item
			int stockedWarehouses = 0; //Count the number of warehouses that have this food in stock
			for (std::map<std::string, warehouse>::iterator warehouseIt = warehouses.begin(); warehouseIt != warehouses.end(); ++warehouseIt) { //Iterate through each warehouse
		  		if(warehouseIt->second.is_stocked(foodIt->first)) { //If the warehouse has the item
					stockedWarehouses++; 
				}
			}
			if(stockedWarehouses > 1) { //If an item is well stocked, add it to the list
				wellstocked.push_back(foodIt->first + " " + foodIt->second);
			} else if (stockedWarehouses == 0) { //If an item is unstocked, add it to the list
				unstocked.push_back(foodIt->first + " " + foodIt->second);
			}
		}		
	
		std::cout << "Report by Caius Worthen and Jeff Souza\n" << std::endl;
		std::cout << "Unstocked Products:" << std::endl;
		//Unstocked
		for(int i = 0; i < unstocked.size(); i++) {
			std::cout << unstocked[i] << std::endl;
		}

		std::cout << "\nWell-Stocked Products:" << std::endl;
		//Well-stocked
		for(int i = 0; i < wellstocked.size(); i++) {
			std::cout << wellstocked[i] << std::endl;
		}
		std::cout << "\n";
	}
}
