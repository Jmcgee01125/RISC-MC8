#!/bin/python3

import mcschematic
import sys
import os

on_block = 'minecraft:redstone_block'
off_block = 'minecraft:blue_ice'

start_pos = (0, 0, 0)
row_dir = (1, 0, 0)
len_dir = (0, 0, 1)
up_dir = (0, -1, 0)

row_step_size = 4
bit_step_size = 2
len_step = 6
up_step = 2

plane_wrap_size = 32

max_bytes = 256

def scale(tup, scale):
    return tuple(i * scale for i in tup)

def add(tup1, tup2):
    return (tup1[0] + tup2[0], tup1[1] + tup2[1], tup1[2] + tup2[2])

def gen_rom_schematic(infile='./source_data', outfile='./rom_data'):
    schem = mcschematic.MCSchematic()
    print("Generating ROM schematic...")
    with open(infile, 'rb') as binfile:
        print(f'Opened file {infile}')
        data = bytearray(binfile.read())
        if len(data) > 256:
            print('Error: program too large! Program size (${len(data)} bytes) exceeds maximum size of 256 bytes!')
            return
        elif len(data) < 256:
            data += bytes(256 - len(data))
        for i in range(len(data)):
            next_byte = data[i]
            ypos = (i >> 5) * up_step
            lencoord = ((i % plane_wrap_size) >> 1) * len_step
            rowoffset = i & 1 # offset for second byte in pair
            for bitpos in range(8):
                bit = next_byte & (1 << bitpos)
                block = on_block if bit else off_block
                
                rowcoord = (bitpos & 0xE) * row_step_size + (bitpos & 0x1) * bit_step_size + rowoffset * row_step_size
                pos = add(start_pos, add(add(scale(row_dir, rowcoord), scale(up_dir, ypos)), scale(len_dir, lencoord)))
                schem.setBlock(pos, block)
    schem.save('', outfile, mcschematic.Version.JE_1_19)
    print('Schematic generated')

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f'Usage: python {os.path.basename(sys.argv[0])} <input file path> <output file path>')
    else:
        inpath = sys.argv[1]
        outpath = sys.argv[2]
        gen_rom_schematic(inpath, outpath)
