import sys

def main():
    if len(sys.argv) < 3:
        print("\033[31;1mExpected an input file\33[0m")
        return
    in_file, out_file = sys.argv[1], sys.argv[2]

    with open(in_file, 'r') as file, open(out_file, 'w+') as write_to:
        r, b = 0, 0
        while (r := file.read(1)):
            if b: write_to.write(", ")
            write_to.write("0x%.2X" % ord(r))
            b = 1
        write_to.write(', 0x00')
	


if __name__ == "__main__":
    main()