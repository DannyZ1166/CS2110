;;=============================================================
;;  CS 2110 - Spring 2024
;;  Final Exam - STRPREFIX
;;=============================================================
;;  Name: Shuohang Zeng
;;============================================================

;;  In this file, you must implement the 'STRPREFIX' subroutine.
 

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (src1, src2, dest)
    ADD R6, R6, -3
    LEA R0, SRC1
    LEA R1, SRC2
    LEA R2, DEST
    STR R0, R6, 0
    STR R1, R6, 1
    STR R2, R6, 2
    JSR STRPREFIX
    LDR R0, R6, 0
    ADD R6, R6, 4
    HALT
    STACK_PTR .fill xF000
    
    ;; You can change these values for debugging!
    SRC1 .stringz "CS 2110 Rules!"
    SRC2 .stringz "CS 2110 Sucks!"
    DEST .stringz ""
    .blkw 100


;;  STRPREFIX Pseudocode  
;;  
;;  STRPREFIX(src1 (addr), src2 (addr), dest (addr)) {
;;      addr new_dest = dest;
;;
;;      while (mem[src1] != '\0' && mem[src1] == mem[src2]) {
;;          mem[new_dest] = mem[src1];
;;          new_dest++;
;;          src1++;
;;          src2++;
;;      }
;;      mem[new_dest] = '\0';
;;      return dest;

STRPREFIX ;; Do not change this label! Treat this as like the name of the function in a function header.
    ;; Code your implementation for the STRPREFIX subroutine here!
    ;; Don't forget to use the Calling Stack Convention!
    
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

    LDR R0, R5, 4 ;src1
    LDR R1, R5, 5 ;src2
    LDR R2, R5, 6 ;dest
    
    
WHILE
    LDR R3, R0, #0 ;mem[src1]
    BRz END_WHILE
    LDR R4, R1, #0 ;mem[src2]
    NOT R4, R4
    ADD R4, R4, #1 ;-mem[src2]
    ADD R4, R3, R4
    BRnp END_WHILE ;R4 free
    
    
    STR R3, R2, #0
    
    ADD R0, R0, 1
    ADD R1, R1, 1
    ADD R2, R2, 1 ;new_dest++
    BR WHILE

END_WHILE
    AND R3, R3, 0
    STR R3, R2, #0
    LDR R2, R5, 6 ;dest
    STR R2, R5, 3
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
.end