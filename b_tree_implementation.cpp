#include <iostream>

using namespace std;

struct btree_ {
    int
} btree;
class BTreeNode {
	int* keys;
	int domain_range;
	BTreeNode** C;
	int no_of_keys;
	bool is_leaf;
	double factor_probability	
	public:
		BTreeNode (int _domain_range, bool _is_leaf);
		// void traverse();
		// BTreeNode* search(int key);
	friend class BTree;
};

class BTree {
	BTreeNode* root;
	int domain_range;
	public:
		BTree(int _domain_range) {
			domain_range = _domain_range;
		}
};

BTreeNode::BTreeNode(int _domain_range, bool _is_leaf) {
	domain_range = _domain_range;
	is_leaf = _is_leaf;
	
}