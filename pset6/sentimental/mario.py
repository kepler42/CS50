from cs50 import get_int

def main():
    print("Enter: ", end="")
    n = get_int()
    while n > 23 or n < 0:
        print("Enter: ", end="")
        n = get_int()
    for i in range(n):
        for q in range(n-i-1):
            print(" ", end="")

        for q in range(i+1):
            print("#", end="")
        print("  ", end="")
        for q in range(i+1):
            print("#", end="")
        print("")

if __name__ == "__main__":
    main()
