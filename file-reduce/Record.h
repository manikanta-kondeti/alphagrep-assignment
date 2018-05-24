// Record.h
#ifndef RECORDH
#include <string>
#include <unordered_map>
class Record {
	public:
		Record(string record) {
		//	cout << "creating record " << endl;
			record_buffer = record;
			fields = split(record_buffer, ',');
		}
		string get_id();
		string get_parent_id();
		char get_type();
		string get_record_buffer();

	private:
		string record_buffer;
		vector<string> fields;
};

string Record::get_id() {
	if(fields.empty()) return "";
	int id_index = fields.size()-2;
	return fields[id_index];
}

string Record::get_parent_id() {
	int id_index = fields.size()-1;
	if(fields[0] == "P")
		return fields[id_index];
	return "0";
}

char Record::get_type() {
	char c;
	return fields[0][0];
}

string Record::get_record_buffer() {
	return record_buffer;
} 

#endif