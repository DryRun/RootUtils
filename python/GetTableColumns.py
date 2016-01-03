import os
import sys
import fileinput

debug = False

def GetTableColumns(p_input_file_path, p_output_file_path, p_column_select):
	print "Welcome to GetTableColumns(" + p_input_file_path + ", " + p_column_select + ")"

	# Run over the input file once to get the number of columns, and also to check that the file is a consistent table
	input_file = open(p_input_file_path, 'r')
	n_separators = 0
	for line in fileinput.input([p_input_file_path]):
		# Skip non-column rows
		if not "&" in line:
			continue
		else:
			c_separators = line.count("&")
			# Check consistency
			if n_separators != 0:
				if n_separators != c_separators:
					print "ERROR : Inconsistent number of columns. Current line = "
					print line
					print "ERROR : Previously found " + str(n_separators) + " columns."
					sys.exit(1)
			n_separators = c_separators
	n_columns = n_separators + 1
	if debug:
		print "Detected " + str(n_columns) + " columns in input table"

	# Make dict specifying which columns to keep. 1=keep, 0=discard
	column_select = {}
	for i in xrange(n_columns):
		column_select[i] = False
	for keep_column in p_column_select.split(":"):
		column_select[int(keep_column)] = True
	if debug:
		for column_index, column_bool in column_select.iteritems():
			print str(column_index) + ":",
			if column_bool:
				print "True\t",
			else:
				print "False\t",
		print "\n"

	# Make output file
	output_file = open(p_output_file_path, 'w')
	for line in fileinput.input([p_input_file_path]):
		if debug:
			print "Read in line " + line
		if not "&" in line:
			print "\tWriting whole line to output"
			output_file.write(line)
		else:
			print "\tWriting selected columns to output"
			column_index = 0
			new_line = ""
			for column in line.split("&"):
				if column_select[column_index]:
					new_line += column
					if column_index != n_columns - 1:
						new_line += "\t&\t"
				column_index += 1
			'\t'.join(new_line.split())
			output_file.write(new_line)
	output_file.close()
	input_file.close()



if __name__ == "__main__":
	import argparse
	parser = argparse.ArgumentParser(description = 'Prepare histograms for limit setting')
	parser.add_argument('--input', type=str, help='Input latex table')
	parser.add_argument('--output', type=str, help='Output latex table')
	parser.add_argument('--columns', type=str, help='Columns to select (colon-separated)')
	args = parser.parse_args()

	if args.output:
		GetTableColumns(args.input, args.output, args.columns)
	else:
		GetTableColumns(args.input, args.input + ".column_select", args.columns)
