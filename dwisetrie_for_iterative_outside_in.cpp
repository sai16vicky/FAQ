#include <iostream>
#include <vector>
#include <algorithm>

#define N 10 // set the maximum number of variables
#define D 2 // set the largest domain D among all variables

using namespace std;

// A D-Wise trie is a generalization of a bitwise(which contains only 0 and 1) trie to 
// D values. Please refer to https://en.wikipedia.org/wiki/Trie#Bitwise_tries for a basic
// understanding of Bitwise tries. The D-Wise trie should easily follow from the same.
typedef struct dwisetrie_ {
	int variable;
	int domain;
	int is_factor;
	double factor_value;
	struct dwisetrie_*  children[N][D];
} dwisetrie;

void insert(dwisetrie* dwise_trie_ptr, vector< pair<int,int> > factor_input, double factor_value) {
	for (int i = 0; i < (int)factor_input.size(); i++) {
		int cur_variable = factor_input[i].first;
		int cur_domain_value = factor_input[i].second;
		if (dwise_trie_ptr -> children[cur_variable][cur_domain_value] != NULL) {
			dwise_trie_ptr = dwise_trie_ptr->children[cur_variable][cur_domain_value];
			if (i == (int)factor_input.size() - 1) {
				dwise_trie_ptr->is_factor = 1;
				dwise_trie_ptr->factor_value = factor_value;
			}
		} else {
			dwisetrie* new_dwise_trie_ptr = new dwisetrie();
			new_dwise_trie_ptr->variable = cur_variable;
			new_dwise_trie_ptr->domain = cur_domain_value;
			if (i == (int)factor_input.size() - 1 ) {
				new_dwise_trie_ptr->is_factor = 1;
				new_dwise_trie_ptr->factor_value = factor_value;
			}
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < N; k ++) {
					new_dwise_trie_ptr->children[j][k] = NULL;
				}
			}
			dwise_trie_ptr->children[cur_variable][cur_domain_value] = new_dwise_trie_ptr;
		}
	}
	return ;
}

void input() {
	dwisetrie dwise_trie_ptr;
	int variables, factors, domain, entries;
	double factor_value;
	// Note : All these inputs will be obtained from the dataset provided to us. As this is a sample implementation,
	// we are taking them from stdin.
	// Enter the number of variables.
	cin >> variables;
	// Enter the number of factors.
	cin >> factors;
	// Enter the domain for each variable. Note that each variable takes values from 0 to domain - 1(both inclusive).
	cin >> domain;
	for (int i = 0; i < factors; i++) {
		// Enter the size of the factor.
		int factor_size;
		cin >> factor_size;
		// Enter the variables involved in the factor.
		vector<int> factor_variables(factor_size, 0);
		for (int j = 0; j < factor_size; j++) {
			cin >> factor_variables[j];
		}
		// Enter the total number of entries for this factor.
		cin >> entries;
		for (int j = 0; j < entries; j++) {
			vector< pair<int,int> > factor_input(factor_size);
			for (int k = 0; k < factor_size; k++) {
				factor_input[k].first = factor_variables[k];
			}
			// Enter the factor values with the domain value followed by the probability.
			for (int k = 0; k < factor_size; k++) {
				cin >> factor_input[k].second; 
			}
			cin >> factor_value;
			if (factor_value > 0.0) {
				insert(&dwise_trie_ptr, factor_input, factor_value);
			}
		}
	}
}


int main() {
	input();
	return 0;
}