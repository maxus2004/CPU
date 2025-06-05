string = ""

with open("code.txt", "r") as text_file:
    for line in text_file:
        line = line.strip()
        line = line.split('#')[0]
        line = line.replace(" ","")
        string += line

with open("code0.bin", "wb") as binary_file0, open("code1.bin", "wb") as binary_file1:
    for i in range(len(string)//16):
        byte_string1 = string[i*16 : i*16+8]
        byte1 = int(byte_string1,2).to_bytes()
        binary_file1.write(byte1)
        byte_string0 = string[i*16+8 : i*16+16]
        byte0 = int(byte_string0,2).to_bytes()
        binary_file0.write(byte0)
