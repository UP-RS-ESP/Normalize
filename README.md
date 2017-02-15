# Normalize
Normalize time series by quantile normalization to the normal distribution.

## Install

	git clone https://github.com/UP-RS-ESP/Normalize.git
	cd Normalize
	sudo python setup.py install

## Dependency
For the mapping to Gaussian percentiles the `erfinv` function of [libit](http://libit.sourceforge.net) is used.

## Example
An example using random data for `num = 55` time series of length `len = 6000` using as many threads as possible:

	import numpy as np
	import Normalize

	num = 55
	len = 6000
	data = np.random.random((num, len))
	trans = Normalize.Quantile(data)

Transformed time series in `trans` are aprox. normal distributed.
