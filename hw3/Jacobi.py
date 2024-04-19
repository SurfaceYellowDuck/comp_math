from helper import *

def givens(n, i, j, theta):
    mat = np.identity(n)
    mat[i][i] = np.cos(theta)
    mat[i][j] = np.sin(theta)
    mat[j][i] = -np.sin(theta)
    mat[j][j] = np.cos(theta)

    return mat


def find_max(mat):
    n = mat.shape[0]
    mx = (n - 2, n - 1)
    for i in range(n - 2):
        for j in range(i + 1, n):
            if abs(mat[i, j]) > abs(mat[mx[0], mx[1]]):
                mx = [i, j]
    return mx[0], mx[1]


def spectral(mat, epsilon=5, iterations=250, ):

    epsilon = 10 ** (-1 * epsilon)
    n = mat.shape[0]
    A = mat
    P = np.identity(n)

    for i in range(iterations):

        k, l = find_max(A)
        if abs(A[k, l]) < epsilon:
            break

        if A[k, k] == A[l, l]:
            tht = np.pi / 4
        else:
            u = 2 * A[k, l] / (A[l, l] - A[k, k])
            tht = .5 * np.arctan(u)

            Q = givens(n, k, l, tht)

            A = (np.transpose(Q) @ A) @ Q
            P = P @ Q

    diag = np.copy(np.diag(A))

    ind = np.where(diag < 0)[0]
    if len(ind) != 0:
        for i in ind:
            diag[i] *= -1
            P[:, i] *= -1

    l = np.argsort(-1 * diag)
    P = P[:, [l]].reshape((n, n))
    diag = np.ravel(diag[l])

    return diag, P


def svd(A):

    m, n = A.shape
    trans = 0
    if n > m:
        A = np.transpose(A)
        trans = 1

    A_t = np.transpose(A)
    S, V = spectral(A_t @ A)
    S = np.sqrt(S)
    U = np.empty(A.shape)

    for i in range(len(S)):
        U[:, i] = (A @ V[:, i]) / S[i]

    if trans == 1:
        U, V = V, U

    return U, S, np.transpose(V)


# compress_image("sample_640x426", svd, "intermediate_jacobi", 2)
# decompress_image("intermediate_jacobi", "sample_640x426-jacobi_decompressed")
