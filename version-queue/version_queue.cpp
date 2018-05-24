
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

#define VERSION_BASE 0

/*  
	Assuming version number starts with 0 and increments for every enqueue and dequeue
	* Enqueue
		- Adds elements to the map with current version number; 
		- time complexity O(1);

	* Dequeue 
		- Returns top element in the queue;
		- time complexity O(1);

	* Print 
		- Print all the elements with the till current version number 
		- Worst case: time complexity O(N)
*/

class VersionQueue {
	public:
		int version_number;
		vector<int> v;
		unordered_map<int, int> m;

		VersionQueue() {
			version_number = VERSION_BASE;
		}
		
		void print(int print_version);
		void enqueue(int val);
		int dequeue();

	private:
		void increment_version() {
			version_number++;
		}
};

void VersionQueue::enqueue(int val) {
	increment_version();
	v.push_back(val);
	m[version_number] = v.size()-1;
	return;
}

int VersionQueue::dequeue() {
	increment_version();
	if(version_number == VERSION_BASE || v.empty()) return -1;
	m[version_number] = v.size()-1;
	return v.back();
}

void VersionQueue::print(int print_version) {
	//cout << "vn = " << version_number << endl;
	if (v.empty()) {
		cout << "The queue is empty "; 
		return;
	}

	if (print_version < 0) {
		cout << "Not a valid version number, please input a valid version number"; 
		return;
	}

	if (version_number < print_version) {
		cout << "Queue version number is still " << version_number << ", so please provide a valid version" << endl;
		return;
 	}

	int p = m[print_version];
	cout << "Priting Queue at version " << print_version << endl;
	for(int i=0; i<=p; i++) {
		cout << v[i] << endl;
	}
	return;
}

int main(int argc, char **argv) {
	VersionQueue vqueue = VersionQueue();
	int cases;
	cin >> cases;
	while(cases--) {
		int val, ver;
		char c; 
		cin >> c;
		switch(c) {
			case 'e':
				cin >> val;
				vqueue.enqueue(val);
				break;
			case 'd':
				cout << "Top element in the queue: " << vqueue.dequeue() << endl;
				break;
			case 'p':
				cin >> ver;
				vqueue.print(ver);
				break;
		}
	}
	return 0;
}
