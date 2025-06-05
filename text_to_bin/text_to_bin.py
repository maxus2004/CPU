string = ""

with open("code.txt", "r") as text_file:
    for line in text_file:
        line = line.strip()
        line = line.split('#')[0]
        line = line.replace(" ","")
        string += line

with open("code.bin", "wb") as binary_file:
    for i in range(len(string)//8):
        byte_string = string[i*8 : i*8+8]
        byte = int(byte_string,2).to_bytes()
        binary_file.write(byte)
