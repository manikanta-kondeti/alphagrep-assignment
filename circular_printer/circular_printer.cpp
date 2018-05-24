/*
	Program: CircularPrinter 
	Author: Manikanta 
*/
// Circularly priting a string via threads given 
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
using namespace std;

atomic<bool> ready (false);
mutex mut;
int x = 0;

void print(string s, int k, int t) {
	
	while(!ready) this_thread::yield();
	
	int temp = 0;
	for(int itr = 0; itr < t; itr++){
		lock_guard<mutex> guard(mut);
		
		string c = s;
		if(k>s.size()) {
			temp = k/s.size()+1;
			while(temp--) c += s;
		}
		string z = c.substr(x, k);
		cout << z << endl;
		x = (x + k) % s.size();
	}
	
	return;
}

int main() {
	string s; 
	int no_chars;
	int no_threads; 
	int no_thread_times, temp = 0;
	cin >> s >> no_chars >> no_threads >> no_thread_times;
	thread threads[no_threads+1];
	for(int i=0; i<no_threads; i++) {
		threads[i] = thread(print, s, no_chars, no_thread_times);
	}
	ready = true;
	for(auto &t: threads) {
		if(t.joinable()) t.join();
	}

	return 0;
}