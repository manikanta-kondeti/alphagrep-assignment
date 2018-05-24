/*
	Program: MarketDataPublisher 
	Author: Manikanta 
*/
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include "MarketDataPublisher.h"
#include <cstdint>
#include <cinttypes>
#include <thread>
#include <chrono>

atomic<uint64_t> secondsPassed;
mutex m;
using namespace std;

void onTimer(MarketDataPublisher *mdp, int n_secs)
{
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // <-- call your function here
    //   cout << "I am getting called for every 1 sec , seconds passed " << secondsPassed << " sec" << endl;
        secondsPassed++;
        if(secondsPassed%n_secs == 0) { 		
        	lock_guard<mutex> guard(m);
			mdp->onTimer(secondsPassed);
		}

        onTimer(mdp, n_secs);
}



void pricing(MarketDataPublisher *mdp) {
	int sym_id; 
	double price;
	while(1) {
		cout << "Pricing thread running,  please input the symbol and price :" << endl;
		cin >> sym_id >> price;
		lock_guard<mutex> guard(m);
		mdp->updatePrice(sym_id, price, secondsPassed);
		cout << endl;
	}
	return;
}


int main() {
	int maxIntervalSeconds;
	double maxPriceMove;
	int maxSymbolsAtATime;

	cout << "Please input, maxIntervalSeconds, maxPriceMove, maxSymbolsAtATime" << endl;
	cin >> maxIntervalSeconds >> maxPriceMove >> maxSymbolsAtATime;
	MarketDataPublisher mdp = MarketDataPublisher(maxIntervalSeconds, maxPriceMove, maxSymbolsAtATime);

	// create two threads,1. pricing, 2. onTimer, 
	thread(onTimer, &mdp, maxIntervalSeconds).detach();
	thread(pricing, &mdp).join();
	return 0;
}