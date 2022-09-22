from logging import root
import os
import glob
import argparse
from re import sub
import subprocess

def args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-dir', '--directory', help='font directory name', nargs='?', const=1, type=str, default=os.getcwd())
    
    return parser.parse_args()

def compressFonts(rootdir):
    include = []
    bindir = "/ttf"
    comdir = "/cpp"
    for file in glob.iglob(rootdir + bindir + "/**/*.ttf", recursive=True):
        fontfamily = os.path.basename(os.path.dirname(file))
        if fontfamily == "ttf":
            fontfamily = ""
        font = os.path.splitext(os.path.basename(file))[0].replace("-", "")
        cppdir = rootdir + comdir + "/" + fontfamily
        if not os.path.exists(cppdir):
            os.mkdir(cppdir)
        cpp = cppdir + '/' + font + ".h"
        print("Compressing Font %s:%s" % (fontfamily, font))
        include.append('#include "assets/fonts/%s/%s.h' % (fontfamily, font))
        os.system("binary_to_compressed_c.exe -nocompress " + file + " %s > %s" % (font, cpp))
    print(include)
if __name__ == '__main__':
    print('Compressing Fonts ...')
    args = args()
    compressFonts(args.directory)
    