import CPU

def test(file):
    X = CPU.CPU(('__PatreonRoms\\' + file + '.nes'))
    X.reset()
    f = open("__PatreonRoms\\Tracelogs\\" + file + '.txt', "r") 
    lines = [line for line in f]


    for i in range(len(lines))[1:]:
        indexPC = lines[i].find("$") + 1 
        expectPC = int(lines[i][indexPC:indexPC + 4], 16)
        
        indexA = lines[i].find("A:") + 2
        expectA = int(lines[i][indexA:indexA + 2], 16)
        indexX = lines[i].find("X:") + 2
        expectX = int(lines[i][indexX:indexX + 2], 16) 
        indexY = lines[i].find("Y:") + 2
        expectY = int(lines[i][indexY:indexY + 2], 16) 
        indexSP = lines[i].find("SP:") + 3
        expectSP = int(lines[i][indexSP:indexSP + 2], 16)
        indexCycle = lines[i].find("Cycle:") + 6
        expectCycle = int(lines[i][indexCycle:])

        isPC = expectPC == X.PC
        isA = expectA == X.A
        isX = expectX == X.X
        isY = expectY == X.Y
        isSP = expectSP == X.SP
        isCycle = expectCycle == X.totalCycles


        fstr = chr(ord('n')- 32 * X.fNegative) + chr(ord('v')- 32 * X.fOverflow) + '--' + chr(ord('d')- 32 * X.fDecimal) + chr(ord('i')- 32 * X.fInterruptDisable) + chr(ord('z')- 32 * X.fZero) + chr(ord('c')- 32 * X.fCarry)

        expectFlag = lines[i][indexSP + 3:indexSP + 11]
        isFlag = expectFlag == fstr

        if not isPC:
            print("PC did not match up, is: ", hex(X.PC))
            print("Expected: ", hex(expectPC))
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isA:
            print("A did not match up, is: ", hex(X.A))
            print("Expected: ", hex(expectA))
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isX:
            print("X did not match up, is: ", hex(X.X))
            print("Expected: ", hex(expectX))
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isY:
            print("Y did not match up, is: ", hex(X.Y))
            print("Expected: ", hex(expectY))
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isSP:
            print("SP did not match up, is: ", hex(X.SP))
            print("Expected: ", hex(expectSP))
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isCycle:
            print("Cycle did not match up, is: ", X.totalCycles)
            print("Expected: ", expectCycle)
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()
        if not isFlag:
            print("Flag did not match up, is: ", fstr)
            print("Expected: ", expectFlag)
            print("Problem at line", i + 1, "and PC is", hex(X.PC), "and opcode is", hex(X.read(X.PC)))
            quit()

        X.runCommand()
    print("All tests passed :3")

print("test 5")
test('5_Instructions1')
print("test 6")
test('6_Instructions2')