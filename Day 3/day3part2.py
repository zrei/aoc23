filename = "day3.in"

def isNumber(char: str) -> bool:
    return ord(char) >= 48 and ord(char) <= 57

def isSymbol(char: str) -> bool:
    return char == "*"

def getNumber(index: int, n: int, s: str) -> int:
    num: int = 0
    lp: int = index
    rp: int = index
    while (True):
        leftFinished: bool = lp == 0 or not isNumber(s[lp - 1])
        if (not leftFinished):
            lp -= 1

        rightFinished: bool = rp == n - 1 or not isNumber(s[rp + 1])
        if (not rightFinished):
            rp += 1

        if (leftFinished and rightFinished):
            break

    while (lp <= rp):
        num *= 10
        num += int(s[lp])
        lp += 1
    return num

def processGear(row: int, col: int, m: int, n: int, lines: list[str]) -> int:
    numNumbers: int = 0
    gearRatio: int = 1
    for i in range(-1, 2):
        if (row + i < 0 or row + i >= m):
            continue
        for j in range(-1, 2):
            if (col + j < 0 or col + j >= n):
                continue
            if (isNumber(lines[row + i][col + j]) and (j < 0 or not isNumber(lines[row + i][col + j - 1]))):
                gearRatio *= getNumber(col + j, n, lines[row + i])
                numNumbers += 1
    if (numNumbers == 2):
        return gearRatio
    else:
        return 0

with open(filename, 'r') as f:
    lines: list[str] = []
    for line in f:
        lines.append(line)
    m: int = len(lines)
    n: int = len(lines[0]) - 1
    finalOutput: int = 0
    for row in range(m):
        for col in range(n):
            if (isSymbol(lines[row][col])):
                finalOutput += processGear(row, col, m, n, lines)
    print(finalOutput)