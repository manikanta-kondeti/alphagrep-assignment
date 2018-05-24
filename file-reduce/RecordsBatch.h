// RecordsBatch
#ifndef RECORDSBATCHH
#include <vector>
#include <string>
#include <unordered_map>
#include <list>
#include <fstream>
using namespace std;

class RecordsBatch {
	public:
		vector<Record*> records;
		unordered_map<string,vector<string>> record_map;
		
		unordered_map<string,vector<string>> child_records;
		unordered_map<string,string> parent_records;

		RecordsBatch(vector<string> record_buffers) {
			Record * rec_ptr;
			for(int i=0; i<record_buffers.size(); i++) {
				rec_ptr = new Record(record_buffers[i]);
				records.push_back(rec_ptr);
			}
		}
		~RecordsBatch(){};
		void aggregate();
		void write_into_files();

	private:
		void update_parent_record(string &parent_id, string &record_buffer);
		void update_child_record(string &parent_id, string &child_id, string &record_buffer);
		void write_record_into_file(string id, string record_buffer);
};

void RecordsBatch::update_parent_record(string &parent_id, string &record_buffer) {
	vector<string> v;
	if(record_map.find(parent_id) == record_map.end()) record_map[parent_id] = v;

	if(parent_records.find(parent_id) == parent_records.end()) parent_records[parent_id] = record_buffer;
	return;
}

void RecordsBatch::update_child_record(string &parent_id, string &child_id, string &child_record_buffer) {
	vector<string> v;
	vector<string> child_records_list;
	if (record_map.find(parent_id) == record_map.end()) record_map[parent_id] = v;
	if (child_records.find(parent_id) == child_records.end()) child_records[parent_id] = child_records_list;

	v = record_map[parent_id];
	child_records_list = child_records[parent_id];
	v.push_back(child_id);
	child_records_list.push_back(child_record_buffer);

	record_map[parent_id] = v;
	child_records[parent_id] = child_records_list;
	return; 
}

void RecordsBatch::aggregate() {
	for(auto &record: records) {
		string id = record->get_id();
		char type = record->get_type();
		string record_buffer = record->get_record_buffer();
		if(type == 'P') {
			string parent_id = record->get_parent_id();
			update_child_record(parent_id, id, record_buffer);
		}
		else if(type == 'T') {
			update_parent_record(id, record_buffer);
		}
	}
	return;
}

void RecordsBatch::write_record_into_file(string parent_id, string record_buffer) {
	ofstream myfile;
	string filename = "./outputs/" + parent_id + ".txt";
	myfile.open(filename, ios::app);
	myfile << record_buffer;
	myfile << "\n";
 	myfile.close();
 	return;
}

void RecordsBatch::write_into_files() {
	for(auto i: parent_records) {
		write_record_into_file(i.first, i.second);	
	}

	for(auto i: child_records) {
		for(auto k: i.second){		
			write_record_into_file(i.first, k);
		}
	}
}

#endif 