filename = "day1.in"

def getLineSum(line):
    lp = 0
    rp = len(line) - 1
    first = -1
    last = -1
    while (first == -1):
        if (ord(line[lp]) >= 48 and ord(line[lp]) <= 57):
            first = int(line[lp])
        lp += 1
    while (last == -1):
        if (ord(line[rp]) >= 48 and ord(line[rp]) <= 57):
            last = int(line[rp])
        rp -= 1
    return first * 10 + last

with open(filename, "r") as f:
    finalSum = 0
    for line in f:
        finalSum += getLineSum(line)
    print(finalSum)