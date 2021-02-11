#!/usr/bin/python3

import numpy as np
from scipy.optimize import curve_fit
from vdc import *

wire_start = -0.77852
wire_spacing = 4.2426e-3
nwires = 368
drift_space = 0.0127 # distance from wires to cathode plane

fake_offset = 200e-9
#--------------------------------------------------------
def fitfunc(x,p0,p1,p2):
    y = p1*(x-p0)
    return np.sign(y) * (drift_distance_to_time(abs(y),p1) + p2)

#--------------------------------------------------------
# Read in data file

#infilename = 'cluster_data_apex_4976_1M.out'
#outfilename = '/zfs/data/apex/RL_u1_cluster_curfit_nopivot.dat'
infilename = 'gendata.dat'
outfilename = '/zfs/data/apex/gendata_clusterfit.dat'
simdata=False
try:
    infile = open(infilename, 'r')
except:
    print('Error opening input file', infilename)
    exit
try:
    outfile = open(outfilename, 'w')
except:
    print('Error opening output file', outfilename)
    exit
    
print_done = False
for line in infile:
    # Read cluster data
    items = line.split()
    # if len(items) < 16:   # want at least 5 wires
    #     continue
    # Line number
    iline = int(items[0])
    # Event number
    iev = int(items[1])
    if iev >= 500000:
        break
    if iev % 1000 == 0 and iev != 0:
        # With real data, several clusters will have the same
        # event number, so make sure we only print it once
        if not print_done:
            print(iev)
        print_done = True
    else:
        print_done = False
    # Spectrometer: 0 = L, 1 = R
    ispec = int(items[2])
    # Projection: 0 = u, 1 = v
    iproj = int(items[3])
    # Plane: 1 or 2
    iplane = int(items[4])
    # Limit to u1 plane in either spectrometer
    if iproj != 0 or iplane != 1:
        continue
    # Number of wires
    npt = int(items[5])
    # We need at least 5 hits to get a meaningful fit. We ignore the pivot
    # wire, and we need at least 4 wires to do a 3-parameter fit
    if npt < 5:
        # Need to write this cluster out for simdata to stay
        # in sync with truthdata. chi2 < 0 indicates no fit done
        if simdata:
            print(iline,iev,ispec,iproj,iplane,i[0],i[-1],\
                  npt, 0.0, 0.0, 0.0, -1.0, 0.0, file=outfile)
        continue
            

    # Wire numbers
    i = np.array([int(s) for s in items[6:6+npt]])
    # Are the wire numbers consecutive? If not, just skip the cluster.
    # This could be improved to allow gaps and duplicate wire numbers (multiple hits on a wire)
    di = np.diff(i) - 1
    if di.any():
        if simdata:
            print(iline,iev,ispec,iproj,iplane,i[0],i[-1],\
                  npt, 0.0, 0.0, 0.0, -1.0, 0.0, file=outfile)
        continue

    # Drift times
    t = np.array([float(s) for s in items[6+npt:6+2*npt]])
    t += fake_offset

    # Wire coordinates
    xw = wire_start + i*wire_spacing

    # Assume all y-values before the smallest one have a negative sign
    tmin = max([ np.min(t), 0.0 ])
    ipivot = np.argmin(t)
    t[:ipivot] *= -1
    # Remove the wire with the smallest drift time since its sign is ambiguous
    xwmod = np.delete(xw,ipivot)
    tmod = np.delete(t,ipivot)

    # Estimate postion, slope, time offset
    tanTheta = 0.8
    init = np.array([xw[ipivot], tanTheta, tmin])

    # Estimate the time measurement uncertainty
    # This gives pretty decent results, but is imperfect because
    # tmin != toff
    sigma = time_uncertainty + \
            relative_uncertainty * np.maximum(np.abs(t)-tmin,0.0)
    smod = np.delete(sigma,ipivot)
    # Fit with the estimated weights
    try:
        popt, pcov = curve_fit(fitfunc, xwmod, tmod, init, smod )
    except RuntimeError:
        print("Error: failed to optimize event",iev)
        if simdata:
            print(iline,iev,ispec,iproj,iplane,i[0],i[-1],\
                  npt, 0.0, 0.0, 0.0, -1.0, 0.0, file=outfile)
        continue
    # x = popt[0]
    # slope = popt[1]
    toff = popt[2]

    # Re-fit with more accurate weights
    sigma = time_uncertainty + \
            relative_uncertainty * np.maximum(np.abs(t)-toff,0.0)
    smod = np.delete(sigma,ipivot)
    init = popt
    try:
        popt, pcov = curve_fit(fitfunc, xwmod, tmod, init, smod )
    except RuntimeError:
        print("Error: failed to optimize event",iev)
        if simdata:
            print(iline,iev,ispec,iproj,iplane,i[0],i[-1],\
                  npt, 0.0, 0.0, 0.0, -1.0, 0.0, file=outfile)
        continue
    # Calculate chi2
    chi2 = np.sum( ((tmod-fitfunc(xwmod,*popt))/smod)**2 )
    # Distance of track crossing point from nearest wire
    dx = popt[0] - xw[ipivot]
    popt[2] -= fake_offset
    # Write results to output file
    print(iline,iev,ispec,iproj,iplane,i[0],i[-1],\
          npt,*popt,chi2,dx, file=outfile)
    # print(n,-x/slope,slope,toff,chi2,*t,*sigma, file=outfile)

infile.close()
outfile.close()
