;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - GCD
;;=============================================================
;;  Name: 
;;============================================================

;;  In this file, you must implement the 'MOD' and 'GCD' subroutines.

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments A and B
    ADD R6, R6, -2
    LD R1, A
    STR R1, R6, 0
    LD R1, B
    STR R1, R6, 1 
    JSR GCD
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT

    STACK_PTR   .fill xF000
    ;; You can change these numbers for debugging!

    A           .fill 10
    B           .fill 4
    
    
;;  MOD Pseudocode (see PDF for explanation and examples)   
;;  
;;  MOD(int a, int b) {
;;      while (a >= b) {
;;          a -= b;
;;      }
;;      return a;
;;  }

MOD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the MOD subroutine here!
    
ADD	R6, R6, -4	; Allocate space
STR	R7, R6, 2	; Save Ret Addr
STR	R5, R6, 1	; Save Old FP
ADD	R5, R6, 0	; Copy SP to FP
ADD	R6, R6, -5	; Make room for saved regs & l.v. 1-n
STR R0, R5, -1	; Save R0	
STR R1, R5, -2	; Save R1
STR R2, R5, -3	; Save R2
STR R3, R5, -4	; Save R3
STR R4, R5, -5	; Save R4

    LDR R1, R5, 4
    LDR R2, R5, 5

NOT R3, R2
ADD R3, R3, #1          ;negate b

; Loop until a is less than b
LOOP_MOD
    ADD R1, R3, R1 
    BRzp LOOP_MOD
    ADD R0, R1, R2
        
        STR	R0, R5, 3       ;save return value on stack
        LDR R4, R5, -5      ;Restore R0
        LDR R3, R5, -4      ;Restore R1
        LDR R2, R5, -3      ;Restore R2
        LDR R1, R5, -2      ;Restore R3
        LDR R0, R5, -1      ;Restore R4
        ADD R6, R5, 0       ;Reset stack pointer to frame pointer (R6 = R5)
        LDR R5, R6, 1       ;Restore old frame pointer to R5
        LDR R7, R6, 2       ;Restore return address to R7
        ADD R6, R6, 3       ;Reallocate space for local, old frame pointer and return address
  
  RET           


;;  GCD Pseudocode (see PDF for explanation and examples)
;;
;;  GCD(int a, int b) {
;;      if (b == 0) {
;;          return a;
;;      }
;;        
;;      while (b != 0) {
;;          int temp = b;
;;          b = MOD(a, b);
;;          a = temp;
;;      }
;;      return a;
;;  }

GCD ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the GCD subroutine here!
    
ADD	R6, R6, -4	; Allocate space
STR	R7, R6, 2	; Save Ret Addr
STR	R5, R6, 1	; Save Old FP
ADD	R5, R6, 0	; Copy SP to FP
ADD	R6, R6, -5	; Make room for saved regs & l.v. 1-n
STR R0, R5, -1	; Save R0	
STR R1, R5, -2	; Save R1
STR R2, R5, -3	; Save R2
STR R3, R5, -4	; Save R3
STR R4, R5, -5	; Save R4



; Base case: if b is 0, return a (a is the GCD)
LDR R1, R5, 4
LDR R2, R5, 5
BRz END_GCD   ; Jump to end if b is 0

LOOP_GCD
        ADD R3, R2, #0
        ADD R6, R6, -2
        STR R1, R6, 0
        STR R2, R6, 1 
        JSR MOD
        ADD R1, R3, #0
        LDR R2, R6, 0
        ADD R6, R6, 3
        ADD R2, R2, 0
BRnp LOOP_GCD

END_GCD STR	R1, R5, 3       ;save return value on stack
TEAR    LDR R4, R5, -5      ;Restore R0
        LDR R3, R5, -4      ;Restore R1
        LDR R2, R5, -3      ;Restore R2
        LDR R1, R5, -2      ;Restore R3
        LDR R0, R5, -1      ;Restore R4
        ADD R6, R5, 0       ;Reset stack pointer to frame pointer (R6 = R5)
        LDR R5, R6, 1       ;Restore old frame pointer to R5
        LDR R7, R6, 2       ;Restore return address to R7
        ADD R6, R6, 3       ;Reallocate space for local, old frame pointer and return address
    RET
.end

