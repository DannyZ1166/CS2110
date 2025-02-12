;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - isPalindrome
;;=============================================================
;;  Name: 
;;============================================================

;;  In this file, you must implement the 'isPalindrome' subroutine.
 

.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (word addr and len)
    ADD R6, R6, -2
    LEA R0, STRING
    LD R1, LENGTH
    STR R0, R6, 0
    STR R1, R6, 1
    JSR IS_PALINDROME
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT
    STACK_PTR .fill xF000
    LENGTH .fill 5                 ;; Change this to be length of STRING
    STRING .stringz "rotor"	       ;; You can change this string for debugging!


;;  IS_PALINDROME **RECURSIVE** Pseudocode
;;
;;  isPalindrome(word (addr), len) { 
;;      if (len == 0 || len == 1) {
;;          return 1;
;;      } else {
;;          if (word[0] == word[len - 1]) {
;;              return IS_PALINDROME(word + 1, len - 2);
;;          } else { 
;;              return 0;
;;          }
;;      }
;;  }
;;
IS_PALINDROME ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the isPalindrome subroutine here!
    ;; NOTE: Your implementation MUST be done recursively
    
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
    
    BRz END_LOOP ;return 1
    ADD R2, R2, -1
    BRz END_LOOP
    
    LDR R3, R1, 0
    ADD R4, R1, R2
    ADD R0, R4, R1
    LDR R0, R4, #0
    NOT R0, R0
    ADD R0, R0, 1
    ADD R3, R0, R3
    
    BRz RECUR
    AND R4, R4, 0
    STR R4, R5, 3
    BR TEAR
    
END_LOOP
        AND R4, R4, 0
        ADD R4, R4, 1
        STR R4, R5, 3
        BR TEAR
   
RECUR   ADD R6, R6, -2
        ADD R1, R1, 1
        ADD R2, R2, -1
        STR R1, R6, 0
        STR R2, R6, 1
        JSR IS_PALINDROME
        LDR R0, R6, 0
        STR R0, R5, 3
        ADD R6, R6, 3
        
        BR TEAR
        
TEAR    
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