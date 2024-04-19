from PIL import Image
import numpy as np


def compress_image(name_in_original, svd, compressed_name, compression_ratio):
    # Load image
    img = Image.open(f"./pictures/original/{name_in_original}.bmp")
    m, n = img.size
    k = int(max(1, ((-(m + n) + np.sqrt((m + n) ** 2 + 4 * m * n / 3 / 3 / compression_ratio)) / 2)))
    print(k)

    # Convert image to array
    img_array = np.array(img, dtype='float32')

    # Separate RGB channels
    red_channel = img_array[:, :, 0]
    green_channel = img_array[:, :, 1]
    blue_channel = img_array[:, :, 2]

    # Apply SVD to each channel
    U_r, S_r, Vt_r = svd(red_channel)
    U_g, S_g, Vt_g = svd(green_channel)
    U_b, S_b, Vt_b = svd(blue_channel)

    # print(np.diag(S_r))

    # Truncate singular values
    S_r_trunc = np.diag(S_r[:k])
    S_g_trunc = np.diag(S_g[:k])
    S_b_trunc = np.diag(S_b[:k])

    # print(S_r_trunc)
    # print(U_r[:, :k])
    # print(Vt_r[:k, :])

    np.savez(f'./pictures/intermediate_view/{compressed_name}.npz',
             U_r=U_r[:, :k], S_r=S_r_trunc, Vt_r=Vt_r[:k, :],
             U_g=U_g[:, :k], S_g=S_g_trunc, Vt_g=Vt_g[:k, :],
             U_b=U_b[:, :k], S_b=S_b_trunc, Vt_b=Vt_b[:k, :])
    print("Compressed")


def decompress_image(name_of_intermediate_view, decompress_name):
    data = np.load(f"./pictures/intermediate_view/{name_of_intermediate_view}.npz")
    U_r = data["U_r"]
    S_r = data["S_r"]
    Vt_r = data["Vt_r"]
    U_g = data["U_g"]
    S_g = data["S_g"]
    Vt_g = data["Vt_g"]
    U_b = data["U_b"]
    S_b = data["S_b"]
    Vt_b = data["Vt_b"]

    red_channel_compressed = np.dot(U_r, np.dot(S_r, Vt_r))
    green_channel_compressed = np.dot(U_g, np.dot(S_g, Vt_g))
    blue_channel_compressed = np.dot(U_b, np.dot(S_b, Vt_b))

    compressed_img_array = np.stack((red_channel_compressed, green_channel_compressed, blue_channel_compressed),
                                    axis=-1)
    compressed_img = Image.fromarray(np.uint8(compressed_img_array))
    compressed_img.save(f"./pictures/decompressed/{decompress_name}.bmp")
    print("Decompressed")
