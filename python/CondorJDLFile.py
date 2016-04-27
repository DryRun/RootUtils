# Make condor .jdl files

import os
import sys

class CondorQueue():
	def __init__(self):
		self.transfer_input_files = []
		self.arguments = None
		self.n_jobs = 1

	def set_transfer_input_files(self, transfer_input_files):
		self.transfer_input_files = transfer_input_files
		print self.transfer_input_files

	def set_arguments(self, arguments):
		self.arguments = arguments

	def set_n_jobs(self, n_jobs):
		self.n_jobs = n_jobs

	def append_to_file(self, file_path):
		f = open(file_path, 'a')
		f.write("\n\n")
		if len(self.transfer_input_files) > 0:
			input_files_string = ""
			for input_file in self.transfer_input_files:
				input_files_string += input_file + ","
			input_files_string.rstrip(",")
			f.write('transfer_input_files = ' + input_files_string + "\n")

		if self.arguments:
			f.write('arguments = ' + self.arguments + "\n")

		f.write("Queue " + str(self.n_jobs) + "\n")
		f.close()


class CondorJDLFile():
	def __init__(self, submit_dir):
		self.submit_dir = submit_dir
		self.jdl_path = self.submit_dir + "/condor_run.jdl"
		self.exec_name = ""
		self.job_name = ""
		self.queues = []
		#self.input_files = []
		#self.arguments = ""
		#self.queue = "1"

		os.system("cp ~/condor_template.jdl " + self.jdl_path)

	def set_executable(self, exec_name):
		self.exec_name = exec_name
		self.call_sed("SUB_EXEC_NAME", self.exec_name, self.jdl_path)

	def set_submit_dir(self, submit_dir):
		self.submit_dir = submit_dir

	def set_job_name(self, job_name):
		self.job_name = job_name
		self.call_sed("SUB_JOB_NAME", self.job_name, self.jdl_path)

	def add_queue(self, transfer_input_files = None, arguments = None, n_jobs = 1):
		new_queue = CondorQueue()
		if transfer_input_files:
			new_queue.set_transfer_input_files(transfer_input_files)
		if arguments:
			new_queue.set_arguments(arguments)
		if n_jobs:
			new_queue.set_n_jobs(n_jobs)
		self.queues.append(new_queue)

	def submit(self, test=False):
		if self.exec_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.exec_name not set."
			sys.exit(1)
		if self.job_name == "":
			print "ERROR : In CondorJDLFile.submit(), self.job_name not set."
			sys.exit(1)

		# Write queues
		if len(self.queues) == 0:
			print "ERROR : In CondorJDLFile.submit(), no queues have been added yet. Please add at least one."
			sys.exit(1)

		for queue in self.queues:
			queue.append_to_file(self.jdl_path)

		# Submit
		cwd = os.getcwd()
		os.chdir(self.submit_dir)
		if not test:
			os.system("condor_submit condor_run.jdl")
		os.chdir(cwd)

	def call_sed(self, old_str, new_str, file_path):
		old_str_safe = old_str.replace("$", "\$").replace("@", "\@")
		new_str_safe = new_str.replace("$", "\$").replace("@", "\@")
		sed_command = "sed -i -e \"s@" + old_str_safe + "@" + new_str_safe + "@\" " + file_path
		#print sed_command
		os.system(sed_command)