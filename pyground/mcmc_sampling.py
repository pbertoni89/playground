#
# http://twiecki.github.io/blog/2015/11/10/mcmc-sampling/


# %matplotlib inline

import numpy as np
import scipy as sp
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

from scipy.stats import norm

sns.set_style('white')
sns.set_context('talk')

np.random.seed(123)

data = np.random.randn(20)

ax = plt.subplot()
sns.distplot(data, kde=False, ax=ax) = ax.set(title='Histogram of observed data', xlabel='x', ylabel='# observations')
