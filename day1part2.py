filename = "day1.in"

numbers = ("one", "two", "three", "four", "five", "six", "seven", "eight", "nine")
appearances = {}

def findAppearancesOfNumbers(line):
    for i in range(1, 10):
        firstAppearance = line.find(numbers[i - 1])
        if (not firstAppearance == -1):
            appearances[firstAppearance] = i
        lastAppearance = line.rfind(numbers[i - 1]) 
        if (not lastAppearance == -1):
            appearances[lastAppearance] = i

def getLineSum(line):
    lp = 0
    rp = len(line) - 1
    first = -1
    last = -1
    while (first == -1):
        if (ord(line[lp]) >= 48 and ord(line[lp]) <= 57):
            first = int(line[lp])
        elif (lp in appearances):
            first = appearances[lp]
        lp += 1
    while (last == -1):
        if (ord(line[rp]) >= 48 and ord(line[rp]) <= 57):
            last = int(line[rp])
        elif (rp in appearances):
            last = appearances[rp]
        rp -= 1
    return first * 10 + last

with open(filename, "r") as f:
    finalSum = 0
    for line in f:
        appearances.clear()
        findAppearancesOfNumbers(line)
        finalSum += getLineSum(line)
    print(finalSum)