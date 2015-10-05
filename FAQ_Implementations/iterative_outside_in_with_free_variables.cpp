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

// Commented by Arti - Oct 5, 2015
// Testing gedit again

using namespace std;

int no_of_variables, no_of_factors, f;
map<int,int> factor_variable_domain_exists[MAXFACTORS][MAXVARIABLES];
vector<int> domain_list;
vector< vector<int> > factors;
vector< vector<int> > factor_index_by_variable;
vector<int> factor_variable_domain[MAXFACTORS][MAXVARIABLES];
vector<int> free_variables;

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
            if (i == (int)factor_input.size() - 1) {
                return dwise_trie_ptr->factor_value;
            }
        } else {
            return 0.0;
        }
    }
    return 0.0;
}

// Optimize this function. Make use of the fact that we can use lo from the previous iteration.
int conditional_query(int yval, int cur_variable, int factor_index) {
    int lo = 0;
    int hi = factor_variable_domain[factor_index][cur_variable].size() - 1;
    int upperbound = INF;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        if (factor_variable_domain[factor_index][cur_variable][mid] > yval) {
            upperbound = min(upperbound, factor_variable_domain[factor_index][cur_variable][mid]);
            hi = mid - 1;
        }
        else {
            lo = mid + 1;
        }
    }
    return upperbound;
}

double calculate_probabilities(vector<int> domain_values) {
    double result = 1.0;
    for (int i = 0; i < (int)factors.size(); i++) {
        vector<int> tmp_domain_values;
        for (int j = 0; j < (int)factors[i].size(); j++) {
            tmp_domain_values.push_back(domain_values[factors[i][j]]);
        }
        result *= get_factor_value(&dwise_trie_ptr[i], tmp_domain_values);
    }
    return result;
}

dwisetrie iterative_outside_in_with_free_variables() {
    double r = 0.0;
    int k = 0;
    if (f == 0) {
        return new dwisetrie();
    }
    vector<int> factor_input;
    vector<int> x(no_of_variables + 1, 0);
    x[k+1] = -INF;
    no_of_factors += 1;
    while (x[1] < INF) {
        int y = x[k+1];
        vector<int> ys;
        while(1) {
            ys.clear();
            for (int i = 0; i < (int)factor_index_by_variable[k+1].size(); i++) {
                int factor_index = factor_index_by_variable[k+1][i];
                ys.push_back(conditional_query(y, k + 1, factor_index));
            }
            int miny = INF;
            for (int i = 0; i < (int)ys.size(); i++) {
                miny = min(miny, ys[i]);
                y = max(y, ys[i]);
            }
            if (y == miny) {
                x[k+1] = y;
                if (k + 1 <= f) {
                    factor_input.push_back(x[k+1]);
                }
                break;
            }
        }
        if (x[k+1] != INF) {
            // The following if condition is not required since we are passing in the required factor trie directly into the insert function.
            // if (k + 1 == f) {
                // new_dwise_trie_ptr = new dwisetrie();
            // }
            if (k + 1 < no_of_variables) {
                k = k + 1;
                x[k+1] = -INF;
            }
            else {
                insert(&dwise_trie_ptr[no_of_factors], factor_input, calculate_probabilities(x));
            }
        }
        else {
            if (k > 0) {
                k = k - 1;
            }
        }
    }
    return dwise_trie_ptr[no_of_factors];
}

void input() {
	freopen("sample_simulation_input.txt", "r", stdin);
	// Note : All these inputs will be obtained from the dataset provided to us. As this is a sample implementation,
	// we are taking them from stdin.
	// Enter the number of variables.
	cin >> no_of_variables;
    domain_list.resize(no_of_variables + 1);
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
        for (int j = 0; j < factor_size; j++) {
            sort(factor_variable_domain[i][factor_variables[j]].begin(), factor_variable_domain[i][factor_variables[j]].end());
        }
	}
    // Enter the size of variable order.
    cin >> f;
    free_variables.resize(f + 1);
    // Enter the variable order one by one.
    for (int i = 1; i <= f; i++) {
        free_variables[i] = i;
    }
    return;
}


int main() {
	input();
    cout << "The final result is : " << iterative_outside_in() << "\n";
    dwisetrie iterative_outside_in_with_free_variables();
	return 0;
}
