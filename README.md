# vdcreco

This is a little experimental project for studying the feasibility and
performance of a 3-parameter fit algorithm for the Vertical Drift
Chambers (VDCs) in the Hall A HRS spectrometers.

The cluster reconstruction is currently implemented in Python. The `python` subdirectory contains the following scripts:

* `generate.py` Generator for simulated data
* `clusterfit_curfit_nopivot.py` Cluster reconstruction code, using the `curve_fit` minimizer from `scikit.optimize`

To help visualize the data and fit results, there are a number of ROOT scripts in the `rootscripts` directory:

* `clusterfit_mkrootfile.C`: Converts output from the Python code above to a ROOT file.
* `plotdata.C` and `plotsim.C`: plot scripts

Currently, there are various hardcoded file paths in these scripts which will have to be adjusted for your local environment. The files referenced in the scripts can be found on JLab CUE under `/work/halla/apex/disk1/ole/vdcreco`.

Results obtained with this code are summarized in the PowerPoint slides in the `talk` directory.

As already said, this code is experimental. Do not use it for production analysis, but as a starting point for a full implementation. For an implementation in the Hall A Analyzer Podd, the Python clusterfit code will have to be converted to C++.
