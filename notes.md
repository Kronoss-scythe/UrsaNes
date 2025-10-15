immediate: read the number to the right and use that as an operand
absolute: 16 bit {n} endian address that you go to to pull the __8 bit__ operand
zero-page: same as absolute, just 8 bit
indirect: 16 bit {n} endian address that you go to to pull the __16 bit__ operand

indexed:
    Absolute x ^ y: add x or y to the 16 bit {n} endian address and then pull the operand there
    Zero-Page,X (and Zero-Page,Y): same as absolute but 8 bit


Wrap Around:
    Absolute Indexed Wraparound
        1: Absolute indexed addressing did not read from the correct address.
        2: When indexing with X beyond address 0xFFFF, the instruction should read from the zero page.
        3: When indexing with Y beyond address 0xFFFF, the instruction should read from the zero page.

    Zero Page Indexed Wraparound
        1: Zero Page indexed addressing did not read from the correct address.
        2: When indexing with X beyond address 0x00FF, the instruction should still read from the zero page.
        3: When indexing with Y beyond address 0x00FF, the instruction should still read from the zero page.
    
    Indirect Addressing Wraparound
        1: JMP (Indirect) did not move the program counter to the correct address.
        2: The address bus should wrap around the page when reading the low and high bytes with indirect addressing.

    Indirect Addressing, X Wraparound
        1: Indirect, X addressing did not read from the correct address.
        2: The indirect indexing should only occur on the zero page, even if X crosses a page boundary.
        3: The address bus should wrap around the page when reading the low and high bytes with indirect addressing.

    Indirect Addressing, Y Wraparound
        1: Indirect, Y addressing did not read from the correct address.
        2: The Y indexing should be able to cross a page boundary, and the high byte should be updated.
        3: The address bus should wrap around the page when reading the low and high bytes with indirect addressing.

    Relative Addressing Wraparound
        1: You should be able to branch from the Zero Page to page 0xFF.
        2: You should be able to branch from page 0xFF to the Zero Page.