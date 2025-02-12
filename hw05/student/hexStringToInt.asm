;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - hexStringToInt
;;=============================================================
;; Name: 
;;=============================================================

;;  Suggested Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String hexString = "F1ED";
;;  int length = mem[LENGTH];
;;  int value = 0;
;;  int i = 0;
;;  while (i < length) {
;;      int leftShifts = 4;
;;      while (leftShifts > 0) {
;;          value += value;
;;          leftShifts--;
;;      }
;;      if (hexString[i] >= 65) {
;;          value += hexString[i] - 55;
;;      } else {
;;          value += hexString[i] - 48;
;;      }
;;      i++;
;;  }
;;  mem[mem[RESULTADDR]] = value;

.orig x3000
LD R0, HEXSTRING    ; R0 -> base address of hex string
LD R1, LENGTH       ; R1 -> length of string (always 4)
AND R4, R4, #0      ; R4 -> i

; Initialization of value
AND R6, R6, #0       ; R6 -> will hold the numerical value (clear initial value) 

LOOP
    AND R3, R3, #0      ;;      int leftShifts = 4;
    ADD R3, R3, #4
    BR LEFT

LEFT    ADD R6, R6, R6
        ADD R3, R3, #-1     
        BRP LEFT

    ADD R7, R0, R4  ; R7 -> Address of the current character in the string
    LDR R7, R7, #0
    LD R5, SIXTYFIVE    ;R5 -> temp
    NOT R5, R5
    ADD R5, R5, #1
    ADD R5, R5, R7
    BRZP FIFTYFIVE
    LD R2, ASCIIDIG     ;R2 -> temp
    NOT R2, R2
    ADD R2, R2, #1
    ADD R7, R7, R2
    ADD R6, R6, R7
    BR CONTINUE
    
FIFTYFIVE   LD R2, ASCIICHAR     ;R2 -> temp
            NOT R2, R2
            ADD R2, R2, #1
            ADD R7, R7, R2
            ADD R6, R6, R7

CONTINUE
    ADD R4, R4, #1   ; Increment hex string pointer 
    ADD R1, R1, #-1  ; Decrement length counter
    BRp LOOP         ; Continue if length is not zero

; Store the result
STI R6, RESULTADDR   ; Store the result at the address indicated by RESULTADDR

    HALT
    
;; Do not change these values!
ASCIIDIG        .fill 48
ASCIICHAR       .fill 55
SIXTYFIVE       .fill 65
HEXSTRING       .fill x5000
LENGTH          .fill 4
RESULTADDR      .fill x4000
.end

.orig x4000                    ;;Don't change the .orig statement
    ANSWER .blkw 1             ;;Do not change this value
.end


.orig x5000                    ;;  Don't change the .orig statement
    .stringz "211F"            ;;  You can change this string for debugging! Hex characters will be uppercase.
.end
