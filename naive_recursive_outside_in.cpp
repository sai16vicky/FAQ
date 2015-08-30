// Note : Each factor in this program is dependent on only two variables.
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

double factorvalues[11][11][11][11];
int variables, factors, domain;

void input() {
	// Note : All these inputs will be obtained from the dataset provided to us. As this is a sample implementation,
	// we are taking them from stdin.
	// Enter the number of variables.
	cin >> variables;
	// Enter the number of factors.
	cin >> factors;
	// Enter the domain for each variable. Note that each variable takes values from 0 to domain - 1(both inclusive).
	cin >> domain;
	for (int i = 0; i < factors; i++) {
		int x1, x2;
		// Enter the variables involved in this factor.
		cin >> x1 >> x2;
		// Enter the factor as a matrix with values of x1 on the row and x2 on the column.
		for (int j = 0; j < domain; j++) {
			for (int k = 0; k < domain; k++) {
				cin >> factorvalues[x1][x2][j][k];
			}
		} 
	}
}

vector<int> get_domain_values_non_zero(vector<int>& variable_values) {
	int cur_variable = (int)variable_values.size() + 1;
	vector<int> non_zero_domain_values;
	for (int i = 0; i < domain; i++) {
		// Check the previous variable values for a 0.
		bool is_domain_value_zero_for_all_factors = true;
		for (int j = 1; j < cur_variable; j++) {
			// Check if there is a non-zero value from factors with already fixed variables
			if (factorvalues[j][cur_variable][variable_values[j-1]][i] != 0) {
				is_domain_value_zero_for_all_factors = false;
				break;
			}
		} 
		if (is_domain_value_zero_for_all_factors) {
			for (int j = 0; j < domain; j++) {
				for (int k = cur_variable + 1; k <= variables; k++) {
					if (factorvalues[cur_variable][k][i][j] != 0) {
						is_domain_value_zero_for_all_factors = false;
						break;
					} 
				}
				if (!is_domain_value_zero_for_all_factors) {
					break;
				}
			}
		}
		if (!is_domain_value_zero_for_all_factors) {
			non_zero_domain_values.push_back(i);
		}
	}
	return non_zero_domain_values;
}

double calculate_final_probabilities(vector<int>& variable_values) {
	double total = 1.0;
	for (int i = 1; i <= variables; i++) {
		for (int j = i+1; j <= variables; j++) {
			total *= factorvalues[i][j][variable_values[i-1]][variable_values[j-1]];
		}
	}
	return total;
}

double recursive_outside_in(vector<int>& variable_values) {
	if ((int)variable_values.size() == variables) {
		return calculate_final_probabilities(variable_values);
	} 		
	double r = 0.0;
	vector<int> I = get_domain_values_non_zero(variable_values);
	for (int i = 0; i < (int)I.size(); i++) {
		vector<int> cur_variable_values(variable_values.begin(), variable_values.end());
		cur_variable_values.push_back(I[i]);
		double si_k_plus_1 = recursive_outside_in(cur_variable_values);
		r += si_k_plus_1;
	}
	return r;
}

int main() {
	input();
	vector<int> variable_values;
	cout << "The final result is : " << recursive_outside_in(variable_values) << "\n";
	return 0;
}
