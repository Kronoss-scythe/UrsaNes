from numpy import place
import CPU
import tkinter as tk

X = CPU.CPU('__PatreonRoms\\5_Instructions1.nes')
X.reset()
width = 16 * 90
height = 9 * 90
window = tk.Tk()
window.title("easyNES")
colour = "#e3ddff"
window.configure(background=colour)
window.geometry(f"{width}x{height}")
fontTerminal = ("terminal", 15)
fontConsole = ("Lucida Console", 15)
lfReg = tk.LabelFrame(window, text='Registers', font=fontConsole, background=colour)

ratioConstant = 12


def placeRegisters(row = 0, column = 0):
    #*___________________________Registers___________________________*#
    lfA = tk.LabelFrame(lfReg, text='A', font=fontConsole, background=colour)
    lfX = tk.LabelFrame(lfReg, text='X', font=fontConsole, background=colour)
    lfY = tk.LabelFrame(lfReg, text='Y', font=fontConsole, background=colour)
    
    
    lfA.grid(row = row + 1, column = column, columnspan=1, rowspan=1, sticky = "NW", padx=2, pady=2)
    lfX.grid(row = row + 1, column = column + 1, columnspan=1, rowspan=1, sticky = "NW", padx=2, pady=2)
    lfY.grid(row = row + 1, column = column + 2, columnspan=1, rowspan=1, sticky = "NW", padx=2, pady=2)

    
    
    regA = tk.Label(lfA, text=f"{hex(X.A)[2:].upper():0>2}", font=fontTerminal, borderwidth=4, relief = "ridge")
    regA.grid(row = row, column = column)
    regX = tk.Label(lfX, text=f"{hex(X.X)[2:].upper():0>2}", font=fontTerminal, borderwidth=4, relief = "ridge")
    regX.grid(row = row, column = column)
    regY = tk.Label(lfY, text=f"{hex(X.Y)[2:].upper():0>2}", font=fontTerminal, borderwidth=4, relief = "ridge")
    regY.grid(row = row, column = column)

def placePC(row = 0, column = 0):
    lfPC = tk.LabelFrame(lfReg, text='PC', font=fontConsole, background=colour) 
    
    lfPC.grid(row = row, column = column, columnspan=1, rowspan=2, sticky = "NW", padx=2, pady=2)
    
    PC = tk.Label(lfPC, text=f"{hex(X.PC)[2:].upper():0>4}", font=fontTerminal, borderwidth=4, relief = "ridge")
    PC.grid(row = row + 1, column = column)
    
def placeSP(row = 0, column = 0):
    lfPC = tk.LabelFrame(lfReg, text='SP', font=fontConsole, background=colour) 
    
    lfPC.grid(row = row, column = column, columnspan=1, rowspan=2, sticky = "NW", padx=2, pady=2)
    
    PC = tk.Label(lfPC, text=f"{hex(X.SP)[2:].upper():0>2}", font=fontTerminal, borderwidth=4, relief = "ridge")
    PC.grid(row = row + 1, column = column)

def placeFlags(row = 0, column = 0):
    lfFlags = tk.LabelFrame(lfReg, text='FLAGS', font=fontConsole, background=colour) 
    lfFlags.grid(row = row, column = column, columnspan=3, rowspan=2, sticky = "NW", padx=2, pady=2)
    
    flag = int(X.fCarry)
    flag += int(X.fZero) << 1
    flag += int(X.fInterruptDisable) << 2
    flag += int(X.fDecimal) << 3
    flag += 0x30
    flag += int(X.fOverflow) << 6
    flag += int(X.fNegative) << 7
    
    Flags = tk.Label(lfFlags, text=f"{bin(flag)[2:]:0>8}", font=fontTerminal, borderwidth=4, relief = "ridge")
    Flags.grid(row = row + 1, column = column) 
    flagsInfo = tk.Label(lfFlags, text="NV--DIZC", font=fontTerminal, background=colour)
    flagsInfo.grid(row = 4, column = 1)
    
def placeCycles(row = 0, column = 0):
    lfCycles = tk.LabelFrame(window, text='Cycles', font=fontConsole, background=colour) 
    lfCycles.grid(row = row, column = column, columnspan=1, rowspan=2, padx=2, pady=10, sticky="W")
    
    Cycles = tk.Label(lfCycles, text=f"{X.totalCycles:0>8}", font=fontTerminal, borderwidth=4, relief = "ridge")
    Cycles.grid(row = row + 1, column = column)
    
def print():
    lfReg.grid(row = 0, column = 0, columnspan=3, rowspan=4, padx=2, pady=10, sticky="W")
    placePC()
    placeSP(row = 2)
    placeFlags(row = 2, column = 1)
    placeRegisters(column = 1)
    
    placeCycles(column = 3) 
    traceButton()


def traceThrough():
    print()
    X.runCommand()

def traceButton():
    tk.Button(
        window, 
        text="Trace", 
        command=traceThrough, 
        font=fontConsole,
        ).grid(row=9*ratioConstant, column=0, columnspan=3, rowspan=1, sticky = "SW")

for i in range(16 * ratioConstant):
    window.grid_columnconfigure(i, weight=1,)
for i in range(9 * ratioConstant):
    window.grid_rowconfigure(i, weight=1,)
# CPU.test('__PatreonRoms\\5_Instructions1.nes')

# print()
traceButton()
window.mainloop()
