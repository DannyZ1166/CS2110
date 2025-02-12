;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - uppercaseInRange 
;;=============================================================
;; Name: 
;;=============================================================

;;  Suggested Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String str = "touppERcase";
;;  int start = mem[START];
;;  int end = mem[END];
;;  int length = mem[LENGTH];
;;  if (end > length) {
;;      end = length;
;;  }
;;
;;  for (int x = start; x < end; x++) {
;;      if (str[x] >= 'a') {
;;          str[x] = str[x] - 32;
;;      }
;;  }


.orig x3000
    ;; YOUR CODE HERE

; Load starting addresses and values
LD R0, STRING      ; R0 holds the base address of the string
LD R1, START       ; R1 holds the START index
LD R2, END         ; R2 holds the END index
LD R3, LENGTH      ; R3 holds the length of the string
LD R4, ASCII_A     ; R4 holds the negative ASCII value of lowercase 'a'
NOT R4, R4
ADD R4, R4, #1

; Handle END index exceeding string length
NOT R5, R2
ADD R5, R5, #1
ADD R5, R5, R3
BRzp END_VALID
ADD R2, R3, #0
BR END_VALID      ; If END is within bounds, skip adjustment


END_VALID
NOT R5, R1
ADD R5, R5, #1
ADD R2, R2, R5
ADD R2, R2, #1
; Loop through characters in the specified range
LOOP
    
    ADD R5, R0, R1      ; Calculate current character address
    LDR R6, R5, #0      ; Load current character
    ADD R2, R2, #-1      
    BRz DONE            ; If end of string reached, exit

    ; Check for lowercase character
    ADD R7, R4, R6       ; Compare character with ASCII 'a'
    BRn NEXT_CHAR     ; If not lowercase, skip conversion
    ADD R6, R6, #-16
    ADD R6, R6, #-16    ; Convert to uppercase (subtract 32)
    STR R6, R5, #0       ; Store uppercase character back

NEXT_CHAR
    ADD R1, R1, #1    ; Increment index
    BR LOOP        ; Continue the loop



DONE
    
    HALT

;; Do not change these values!
STRING          .fill x5000
ASCII_A         .fill 97

;; You can change these numbers for debugging!
LENGTH          .fill 43
START           .fill 14
END             .fill 25 

.end

.orig x5000                    ;;  Don't change the .orig statement
    .stringz "the qUiCk bRoWn foX jUmPs oVeR tHe lAzy doG"     ;;  You can change this string for debugging!
.end
