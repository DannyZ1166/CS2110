;;=============================================================
;; CS 2110 - Spring 2024
;; Homework 5 - palindromeWithSkips
;;=============================================================
;; Name:
;;=============================================================

;;  NOTE: Let's decide to represent "true" as a 1 in memory and "false" as a 0 in memory.
;;
;;  Pseudocode (see PDF for explanation)
;;  Pseudocode values are based on the labels' default values.
;;
;;  String str = "aibohphobia";
;;  char skipChar = mem[mem[CHARADDR]];
;;  int length = 0;
;;  while (str[length] != '\0') {
;;		length++;
;;	}
;; 	
;;	int left = 0;
;;  int right = length - 1;
;;  boolean isPalindrome = true;
;;  while(left < right) {
;;      if (str[left] == skipChar) {
;;          left++;
;;          continue;
;;      }
;;      if (str[right] == skipChar) {
;;          right--;
;;          continue;
;;      }
;;		if (str[left] != str[right]) {
;;			isPalindrome = false;
;;          break;
;;		}
;;
;;		left++;
;;		right--;
;;	}
;;	mem[mem[ANSWERADDR]] = isPalindrome;

.orig x3000
	; Load addresses and skip character
LD R0, STRING          ; R0 -> base address of string
LDI R1, CHARADDR        ; R1 -> address of skip character
NOT R1, R1
ADD R1, R1, #1

; Calculate string length
LD R2, STRING           ; R2 -> current string position
LENGTH_LOOP
    LDR R3, R2, #0      ; R3 -> load current character
    BRz END_LENGTH      ; End of string if NULL (zero)
    ADD R2, R2, #1      ; Increment string position
    BR LENGTH_LOOP   ; Continue loop
END_LENGTH
    NOT R7, R0
    ADD R7, R7, #1
    ADD R2, R7, R2
    ADD R2, R2, #-1     ; R2 -> length -1

; Check for palindrome
AND R3, R3, #0
ADD R3, R3, #1          ; R3 -> boolean flag (initially true)
AND R4, R4, #0          ; R4 -> left index (start of string)
ADD R5, R2, #0          ; R5 -> right index (end of string)

PALINDROME_LOOP
    NOT R7, R5
    ADD R7, R7, #1
    ADD R7, R7, R4
    BRzp DONE
    
    ADD R6, R0, R4      ; R6 -> address of left character
    LDR R6, R6, #0      ; R6 -> left character

    ADD R6, R6, R1      ; Check if left character is the skip character
    BRz SKIP_LEFT       ; If yes, skip it

    ADD R7, R0, R5      ; R7 -> address of right character
    LDR R7, R7, #0      ; R7 -> right character

    ADD R7, R7, R1      ; Check if right character is the skip character
    BRz SKIP_RIGHT      ; If yes, skip it

    NOT R7, R7
    ADD R7, R7, #1
    ADD R7, R7, R6          ; Compare left and right characters
    BRnp NOT_PALINDROME ; If not equal, not a palindrome

    ; Move indices inward 
    ADD R4, R4, #1      
    ADD R5, R5, #-1    
    BR PALINDROME_LOOP

NOT_PALINDROME
    AND R3, R3, #0     ; Set boolean flag to false
    BR DONE

SKIP_LEFT   
    ADD R4, R4, #1     ; Move left index forward
    BR PALINDROME_LOOP ; Continue loop

SKIP_RIGHT
    ADD R5, R5, #-1     ; Move right index backward
    BR PALINDROME_LOOP  ; Continue loop

DONE
; Store result
STI R3, ANSWERADDR
	HALT

;; Do not change these values!
CHARADDR    .fill x4004
STRING	    .fill x4019
ANSWERADDR 	.fill x5005
.end

;; Do not change any of the .orig lines!

.orig x4004
    .stringz "c"           ;;Feel free to change this char for debugging
.end

.orig x4019				   
	.stringz "cccccccc" ;; Feel free to change this string for debugging.
.end

.orig x5005
	ANSWER  .blkw 1        ;;Do not change this value
.end