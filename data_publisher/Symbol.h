/*
	Program: MarketDataPublisher 
	Author: Manikanta 
*/
//Symbol.h
#ifndef SYMBOLH
#include <limits.h>
#include <cstdint>

using namespace std;
class Symbol {
	public:
		Symbol(int passed_id) {
			id = passed_id;
			curr_price = -1;
			prev_price = INT_MIN;
			last_published_price = -1;
			setPublishedTime(0);
		}

		void setPublishedTime(uint64_t setTime) {
			last_published_time = setTime;
		}

		void updatePrice(double price) {
			prev_price = curr_price;
			curr_price = price;
		}

		bool shouldBroadcast(double maxPriceMove) {
		//cout << "cur price " << curr_price << " last_published_price " << last_published_price << " prevPrice " << prev_price << " maxPriceMove " << maxPriceMove << endl;
			if(prev_price < 0) return false;

			if(last_published_price == -1) {
				if(curr_price-prev_price > maxPriceMove) return true;
				else return false;
			}

			if(curr_price-last_published_price > maxPriceMove) 
				return true;
			return false;
		}

		void updateLastPublishedPriceTime(uint64_t secondsNow) {
			last_published_price = curr_price;
			last_published_time = secondsNow;
			return;
		}

		int getId() {
			return id;
		}

		int getLastPublishedPrice() {
			return last_published_price;
		}

		uint64_t getLastPublishedTime() {
			return last_published_time;
		}

		int getCurrPrice() {
			return curr_price;
		}
	private:
		int id;
		double curr_price;
		double prev_price;
		double last_published_price;
		uint64_t last_published_time;
		double avg_moving_price;
};

#endif 
