import collections
import math
import numpy
import random #as stdlib_random
import scipy

gen_prob_index = 0
zero_index = 0

file_write = open('sample_simulation_input_tree.txt', 'w')

##---- beginning functions-----------------------##
# This function recursively fills a factor for each combination of the domain of its variables.
def recursively_fill_values_of_domain(cur_factor, factor_index, factor, domain_dictionary, generated_probabilities, domain_list):
    if factor_index == len(factor):
        global gen_prob_index
        global factor_model
        global zero_index # counting the zero
        global number_of_zeros
        global number_of_non_zeros
        toss = numpy.random.randint(0, 2)
        tmp_domain_list = list(domain_list)
        if (toss == 1 or zero_index + 1 >=  number_of_zeros) and gen_prob_index < number_of_non_zeros:# is for non-zero case
            tmp_domain_list.append(generated_probabilities[0][gen_prob_index]) # gen_prob_index is for traversing generated probabilities
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
    for cur_var_domain in range(0, domain_dictionary[factor[factor_index]]): # factor -> array of the variables, factor_index -> indices of the array
        tmp_domain_list = list(domain_list) # building the factor domain entry; e.g. [0 0 0], [0 0 1] etc. for a three variable factor
        tmp_domain_list.append(cur_var_domain)
        recursively_fill_values_of_domain(cur_factor, factor_index + 1, factor, domain_dictionary, generated_probabilities, tmp_domain_list)

#------------------------------------------------------------
# function to create a tree-shaped graphical model
def create_tree(num_var):
	
	global factor_list

	print ('num_var:' + str(num_var))
	
	# Base case - num_var = 0
	if (num_var < 1):
		return

	# initialize max num of edges for root
	num_edges = num_var - 1

	# create array for visited flag
	vis = numpy.array([0]*num_var)
	##print ('visited:' + str(vis))

	# create a list of active variables
	var_list = []
	factor_list = []
	
	# generate a root - number between 1 to num_var and make as root
	root = numpy.random.randint(0, num_var)
	print('root: ' + str(root))
	vis[root] = 1
	var_list.append(root)
	##print ('visited: ' + str(vis))
	##print('var_list: ' + str(var_list))
	
	# get the free (untaken) varibles
	non_vis = numpy.where(vis == 0)[0]
	##print('non_vis: ' + str(non_vis))

	# while loop
	while(len(non_vis) > 0 and len(var_list) > 0):
		
		##print('var_list:' + str(var_list))
		p = random.choice(var_list)
		##print('p:' + str(p))
		var_list.remove(p)
		
		# rand generator for num factors
		if(num_edges > 1):
			num = random.randint(1,num_edges) 
		else:
			num = num_edges

		##print(num)
		
		# generate factors bet root and var
		for i in range(0, num):
			# randomly generate edge factors
			non_vis = numpy.where(vis == 0)[0]
			edge_var = random.choice(non_vis)		
			# create factor
			factor_list.append([p,edge_var])
			# update visited list
			vis[edge_var] = 1
			# update num_edge
			num_edges = num_edges - 1
			# push edge variable in var_list
			var_list.append(edge_var)
	if(len(factor_list) < 1):
		factor_list.append(root)
	print(factor_list)
	print('----------')
##------------------------xxxxxxxxxxxxx----------------------------------##
# A few inputs...
#create_tree(0)
#create_tree(1)
#create_tree(2)
#create_tree(3)
#create_tree(5)
#create_tree(7)
#create_tree(9)
#create_tree(13)
##-----------------------xxxxxxxxxxxxxxxxxxxx-----------------------------##

# Starting main code
#------------------------------------------------------------

# Getting the max_variables and randomly generating factors for a randomly chosen set of variables.
print ("Enter the maximum number of variables.")
max_variables = input()

# Randomly generating the number of variables
no_of_variables = numpy.random.randint(2, int(max_variables))

# Writing to a file
file_write.write(str(no_of_variables))
file_write.write('\n')

# Calculating max no of factors possible and generating the factor set
no_of_factors = no_of_variables + (no_of_variables*(no_of_variables - 1))/2
factor_model = [list()]*int(no_of_factors)
factor_set = set()

# Create the graphical tree (factors)
create_tree(no_of_variables)

# Creating the factors
for i in range(0,len(factor_list)):
	factor_set. add(frozenset(factor_list[i]))
	
no_of_factors = len(factor_set)
file_write.write(str(no_of_factors))
file_write.write('\n')
print(factor_set)

# Getting the max_domain value and randomly generating domains for each variable.
print ("Enter the maximum domain that variables can take.")
max_domain = input()
domain_dictionary = dict()

# Randomly generating the domain of each variable
for each_variable in range(0, no_of_variables):
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
	
    print("Enter the % sparsity:")
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

