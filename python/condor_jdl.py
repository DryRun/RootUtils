# Make condor .jdl files

import os
import sys

class CondorJDLFile():
	def __init__(self, jdl_path):
		self.jdl_path = jdl_path
		self.exec_name = ""
		self.job_name = ""
		self.input_files = []

		os.system("cp ~/condor_template.jdl {}".format(self.jdl_path))

	def set_executable(self, exec_name):
		self.exec_name = exec_name
		os.system("sed -i .bak 's/SUB_EXEC_NAME/{}/' {}".format(self.exec_name, self.jdl_path))

	def set_job_name(self, job_name):
		self.job_name = job_name
		os.system("sed -i .bak 's/SUB_JOB_NAME/{}/' {}".format(self.job_name, self.jdl_path))

	def set_input_files(self, input_files):
		self.input_files = input_files
		input_file_string = ""
		for input_file in input_files:
			input_files_string += input_file + " "
		os.system("sed -i .bak 's/SUB_INPUT_FILES/{}/' {}".format(input_files_string, self.jdl_path))

	def add_arguments(self, arguments):
		os.system("sed -i .bak 's/SUB_ARGUMENTS/{}/' {}".format(arguments, self.jdl_path))
		os.system("sed -i .bak 's/#Arguments/Arguments/' {}".format(arguments, self.jdl_path))

	def set_queue(self, arguments):
		os.system("sed -i .bak 's/SUB_QUEUE/{}/' {}".format(arguments, self.jdl_path))
		os.system("sed -i .bak 's/#Queue/Queue/' {}".format(arguments, self.jdl_path))


	def submit(self, ):
		if self.exec_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.exec_name not set."
			sys.exit(1)
		if self.job_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.job_name not set."
			sys.exit(1)

		os.system("condor_submit {}".format(self.jdl_path))