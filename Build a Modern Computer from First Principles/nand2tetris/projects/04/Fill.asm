// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

@KBD
D = A
@endAddress
M = D  // Store the based address of the keyboard here. Due to the architecture, the base address of the keyboard shows where the memory of screen ends.

(INFINITE_LOOP)
  @SCREEN
  D = A
  @currentAddress
  M = D  // currentAddress = 16384 (base address of the screen)
  
  @KBD
  D = M
  @clearScreenLoop
  D;JEQ // if KBD = 0 (if no key is pressed)
  @fillScreenLoop
  0;JMP
  
  (clearScreenLoop)
    @currentAddress
    A = M
    M = 0
    @currentAddress
    M = M + 1
    @endAddress
    D = M
    @currentAddress
    D = D - M  
  @clearScreenLoop
  D;JGT // Goto LOOP
  @INFINITE_LOOP
  0;JMP
  
  (fillScreenLoop)
    @currentAddress
    A = M
    M = -1
    @currentAddress
    M = M + 1
    @endAddress
    D = M
    @currentAddress
    D = D - M  
  @fillScreenLoop
  D;JGT // Goto LOOP
  
  @INFINITE_LOOP
  0;JMP