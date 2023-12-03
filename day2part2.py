filename = "day2.in"

def getGameId(sepStrings: list[str]) -> int:
    return int(sepStrings[1][:-1])

def getPower(sepStrings: list[str]) -> int:
    global index
    index = 2
    global red
    red = 0
    global green
    green = 0
    global blue
    blue = 0
    def getMinimumInOneSet() -> None:
        global index
        global red
        global green
        global blue
    
        finished: bool = False
        while (not finished):
            startingChar: str = sepStrings[index + 1][0]
            if (startingChar == "r"):
                red = max(red, int(sepStrings[index]))
            elif (startingChar == "g"):
                green = max(green, int(sepStrings[index]))
            else:
                blue = max(blue, int(sepStrings[index]))
            endingChar: str = sepStrings[index + 1][-1]
            if (endingChar != ","):
                finished = True
            index += 2
    
    while (index < len(sepStrings)):
        getMinimumInOneSet()
    return red * green * blue

with open(filename, 'r') as f:
    finalOutput: int = 0
    for line in f:
        sepStrings: list[int] = line.split(' ')
        finalOutput += getPower(sepStrings)
    print(finalOutput)