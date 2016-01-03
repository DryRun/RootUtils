import sys
import math
import ROOT

def WeightedKSTest(data_1, data_2):
	#print "[WeightedKSTest] DEBUG : data_1 = ",
	#print data_1
	#print "[WeightedKSTest] DEBUG : data_2 = ",
	#print data_2

	prob = -1
	# Require at least two points
	n_1 = len(data_1)
	if n_1 <= 2:
		print "[WeightedKSTest] ERROR : data_1 needs at least three points for KS test"
		sys.exit(1)
	n_2 = len(data_2)
	if n_2 <= 2:
		print "[WeightedKSTest] ERROR : data_2 needs at least three points for KS test"
		sys.exit(1)

	total_weight_1 = sum([x[1] for x in data_1])
	total_weight_2 = sum(x[1] for x in data_2)
	rdiff = 0.
	rdmax = 0.
	i_1 = 0
	i_2 = 0
	ok = False
	for i in xrange(n_1 + n_2):
		if data_1[i_1][0] < data_2[i_2][0]:
			rdiff -= 1. * data_1[i_1][1] / total_weight_1
			i_1 += 1
			if i_1 >= n_1:
				ok = True
				break
		elif data_1[i_1][0] > data_2[i_2][0]:
			rdiff += 1. * data_2[i_2][1] / total_weight_2
			i_2 += 1
			if i_2 >= n_2:
				ok = True
				break
		else:
			x = data_1[i_1][0]
			while data_1[i_1][0] == x and i_1 < n_1:
				rdiff -= 1. * data_1[i_1][1] / total_weight_1
				i_1 += 1
				if i_1 >= n_1:
					break
			while data_2[i_2][0] == x and i_2 < n_2:
				rdiff += 1. * data_2[i_2][1] / total_weight_2
				i_2 += 1
				if i_2 >= n_2:
					break
			if i_1 >= n_1:
				ok = True
				break
			if i_2 >= n_2:
				ok = True
				break
		rdmax = max(rdmax, abs(rdiff))
	if ok:
		rdmax = max(rdmax, abs(rdiff))
		z = rdmax * (total_weight_1 * total_weight_2 / (total_weight_1 + total_weight_2))**0.5
		prob = ROOT.TMath.KolmogorovProb(z)

	return (prob, rdmax)