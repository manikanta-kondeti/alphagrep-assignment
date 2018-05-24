/*
	Program: FileReduce
	Author: Manikanta 
*/
//RecordUtil.h
#ifndef RECORDUTILH
#include <string>
#include <vector>
#include <unordered_map>
#include "Util.h"
#include "Record.h"

using namespace std;

class RecordHandler {
	public:
		RecordHandler(){};
		unordered_map<string, string> filemap;
		unordered_map<string, vector<string>> parent_child_map;

		void handle_record(const string &record_buf);
		void update_parent_child_map(unordered_map<string, vector<string>> batch_record_map);
		void print_map();

	private:
		bool check_if_exists(string filename);
		void update_parent_record(string parent_id);
		void update_child_record(string parent_id, string child_id);

};

//bool RecordHandler::check_if_exists(string filename) {}


void RecordHandler::handle_record(const string &record_buf) {
	if(record_buf == " " || record_buf.empty()) return; 
	Record * record = new Record(record_buf);
	char type = record->get_type();

	string record_id = record->get_id(); 

	if(type == 'P') update_parent_record(record_id);
	else  {
		string parent_id = record->get_parent_id();
		update_child_record(parent_id, record_id);
	}
	return;
}

void RecordHandler::update_parent_record(string parent_id) {
	vector<string> v;
	if(parent_child_map.find(parent_id) == parent_child_map.end()) parent_child_map[parent_id] = v;
	return;
}

void RecordHandler::update_child_record(string parent_id, string child_id) {
	vector<string> v;
	if(parent_child_map.find(parent_id) == parent_child_map.end()) update_parent_record(parent_id);

	v = parent_child_map[parent_id];
	if (find(v.begin(), v.end(), child_id) == v.end()) v.push_back(child_id);
	parent_child_map[parent_id] = v;
	return; 
}


void RecordHandler::update_parent_child_map(unordered_map<string, vector<string>> batch_record_map) {
	// update records batchwise;
	string record_buffer;
	for (auto i: batch_record_map) {
		update_parent_record(i.first);
		for(auto child_id: i.second) {
			update_child_record(i.first, child_id);
		}
	}
	return;
}

void RecordHandler::print_map() {
	cout << "Printing parent child map " << endl;
	for(auto &item : parent_child_map) {
		cout << "parent id : " << item.first << ": ";
		for(auto &c: item.second) {
			cout << c << " ";
		} 
		cout << endl;
	}
}


#endif