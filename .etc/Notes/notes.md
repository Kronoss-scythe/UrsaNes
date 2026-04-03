modes:
    immediate: read the number to the right and use that as an operand
    absolute: 16 bit {n} endian address that you go to to pull the __8 bit__ operand
    zero-page: same as absolute, just 8 bit
    indirect: 16 bit {n} endian address that you go to to pull the __16 bit__ operand
    example:
        A           Accumulator             OPC A	        operand is AC (implied single byte instruction)
        abs	        absolute	            OPC $LLHH	    operand is address $HHLL *
        abs,X       absolute, X-indexed	    OPC $LLHH,X	    operand is address; effective address is address incremented by X with carry **
        abs,Y       absolute, Y-indexed	    OPC $LLHH,Y	    operand is address; effective address is address incremented by Y with carry **
        #	        immediate	            OPC #$BB    	operand is byte BB
        impl	    implied	                OPC	            operand implied
        ind	        indirect	            OPC ($LLHH)	    operand is address; effective address is contents of word at address: C.w($HHLL)
        X,ind	    X-indexed, indirect	    OPC ($LL,X)	    operand is zeropage address; effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)
        ind,Y	    indirect, Y-indexed	    OPC ($LL),Y	    operand is zeropage address; effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y
        rel	        relative	            OPC $BB     	branch target is PC + signed offset BB ***
        zpg	        zeropage	            OPC $LL	        operand is zeropage address (hi-byte is zero, address = $00LL)
        zpg,X	    zeropage, X-indexed	    OPC $LL,X	    operand is zeropage address; effective address is address incremented by X without carry **
        zpg,Y	    zeropage, Y-indexed	O   PC $LL,Y	    operand is zeropage address; effective address is address incremented by Y without carry **

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