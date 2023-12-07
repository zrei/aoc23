from collections import Counter
from functools import cmp_to_key

filename = "day7.in"

def getIndex(card):
    return '23456789TJQKA'.index(card) + 1

def identifyHand(card):
    count = Counter(card)
    values = list(count.values())
    if (len(values) == 1):
        return 7 # five of a kind
    
    if (len(values) == 2):
        if (values[0] == 4 or values[0] == 1):
            return 6 # four of a kind
        else:
            return 5 # full house
    
    if (len(values) == 3):
        for i in values:
            if (i == 3):
                return 4 # three of a kind
        return 3 # two pair
    
    if (len(values) == 4):
        return 2 # one pair
    
    return 1 # high card

def comparator(cardA, cardB):
    cardAHand = identifyHand(cardA[0])
    cardBHand = identifyHand(cardB[0])
    if (cardAHand > cardBHand):
        return 1
    elif (cardAHand < cardBHand):
        return -1
    for i, j in zip(cardA[0], cardB[0]):
        if i != j:
            return getIndex(i) - getIndex(j)
    return 0

with open(filename, 'r') as f:
    cards = []
    for line in f:
        if line:
            sep = line.split()
            cards.append((sep[0], int(sep[1])))
    cards.sort(key=cmp_to_key(comparator))
    finalOutput = 0
    for idx, i in enumerate(cards):
        finalOutput += (idx + 1) * i[1]
    print(finalOutput)