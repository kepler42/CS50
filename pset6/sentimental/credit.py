from cs50 import get_int

def main():
    print("Number: ", end="")
    while True:
        i = get_int()
        if i > 0:
            break

    count = 1
    sum = 0
    while i != 0:
        d = i%10
        i=i//10

        if count%2 == 0:
            first = d
            d = d*2
            if d>9:
                d=d//10+d%10
            sum += d
            count+=1
        else:
            sum +=d
            count+=1
            second = d
    count-=1
    if count%2 != 0:
        first, second = second, first

    if sum%10 == 0:
        if (count == 15 and first == 3) and (second == 4 or second ==7):
            print("AMEX")

        elif ((count ==16 or count==13) and first==4):
            print("VISA")

        elif ((first==5 and count==16) and (second<6 and second!=0)):
            print("MASTERCARD")

        else:
            print("INVALID")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()