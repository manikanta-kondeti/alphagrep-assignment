/*
	Program: MarketDataPublisher 
	Author: Manikanta 
*/
#ifndef MARKETDATAPUBLISHERH

#include <vector>
#include <unordered_map> 
#include "Broadcaster.h"
#include <list>
#include <chrono>
#include <algorithm>
#include "Symbol.h"
#include <cstdint>
#include <stdio.h>

// Compile time constant
#define N 15 
#define mp make_pair 

using namespace std;
 
class MarketDataPublisher {
	public:
		MarketDataPublisher(int maxIntervalSeconds, double maxPriceMove, int maxSymbolsAtATime) {
			for(int i=0; i<N+1; i++) {
				Symbol * s = new Symbol(i);
				symbols.push_back(s);
			}

			broadcaster = Broadcaster();
			
			n_secs = maxIntervalSeconds;
			m_symbols = maxSymbolsAtATime;
			threshold = maxPriceMove;
			broadcaster = Broadcaster();
		}

		void updatePrice(int symbol_id, double price, uint64_t secondsSinceEpoch);
		void onTimer(uint64_t secondsSinceEpoch);
		void printList(list<Symbol*> l);

	private:
		unordered_map<int, pair<list<Symbol*>*, list<Symbol*>::iterator> > symbol_position;
		list<Symbol*> RecentBroadcast;
		list<Symbol*> ShouldBroadcast;

		vector<Symbol*> symbols;
		int m_symbols;
		double threshold;
		int n_secs;
		Broadcaster broadcaster;
};

void MarketDataPublisher::updatePrice(int symbol_id, double price, uint64_t secondsSinceEpoch) {
	if(symbol_id < 0 || symbol_id > N) return;
	// remove from the list if present
	pair<list<Symbol*>*, list<Symbol*>::iterator > symbol_position_pair;
	list<Symbol*> * present_list;
	list<Symbol*>::iterator it;

	if(symbol_position.find(symbol_id) != symbol_position.end()) {
		symbol_position_pair = symbol_position[symbol_id];
		present_list = symbol_position_pair.first;
		it = symbol_position_pair.second;
		present_list->erase(it);
	}

	symbols[symbol_id]->updatePrice(price);
	if(symbols[symbol_id]->shouldBroadcast(threshold)) {
		// add into broadcast list
		cout << "seconds passed " << secondsSinceEpoch << endl;
		RecentBroadcast.push_back(symbols[symbol_id]);
		it = RecentBroadcast.end();
		it--;
		symbol_position[symbol_id] = make_pair(&RecentBroadcast, it);

		symbols[symbol_id]->updateLastPublishedPriceTime(secondsSinceEpoch);
		broadcaster.broadcast(symbol_id, symbols[symbol_id]->getCurrPrice());
		
	//	cout << "get published time of sym " << symbols[symbol_id]->getLastPublishedTime() << endl;
	} else {
		// add into non-broadcast list 
		ShouldBroadcast.push_back(symbols[symbol_id]);
		it = ShouldBroadcast.end();
		it--;
		symbol_position[symbol_id] = make_pair(&ShouldBroadcast, it);
	}
	cout << "Updated price successfully at " << secondsSinceEpoch << " secs."<< endl;
	return;
}

void MarketDataPublisher::printList(list<Symbol*> l) {

	for(list<Symbol*>::iterator it=l.begin(); it!= l.end(); ++it) {
		cout << (*it)->getId() << " time " << (*it)->getLastPublishedTime() << endl;
	}
	cout << endl;
}

void MarketDataPublisher::onTimer(uint64_t secondsSinceEpoch) {
	list<Symbol*>::iterator it;
	list<Symbol*>::iterator it_temp;
	int total_broadcasted = 0;
	bool symbols_to_broadcast[N+1] = {false};

	cout << "MDP.onTimer called after " << secondsSinceEpoch << endl;
	/*
	cout << "Printing shouldBroadcast "<< endl;
	printList(ShouldBroadcast);

	cout << "Printing RecentBroadcast "<< endl;
	printList(RecentBroadcast);
	*/
	// First iterate from RecentBroadcast List 
	for(list<Symbol*>::iterator it = RecentBroadcast.begin(); it != RecentBroadcast.end(); ) {
		Symbol * s = *it;
	//	cout << "s val " << s->getId() << "curr price " << s->getCurrPrice() << " secondsSinceEpoch "<< secondsSinceEpoch << "s.getLastPublishedTime " <<  s->getLastPublishedTime()  << "secondsSinceEpoch - s.getLastPublishedTime() " << secondsSinceEpoch - s->getLastPublishedTime()  << endl;
		if(secondsSinceEpoch - s->getLastPublishedTime() > n_secs) {
			// remove it add to next list;
			RecentBroadcast.erase(it);
			++it;
			ShouldBroadcast.push_back(s);
			it_temp = ShouldBroadcast.end();
			it_temp--;
			symbol_position[s->getId()] = make_pair(&ShouldBroadcast, it_temp);
		} else {
			++it;
		}
	}

	if(ShouldBroadcast.empty()) return;

	while(total_broadcasted != m_symbols) {
		Symbol * s = ShouldBroadcast.front();
		if(symbols_to_broadcast[s->getId()] == true) break;
		ShouldBroadcast.pop_front();
		ShouldBroadcast.push_back(s);
		it = ShouldBroadcast.end();
		it--;
		symbol_position[s->getId()] = make_pair(&ShouldBroadcast, it);
		symbols_to_broadcast[s->getId()] = true;
		broadcaster.broadcast(s->getId(), s->getCurrPrice());
		s->updateLastPublishedPriceTime(secondsSinceEpoch);
		total_broadcasted++;
	}
	cout << endl;
		return;
}

#endif 