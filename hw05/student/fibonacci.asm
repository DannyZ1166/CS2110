;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - Fibonacci
;;=============================================================
;; Name: 
;;=============================================================


;; Suggested Pseudocode (see PDF for explanation)
;;
;; n == R1, resAddr == R2
;; n = mem[N];
;; resAddr = mem[RESULT]
;; 
;; if (n == 1) {
;;     mem[resAddr] = 0;
;; } else if (n > 1) {
;;     mem[resAddr] = 0;
;;     mem[resAddr + 1] = 1;
;;     for (i = 2; i < n; i++) {
;;         x = mem[resAddr + i - 1];
;;         y = mem[resAddr + i - 2];
;;         mem[resAddr + i] = x + y;
;;     }
;; }

.orig x3000

LD R1, N
LD R2, RESULT

AND R3, R1, R1
AND R4, R4, #0
ADD R3, R3, #-1
BRZ ONE
BRP CONT
BR DONE

ONE             STR R4, R2, #0
                BR DONE


CONT            STR R4, R2, #0
                ADD R2, R2, #1
                ADD R4, R4, #1
                STR R4, R2, #0
                ADD R2, R2, #1
                AND R4, R4, #0
                ADD R4, R4, #-2
                ADD R5, R1, R4
                BRP FOR

FOR     
                ADD R6, R2, #-1
                LDR R7, R6, #-1
                LDR R6, R6, #0
                ADD R5, R6, R7
                STR R5, R2, #0
                ADD R2, R2, #1
                ADD R4, R4, #-1
                ADD R5, R1, R4
                BRP FOR
            

DONE        
    ;; YOUR CODE HERE
HALT

;; Do not rename or remove any existing labels
;; You may change the value of N for debugging
N       .fill 100
RESULT  .fill x4000
.end

.orig x4000
.blkw 100
.end