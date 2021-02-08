import numpy as np

drift_velocity = 5e4     # m/s = 50 um/ns
#relative_uncertainty = 0.05 # 5% relative uncertainty in drift dist
#time_uncertainty = 0.5e-9  # 0.5 ns absolute TDC time uncertainty
relative_uncertainty = 0.00
time_uncertainty = 1.92e-9 # 4.5 ns FWHM resolution (from VDC paper)
                           # sigma = FWHM/2.355
# Time-to-distance polynomial coefficients
a1coef = [ 2.12e-03, 0.00e+00, 0.00e+00, 0.00e+00 ]
a2coef = [-4.20e-04, 1.30e-03, 1.06e-04, 0.00e+00 ]

#--------------------------------------------------------
def poly(x,coef):
    y = 0.0
    for i in range(len(coef)-1,0,-1):
        y = x * (y + coef[i])
    y += coef[0]
    return y

#--------------------------------------------------------
def getCoef(x):
    a1 = poly(x,a1coef)
    a2 = poly(x,a2coef)
    return a1, a2

#--------------------------------------------------------
def drift_distance_to_time(dist, tanTheta):
    '''
    Convert distance (m) to time (s) for track with tan(angle) = tanTheta
    where the angle is measured wrt the wire plane.
    Both arguments can be a scalars or numpy arrays.
    If both are arrays, they must have the same size are assumed to
    be parallel. If dist is a scalar, then tanTheta must also be a scalar.
    Returns the drift time with the same type as dist (scalar or array).
    '''
    if isinstance(tanTheta,float) and isinstance(dist,float):
        TT = tanTheta
    elif isinstance(tanTheta,float):
        TT = np.full(dist.shape,tanTheta)
    elif isinstance(dist,float) or (dist.shape != tanTheta.shape):
        raise ValueError('Incompatible arguments')

    a1, a2 = getCoef(TT)
    d1 = a1+a2

    dist = abs(dist)
    if isinstance(dist,float):
        if dist < d1:
            time = dist*a1/d1
        else:
            time = dist-a2
    else:
        time = np.zeros(dist.shape)
        for index, d in np.ndenumerate(dist):
            if d < d1[index]:
                time[index] = (d*a1[index])/(a1[index]+a2[index])
            else:
                time[index] = d-a2[index]
    time /= drift_velocity

    return time

#--------------------------------------------------------
def drift_time_to_distance(time, tanTheta):
    '''
    Convert time (s) to distance (m) for track with tan(angle) = tanTheta
    where the angle is measured wrt the wire plane.
    Both arguments can be a scalars or numpy arrays.
    If both are arrays, they must have the same size are assumed to
    be parallel. If time is a scalar, then tanTheta must also be a scalar.
    Returns the drift distance with the same type as time (scalar or array).
    '''
    if isinstance(tanTheta,float) and isinstance(time,float):
        TT = tanTheta
    elif isinstance(tanTheta,float):
        TT = np.full(time.shape,tanTheta)
    elif isinstance(time,float) or (time.shape != tanTheta.shape):
        raise ValueError('Incompatible arguments')

    a1, a2 = getCoef(TT)
    d1 = a1+a2

    dist = drift_velocity * time
    if isinstance(time,float):
        # ddist = drift_velocity * time_uncertainty
        if dist < 0.0:
            dist = 0.0 #print("Warning, t < 0:", time)
        elif dist < a1:
            dist  *= 1.0 + a2/a1
            # ddist *= 1.0 + a2/a1
        else:
            dist += a2
        # ddist += relative_error * dist
    else:
        # ddist = np.full(dist.shape, drift_velocity * time_uncertainty)
        for index, d in np.ndenumerate(dist):
            if d < 0.0:
                dist[index] = 0.0 #print("Warning, t < 0:", time[index])
            elif d < a1[index]:
                dist[index]  *= 1.0 + a2[index]/a1[index]
                ddist[index] *= 1.0 + a2[index]/a1[index]
            else:
                dist[index] += a2[index]
            ddist[index] += relative_error * dist[index]

    # return dist, ddist
    return dist
