#include "dwisetrie.h"

#define INF (int)1e9
#define MAXENTRIES 20
#define MAXVARIABLES 20
#define N 10 // set the maximum number of variables.

using namespace std;

int no_of_variables, no_of_factors, f, vis_factor[MAXFACTORS];
map<vector<int>, double> factor_entry_map[MAXFACTORS][MAXENTRIES];
vector< vector< vector<int> > > factor_entry_vector;
map<int,int> factor_variable_domain_exists[MAXFACTORS][MAXVARIABLES];
vector<int> domain_list;
vector< vector<int> > factors, factors_variable_order, factor_index_by_variable;
vector<int> factor_variable_domain[MAXFACTORS][MAXVARIABLES];

int vis_factor[MAXFACTORS];
map<int, int> var_map, r_var_map;

typedef struct _trienode{
    double f_val;
    int dom_val;
    struct _trienode* right;
    struct _trienode* next;
}trienode;

typedef struct _ionode{
    int var_id;
    vector<int> factors[MAXFACTORS];
}ionode;

trienode tr_node[no_of_factors];
ionode node[no_of_variables];

dwisetrie* iterative_outside_in_with_free_variables(int _f, vector< vector<int> > _factor_index_by_variable) {
    int k = 0;
    if (_f == 0) {
        return NULL;
    }
    dwisetrie new_factor;
    vector<int> factor_input;
    vector<int> x(no_of_variables + 1, 0);
    x[k+1] = -INF;
    while (x[1] < INF) {
        int y = x[k+1];
        vector<int> ys;
        while(1) {
            ys.clear();
            for (int i = 0; i < (int)_factor_index_by_variable[k+1].size(); i++) {
                int factor_index = _factor_index_by_variable[k+1][i];
                ys.push_back(conditional_query(y, k + 1, factor_index));
            }
            int miny = INF;
            for (int i = 0; i < (int)ys.size(); i++) {
                miny = min(miny, ys[i]);
                y = max(y, ys[i]);
            }
            if (y == miny) {
                x[k+1] = y;
                if (k + 1 <= _f) {
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
                insert(&new_factor, factor_input, calculate_probabilities(x));
            }
        }
        else {
            if (k > 0) {
                k = k - 1;
            }
        }
    }
    return &new_factor;
}

class cmp {
    vector<int> var_order;
public:
    cmp(vector<int> _var_order) : var_order(_var_order) {}
    bool operator(vector<int> v1, vector<int> v2) {
        int n = var_order.size();
        for(int i=n-1; i>=0; i--) {
            if(v1[var_order[i]] != v2[var_order[i]]) {
                return v1[var_order[i]] < v2[var_order[i]];
            }
        }
        return true;
    }
};

void construct_trie(vector< vector<int> >& sorted_factors, vector<int>& variable_order, trienode* root) {
    int n = variable_order.size();
    for(int i=0; i <(int)sorted_factors.size(); i++) {
        trienode* start = root;
        for(int j = n-1; j>=0; j--) {
            int cur = sorted_factors[i][variable_order[j]];
            if(start->down == NULL) {
                trienode* tmp = new trienode();
                tmp->dom_val = cur;
                if(j == 0) {
                    tmp->f_val = sorted_factors[i][n];
                }
                start->down = tmp;
                start = start->down;
            }
            else {
                start = start->down;
                while(start->next != NULL) {
                    start = start->next;
                }
                trienode* tmp = new trienode();
                tmp->dom_val = cur;
                if(j == 0) {
                    tmp->f_val = sorted_factors[i][n];
                }
                start->right = tmp;
                start = start->right;
            }
        }
    }
    return ;
}

void preprocess(vector<int>& variable_order, vector< vector < vector<int> > >& factor_entry) {
    factors_variable_order.resize(no_of_factors);
    factor_index_by_variable.resize(no_of_variables + 1);
    memset(vis_factor, 0, sizeof vis_factor);
    int n = variable_order.size();
    for(int i=n-1; i>=0 ;i--) {
        var_map[i] = variable_order[i];
        r_var_map[variable_order[i]] = i;
    }
    for(int i=n-1; i>=0; i--) {
        int st = 0;
        node[i].var_id = variable_order[i];
        for(int j=0; j<MAXFACTORS; j++) {
            node[i].factors[j].clear();
        }
        for(int j=0; j<no_of_factors; j++) {
            int var_in_factor = 0;
            for(int k = 0; k < (int)factors[j].size(); k++) {
                if(factors[j][k] == variable_order[i]) {
                    var_in_factor = 1;
                }
            }
            if(var_in_factor and !vis_factor[j]) {
                vis_factor[j] = 1;
                vector< pair<int,int> > tmp_factor;
                for(int k=0; k<(int)factors[j].size(); k++) {
                    tmp_factor.push_back(make_pair(r_var_map[factors[j][k]], factors[j][k]));
                }
                sort(tmp_factor.begin(), tmp_factor.end());
                for(int k=0; k<(int)tmp_factor.size(); k++) {
                    node[i].factors[st].push_back(tmp_factor[k].second);
                }
                sort(factor_entry[j].begin(), factor_entry[j].end(), cmp(variable_order));
                construct_trie(factor_entry[j], variable_order, &trienode[st]);
                st++;
            }
        }
    }
    
    return ;
}

dwisetrie* inside_out_without_free_variables(vector<int>& variable_order) {
    preprocess(variable_order, factor_entry_vector);
    vector<int> E[no_of_variables + 1];
    E[no_of_variables].resize(factors.size());
    for(int i = 0; i < (int)factor_index_by_variable.size(); i++) {
        E[no_of_variables].push_back(i);
    }
    int k = no_of_variables;
    while(k >= 1) {
        int var_k = variable_order[k - 1];
        map<int,int> U_k;
        map<int,int> delta_k;
        map<int,int>::iterator it;
        vector< vector<int> > factor_index_by_variable_tmp;
        factor_index_by_variable_tmp.resize(no_of_variables + 1);
        for(int i = 0; i < (int)factor_index_by_variable[var_k].size(); i++) {
            delta_k[factor_index_by_variable[var_k][i]] += 1;
            for(int j = 0; j < (int)factors[factor_index_by_variable[k][i]].size(); j++) {
                U_k[factors[factor_index_by_variable[k][i]][j]] += 1;
                factor_index_by_variable_tmp[factors[factor_index_by_variable[k][i]][j]].push_back(factor_index_by_variable[k][i]);
            }
        }
        vector<int> new_factor_variables;
        for(it = U_k.begin(); it != U_k.end(); it++) {
            if(it->first != k) {
                new_factor_variables.push_back(it->first);
            }
        }
        dwisetrie* outside_in_result = iterative_outside_in_with_free_variables(new_factor_variables.size(), factor_index_by_variable_tmp);
        for(int i = 0; i < (int)E[k].size(); i++) {
            if(delta_k.find(E[k][i]) == delta_k.end()) {
                E[k - 1].push_back(E[k][i]);
            }
        }
        k = k - 1;
    }
}

void input() {
	freopen("sample_simulation_input.txt", "r", stdin);
    cin >> no_of_variables;
    domain_list.resize(no_of_variables);
    factor_index_by_variable.resize(no_of_variables);
    cin >> no_of_factors;
    factors.resize(no_of_factors);
   	for (int i = 0; i < no_of_variables; i++) {
        cin >> domain_list[i];
    }
    factor_entry_vector.resize(no_of_factors);
	for (int i = 0; i < no_of_factors; i++) {
        int factor_size;
		cin >> factor_size;
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
        factor_entry_vector.resize(entries);
        for (int j = 0; j < entries; j++) {
            vector< int > factor_input(factor_size);
            for (int k = 0; k < factor_size; k++) {
				cin >> factor_input[k];
                factor_entry_vector[i][j].push_back(factor_input[k]);
            }
            string factor_value_string;
            cin >> factor_value_string;
            double factor_value = stod(factor_value_string);
            factor_entry_map[i][j][factor_input] = factor_value;
            factor_entry_vector[i][j].push_back(factor_value);
        }
    }
    return;
}

vector<int> calculate_variable_order() {
    return vector<int>();
}

int main() {
    input();
    vector<int> variable_order = calculate_variable_order();
    dwisetrie* result = iterative_outside_in_with_free_variables(variable_order);
	return 0;
}
