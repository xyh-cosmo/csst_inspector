from pylab import *

d1 = loadtxt('diff_zhang.txt')
d2 = loadtxt('diff_lagrange.txt')

semilogy(d1[:,0],'r.',label='X',alpha=0.25)
semilogy(d1[:,1],'rx',label='Y',alpha=0.25)

semilogy(d2[:,0],'b.',label='X',alpha=0.25)
semilogy(d2[:,1],'bx',label='Y',alpha=0.25)

legend(loc='lower right')

show()