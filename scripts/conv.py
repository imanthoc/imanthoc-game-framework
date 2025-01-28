import sys



def main():
    if len(sys.argv) < 3:
        print("Minimum 3 args")
        exit()

    txt_fname = sys.argv[1]
    bin_fname = sys.argv[2]

    tf = open(txt_fname, 'r')
    bf = open(bin_fname, 'wb')

    n = tf.readline().strip().split()
    width  = int(n[0])
    height = int(n[1])
    
    bf.write(width.to_bytes(2, byteorder="little"))
    bf.write(height.to_bytes(2, byteorder="little"))
    
    line = tf.readline().strip()
    while line != "":
        for number in line.split():
            bf.write(int(number).to_bytes(1))
        line = tf.readline().strip()

    tf.close()
    bf.close()



if __name__ == "__main__": main()
