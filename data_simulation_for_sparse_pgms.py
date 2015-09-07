import collections
import math
import numpy
import random as stdlib_random
import scipy
import sets

gen_prob_index = 0
zero_index = 0

# This function recursively fills a factor for each combination of the domain of its variables.
def recursively_fill_values_of_domain(cur_factor, factor_index, factor, domain_dictionary, generated_probabilities, domain_list):
		if factor_index == len(factor):
				global gen_prob_index
				global factor_model
				global zero_index
				global number_of_zeros
				toss = numpy.random.randint(0, 1)
				tmp_domain_list = list(domain_list)
				if toss == 1 or zero_index == number_of_zeros: 
						tmp_domain_list.append(generated_probabilities[0][gen_prob_index])
						gen_prob_index = gen_prob_index + 1
						print tmp_domain_list
				else :
						tmp_domain_list.append(0)
						zero_index = zero_index + 1
				return 
		for cur_var_domain in range(0, domain_dictionary[factor[factor_index]]):
				tmp_domain_list = list(domain_list)
				tmp_domain_list.append(cur_var_domain)
				recursively_fill_values_of_domain(cur_factor, factor_index + 1, factor, domain_dictionary, generated_probabilities, tmp_domain_list)

# Getting the max_variables and randomly generating factors for a randomly chosen set of variables.
max_variables = input()
no_of_variables = numpy.random.randint(2, max_variables)
no_of_factors = numpy.random.randint(1, 2**max_variables - 1)
factor_model = [list()]*no_of_factors
factor_set = set()
for each_factor in range(0, no_of_factors):
		cur_factor = list()
		no_of_variables_in_factor = numpy.random.randint(1, no_of_variables)
		last = 0
		for each_variable_in_factor in range(0, no_of_variables_in_factor):
				if last + 1 < no_of_variables:
						last = numpy.random.randint(last + 1, no_of_variables)
						cur_factor.append(last)
		if len(cur_factor) == no_of_variables_in_factor:
				factor_set.add(frozenset(cur_factor))#
no_of_factors = len(factor_set)
print(factor_set)

# Getting the max_domain value and randomly generating domains for each variable.
max_domain = input()
domain_dictionary = dict()
for each_variable in range(1, no_of_variables + 1):
		domain_of_variable = numpy.random.randint(2, max_domain)
		domain_dictionary[each_variable] = domain_of_variable
print(domain_dictionary)

# We need to get this feature up. As of now we are generating random models.
cur_factor = 0
for factor in factor_set:
		factor_size = 1
		factor = sorted(list(factor))
		print factor
		for factor_var in factor:
				factor_size = factor_size * domain_dictionary[factor_var]
		sparsity_factor = input()
		number_of_zeros = (sparsity_factor/100.0) * factor_size
		non_zero_factor_size = factor_size - number_of_zeros
		# The generated probabilities sum to 1 as we use Dirichlet distribution.
		generated_probabilities = numpy.random.dirichlet(numpy.ones(non_zero_factor_size), size=1).tolist()
		gen_prob_index = 0
		zero_index = 0
		recursively_fill_values_of_domain(cur_factor, 0, factor, domain_dictionary, generated_probabilities, list())
		print
		cur_factor = cur_factor + 1

		



