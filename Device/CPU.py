


class CPU:
    def __init__(self, string, debug = False):
        self.file = string
        # if you want to print debug
        self.shouldPrint = debug
        return
    
    
    PC = 0  # Program counters bitch (16 bit)
    SP = 0  # stack pointers fucker  (8 bit)
    A = 0   # a register shithead. also called accumalator because why the fuck not (8 bit)
    X = 0   # x register dumbass (8 bit)
    Y = 0   # y register [DATA EXPUNGED] (8 bit)
    cycles = 0 # cycles instruction took to make
    totalCycles = 0 # total cycles for debugging purposes
    
    # RAM, 2048 bytes, with some mirroring from address 0 to address 8000
    # She a thick girl, 2.048 KILOBYTES OF RAM WHOOOOOOOOOOOOOOOOOOOOOOOO
    RAM = bytearray(0x800)
    # ROM, 32.768 bytes of ROM, implement Mapper chips outside 
    ROM = bytearray(0x8000)
    # grab the header because fuck you
    header = bytearray(0x10)

    # should we hault
    hault = False
    # the string to the file
    file: str 
    
    # our flags
    # when put in a register it should look like NV--DIZC
    fCarry = False
    fZero = False
    fInterruptDisable = True
    fDecimal = False
    fOverflow = False
    fNegative = False
    
    # honey if you can't read I can't save you
    def incrementPC(self, value = 1) -> None:
        self.PC = (self.PC + value) & 0xFFFF
        
    # so the RAM and ROM are all in one like, lookup table, so from 0 - 0x8000 is ram, where it reset every 0x800 indexes to 0, because ram mirroring (0x00 == 0x800)
    # onwards is just ROM
    def read(self, Address: int) -> int:
        if Address < 0x8000:
            return self.RAM[Address % 0x800]
        else: # ROM stuff from this point on
            return self.ROM[(Address - 0x8000)]

    # Ditto, except we have an error message, that i will remove in a bit
    def write(self, Address: int, Value: int) -> None:
        if Address < 0x8000:
            self.RAM[Address % 0x800] = Value
        else: # ROM stuff from this point on
            string = f'\tYou were trying to write to the ROM. It literally stands for READ ONLY MEMORY, dipshit :3. \n\t\t\tThe Address you were trying to write too was <{hex(Address)}>. \n\t\tRemember only addresses between 0x0000 and 0x7fff are part of RAM and its mirrors. \n\t\t0x000 to 0x7ff are the base RAM adresses and it repeats from there (So 0x0800 = 0x1000 = 0x0000)'
            print(string)

    # Push a value to stack, make sure to only push 8 bit values
    def push(self, Value) -> None:
        self.write(0x100 + self.SP, Value)
        self.SP = (self.SP - 1) & 0xFF
    # we pull from the stack, come on girl you know this, and if you dont, search up stacks in computers
    def pull(self) -> int:
        self.SP = (self.SP + 1) & 0xFF
        return self.read(0x100 + self.SP)
    
    # basic function to turn two 8 bit values to a 16 bit value
    #^ SHOULD HARD CODE THIS IN EVENTUALLY
    def littleN(self, least, most) -> int:
        x = most << 8
        x = x + least
            
        return x
    
                
            
    
    
    # absolute addressing, how that works is it reads a 16 bit value, adds any arguements, and then returns the address it read, so that we can read it in the function
    # its done this way so that we can save the address for specific instructions
    # also if we pass a page boundary we add a cycle (if specified (I have no idea why some instructions dont add cycles if it crosses a boundary thats insane))
    # so for example if we have an address read of [0x20ff], and we have [args = 2] then we add a cycle because we crossed a page boundary 
    # (instead of being [0x20NN] its [0x21NN], essentialy a page is 0xff entries) 
    # also apparently if statements are faster then bit operations nowadays, til
    def absolute(self, args = 0, addCycles = True) -> int:
        least = self.read(self.PC)
        self.incrementPC()
        most = self.read(self.PC)
        least = (least + args)
        
        if least > 0xff and addCycles:
            self.cycles += 1
        
        address = (self.littleN(least, most))
        return address & 0xFFFF # wrap around
    
    # zeroPage, just read one address, then return it
    # dont forget wrap around
    def zeroPage(self, args = 0) -> int:
        address = self.read(self.PC) + args 
        return address & 0xFF

    # pure evil. pure pure evil.
    # ok, so in preinedxed indirect, for some reason, crossing a page boundary rewraps to the zero page
    # yet
    # for some FUCKING reason
    # post indexed indirect crosses a page boundary
    # i spent
    # two days
    # TWO DAYS
    # trying to figure this shit out
    #     :return: The `indirectPreIndexed` method returns an address calculated using the value at the
    #     Program Counter (PC) plus the X register, reading two bytes from memory at the calculated address
    #     and the next address, combining them into a 16-bit address, and returning the result.
    # """
    # do you see this shit. this is why you shouldnt let ai auto document for you. god what the fuck is this, this is such an awful comment
    def indirectPreIndexed(self) -> int:
        inAddress = self.read(self.PC) + self.X
        inAddress &= 0xFF
        
        least = self.read(inAddress)
        most = self.read((inAddress + 1) & 0xFFFF)
        
        address = (self.littleN(least, most))
        return address & 0xFFFF
    
    # same as before, jsut without any fancy zeropaging because we instead increment after we read the first value, and properly implement wraparound
    def indirectPostIndexed(self, addCycles = True) -> int:
        inAddress = self.read(self.PC)
        
        least = self.read(inAddress) + self.Y
        most = self.read((inAddress + 1) & 0xFFFF)
        if least > 0xff and addCycles:
            self.cycles += 1
        
        address = (self.littleN(least, most))
        return address & 0xFFFF

    # so we just return the amount we should increment the program counter, by reading a byte value
    def relative(self) -> int:
        temp =  self.read(self.PC)
        self.incrementPC()
        if temp > 127:
            temp -= 256
        return temp

    # set the over flow flag
    # essentially if the last bit is the same on both the two inputs AND the first input and the output, then we did over flow AKA if both the inputs are negative or positive and the output is the opposite we overflowd
    # if is flipped then we instead check if both inputs are different and the first input is the same as the output, 
    #^ HARD CODE THIS
    def setOverflow(self, input1: int, input2: int, output: int, isFlipped = False) -> None:
        if isFlipped:
            self.fOverflow = ((input1 ^ input2) & (input1 ^ output) & 0x80) != 0
        else:
            self.fOverflow = (~(input1 ^ input2) & (input1 ^ output) & 0x80) != 0
             
    
    
    #^______________________________________________________Common functions______________________________________________________^#
    
    # arithmetic shift
    def ASL(self, value: int) -> int:
        self.fCarry = value > 127
        value <<= 1
        value &= 0xFF
        self.fZero = value == 0
        self.fNegative = value > 127
        return value
    
    # logical shift right
    def LSR(self, value: int) -> int:
        self.fCarry = (value & 0b1) != 0
        value >>= 1
        self.fZero = value == 0
        self.fNegative = False
        return value
    
    # rotate left
    def ROL(self, value: int) -> int:
        value <<= 1
        value += self.fCarry
        
        self.fCarry = value > 0xFF
        value &= 0xFF
        
        self.fZero = value == 0
        self.fNegative = value >= 0x80
        return value
    
    # rotate right
    def ROR(self, value: int) -> int:
        temp = (value & 0b1) != 0
        value >>= 1
        value += (self.fCarry * 128)
        
        self.fCarry = temp
        
        self.fZero = value == 0
        self.fNegative = value > 127
        return value
    
    def ORA(self, value: int) -> None: # Or function
        self.A |= value
        self.fNegative = self.A > 127
        self.fZero = self.A == 0
        
    def AND(self, value: int) -> None: # And function
        self.A &= value
        self.fNegative = self.A > 127
        self.fZero = self.A == 0
        
    def EOR(self, value: int) -> None: # Xor function
        self.A ^= value
        self.fNegative = self.A > 127
        self.fZero = self.A == 0
        
    def ADC(self, value: int) -> None: # add with carry function
        temp = self.A + value + self.fCarry
        
        self.setOverflow(self.A, value, temp)
        self.fCarry = temp > 0xFF
        self.A = temp & 0xFF
        
        self.fNegative = self.A > 127
        self.fZero = self.A == 0

    def SBC(self, value: int) -> None: # sub with carry function
        temp = self.A - value - (self.fCarry == 0)
        
        self.setOverflow(self.A, value, temp, isFlipped = True)
        self.fCarry = temp >= 0
        self.A = temp & 0xFF
        
        self.fNegative = self.A > 127
        self.fZero = self.A == 0
    
    def CMP(self, value: int, register: int) -> None: #compare with A
        self.fCarry = value <= register
        self.fZero = value == register
        self.fNegative = ((register - value) & 0xFF) > 127
        
    def BIT(self, value: int) -> None: # bit test with A
        self.fZero = (self.A & value) == 0
        self.fNegative = (value & 0x80) != 0
        self.fOverflow = (value & 0x40) != 0

    
    
    
    
    
        
    
    
    
    def CPU(self) -> None:
        
        opcode = self.read(self.PC)
        self.incrementPC()
        if self.shouldPrint == True:
            fstr = chr(ord('n')- 32 * self.fNegative) + chr(ord('v')- 32 * self.fOverflow) + '--' + chr(ord('d')- 32 * self.fDecimal) + chr(ord('i')- 32 * self.fInterruptDisable) + chr(ord('z')- 32 * self.fZero) + chr(ord('c')- 32 * self.fCarry)
            print(f"PC: {hex(self.PC - 1)[2:].upper():0>4}        {hex(opcode)[2:].upper():0>2} {hex(self.read(self.PC))[2:].upper():0>2}       			        A:{hex(self.A)[2:].upper():0>2}\tX:{hex(self.X)[2:].upper():0>2}\tY:{hex(self.Y)[2:].upper():0>2}\tSP:{hex(self.SP)[2:].upper():0>2}\t{fstr}\tCycles:{self.totalCycles}")
        
        
        match opcode:
            case 0x02: # HTL
                self.hault = True
                pass
            
        #&__________________________LDA__________________________&#
            case 0xA9: # LDA, load to a, immediate
                value = self.read(self.PC) 
                self.incrementPC()
                
                self.A = value
                
                self.cycles += 2
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
            
            case 0xA5: # LDA, load to a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 3
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
            
            case 0xB5: # LDA, load to a, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 4
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
                
            case 0xAD: # LDA, load to a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 4
                self.fZero = self.A == 0
                self.fNegative = self.A > 127

            case 0xBD: # LDA, load to a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 4
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
                
            case 0xB9: # LDA, load to a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 4
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
                
            case 0xA1: # LDA, load to a, (indirect,X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 6
                self.fZero = self.A == 0
                self.fNegative = self.A > 127
                
            case 0xB1: # LDA, load to a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.A = value
                
                self.cycles += 5
                self.fZero = self.A == 0
                self.fNegative = self.A > 127


        #&__________________________LDX__________________________&#
            case 0xA2: # LDX, load to x, immediate
                value = self.read(self.PC) 
                self.incrementPC()
                
                self.X = value
                
                self.cycles += 2
                self.fZero = self.X == 0
                self.fNegative = self.X > 127
            
            case 0xA6: # LDX, load to x, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.X = value
                
                self.cycles += 3
                self.fZero = self.X == 0
                self.fNegative = self.X > 127
            
            case 0xB6: # LDX, load to x, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.X = value
                
                self.cycles += 4
                self.fZero = self.X == 0
                self.fNegative = self.X > 127
                
            case 0xAE: # LDX, load to x, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.X = value
                
                self.cycles += 4
                self.fZero = self.X == 0
                self.fNegative = self.X > 127
                
            case 0xBE: # LDX, load to x, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.X = value
                
                self.cycles += 4
                self.fZero = self.X == 0
                self.fNegative = self.X > 127
                
                
        #&__________________________LDY__________________________&#
            case 0xA0: # LDY, load to y, immediate
                value = self.read(self.PC) 
                self.incrementPC()
                
                self.Y = value
                
                self.cycles += 2
                self.fZero = self.Y == 0
                self.fNegative = self.Y > 127
            
            case 0xA4: # LDY, load to y, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.Y = value
                
                self.cycles += 3
                self.fZero = self.Y == 0
                self.fNegative = self.Y > 127
            
            case 0xB4: # LDY, load to y, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.Y = value
                
                self.cycles += 4
                self.fZero = self.Y == 0
                self.fNegative = self.Y > 127
                
            case 0xAC: # LDY, load to y, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.Y = value
                
                self.cycles += 4
                self.fZero = self.Y == 0
                self.fNegative = self.Y > 127

            case 0xBC: # LDY, load to y, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.Y = value
                
                self.cycles += 4
                self.fZero = self.Y == 0
                self.fNegative = self.Y > 127
                
            
            
        #&__________________________STA__________________________&#               
            case 0x85: # STA, store a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 3
        
            case 0x95: # STA, store a, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 4
                
            case 0x8D: # STA, store a, absolute
                address = self.absolute()
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 4

            case 0x9D: # STA, store a, absolute,X
                address = self.absolute(self.X, addCycles = False)
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 5
                
            case 0x99: # STA, store a, absolute,Y
                address = self.absolute(self.Y, addCycles = False)
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 5
            
            case 0x81: # STA, store a, (indirect,X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 6

            case 0x91: # STA, store a, (indirect),Y
                address = self.indirectPostIndexed(False)
                self.incrementPC()
                
                self.write(address, self.A)
                self.cycles += 6
                
                
        #&__________________________STX__________________________&#               
            case 0x86: # STX, store x, zeropage
                address = self.zeroPage()
                self.incrementPC()
                
                self.write(address, self.X)
                self.cycles += 3
        
            case 0x96: # STX, store x, zeropage,Y
                address = self.zeroPage(self.Y)
                self.incrementPC()
                
                self.write(address, self.X)
                self.cycles += 4
                
            case 0x8E: # STX, store x, absolute
                address = self.absolute()
                self.incrementPC()
                
                self.write(address, self.X)
                self.cycles += 4


        #&__________________________STY__________________________&#               
            case 0x84: # STY, store y, zeropage
                address = self.zeroPage()
                self.incrementPC()
                
                self.write(address, self.Y)
                self.cycles += 3
        
            case 0x94: # STY, store y, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                
                self.write(address, self.Y)
                self.cycles += 4
                
            case 0x8C: # STY, store y, absolute
                address = self.absolute()
                self.incrementPC()
                
                self.write(address, self.Y)
                self.cycles += 4
            
            
          
            case 0x10: # BPL pranch on plus, !negative flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if not self.fNegative:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    
                    self.cycles += 3
                else:
                    self.cycles += 2
                    
            case 0x30: # BMI branch on minus, negative flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if self.fNegative:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
                    
            case 0x50: # BVC branch on _overflow clear, !_overflow flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if not self.fOverflow:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
            
            case 0x70: # BVS branch on _overflow, _overflow flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if self.fOverflow:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
            
            case 0x90: # BCC branch on carry clear, !carry flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if not self.fCarry:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
            
            case 0xB0: # BVS branch on carry, carry flag
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if self.fCarry:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
                    
            case 0xD0: # BNE Branch on not equal clear, !zero flag 
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if not self.fZero:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2

            case 0xF0: # BEQ Branch on not equal, zero flag 
                temp =  self.read(self.PC)
                self.incrementPC()
                
                if self.fZero:
                    if temp > 127:
                        temp -= 256
                    if (temp + self.PC) & 0xff00 > self.PC:
                        cycles += 1
                    self.incrementPC(temp)
                    self.cycles += 3
                else:
                    self.cycles += 2
            
            
            
            case 0x48: # PHA or push a
                self.push(self.A)
                self.cycles += 3
            case 0x68: # PLA or pull a
                self.A = self.pull()
                self.fZero = (self.X == 0)
                self.fNegative = (self.X > 127)
                self.cycles += 4

            case 0x20: # JSR, jump to subroutine n where n is a 2 byte little endian address, and push PC to stack
                address = self.absolute()
                
                self.push(self.PC >> 8)
                self.push(self.PC & 0xFF)
                self.PC = address
                self.cycles += 6
            case 0x60: # RTS, return from subroutine n where n is a 2 byte little endian address pulled from stack and set PC to n
                least = self.pull()
                most = self.pull()
                self.PC = self.littleN(least, most)
                self.incrementPC()

                self.cycles += 6
                
        #&__________________________JMP__________________________&#
            case 0x4C: # JMP absolute
                address = self.absolute()
                self.incrementPC()
                
                self.PC = address
                self.cycles += 3
                
            case 0x6C: # JMP indirect
                address = self.absolute()
                self.incrementPC()
                
                low = self.read(address)
                high = self.read((address + 1) & 0xFFFF)
                value = self.littleN(low, high)
                
                self.PC = value
                self.cycles += 5
                

        #&__________________________INC__________________________&#
            case 0xE6: # INC, increment memory, zeropage
                address = self.zeroPage()
                self.incrementPC()
                
                value = self.read(address)
                value = (value + 1) & 0xff
                self.fZero = (value == 0)
                self.fNegative = (value > 127)
                
                self.write(address, value)
                self.cycles += 5
                
            case 0xF6: # INC, increment memory, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                
                value = self.read(address)
                value = (value + 1) & 0xff
                self.fZero = (value == 0)
                self.fNegative = (value > 127)
                
                self.write(address, value)
                self.cycles += 6
                
            case 0xEE: # INC, increment memory, absolute
                address = self.absolute()
                self.incrementPC()
                
                value = self.read(address)
                value = (value + 1) & 0xff
                self.fZero = (value == 0)
                self.fNegative = (value > 127)
                
                self.write(address, value)
                self.cycles += 6

            case 0xEE: # INC, increment memory, absolute
                address = self.absolute(self.X, False)
                self.incrementPC()
                
                value = self.read(address)
                value = (value + 1) & 0xff
                self.fZero = (value == 0)
                self.fNegative = (value > 127)
                
                self.write(address, value)
                self.cycles += 6
                
            case 0xE8: # INX, increment x by 1
                self.X = (self.X + 1) & 0xFF
                self.fZero = (self.X == 0)
                self.fNegative = (self.X > 127)    
                
                self.cycles += 2
            
            case 0xC8: # INY, increment y by 1
                self.Y = (self.Y + 1) & 0xFF
                self.fZero = (self.Y == 0)
                self.fNegative = (self.Y > 127)
                
                self.cycles += 2
            
        #&__________________________DEC__________________________&#
            case 0xCA: # DEX, decrement x by 1
                self.X = (self.X - 1) & 0xFF
                self.fZero = (self.X == 0)
                self.fNegative = (self.X > 127)
                
                self.cycles += 2
                
            case 0x88: # DEY, decrement y by 1
                self.Y = (self.Y - 1) & 0xFF
                self.fZero = (self.Y == 0)
                self.fNegative = (self.Y > 127)
                
                self.cycles += 2
                
        #&__________________________TNM__________________________&#
            # transfer
            case 0xAA: # TAX, transfer a to x
                self.X = self.A
                self.fZero = (self.X == 0)
                self.fNegative = (self.X > 127)
                self.cycles += 2
            case 0xA8: # TAY, transfer a to y
                self.Y = self.A
                self.fZero = (self.Y == 0)
                self.fNegative = (self.Y > 127)
                self.cycles += 2
            
            case 0x8A: # TXA, transfer x to a
                self.A = self.X
                self.fZero = (self.A == 0)
                self.fNegative = (self.A > 127)
                self.cycles += 2
            case 0x98: # TYA, transfer y to a
                self.A = self.Y
                self.fZero = (self.A == 0)
                self.fNegative = (self.A > 127)
                self.cycles += 2
            
            case 0xBA: # TSX, transfer stack pointer to x
                self.X = self.SP
                self.fZero = (self.X == 0)
                self.fNegative = (self.X > 127)
                self.cycles += 2
            
            case 0x9A: # TXS, transfer x to stack pointer
                self.SP = self.X
                self.cycles += 2
                
                
        #&__________________________SET__________________________&#
            case 0x38: # SEC, set carry flag, implied
                self.fCarry = True
                self.cycles += 2
            case 0x18: # CLC, clear carry, implied
                self.fCarry = False
                self.cycles += 2
            case 0xD8: # CLD, clear decimal flag, implied
                self.fDecimal = False
                self.cycles += 2
            case 0x58: # CLI, clear interupt, implied
                self.fInterruptDisable = True
                self.cycles += 2
            case 0xB8: # CLV, clear overflow, implied
                self.fOverflow = False
                self.cycles += 2
            case 0x78: # SEI, set interupt, implied
                self.fCarry = True
                self.cycles += 2
            case 0xF8: # SED, set decimal, implied
                self.fDecimal = True
                self.cycles += 2
            
            
        #&__________________________PSH__________________________&#
            case 0x08: # PHP, push flags to stack, implied
                flag = self.fCarry
                flag += self.fZero << 1
                flag += self.fInterruptDisable << 2
                flag += self.fDecimal << 3
                flag += 0x30
            
                flag += self.fOverflow << 6
                flag += self.fNegative << 7
                
                self.push(flag)
                
                self.cycles += 3
            
            case 0x28: # PLP, pull flags from stack, implied
                flag = self.pull()
                
                self.fCarry = (flag & 0b1) != 0
                self.fZero = (flag & 0b10) != 0
                self.fInterruptDisable = (flag & 0b100) != 0
                self.fDecimal = (flag & 0b1000) != 0
                
                
                self.fOverflow = (flag & 0b1000000) != 0
                self.fNegative = (flag & 0b10000000) != 0
                
                self.cycles += 4
                
                
    
        #&__________________________ASN__________________________&#
            case 0x0A: # ASL A, arithmetic shift left the a register, implied
               self.A = self.ASL(self.A)
               self.cycles += 2
                
            case 0x06: # ASL, arithmetic shift left, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ASL(value)
                
                self.write(address, value)
                self.cycles += 5
            
            case 0x16: # ASL, arithmetic shift left, zeropage,x
                address = self.zeroPage(args = self.X)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ASL(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x0E: # ASL, arithmetic shift left, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ASL(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x0E: # ASL, arithmetic shift left, absolute,X
                address = self.absolute(self.X, addCycles = False)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ASL(value)
                
                self.write(address, value) 
                self.cycles += 7 
                
                
                
            case 0x4A: # LSR A, logical shift right the a register, implied
               self.A = self.LSR(self.A)
               self.cycles += 2
                
            case 0x46: # LSR, logical shift right, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                value = self.LSR(value)
                
                self.write(address, value)
                self.cycles += 5
            
            case 0x56: # LSR, logical shift right, zeropage,x
                address = self.zeroPage(args = self.X)
                self.incrementPC()
                value = self.read(address)
                
                value = self.LSR(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x4E: # LSR, logical shift right, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                value = self.LSR(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x5E: # LSR, logical shift right, absolute,X
                address = self.absolute(self.X, addCycles = False)
                self.incrementPC()
                value = self.read(address)
                
                value = self.LSR(value)
                
                self.write(address, value) 
                self.cycles += 7 

                
                
        #&__________________________RON__________________________&#
            case 0x2A: # ROL A, rotate left with a (ASL + Carry flag), implied
                self.A = self.ROL(self.A)
                self.cycles += 2
            
            case 0x26: # ROL, rotate left, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROL(value) 
                
                self.write(address, value)
                self.cycles += 5
                
            case 0x36: # ROL, rotate shift left, zeropage,x
                address = self.zeroPage(args = self.X)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROL(value)
                
                self.write(address, value) 
                self.cycles += 6 

            case 0x2E: # ROL, rotate shift left, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROL(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x3E: # ROL, rotate shift left, absolute,X
                address = self.absolute(self.X, addCycles = False)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROL(value)
                
                self.write(address, value) 
                self.cycles += 7
            
            
            
            case 0x6A: # ROR A, rotate right with a (ASL + Carry flag), implied
                self.A = self.ASL(self.A)
                self.cycles += 2
            
            case 0x66: # ROR, rotate right, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROR(value) 
                
                self.write(address, value)
                self.cycles += 5
                
            case 0x76: # ROR, rotate right, zeropage,x
                address = self.zeroPage(args = self.X)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROR(value)
                
                self.write(address, value) 
                self.cycles += 6 

            case 0x6E: # ROR, rotate right, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROR(value)
                
                self.write(address, value) 
                self.cycles += 6 
                
            case 0x7E: # ROR, rotate left, absolute,X
                address = self.absolute(self.X, addCycles = False)
                self.incrementPC()
                value = self.read(address)
                
                value = self.ROR(value)
                
                self.write(address, value) 
                self.cycles += 7
                 
                
        #&__________________________ORA__________________________&#
            case 0x09: # ORA, or with a and store at a, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.ORA(value)
                
                self.cycles += 2
                
            case 0x05: # ORA, or with a and store at a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 3
            
            case 0x15: # ORA, or with a and store at a, zeropage,x
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 4
                
            case 0x0D: # ORA, or with a and store at a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 4
                
            case 0x1D: # ORA, or with a and store at a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 4
            
            case 0x19: # ORA, or with a and store at a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 4

            case 0x01: # ORA, or with a and store at a, (indirect, X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 6
            
            case 0x11: # ORA, or with a and store at a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.ORA(value)
                
                self.cycles += 5
            
            
            
            
        #&__________________________AND__________________________&#
            case 0x29: # AND, and with a and store at a, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.AND(value)
                
                self.cycles += 2
                
            case 0x25: # AND, and with a and store at a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 3
            
            case 0x35: # AND, and with a and store at a, zeropage,x
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 4
                
            case 0x2D: # AND, and with a and store at a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 4
                
            case 0x3D: # AND, and with a and store at a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 4
            
            case 0x39: # AND, and with a and store at a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 4

            case 0x21: # AND, and with a and store at a, (indirect, X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 6
            
            case 0x31: # AND, and with a and store at a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.AND(value)
                
                self.cycles += 5
                
                
        #&__________________________EOR__________________________&#
            case 0x49: # EOR, xor with a and store at a, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.EOR(value)
                
                self.cycles += 2
                
            case 0x45: # EOR, xor with a and store at a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 3
            
            case 0x55: # EOR, xor with a and store at a, zeropage,x
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 4
                
            case 0x4D: # EOR, xor with a and store at a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 4
                
            case 0x5D: # EOR, xor with a and store at a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 4
            
            case 0x59: # EOR, xor with a and store at a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 4

            case 0x41: # EOR, xor with a and store at a, (indirect, X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 6
            
            case 0x51: # EOR, xor with a and store at a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.EOR(value)
                
                self.cycles += 5
                
                
        #&__________________________ADC/SBC______________________&#
            case 0x69: # ADC, add with carry and store in a, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.ADC(value)
                
                self.cycles += 2
            
            case 0x65: # ADC, add with carry and store in a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 3
            
            case 0x75: # ADC, add with carry and store in a, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 4
            
            case 0x6D: # ADC, add with carry and store in a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 4
        
            case 0x7D: # ADC, add with carry and store in a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 4
        
            case 0x79: # ADC, add with carry and store in a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 4
                
            case 0x61: # ADC, add with carry and store in a, (indirect,X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 6       

            case 0x61: # ADC, add with carry and store in a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.ADC(value)
                
                self.cycles += 5   
            
            
            
            case 0xE9: # SBC, subtract with carry and store in a, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.SBC(value)
                
                self.cycles += 2
            
            case 0xE5: # SBC, subtract with carry and store in a, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 3
            
            case 0xF5: # SBC, subtract with carry and store in a, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 4
            
            case 0xED: # SBC, subtract with carry and store in a, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 4
        
            case 0xFD: # SBC, subtract with carry and store in a, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 4
        
            case 0xF9: # SBC, subtract with carry and store in a, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 4
                
            case 0xE1: # SBC, subtract with carry and store in a, (indirect,X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 6       

            case 0xF1: # SBC, subtract with carry and store in a, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.SBC(value)
                
                self.cycles += 5   
            
            
        #&__________________________CMP/BIT______________________&#
            case 0xC9: # CMP, compare with a and store in flag, immediate
                value = self.read(self.PC)
                self.incrementPC()
                self.CMP(value, self.A)
                
                self.cycles += 2
            
            case 0xC5: # CMP, compare with a and store in flag, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 3
            
            case 0xD5: # CMP, compare with a and store in flag, zeropage,X
                address = self.zeroPage(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 4
            
            case 0xCD: # CMP, compare with a and store in flag, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 4
        
            case 0xDD: # CMP, compare with a and store in flag, absolute,X
                address = self.absolute(self.X)
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 4
        
            case 0xD9: # CMP, compare with a and store in flag, absolute,Y
                address = self.absolute(self.Y)
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 4
                
            case 0xC1: # CMP, compare with a and store in flag, (indirect,X)
                address = self.indirectPreIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 6       

            case 0xD1: # CMP, compare with a and store in flag, (indirect),Y
                address = self.indirectPostIndexed()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.A)
                
                self.cycles += 5 
            
            
            case 0x24: # BIT, bit test with a and store in flag, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.BIT(value)
                
                self.cycles += 3
        
            case 0x2C: # BIT, bit test with a and store in flag, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.BIT(value)
                
                self.cycles += 4
            
            
        #&______________________CPX______________________&#
            case 0xE0: # CPX, compare with x and store in flag, immediate
                value = self.read(self.PC)
                self.incrementPC()
                
                self.CMP(value, self.X)
                
                self.cycles += 2
            
            case 0xE4: # CPX, compare with x and store in flag, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                self.CMP(value, self.X)
                
                self.cycles += 3
            
            case 0xEC: # CPX, compare with x and store in flag, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.X)
                
                self.cycles += 4

        #&______________________CPY______________________&#
            case 0xC0: # CPY, compare with y and store in flag, immediate
                value = self.read(self.PC)
                self.incrementPC()
                self.CMP(value, self.Y)
                
                self.cycles += 2
            
            case 0xC4: # CPY, compare with y and store in flag, zeropage
                address = self.zeroPage()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.Y)
                
                self.cycles += 3
            
            case 0xCC: # CPY, compare with y and store in flag, absolute
                address = self.absolute()
                self.incrementPC()
                value = self.read(address)
                
                self.CMP(value, self.Y)
                
                self.cycles += 4
            
            
            
            
            
            case 0x00: # BRK, break routine and jump to the address from 0xfffe to 0xffff, implied
                self.incrementPC()
                
                self.push((self.PC >> 8) & 0xff)
                self.push((self.PC) & 0xff)
                
                flag = self.fCarry
                flag += self.fZero << 1
                flag += self.fInterruptDisable << 2
                flag += self.fDecimal << 3
                flag += 0x30
            
                flag += self.fOverflow << 6
                flag += self.fNegative << 7
                
                self.push(flag)
                
                least = self.read(0xFFFE)
                most = self.read(0xFFFF)
                self.PC = self.littleN(least, most)
                self.cycles += 7
                
            case 0x40: # RTI, return from interupt, implied
                flag = self.pull()
                
                self.fCarry = (flag & 0b1) != 0
                self.fZero = (flag & 0b10) != 0
                self.fInterruptDisable = (flag & 0b100) != 0
                self.fDecimal = (flag & 0b1000) != 0
                
                
                self.fOverflow = (flag & 0b1000000) != 0
                self.fNegative = (flag & 0b10000000) != 0
                
                least = self.pull()
                most = self.pull()
                self.PC = self.littleN(least, most)
                self.cycles += 6
                
            case 0xEA: #NOP, yk, none
                self.cycles += 2

            case _:
                RAMTEST = list(self.RAM)
                # print("UNDETERMINED ", hex(opcode), '  ', hex(self.PC))
                return
        
        self.totalCycles += self.cycles
        self.cycles = 0

            
            
        


    def reset(self) -> None:
        with open(self.file, 'rb') as f: #^ change the hard coding
            headeredROM = bytearray(f.read())
            self.ROM = headeredROM[0x10:0x8010] # the rom is 0x8010 lines long but we want to skip the header hence the weird concacting
            self.header = headeredROM[:0x10] #store header
            PCL = self.read(0xFFFc) # low number
            PCH = self.read(0xFFFD) # high number
            self.PC = int((PCH * 0x100) + PCL) # turn into little endian
            self.SP = 0xFD
            self.fInterruptDisable = True
            self.totalCycles = 7
        
        
    
    
    
    def run(self) -> None:
        print(f"\n\nPC: {hex(self.PC)[2:].upper():04}	--		RESET			A: 00   X: 00    Y :00      SP: {hex(self.SP)[2:].upper():02}    Cycles: 0\n")
        while not self.hault:
            self.CPU()
        # RAMTEST = list(self.RAM)
        # print("done")
        
    
    def runCommand(self) -> None:
        if not self.hault:
            self.CPU()

            

def test(string): # for testing purpose only
    
    X = CPU(string, True)
    X.reset()
    X.run()
    
    
if __name__ == '__main__':
    # test('__PatreonRoms\\1_Example.nes')  
    # test('__PatreonRoms\\2_ReadWrite.nes')
    # test('__PatreonRoms\\3_Branches.nes')
    # test('__PatreonRoms\\4_TheStack.nes')
    # test('__PatreonRoms\\5_Instructions1.nes')
    test('__PatreonRoms\\6_Instructions2.nes')
    # test('official_only.nes')
    with open("ramTest.txt", "wb") as binary_file:
        binary_file.write(CPU.RAM)