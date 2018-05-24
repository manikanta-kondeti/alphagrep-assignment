//hashmap.h
/*
    Program: onTheFlySort 
    Author: Manikanta 
*/
#ifndef HASHMAP
#include <iostream>
#include "balancedbst.h"

using namespace std;

class HashMap {
	public:
		
		class HashNode{
			public:
				int key; 
				TreeNode * root;
				HashNode() {
					key = -1, root = NULL;
				}

		};

		HashNode nodes[1000005];

		HashMap() {
			for(auto i: nodes) {
				i = HashNode();
			}
		}

		TreeNode* operator [] (const int& key) {
			int key_val = hashCode(key);
			TreeNode * root = get(key_val);
    		return root;
		}


		bool put(int key, long long int val) {
			int key_hash = hashCode(key);
			if(nodes[key_hash].key == -1) {
				nodes[key_hash].key = key_hash;
			}

			if(val == 0) {
				inorder(nodes[key_hash].root);
				return true;
			}
	
			try {
				nodes[key_hash].root = insertTreeNode(nodes[key_hash].root, val);
			}
			catch(int e) {
				cout << "Exception while inserting into tree " << endl;
				return false;
			}
			return true;
		}

		bool find(int key) {
			int key_hash = hashCode(key);
			if(nodes[key_hash].key == -1) return false;
			return true;
		}

	private:
		int hashCode(int key) {
			return key % 1000007;
		}

		TreeNode * get(int key) {
			HashNode node = nodes[key];
			if(node.key == -1) return NULL;

			return node.root;
		}

}; 
#endif 
