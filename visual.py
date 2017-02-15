import numpy as np
import Normalize
from matplotlib import pyplot as pl

num = 5
tlen = 5000

# exponentially skewed AR(1)
data = np.random.normal(size = (num, tlen+100))
for t in xrange(1, tlen+100):
    data[:, t] += data[:, t-1] * 0.995 + 0.1
data = data[:, 100:]
data = np.exp(data)

# transformation
trans = Normalize.Quantile(data)

# visual comparison
f, ax = pl.subplots(2, 1, figsize = (8, 6), sharex = True)
ax[0].semilogy(data[:,-1000:].T)
ax[1].plot(trans[:,-1000:].T)
ax[0].text(0.95, 0.8, 'raw input', horizontalalignment = 'right', transform = ax[0].transAxes,
           bbox={'facecolor':'white', 'alpha':0.7, 'pad':15})
ax[1].text(0.95, 0.8, 'normalized', horizontalalignment = 'right', transform = ax[1].transAxes,
           bbox={'facecolor':'white', 'alpha':0.7, 'pad':15})
#pl.show()
pl.tight_layout()
pl.savefig('visual.png')
