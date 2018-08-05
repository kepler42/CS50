import sys
import cs50

def main():

    if len(sys.argv) !=2 or sys.argv[1].isalpha() == False:
        print("Usage: ./vigenere keyword")
        exit(1)
    else:
        shift = []
        for q in sys.argv[1]:
            if q.islower():
                shift.append(ord(q) -97)
            else:
                shift.append(ord(q) -65)

    print("plaintext: ", end="")
    a = cs50.get_string()
    print("ciphertext: ", end="")

    n = len(shift)
    wrap = 0

    for i in a:
        if i.isalpha():
            index = wrap%n
            quer = ord(i) + shift[index]

            if i.islower() and quer>122:
                quer = quer - 26
            elif i.isupper() and quer>90:
                quer = quer - 26
            print("{}".format(chr(quer)), end="")
            wrap+=1

        else:
            print("{}".format(i), end="")
    print()

if __name__ == "__main__":
    main()