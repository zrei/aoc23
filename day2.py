filename = "day2.in"

red: int = 12
green: int = 13
blue: int = 14
def getGameId(sepStrings: list[str]) -> int:
    return int(sepStrings[1][:-1])

def checkValidity(red: int, green: int, blue: int, sepStrings: list[str]) -> bool:
    global index
    index = 2
    def checkOneSet() -> bool:
        global index
        finished: bool = False
        while (not finished):
            valid: bool = True
            startingChar: str = sepStrings[index + 1][0]
            if (startingChar == "r"):
                valid = valid and int(sepStrings[index]) <= red
            elif (startingChar == "g"):
                valid = valid and int(sepStrings[index]) <= green
            else:
                valid = valid and int(sepStrings[index]) <= blue
            if (not valid):
                return False
            endingChar: str = sepStrings[index + 1][-1]
            if (endingChar != ","):
                finished = True
            index += 2
        return True
    
    while (index < len(sepStrings)):
        if (not checkOneSet()):
            return False
    return True

with open(filename, 'r') as f:
    finalOutput: int = 0
    for line in f:
        sepStrings: list[int] = line.split(' ')
        gameId: int = getGameId(sepStrings)
        if (checkValidity(red, green, blue, sepStrings)):
            finalOutput += gameId
    print(finalOutput)