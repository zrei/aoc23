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
    times = []
    distances = []
    for line in f:
        sep = line.split(' ')[1:]
        filtered = filter(lambda x : x, sep)
        mapped = map(lambda x : int(x), filtered)
        if index == 0:
            times = list(mapped)
            index += 1
        else:
            distances = list(mapped)
    finalOutput: int = 0
    for i in range(len(times)):
        if (finalOutput == 0):
            finalOutput = findNumberOfWays(times[i], distances[i])
        else:
            finalOutput *= findNumberOfWays(times[i], distances[i])
    print(finalOutput)