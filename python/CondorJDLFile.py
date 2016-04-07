# Make condor .jdl files

import os
import sys

class CondorJDLFile():
	def __init__(self, submit_dir):
		self.submit_dir = submit_dir
		self.jdl_path = self.submit_dir + "/condor_run.jdl"
		self.exec_name = ""
		self.job_name = ""
		self.input_files = []
		self.arguments = ""
		self.queue = "1"

		os.system("cp ~/condor_template.jdl {}".format(self.jdl_path))

	def set_executable(self, exec_name):
		self.exec_name = exec_name
		self.call_sed("SUB_EXEC_NAME", self.exec_name, self.jdl_path)

	def set_submit_dir(self, submit_dir):
		self.submit_dir = submit_dir

	def set_job_name(self, job_name):
		self.job_name = job_name
		self.call_sed("SUB_JOB_NAME", self.job_name, self.jdl_path)

	def set_input_files(self, input_files):
		self.input_files = input_files
		input_files_string = ""
		for input_file in input_files:
			input_files_string += input_file + ", "
		self.call_sed("SUB_INPUT_FILES", input_files_string, self.jdl_path)

	def add_arguments(self, arguments):
		self.arguments = arguments
		self.call_sed("SUB_ARGUMENTS", self.arguments, self.jdl_path)

		# Uncomment the Arguments line
		self.call_sed("#Arguments", "Arguments", self.jdl_path)

	def set_queue(self, queue):
		self.queue = queue


	def submit(self, ):
		if self.exec_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.exec_name not set."
			sys.exit(1)
		if self.job_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.job_name not set."
			sys.exit(1)

		# Set queue
		self.call_sed("SUB_QUEUE", self.queue, self.jdl_path)

		# Submit
		cwd = os.getcwd()
		os.chdir(self.submit_dir)
		os.system("condor_submit condor_run.jdl")
		os.chdir(cwd)

	def call_sed(self, old_str, new_str, file_path):
		old_str_safe = old_str.replace("$", "\$").replace("@", "\@")
		new_str_safe = new_str.replace("$", "\$").replace("@", "\@")
		sed_command = "sed -i -e \"s@{}@{}@\" {}".format(old_str_safe, new_str_safe, file_path)
		#print sed_command
		os.system(sed_command)