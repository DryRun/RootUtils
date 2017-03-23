
# model1 = RooAbsPdf
# model2 = RooAbsPdf
# data = TH1
# x = RooRealVar
def get_F(model1, model2, data, x):
	N = data.GetNbinsX()
	obs = RooArgSet(x)
	n1 = model1.getParameters(obs).selectByAtrib("Constant", False)
	n2 = model2.getParameters(obs).selectByAtrib("Constant", False)
	rss1 = 0.
	rss2 = 0.
	for bin in xrange(1, data.GetNbinsX() + 1):
		if data.GetBinContent(bin) == 0:
			continue
		rss1 += (data.GetBinContent(bin) - model1.eval(data.GetXaxis().GetBinCenter(bin)))**2
		rss2 += (data.GetBinContent(bin) - model2.eval(data.GetXaxis().GetBinCenter(bin)))**2
	if n2 > n1:
		f21 = ((rss1 - rss2) / (n2 - n1)) / (rss2 / (N - n2))
	elif n1 > n2:
		f21 = ((rss2 - rss1) / (n1 - n2)) / (rss1 / (N - n1))
	else:
		print "[get_F] ERROR : Models have identical degrees of freedom, so F is not defined."
		sys.exit(1)
	return f21

# f = value of test statistic F
# n1, n2 = degrees of freedom, with n1 < n2
def get_CL_from_F(f, n1, n2):
	return 1. - FDistI(f, n1, n2)

def get_CL(model1, model2, data, x):
	n1 = model1.getParameters(obs).selectByAtrib("Constant", False)
	n2 = model2.getParameters(obs).selectByAtrib("Constant", False)
	return get_CL_from_F(get_F(model1, model2, data, x), n1, n2)
