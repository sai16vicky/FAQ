import collections
import math
import numpy
import random #as stdlib_random
import scipy

# function to create a tree-shaped graphical model
def create_tree(num_var):
	print (num_var)
	
	# Base case - num_var = 0
	if (num_var < 1):
		return

	# initialize max num of edges for root
	num_edges = num_var - 1

	# create array for visited flag
	vis = numpy.array([0]*num_var)
	#print (vis)

	# create a list of active variables
	var_list = []
	factor_list = []
	
	# generate a root - number between 1 to num_var and make as root
	root = numpy.random.randint(0, num_var)
	print('root')
	print (root)
	vis[root] = 1
	var_list.append(root)
	#print (vis)
	#print(var_list)
	
	non_vis = numpy.where(vis == 0)[0]
	#print(non_vis)

	# while loop
	while(len(non_vis) > 0 and len(var_list) > 0):
		#p = var_list.pop()
		print('var_list:')
		print(var_list)
		p = random.choice(var_list)
		print('p:')
		print(p)
		var_list.remove(p)
		# rand generator for num factors
		#num = numpy.random.randint(0,num_edges)
		if(num_edges > 1):
			num = random.randint(1,num_edges) 
		else:
			num = num_edges

		#print(num)
		# generate factors bet root and var
		for i in range(0, num):
			# creating a map for generation of variables
			non_vis = numpy.where(vis == 0)[0]
			#print(non_vis)	
			#print(len(non_vis))		
			var_map = dict()	
			for k in range(0,len(non_vis)):
				var_map[k] = non_vis[k]
			# randomly generate edge factors
			edge_var_map = numpy.random.randint(0,len(non_vis))
			edge_var = var_map[edge_var_map]
			# create factor
			factor_list.append([p,edge_var])
			# update visited list
			vis[edge_var] = 1
			# update num_edge
			num_edges = num_edges - 1
			# push edge variable in var_list
			var_list.append(edge_var)

		#print(non_vis)
		#print(factor_list)
	if(len(factor_list) < 1):
		factor_list.append(root)
	print(factor_list)
	print('----------')
# A few inputs...
#create_tree(0)
#create_tree(1)
#create_tree(2)
create_tree(3)
create_tree(5)
create_tree(7)
#create_tree(9)
#create_tree(13)

