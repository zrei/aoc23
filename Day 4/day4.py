filename = "day4.in"

def splitLine(s: str) -> list[str]:
    return s.split(' | ')

def splitCardNums(cardNums: str) -> list[str]:
    return cardNums.split(': ')[1].split(' ')

def splitWinningCardNums(winningCardNums: str) -> list[str]:
    return winningCardNums.split(' ')

def numWinningCards(cardNums: list[str], winningCardNums: list[str]) -> int:
    final: int = 0
    for num in cardNums:
        if num and num in winningCardNums:
            if final == 0:
                final = 1
            else:
                final *= 2
    return final

with open(filename, 'r') as f:
    finalOutput: int = 0
    for line in f:
        cardNums, winningCardNums = splitLine(line[:-1])
        finalOutput += numWinningCards(splitCardNums(cardNums), splitWinningCardNums(winningCardNums))
    print(finalOutput)