import numpy as np
N = 2000
D = 2  # dimension
ppt = 8  # property number
result = np.zeros([D * 2 + ppt, N])
# np.random.rand() gives random number in [0,1)
for i in range(D):
    # X and Y columns
    result[i] = (np.random.rand(N)-0.5)*200
for i in range(D):
    # Vx and Vy columns
    result[D + i] = 0
for i in range(ppt):
    if (i == 0):
        # mass
        result[D * 2 + i] = np.array(abs(np.random.randn(N)+0.01))
    elif (i == 7):
        # exist flag
        result[D * 2 + i] = np.ones(N)
    else:
        result[D * 2 + i] = np.zeros(N)
print(result.shape)
np.savetxt('initial-'+str(N)+'.txt', result.T, delimiter='\t')
print("output done!")
