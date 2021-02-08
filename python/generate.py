#!/usr/bin/python3

# Simulate tracks through the HRS VDC 

import numpy as np
from vdc import *

wire_start = -0.77852
wire_spacing = 4.2426e-3
nwires = 368
drift_space = 0.0127 # distance from wires to cathode plane

#--------------------------------------------------------
# Generate random tracks

N = 200000
np.random.seed(1)

datafile = open('gendata.dat', 'w')
truthfile = open('truthdata.dat', 'w')
sqrt2 = np.sqrt(2.0)

for n in range(N):
    if n % 1000 == 0 and n != 0:
        print(n)
    # Generate u position within [-wire_start, +wire_start]
    u  = np.random.uniform( wire_start-0.5*wire_spacing, \
                            wire_start + (nwires-0.5)*wire_spacing )
    # Find wire number closest to u
    ic = np.around((u-wire_start)/wire_spacing).astype('int')

    # Generate track "slope" (TRANSPORT convention, i.e. angle with
    # the normal to plane), peaked around 45 degrees wrt x.
    #
    # The mean trajectory angle varies from ~52 degrees at small
    # wire numbers to ~39 degrees at large wire numbers
    # (angles in x-z plane, normal convention, i.e. wrt x).
    #
    # The VDC paper assumes a Gaussian distribution with 
    # FWHM = 7 degrees -> sigma = 7/2.355 = 3 degrees
    angle = 0.0
    while angle < 30.0 or angle > 60.0:
        angle = np.random.normal( 45.0 + 7.0*(ic-184.0)/184.0, 3. )
    invSlope = np.tan( np.radians(angle) )
    # Project slope onto u-z plane
    invSlope /= sqrt2
    # Convert TRANSPORT slope to conventional slope (angle with
    # wire coordinate axis)
    slope = 1.0/invSlope

    # Lower/upper bounds of track crossing region
    umin = u - drift_space*invSlope
    umax = u + drift_space*invSlope
    # Wire numbers of lower/upper bounds
    imin = np.around( (umin-wire_start)/wire_spacing).astype('int')
    imax = np.around( (umax-wire_start)/wire_spacing).astype('int')
    i = np.arange(imin, imax+1)
    npt = len(i)
    uw = wire_start + i*wire_spacing
    y = slope * (uw - u)
    t = drift_distance_to_time(abs(y), slope)

    # dist, dsigma =  drift_time_to_distance(t,us)
    sigma = time_uncertainty + relative_uncertainty * t
    terr = np.random.normal(0.0, sigma)
    # Generate random positive time offsets to simulate accidental
    # coincidences
    toff = np.random.uniform( -100e-9, 150e-9 )
    tmeas = t + terr + toff
    print(n,n,0,0,1,npt,*i,*tmeas, file=datafile)
    # print(n,u,slope,toff,*t,*sigma, file=truthfile)
    print(n,u,slope,toff, file=truthfile)

datafile.close()
truthfile.close()
