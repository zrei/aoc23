import math

fileName = "day6.in"

# inequality: (time - x) * x >= distance
# -x^2 + time * x - distance >= 0
# x^2 - time * x + distance <= 0
def calculateSolution(a, b, c):
    solution1 = (-b + math.sqrt(b * b - 4 * a * c)) / (2 * a)
    solution2 = (-b - math.sqrt(b * b - 4 * a * c)) / (2 * a)
    return solution1, solution2

def findNumberOfWays(time, distance):
    solution1, solution2 = calculateSolution(1, -time, distance)
    if (solution1 > solution2):
        return math.floor(solution1) - math.ceil(max(solution2, 0)) + 1
    else:
        return math.floor(solution2) - math.ceil(max(solution1, 0)) + 1

with open(fileName, 'r') as f:
    index: int = 0
    for line in f:
        if (index == 0):
            line = line[:-1]
        sep = line.split(' ')[1:]
        filtered = filter(lambda x : x, sep)
        final = 0
        for i in filtered:
            final *= math.pow(10, len(i))
            final += int(i)
        if index == 0:
            time = final
            index += 1
        else:
            distance = final
    print(time)
    print(distance)
    finalOutput = findNumberOfWays(time, distance)
    print(finalOutput)