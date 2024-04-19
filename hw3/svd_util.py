#!/usr/bin/env python3

import argparse

import numpy as np

import Householder
import SVD
import helper


def parse_args():
    parser = argparse.ArgumentParser(description="Image compression/decompression program")
    parser.add_argument("--mode", choices=["compress", "decompress"], required=True, help="Mode of operation: compress "
                                                                                          "or decompress")
    parser.add_argument("--method", choices=["numpy", "simple", "advanced"], help="Method of singular value "
                                                                                  "decomposition (only required if "
                                                                                  "mode is compress)")
    parser.add_argument("--compression", type=int, help="Compression factor (only required if mode is compress)")
    parser.add_argument("--in_file", required=True, help="Input file path")
    parser.add_argument("--out_file", required=True, help="Output file path")
    return parser.parse_args()


def main():
    args = parse_args()
    mode = args.mode
    method = args.method
    compression = args.compression
    in_file = args.in_file
    out_file = args.out_file

    if mode == "compress":
        if method is None or compression is None:
            print("Error: --method and --compression are required for compression mode.")
            return
        if method == "numpy":
            helper.compress_image(in_file, np.linalg.linalg.svd, out_file, int(compression))
        elif method == "simple":
            helper.compress_image(in_file, SVD.svd, out_file, int(compression))
        elif method == "advanced":
            helper.compress_image(in_file, Householder.svd, out_file, int(compression))

    elif mode == "decompress":
        helper.decompress_image(in_file, out_file)
    else:
        print("Error: Invalid mode. Mode must be 'compress' or 'decompress'.")


main()
