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

		# Effective weights
		sum_w = 0.
		sum_w2 = 0.
		for item in data_1:
			sum_w += item[1]
			sum_w2 += item[1]**2
		if sum_w2 > 0:
			effective_weight_1 = sum_w**2 / sum_w2
		else:
			print "[WeightedKSTest] ERROR : Sum of weights is zero"
			sys.exit(1)

		sum_w = 0.
		sum_w2 = 0.
		for item in data_2:
			sum_w += item[1]
			sum_w2 += item[1]**2
		if sum_w2 > 0:
			effective_weight_2 = sum_w**2 / sum_w2
		else:
			print "[WeightedKSTest] ERROR : Sum of weights is zero"
			sys.exit(1)



		print "Effective weight 1 = " + str(effective_weight_1) + " / 2 = " + str(effective_weight_2)
		z = rdmax * (effective_weight_1 * effective_weight_2 / (effective_weight_1 + effective_weight_2))**0.5
		prob = ROOT.TMath.KolmogorovProb(z)

	return (prob, rdmax)

def BinnedKSTest(data_1, data_2):
	# Turn data into histograms
	data_min = 1.e20
	data_max = -1.e20
	for item in data_1:
		if item[0] < data_min:
			data_min = item[0]
		if item[0] > data_max:
			data_max = item[0]
	for item in data_2:
		if item[0] < data_min:
			data_min = item[0]
		if item[0] > data_max:
			data_max = item[0]
	# Round to the nearest 5
	data_min = 5. * math.floor(data_min / 5.)
	data_max = 5. * math.ceil(data_max / 5.)
	bin_width = 1.
	nbins = int((data_max - data_min) / bin_width)
	h1 = ROOT.TH1D("h_data_1", "h_data_1", nbins, data_min, data_max)
	h1.Sumw2()
	h2 = ROOT.TH1D("h_data_2", "h_data_2", nbins, data_min, data_max)
	h2.Sumw2()
	for item in data_1:
		h1.Fill(item[0], item[1])
	for item in data_2:
		h2.Fill(item[0], item[1])

	ks = (h1.KolmogorovTest(h2), h1.KolmogorovTest(h2, "M"))
	h1.IsA().Destructor(h1)
	h2.IsA().Destructor(h2)
	return ks