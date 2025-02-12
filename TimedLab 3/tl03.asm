;;=============================================================
;; CS 2110 - Spring 2024
;; Timed Lab 3
;;=============================================================
;; Name: 
;;=============================================================

;; For this Timed Lab, you are asked to implement two subroutines: DIVIDE and MOD.


.orig x3000
    LD R6, STACK
    
    LD R0, A
    LD R1, B
    
    ;; Call DIVIDE
    ADD R6, R6, #-2
    STR R0, R6, 0
    STR R1, R6, 1
    
    JSR DIVIDE
    
    LDR R3, R6, 0
    ADD R6, R6, 2
    ST R3, ANSWER_DIV
    
    ;; Call MOD
    ADD R6, R6, #-2
    STR R0, R6, 0
    STR R1, R6, 1
    
    JSR MOD
    
    LDR R3, R6, 0
    ADD R6, R6, 2
    ST R3, ANSWER_MOD
    
    ; Answer should be found in ANSWER_DIV and ANSWER_MOD
    
    HALT

STACK .fill xF000
A .fill 20 ; Change this number for testing
B .fill 3 ; Change this number for testing
ANSWER_DIV .fill 0
ANSWER_MOD .fill 0

;;  In this file, you must implement the 'DIVIDE' and 'MOD' subroutines.


;; DIVIDE
;;
;; Pseudocode (from PDF):
;; 
;; DIVIDE(int a, int b) {
;;   int count = 0;
;;   while (a >= b) {
;;     a = a - b;
;;     count = count + 1;
;;   }
;;   return count;
;; }
;;

DIVIDE
    ;; YOU CODE HERE
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
    
    LDR R0, R5, 4
    LDR R1, R5, 5
    NOT R3, R1
    ADD R3, R3, 1
AND R2, R2, 0 ;count
WHILE_DIVIDE
    ADD R4, R0, R3
    BRn END_WHILE_DIVIDE

    ADD R0, R0, R3
    ADD R2, R2, 1
    BR WHILE_DIVIDE
END_WHILE_DIVIDE
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



;; MOD
;;
;; Pseudocode (from PDF):
;; 
;; MOD(int a, int b) {
;;   if (a < 0) {
;;     return mod(a + b, b);
;;   }
;;   if (a < b) {
;;     return a;
;;   }
;;   return mod(a - b, b);
;; }
;;

MOD
    ;; YOUR CODE HERE
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
    
    LDR R1, R5, 5
    LDR R0, R5, 4
    
    BRzp IF1
        ADD R6, R6, -2
        ADD R0, R0, R1
        STR R0, R6, 0
        STR R1, R6, 1
        JSR MOD
        LDR R0, R6, 0
        STR R0, R5, 3
        ADD R6, R6, 3
        BR END_MOD

IF1
    NOT R2, R1
    ADD R2, R2, 1
    ADD R2, R0, R2
    BRzp IF2
        STR R0, R5, 3
        ;ADD R6, R6, 1
        BR END_MOD

IF2
    ADD R6, R6, -2
        STR R2, R6, 0
        STR R1, R6, 1
        JSR MOD
        LDR R0, R6, 0
        STR R0, R5, 3
        ADD R6, R6, 3
        BR END_MOD
        
END_MOD
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
