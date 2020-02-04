import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("residuals.txt")

x = data[:,1] + 3*( data[:,0]-1 )

fig = plt.figure()

ax0 = fig.add_subplot(211)
plt.plot( [0,19],[0,0],"k-")
plt.plot( x, data[:,2], "X", label="before" )
plt.plot( x, data[:,4], "o", label="after" )
plt.ylim( -1.,1. )
plt.xlim( 0,19 )
plt.xticks( np.arange(1,19) )
plt.grid( linestyle=':', axis='y')
plt.ylabel( "residual (mm)" )
plt.xlabel( "detector number" )
plt.legend( loc='best' )

ax0 = fig.add_subplot(212)
plt.plot( x, data[:,3], "X" )
plt.plot( x, data[:,5], "o" )
plt.ylim( 0.,0.3 )
plt.xlim( 0,19 )
plt.xticks( np.arange(1,19) )
plt.grid( linestyle=':', axis='y')
plt.ylabel( "resolution (mm)" )
plt.xlabel( "detector number" )


plt.tight_layout()
fig.savefig("residuals_and_resolutions.pdf")
plt.show()
