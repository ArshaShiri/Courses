// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@R2
M = 0 // 
@i
M = 0 // i = 0

@R1
D = M // Get the content of R1 and store it in D register.
@numberOfRemainingSums
M = D + 1

(LOOP)
  @numberOfRemainingSums
  D = M - 1
  M = D
  @END
  D;JEQ
  @R0
  D = M // Get the value in R0
  @R2
  M = M + D // R2 = R2 + R1 
  @i
  M = M + 1 // i = i + 1
  @LOOP
  0;JMP // Goto LOOP
(END)
 
@END
0;JMP // Infinite loop