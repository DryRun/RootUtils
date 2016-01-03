import os
import sys
import fileinput
import linecache

debug = False

class RowConsistencyError(Exception):
	def __init__(self, value):
		self.value = value
	def __str__(self):
		return repr(self.value)

class LineConsistencyError(Exception):
	def __init__(self, value):
		self.value = value
	def __str__(self):
		return repr(self.value)

def GetColumnSelect(p_column_select_string, p_n_columns):
	column_select = {}
	for i in xrange(p_n_columns):
		column_select[i] = False
	for keep_column in p_column_select_string.split(":"):
		column_select[int(keep_column) % p_n_columns] = True
	return column_select

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
	column_select = GetColumnSelect(p_column_select, n_columns)

	# Make output file
	output_file = open(p_output_file_path, 'w')
	for line in fileinput.input([p_input_file_path]):
		if debug:
			print "Read in line " + line
		if not "&" in line:
			output_file.write(line)
		else:
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

# p_tables_columns = [{"TablePath":path, "ColumnSelectString":{0:True, 1:False, 2:False, ...}}]
def CombineColumns(p_tables_columns, p_output_file_path):
	print "Welcome to CombineColumns. Inputs:"
	print p_tables_columns
	print p_output_file_path
	# Check inputs. All tables must have the same number of lines and table rows.
	n_rows = 0
	n_lines = 0
	for c_iter in p_tables_columns:
		c_n_rows = 0
		c_n_lines = 0
		for line in fileinput.input([c_iter["TablePath"]]):
			c_n_lines += 1
			if "&" in line:
				c_n_rows += 1
		if n_rows == 0:
			n_rows = c_n_rows
		else:
			if n_rows != c_n_rows:
				raise RowConsistencyError("Input tables do not have the same number of rows.")
		if n_lines == 0:
			n_lines = c_n_lines
		else:
			if n_lines != c_n_lines:
				raise LineConsistencyError("Input tables do not have the same number of lines.")
		fileinput.close()

	# Create a column select mask for each file
	for i in xrange(len(p_tables_columns)):
		# Get number of columns
		c_n_columns = 0
		for line in fileinput.input(p_tables_columns[i]["TablePath"]):
			if "&" in line:
				c_n_columns = line.count("&") + 1
				break
		fileinput.close()
		p_tables_columns[i]["ColumnSelect"] = GetColumnSelect(p_tables_columns[i]["ColumnSelectString"], c_n_columns)

	output_file = open(p_output_file_path, 'w')
	for c_line_number in xrange(n_lines):
		first = True
		c_line = ""
		for c_iter in p_tables_columns:
			c_table_line = linecache.getline(c_iter["TablePath"], c_line_number).rstrip()
			# If no '&', print line from first file only
			if not "&" in c_table_line:
				if first:
					first = False
					c_line = c_table_line
					break
			else:
				c_table_line_elements = c_table_line.split("&")
				for c_column_number in xrange(len(c_table_line_elements)):
					if c_iter["ColumnSelect"][c_column_number]:
						#print "Adding column " + str(c_column_number) + " from file " + c_iter["TablePath"]
						c_line += c_table_line_elements[c_column_number].replace("\\\\","") + "\t&\t"
			first = False
		c_line.rstrip("\t&")
		if "&" in c_line:
			c_line += "\\\\"
		c_line += "\n"
		output_file.write(c_line)




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
