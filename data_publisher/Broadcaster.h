/*
	Program: MarketDataPublisher 
	Author: Manikanta 
*/
//Broadcaster.h
#ifndef BROADCASTERH
#include <iostream>
#include <cstdint>
using namespace std;

class Broadcaster {
	public:
		Broadcaster() {}

		void broadcast(int symbol_id, double price);
};


void Broadcaster::broadcast(int symbol_id, double price) {
	cout << "Broadcasting symbol_id: " << symbol_id << " with price: " << price << endl;
	return;
}

#endif 