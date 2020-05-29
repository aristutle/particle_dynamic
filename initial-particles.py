import numpy as np
N = 1000000
D = 2  # dimension
ppt = 2  # property number
result = np.zeros([D * 2 + ppt, N])
# np.random.rand() gives random number in [0,1)
for i in range(D):
    # X and Y columns
    result[i] = (np.random.rand(N)-0.5)*200
for i in range(D):
    # Vx and Vy columns
    result[D + i] = np.random.rand(N)-0.5
for i in range(ppt):
    if (i == 0):
        # mass
        result[D * 2 + i] = abs(np.random.randn(N))
    if (i == 1):
        # flag
        result[D * 2 + i] = 1
print(result.shape)
np.savetxt('initial-1000000.txt', result.T, delimiter='\t')
print("output done!")
