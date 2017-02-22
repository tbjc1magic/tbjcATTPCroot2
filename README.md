# tbjcATTPCroot2

Root version > 6.xx is required for this program

to build:
mkdir build
cd build
cmake ..

To visualize the entries, the jupyter script is needed in the test folder.


#######################
##### description #####
#######################

this program fake up the TClonesArray in the root and FairRootManager, FairTask, FairRun, FairRunAna and also FairParset from the FairRoot and FairSoft. So it loses the ability to visualize the trajectories in FairRoot.


#####################
###### test #########
#####################

This problem has been tested under local workstations, google cloud Ubuntu images, and Regulus HPC node. But it failed the test in HPC node "crcfe01", which has a corrupted root package.

