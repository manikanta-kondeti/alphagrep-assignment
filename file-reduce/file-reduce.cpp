#include <iostream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "RecordUtil.h"
#include "RecordsBatch.h"

using namespace std;

#define BATCH 100
/* 
	RecordHandler is a class which has the details of the corresponding mapping. 
	Read file line by line.
	For every 100 records, segregate parents and child at one place and write files in parallel 
*/
void Handler(string filename) {
	ifstream fin(filename);
	RecordHandler * rhandler = new RecordHandler();

	vector<string> record_buffers;
	long long int total_lines = 0;
	string line;
	while (std::getline(fin, line))
	{
  	  	if(line.empty() || line == " ") continue;
  	  	total_lines++;
  	  	record_buffers.push_back(line);
  	  	if(total_lines == BATCH) {
  	  		RecordsBatch * records_batch = new RecordsBatch(record_buffers);	
  	  		records_batch->aggregate(); 
  	  		rhandler->update_parent_child_map(records_batch->record_map); 
  	  		records_batch->write_into_files();		
  	  		total_lines = 0;
  	  	}
	}

	if(total_lines != 0) {
		RecordsBatch * records_batch = new RecordsBatch(record_buffers);	
  	  		records_batch->aggregate(); 
  	  		rhandler->update_parent_child_map(records_batch->record_map); 		
  	  		records_batch->write_into_files();		
	}
	cout << "Priting map after: " << endl;
	rhandler->print_map();
	return;
}

int main(int argc, char** argv) {
	if(argc < 3 || argc > 3) { 
		cout << "Arguments given are wrong. Please retry" << endl;
		return 0;
	}
	string records_file_name = argv[1];
	Handler(records_file_name);
	return 0;
}