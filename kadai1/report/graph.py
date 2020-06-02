#
# graph.py
# Created by pira on 2017/12/25.
#

#coding: utf-8

import numpy as np
import matplotlib.pyplot as plt
import math

x = np.arange(0,5,0.1)
data1 = np.sin(x*1/2*math.pi)
data2 = np.cos(x*1/2*math.pi)

plt.plot(x,data1)
plt.plot(x,data2)
plt.xlim(0,4,0.1)
plt.ylim(-1.5,1.5,1)
plt.xticks([0,1,2,3,4])
plt.yticks([-1,0,1])
#plt.grid(which='major',color='black',linestyle='-')
plt.xlabel('sample')
plt.ylabel('amplitude')
plt.legend(['data1','data2'])
plt.savefig('data.png')
plt.show()