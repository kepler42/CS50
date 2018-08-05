from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3


    def __str__(self):
        return str(self.name.lower())



def distances(a, b):
    A = len(a)
    B = len(b)

    matrix = [[0 for x in range(B+1)] for x in range(A+1)]

    for i in range(A+1):
        for j in range(B+1):

            if i==0 and j==0:
                matrix[i][j] = (0,'')
            # insertion
            elif i == 0:
                matrix[i][j] = (j,'I')
            #deletion
            elif j==0:
                matrix[i][j] = (i,'D')
            #DO NOTHING
            elif a[i-1] == b[j-1]:
                temp1 = matrix[i-1][j-1]
                matrix[i][j] = (temp1[0],'S')

            else:
                t1 = matrix[i][j-1]
                t2 = matrix[i-1][j]
                t3 = matrix[i-1][j-1]
                if t1[0]<=t2[0] and t1[0]<=t3[0]:
                    matrix[i][j] = (1+t1[0], 'I')

                elif t2[0]<=t3[0]:
                    matrix[i][j] = (1+t2[0], 'D')

                else:
                    matrix[i][j] = (1+t3[0], 'S')

    return matrix
