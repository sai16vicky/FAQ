#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#define D 20 // set the largest domain D among all variables.
#define INF (int)1e9
#define MAXFACTORS 1000
#define MAXVARIABLES 20
#define N 10 // set the maximum number of variables.

using namespace std;

int no_of_variables, no_of_factors;
map<int,int> factor_variable_domain_exists[MAXFACTORS][MAXVARIABLES];
vector<int> domain_list;
vector< vector<int> > factors;
vector< vector<int> > factor_index_by_variable;
vector<int> factor_variable_domain[MAXFACTORS][MAXVARIABLES];

// A D-Wise trie is a generalization of a bitwise(which contains only 0 and 1) trie to
// D values. Please refer to https://en.wikipedia.org/wiki/Trie#Bitwise_tries for a basic
// understanding of Bitwise tries. The D-Wise trie should easily follow from the same.
typedef struct dwisetrie_ {
	int domain;
	int is_factor;
	double factor_value;
	struct dwisetrie_* children[D];
} dwisetrie;

dwisetrie dwise_trie_ptr[MAXFACTORS];

void insert(dwisetrie* dwise_trie_ptr, vector<int> factor_input, double factor_value) {
    for (int i = 0; i < (int)factor_input.size(); i++) {
		int cur_domain_value = factor_input[i];
		if (dwise_trie_ptr -> children[cur_domain_value] != NULL) {
			dwise_trie_ptr = dwise_trie_ptr->children[cur_domain_value];
			if (i == (int)factor_input.size() - 1) {
				dwise_trie_ptr->is_factor = 1;
				dwise_trie_ptr->factor_value = factor_value;
			}
		} else {
			dwisetrie* new_dwise_trie_ptr = new dwisetrie();
			new_dwise_trie_ptr->domain = cur_domain_value;
			if (i == (int)factor_input.size() - 1 ) {
				new_dwise_trie_ptr->is_factor = 1;
				new_dwise_trie_ptr->factor_value = factor_value;
			}
			for (int j = 0; j < D; j++) {
                new_dwise_trie_ptr->children[j] = NULL;
            }
			dwise_trie_ptr->children[cur_domain_value] = new_dwise_trie_ptr;
		}
	}
    return ;
}

double get_factor_value(dwisetrie* dwise_trie_ptr, vector<int> factor_input) {
    for (int i = 0; i < (int)factor_input.size(); i++) {
        int cur_domain_value = factor_input[i];
        if (dwise_trie_ptr -> children[cur_domain_value] != NULL) {
            dwise_trie_ptr = dwise_trie_ptr->children[cur_domain_value];
            if (i == (int)factor_input.size() - 1 and dwise_trie_ptr->is_factor) {
                return dwise_trie_ptr->factor_value;
            }
        } else {
            return 0.0;
        }
    }
    return 0.0;
}

vector<int> get_domain_values_non_zero(int cur_variable) {
    vector<int> non_zero_domain_values;
    map<int,int> non_zero_domain_values_unique;
    for (int i = 0; i < no_of_factors; i++) {
        for(int j = 0; j < domain_list[cur_variable]; j++) {
            if (non_zero_domain_values_unique.find(j) == non_zero_domain_values_unique.end()) {
                if (factor_variable_domain_exists[i][cur_variable].find(j) != factor_variable_domain_exists[i][cur_variable].end()) {
                    non_zero_domain_values.push_back(j);
                    non_zero_domain_values_unique[j] += 1;
                }
            }
        }
    }
    return non_zero_domain_values;
}


double calculate_probabilities(vector<int> domain_values) {
    double result = 1.0;
    for (int i = 0; i < (int)factors.size(); i++) {
        vector<int> tmp_domain_values;
        for (int j = 0; j < (int)factors[i].size(); j++) {
            tmp_domain_values.push_back(domain_values[factors[i][j] - 1]);
        }
        result *= get_factor_value(&dwise_trie_ptr[i], tmp_domain_values);
    }
    return result;
}

double recursive_outside_in(vector<int>& variable_values) {
	if ((int)variable_values.size() == no_of_variables) {
		return calculate_probabilities(variable_values);
	} 		
	double r = 0.0;
	vector<int> I = get_domain_values_non_zero((int)variable_values.size() + 1);
	for (int i = 0; i < (int)I.size(); i++) {
		vector<int> cur_variable_values(variable_values.begin(), variable_values.end());
		cur_variable_values.push_back(I[i]);
		r += recursive_outside_in(cur_variable_values);
	}
	return r;
}

void input() {
	freopen("sample_simulation_input.txt", "r", stdin);
	// Note : All these inputs will be obtained from the dataset provided to us. As this is a sample implementation,
	// we are taking them from stdin.
	// Enter the number of variables.
	cin >> no_of_variables;
    domain_list.resize(no_of_variables);
    factor_index_by_variable.resize(no_of_variables + 1);
    // Enter the number of factors.
	cin >> no_of_factors;
    factors.resize(no_of_factors);
    // Enter the domain for each variable. Note that each variable takes values from 0 to domain - 1(both inclusive).
	for (int i = 1; i <= no_of_variables; i++) {
        cin >> domain_list[i];
    }
	for (int i = 0; i < no_of_factors; i++) {
		// Enter the size of the factor.
		int factor_size;
		cin >> factor_size;
		// Enter the variables involved in factor i.
		vector<int> factor_variables(factor_size, 0);
		int entries = 1;
        for (int j = 0; j < factor_size; j++) {
            cin >> factor_variables[j];
            factors[i].push_back(factor_variables[j]);
            factor_index_by_variable[factor_variables[j]].push_back(i);
            entries *= domain_list[factor_variables[j]];
        }
        int sparsity_factor;
		cin >> sparsity_factor;
		entries -= floor((double)((1.0*entries*sparsity_factor)/100));
        for (int j = 1; j <= entries; j++) {
            vector< int > factor_input(factor_size);
			// Enter the factor values with the domain value followed by the probability.
			for (int k = 0; k < factor_size; k++) {
				cin >> factor_input[k];
                if (factor_variable_domain_exists[i][factor_variables[k]].find(factor_input[k]) == factor_variable_domain_exists[i][factor_variables[k]].end()) {
                    factor_variable_domain[i][factor_variables[k]].push_back(factor_input[k]);
                    factor_variable_domain_exists[i][factor_variables[k]][factor_input[k]] += 1;
                }
			}
            string factor_value_string;
            cin >> factor_value_string;
            double factor_value = stod(factor_value_string);
            if (factor_value > 0.0) {
                insert(&dwise_trie_ptr[i], factor_input, factor_value);
            }
		}
	}
    return;
}

int main() {
	input();
	vector<int> variable_values;
	cout << "The final result is : " << recursive_outside_in(variable_values) << "\n";
	return 0;
}
