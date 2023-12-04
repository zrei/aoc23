filename = "day4.in"

def splitLine(s: str) -> list[str]:
    return s.split(' | ')

def splitCardNums(cardNums: str) -> list[str]:
    return cardNums.split(': ')[1].split(' ')

def splitWinningCardNums(winningCardNums: str) -> list[str]:
    return winningCardNums.split(' ')

def numWinningCards(cardNums: list[str], winningCardNums: list[str], overallCardNums: list[int], cardIndex: int) -> None:
    numMatchingCards: int = 0
    for num in cardNums:
        if num and num in winningCardNums:
            numMatchingCards += 1
    for i in range(index + numMatchingCards + 1 - len(overallCardNums)):
        overallCardNums.append(0)
    for i in range(1, numMatchingCards + 1):
        overallCardNums[index + i] += overallCardNums[index]

with open(filename, 'r') as f:
    finalOutput: int = 0
    overallCardNums: list[int] = []
    index: int = 0
    for line in f:
        if (len(overallCardNums) == index):
            overallCardNums.append(0)
        overallCardNums[index] += 1
        cardNums, winningCardNums = splitLine(line[:-1])
        numWinningCards(splitCardNums(cardNums), splitWinningCardNums(winningCardNums), overallCardNums, index)
        index += 1
    for i in overallCardNums:
        finalOutput += i
    print(finalOutput)