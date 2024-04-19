import numpy as np
import numpy.linalg.linalg

from helper import *

compress_image("360-360-sample", np.linalg.linalg.svd, "intermediate_np", 2)
decompress_image("intermediate_np", "360-360-sample-np_decompressed")
