#!/usr/bin/python3

import sys

if __name__ == "__main__":

    inputFn = sys.argv[1]
    outputFn = sys.argv[2]

    fi = open(inputFn, 'rb')
    fdat = fi.read()
    fi.close()

    splt =outputFn.split('.')
    name = splt[0].upper()
    fo = open(outputFn, 'w')
    fo.write("#ifndef __{0}_H__\n#define __{0}_H__\n\n".format(name))
    fo.write("uint32_t {}_length = {};\n".format(name.lower(), len(fdat)))
    fo.write("const char {}[] PROGMEM {}\n".format(name.lower(), '{'))
    cnt = 0
    for b in fdat:
        cnt += 1
        fo.write("0x{:02X}{}".format(b, '};\n' if (cnt == len(fdat)) else (', ' if (cnt & 0xF) != 0 else ',\n')))
    fo.write("\n#endif __{}_H__\n".format(name))
