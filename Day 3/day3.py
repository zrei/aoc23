filename = "day3.in"

def isNumber(char: str) -> bool:
    return ord(char) >= 48 and ord(char) <= 57

def isSymbol(char: str) -> bool:
    return not isNumber(char) and char != "."

with open(filename, 'r') as f:
    lines: list[str] = []
    for line in f:
        lines.append(line)
    m: int = len(lines)
    n: int = len(lines[0]) - 1
    finalOutput: int = 0
    for row in range(m):
        currNum: int = 0
        rp: int = 0
        toInclude: bool = False
        while (rp < n):
            if (not isNumber(lines[row][rp])):
                if (toInclude):
                    finalOutput += currNum
                currNum = 0
                toInclude = False
            else:
                currNum *= 10
                currNum += int(lines[row][rp])
                if (rp > 0):
                    toInclude = toInclude or isSymbol(lines[row][rp - 1])
                if (rp < n - 1):
                    toInclude = toInclude or isSymbol(lines[row][rp + 1])
                if (row > 0):
                    toInclude = toInclude or isSymbol(lines[row - 1][rp])
                if (row < m - 1):
                    toInlcude = toInclude or isSymbol(lines[row + 1][rp])
                if (row > 0 and rp > 0):
                    toInclude = toInclude or isSymbol(lines[row - 1][rp - 1])
                if (row > 0 and rp < n - 1):
                    toInclude = toInclude or isSymbol(lines[row - 1][rp + 1])
                if (row < m - 1 and rp > 0):
                    toInclude = toInclude or isSymbol(lines[row + 1][rp - 1])
                if (row < m - 1 and rp < n - 1):
                    toInclude = toInclude or isSymbol(lines[row + 1][rp + 1])            
            rp += 1
        if (toInclude):
            finalOutput += currNum
    print(finalOutput)

