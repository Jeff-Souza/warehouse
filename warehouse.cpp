/* This class represents a warehouse that stores various food items with expiration dates.
 * Identical food items are stored in a queue as integers representing the number of  days
 * before they expire. These queues are stored in a map that maps the UPC code of the food
 * to the relevant queue. 
 * This class can:
 * 	Add X amount of food items to storage
 * 	Remove X amount of food items from storage (stopping at 0)
 * 	Decrement all stored food items by 1 (simulating the passage of 1 day)
 * 	Return whether it has more than 0 of a food item stored
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "warehouse.h"

namespace cs3505 {

  //The constructor, takes a map that maps a UPC code string to the integer shelf life of an item
  warehouse::warehouse(std::map<std::string, int>& upcToShelfLife_) {
  	upcToShelfLife = upcToShelfLife_;
  }

  warehouse::~warehouse() {
  }

  //Adds food items to the relevant queue (creating a new queue if it doesn't exist) until the
  //quantity has been reached
  void warehouse::add_food(std::string code, int quantity) {
  	int shelfLife = upcToShelfLife[code];
  	for(int i = 0; i < quantity; i++) {
  		foodStorage[code].push(shelfLife);
  	}
  }

  //Removes food items from the queue until either the queue is empty or the quantity
  //to remove has been reached (meaning the order has been fulfilled)
  void warehouse::remove_food(std::string code, int quantity) {
  	while(!foodStorage[code].empty() && quantity > 0) { 
  		foodStorage[code].pop();
  		quantity--;
  	}
  }

  //Iterates through every food integer, decrementing it by 1. If this operation brings 
  //it to 0, it is removed from the queue; it has expired.
  void warehouse::age_food() {
  	for (std::map<std::string, std::queue<int> >::iterator it = foodStorage.begin(); it != foodStorage.end(); ++it) { //For every queue of food items
		int queueSize = it->second.size(); //The size of the current queue before looping through
  		for (int i = 0; i < queueSize; i++) {
  			int food = it->second.front(); //pull the item out
			food--; //age it by 1 day
  			it->second.pop(); //remove it from the queue
  			if(food > 0) { //If it hasn't yet expired
  				it->second.push(food); //Put it back in the queue
  			}
  		}
  	}
  }

  //Returns true if there is at least 1 of the item in stock, false otherwise
  bool warehouse::is_stocked(std::string code) {
	return foodStorage[code].size() > 0;
  }

}
