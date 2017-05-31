# tbjcATTPCroot2

########################
##### instruction ######
########################

Root version > 6.xx is required for this program

to build:
mkdir build
cd build
cmake ..

for multiprocess running, use mult.py and change the path for the data resources and data output path.

To visualize the entries, the jupyter script is needed in the tbjcAnalysis folder.

#######################
##### description #####
#######################

this program fake up the TClonesArray in the root and FairRootManager, FairTask, FairRun, FairRunAna and also FairParset from the FairRoot and FairSoft. So it loses the ability to visualize the trajectories in FairRoot.

#####################
##### test ##########
#####################

This problem has been tested under local workstations, google cloud Ubuntu images, and Regulus HPC node. But it failed the test in HPC node "crcfe01", which has a corrupted root package.

########################
##### responsibility ###
########################

This project reuses most codes from the one by Yassid, https://github.com/ATTPC/ATTPCROOT. I will not be liable for the code maintainence, or any bugs.
