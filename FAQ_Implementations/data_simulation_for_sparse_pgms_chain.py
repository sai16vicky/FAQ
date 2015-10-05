import collections
import math
import numpy
import random as stdlib_random
import scipy
#import sets

gen_prob_index = 0
zero_index = 0

file_write = open('sample_simulation_input.txt', 'w')

# This function recursively fills a factor for each combination of the domain of its variables.
def recursively_fill_values_of_domain(cur_factor, factor_index, factor, domain_dictionary, generated_probabilities, domain_list):
    if factor_index == len(factor):
        global gen_prob_index
        global factor_model
        global zero_index
        global number_of_zeros
        global number_of_non_zeros
        toss = numpy.random.randint(0, 2)
        tmp_domain_list = list(domain_list)
        if (toss == 1 or zero_index + 1 >=  number_of_zeros) and gen_prob_index < number_of_non_zeros:
            tmp_domain_list.append(generated_probabilities[0][gen_prob_index])
            gen_prob_index = gen_prob_index + 1
            print (tmp_domain_list)
            final_domain_list = str(tmp_domain_list).replace(',', ' ')
            final_domain_list = final_domain_list.replace('[', '')
            final_domain_list = final_domain_list.replace(']', '')
            file_write.write(final_domain_list)
            file_write.write('\n')
        else :
            tmp_domain_list.append(0)
            print (tmp_domain_list)
            zero_index = zero_index + 1
        return
    for cur_var_domain in range(0, domain_dictionary[factor[factor_index]]):
        tmp_domain_list = list(domain_list)
        tmp_domain_list.append(cur_var_domain)
        recursively_fill_values_of_domain(cur_factor, factor_index + 1, factor, domain_dictionary, generated_probabilities, tmp_domain_list)

# Getting the max_variables and randomly generating factors for a randomly chosen set of variables.
print ("Enter the maximum number of variables.")
max_variables = input()
no_of_variables = numpy.random.randint(2, int(max_variables))
file_write.write(str(no_of_variables))
file_write.write('\n')
no_of_factors = no_of_variables + (no_of_variables*(no_of_variables - 1))/2
factor_model = [list()]*int(no_of_factors)
factor_set = set()
# Factors of length 1 for each variable.
for each_variable in range(1, no_of_variables + 1):
    cur_factor = list()
    cur_factor.append(each_variable)
    factor_set.add(frozenset(cur_factor))
# Factors of length 2 for every adjacent pair of variables.
for each_factor in range(2, no_of_variables + 1):
    cur_factor = list()
    cur_factor.append(each_factor - 1)
    cur_factor.append(each_factor)
    factor_set. add(frozenset(cur_factor))
no_of_factors = len(factor_set)
file_write.write(str(no_of_factors))
file_write.write('\n')
#print(factor_set)

# Getting the max_domain value and randomly generating domains for each variable.
print ("Enter the maximum domain that variables can take.")
max_domain = input()
domain_dictionary = dict()

for each_variable in range(1, no_of_variables + 1):
    domain_of_variable = numpy.random.randint(2, int(max_domain))
    file_write.write(str(domain_of_variable))
    if each_variable < no_of_variables:
        file_write.write(' ')
    domain_dictionary[each_variable] = int(domain_of_variable)
print (domain_dictionary)
file_write.write('\n')

# We need to get this feature up. As of now we are generating random models.
cur_factor = 0
for factor in factor_set:
    factor_size = 1
    factor = sorted(list(factor))
    print (factor)
    final_factor = str(factor).replace(',', ' ')
    final_factor = final_factor.replace('[', '')
    final_factor = final_factor.replace(']', '')
    file_write.write(str(len(factor)))
    file_write.write('\n')
    file_write.write(final_factor)
    file_write.write('\n')
    for factor_var in factor:
        factor_size = factor_size * domain_dictionary[factor_var]
    sparsity_factor = input()
    file_write.write(str(sparsity_factor))
    file_write.write('\n')
    number_of_zeros = int(float(sparsity_factor)/100.0 * factor_size)
    non_zero_factor_size = factor_size - number_of_zeros
    # The generated probabilities sum to 1 as we use Dirichlet distribution.
    generated_probabilities = numpy.random.dirichlet(numpy.ones(non_zero_factor_size), size=1).tolist()
    number_of_non_zeros = non_zero_factor_size
    gen_prob_index = 0
    zero_index = 0
    recursively_fill_values_of_domain(cur_factor, 0, factor, domain_dictionary, generated_probabilities, list())
    cur_factor = cur_factor + 1




