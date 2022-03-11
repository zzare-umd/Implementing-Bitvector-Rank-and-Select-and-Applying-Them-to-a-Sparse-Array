import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import os,sys
import pdb


# setup LaTeX path
os.environ['PATH'] = os.environ['PATH'] + ':/usr/texbin'

# Define Key Parameters for Font
mpl.rc('text',usetex=True)
mpl.rcParams['text.latex.preamble']=[r'\usepackage{amsmath,amssymb,newtxtt,newtxmath}']
mpl.rc('font',family='serif')


def plot_rank_time_overhead():

	# Size Array
	arr = np.array([100,1000,1e4,1e5])


	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)


	# X-AXIS
	arr = np.array([100,1000,1e4,1e5])

	# Y-AXIS
	data_rank = np.loadtxt("overhead_rank.txt",unpack=True)
	data_time = np.loadtxt("time_required_rank.txt",unpack=True)


	# Plot the Rank Overhead
	ax1.plot(arr,data_rank,marker="o",ls="--",color="dodgerblue",ms=8,mew=1.5,mec="black")

	# Plot the Rank Time
	ax2.plot(arr,data_time,marker="o",ls="--",color="dodgerblue",ms=8,mew=1.5,mec="black")


	# Define Scale and Limits
	ax1.set_yscale("log")
	ax1.set_xscale("log")
	ax2.set_xscale("log")
	ax2.set_ylim(50,60)


	# Define Labels and Titles
	ax1.set_xlabel(r"Size (Bit)",ha="center",fontsize=12)
	ax1.set_ylabel(r"Overhead (Bit)",fontsize=12)
	ax1.set_title("rank() Overhead",fontsize=12)

	ax2.set_xlabel(r"Size (Bit)",ha="center",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_title("rank() Time",fontsize=12)

	plt.savefig("rank_time_overhead.png",dpi=300,format="png",bbox_inches="tight")





def plot_select_time_overhead():


	# Size Array
	arr = np.array([100,1000,1e4,1e5])


	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)


	# X-AXIS
	arr = np.array([100,1000,1e4,1e5])

	# Y-AXIS
	data_select = np.loadtxt("overhead_select.txt",unpack=True)
	data_time = np.loadtxt("time_required_select.txt",unpack=True)


	# Plot the Rank Overhead
	ax1.plot(arr,data_select,marker="s",ls="--",color="mediumpurple",ms=8,mew=1.5,mec="black")

	# Plot the Rank Time
	ax2.plot(arr,data_time,marker="s",ls="--",color="mediumpurple",ms=8,mew=1.5,mec="black")


	# Define Scale and Limits
	ax1.set_yscale("log")
	ax1.set_xscale("log")
	ax2.set_xscale("log")


	# Define Labels and Titles
	ax1.set_xlabel(r"Size (Bit)",ha="center",fontsize=12)
	ax1.set_ylabel(r"Overhead (Bit)",fontsize=12)
	ax1.set_title("select() Overhead",fontsize=12)

	ax2.set_xlabel(r"Size (Bit)",ha="center",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_title("select() Time",fontsize=12)

	plt.savefig("select_time_overhead.png",dpi=300,format="png",bbox_inches="tight")





def plot_time_sparse_append():
	
	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)



	# Make 2D grid of sparsity and size
	data = np.empty((3,4))

	# Load the Data
	raw = np.loadtxt("time_required_sparse_append.txt",unpack=True)

	# Fill in the 2D grid from the raw data
	kk = 0
	for ii in range(len(data[:,0])):
		data[ii,0] = raw[kk]
		data[ii,1] = raw[kk+1]
		data[ii,2] = raw[kk+2]
		data[ii,3] = raw[kk+3]

		kk = kk+4

	size_arr = np.array([100.,1000.,1e4])
	sparsity = np.array([1,5,10,15])

	# Now Plot
	ax1.plot(size_arr,data[:,0],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[0])+r"$\%$")
	ax1.plot(size_arr,data[:,1],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[1])+r"$\%$")
	ax1.plot(size_arr,data[:,2],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[2])+r"$\%$")
	ax1.plot(size_arr,data[:,3],marker="p",ls="--",color="crimson",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[3])+r"$\%$")

	ax1.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="upper left")
	ax1.set_xscale("log")
	ax1.set_yscale("log")

	ax2.plot(sparsity,data[0,:],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[0]))
	ax2.plot(sparsity,data[1,:],marker="s",ls="--",color="dodgerblue",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[1]))
	ax2.plot(sparsity,data[2,:],marker="d",ls="--",color="limegreen",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[2]))

	ax2.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="lower right")
	ax2.set_yscale("log")
	ax2.set_xlim(0,16)
	ax2.set_ylim(1,1e6)


	# Set Labels
	ax1.set_xlabel("Size (bit)",fontsize=12)
	ax1.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_xlabel(r"Sparsity ($\%$)",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)

	# Set X-tick label for Sparsity
	ax2.set_xticks(sparsity)

	# Plot Title
	fig.text(0.5,0.92,r"Elapsed Time for append()",ha="center",fontsize=12)

	# Save Figure
	plt.savefig("sparse_append_time.png",dpi=300,format="png",bbox_inches="tight")


def plot_time_sparse_get_at_index():
	
	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)


	# Make 2D grid of sparsity and size
	data = np.empty((3,4))

	# Load the Data
	raw = np.loadtxt("time_required_sparse_get_at_index.txt",unpack=True)

	# Fill in the 2D grid from the raw data
	kk = 0
	for ii in range(len(data[:,0])):
		data[ii,0] = raw[kk]
		data[ii,1] = raw[kk+1]
		data[ii,2] = raw[kk+2]
		data[ii,3] = raw[kk+3]

		kk = kk+4

	size_arr = np.array([100.,1000.,1e4])
	sparsity = np.array([1,5,10,15])

	# Now Plot
	ax1.plot(size_arr,data[:,0],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[0])+r"$\%$")
	ax1.plot(size_arr,data[:,1],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[1])+r"$\%$")
	ax1.plot(size_arr,data[:,2],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[2])+r"$\%$")
	ax1.plot(size_arr,data[:,3],marker="p",ls="--",color="crimson",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[3])+r"$\%$")

	ax1.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="lower right")
	ax1.set_xscale("log")
	ax1.set_ylim(5,12)
	#ax1.set_yscale("log")

	ax2.plot(sparsity,data[0,:],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[0]))
	ax2.plot(sparsity,data[1,:],marker="s",ls="--",color="dodgerblue",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[1]))
	ax2.plot(sparsity,data[2,:],marker="d",ls="--",color="limegreen",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[2]))

	ax2.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="lower right")

	ax2.set_xlim(0,16)
	ax2.set_ylim(5,12)
	ax2.set_xticks(sparsity)

	# Set Labels
	ax1.set_xlabel("Size (bit)",fontsize=12)
	ax1.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_xlabel(r"Sparsity ($\%$)",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)

	# Plot Title
	fig.text(0.5,0.92,r"Elapsed Time for get\_at\_index()",ha="center",fontsize=12)

	# Save Figure
	plt.savefig("sparse_get_at_index_time.png",dpi=300,format="png",bbox_inches="tight")







def plot_time_sparse_get_at_rank():

	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)


	# Make 2D grid of sparsity and size
	data = np.empty((3,4))

	# Load the Data
	raw = np.loadtxt("time_required_sparse_get_at_rank.txt",unpack=True)

	# Fill in the 2D grid from the raw data
	kk = 0
	for ii in range(len(data[:,0])):
		data[ii,0] = raw[kk]
		data[ii,1] = raw[kk+1]
		data[ii,2] = raw[kk+2]
		data[ii,3] = raw[kk+3]

		kk = kk+4

	size_arr = np.array([100.,1000.,1e4])
	sparsity = np.array([1,5,10,15])

	# Now Plot
	ax1.plot(size_arr,data[:,0],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[0])+r"$\%$")
	ax1.plot(size_arr,data[:,1],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[1])+r"$\%$")
	ax1.plot(size_arr,data[:,2],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[2])+r"$\%$")
	ax1.plot(size_arr,data[:,3],marker="p",ls="--",color="crimson",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[3])+r"$\%$")

	ax1.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="upper left")
	ax1.set_xscale("log")
	ax1.set_ylim(0,4)

	ax2.plot(sparsity,data[0,:],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[0]))
	ax2.plot(sparsity,data[1,:],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[1]))
	ax2.plot(sparsity,data[2,:],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[2]))

	ax2.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="upper left")
	ax2.set_xlim(0,16)
	ax2.set_ylim(0,4)
	ax2.set_xticks(sparsity)

	# Set Labels
	ax1.set_xlabel("Size (bit)",fontsize=12)
	ax1.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_xlabel(r"Sparsity ($\%$)",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)

	# Plot Title
	fig.text(0.5,0.92,r"Elapsed Time for get\_at\_rank()",ha="center",fontsize=12)

	# Save Figure
	plt.savefig("sparse_get_at_rank_time.png",dpi=300,format="png",bbox_inches="tight")






def plot_time_sparse_num_elem_at():
	
	# Prepare Plot
	fig = plt.figure()
	fig.set_size_inches(8.0, 4.0)
	fig.subplots_adjust(hspace=0.1, wspace=0.3)

	ax1 = fig.add_subplot(121)
	ax2 = fig.add_subplot(122)


	# Make 2D grid of sparsity and size
	data = np.empty((3,4))

	# Load the Data
	raw = np.loadtxt("time_required_sparse_num_elem_at.txt",unpack=True)

	# Fill in the 2D grid from the raw data
	kk = 0
	for ii in range(len(data[:,0])):
		data[ii,0] = raw[kk]
		data[ii,1] = raw[kk+1]
		data[ii,2] = raw[kk+2]
		data[ii,3] = raw[kk+3]

		kk = kk+4

	size_arr = np.array([100.,1000.,1e4])
	sparsity = np.array([1,5,10,15])

	# Now Plot
	ax1.plot(size_arr,data[:,0],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[0])+r"$\%$")
	ax1.plot(size_arr,data[:,1],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[1])+r"$\%$")
	ax1.plot(size_arr,data[:,2],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[2])+r"$\%$")
	ax1.plot(size_arr,data[:,3],marker="p",ls="--",color="crimson",ms=5,mew=1,mec="black",label=r"Sparsity = "+str(sparsity[3])+r"$\%$")

	ax1.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="lower right")
	ax1.set_xscale("log")
	ax1.set_ylim(0,10)

	ax2.plot(sparsity,data[0,:],marker="o",ls="--",color="mediumpurple",ms=8,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[0]))
	ax2.plot(sparsity,data[1,:],marker="s",ls="--",color="dodgerblue",ms=6,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[1]))
	ax2.plot(sparsity,data[2,:],marker="d",ls="--",color="limegreen",ms=5,mew=1,mec="black",label=r"Size = 10$^%i$ bit" % np.log10(size_arr[2]))

	ax2.legend(frameon=False,numpoints=1,ncol=1,fontsize=12,loc="lower right")
	ax2.set_xlim(0,16)
	ax2.set_ylim(0,10)
	ax2.set_xticks(sparsity)

	# Set Labels
	ax1.set_xlabel("Size (bit)",fontsize=12)
	ax1.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)
	ax2.set_xlabel(r"Sparsity ($\%$)",fontsize=12)
	ax2.set_ylabel(r"Elapsed Time ($\mu$s)",fontsize=12)

	# Plot Title
	fig.text(0.5,0.92,r"Elapsed Time for num\_elem\_at()",ha="center",fontsize=12)

	# Save Figure
	plt.savefig("sparse_num_elem_at_time.png",dpi=300,format="png",bbox_inches="tight")




def main():

	plot_rank_time_overhead()
	plot_select_time_overhead()
	plot_time_sparse_append()
	plot_time_sparse_get_at_index()
	plot_time_sparse_get_at_rank()
	plot_time_sparse_num_elem_at()

if __name__ == "__main__":
	main()


