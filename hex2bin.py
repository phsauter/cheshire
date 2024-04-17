#Mikail Gedik
#Converts a file, whose contents are binary data noted in ASCII (in hex) to a raw .bin file
#One byte per line in the input

# ./util/gen_bootrom.py --sv-module FontROM tmp.bin > TmpFontRom.sv

# Read the bootrom binary.
with open("axi2hdmi_fonts/font_meminit.txt", "r") as file:
    with open("tmp.bin", "wb") as of:
        for i in file:
            sub = i[:-1]
            t = int(sub, 16);
            #print(t.to_bytes(2, 'big'))
            of.write(t.to_bytes(1, 'big'))
