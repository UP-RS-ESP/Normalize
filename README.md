# Normalize
Normalize time series by quantile normalization to the normal distribution.

## Install

	git clone https://github.com/UP-RS-ESP/Normalize.git
	cd Normalize
	sudo python setup.py install

## Dependency
For the mapping to Gaussian percentiles the `erfinv` function of [libit](http://libit.sourceforge.net) is used.

## Examples
An example using random data for `num = 55` time series of length `len = 6000` using as many threads as possible:

	import numpy as np
	import Normalize

	num = 55
	len = 6000
	data = np.random.random((num, len))
	trans = Normalize.Quantile(data)

Transformed time series in `trans` are approximately normal distributed.

A more visual example of exponentially skewed AR(1) processes:

    import numpy as np
    import Normalize
    from matplotlib import pyplot as pl
    
    num = 5
    tlen = 5000
    
    # exponentially skewed AR(1)
    data = np.random.normal(size = (num, tlen+100))
    for t in xrange(1, tlen+100):
        data[:, t] += data[:, t-1] * 0.99 + 0.1
    data = data[:, 100:]
    data = np.exp(data)
    
    # transformation
    trans = Normalize.Quantile(data)
    
    # visual comparison
    f, ax = pl.subplots(2, 1, sharex = True)
    ax[0].semilogy(data.T)
    ax[1].plot(trans.T)
    pl.show()

![visual comparison](visual.png?raw=true "visual comparison")
