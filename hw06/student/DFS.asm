;;=============================================================
;;  CS 2110 - Spring 2024
;;  Homework 6 - DFS
;;=============================================================
;;  Name: 
;;============================================================

;;  In this file, you must implement the 'SET_VISITED', 'IS_VISITED', and 'DFS' subroutines.


.orig x3000
    ;; You do not need to write anything here
    LD R6, STACK_PTR

    ;; Pushes arguments (address of node 1, target node 5)
    ADD R6, R6, -1
    AND R1, R1, 0
    ADD R1, R1, 5
    STR R1, R6, 0
    ADD R6, R6, -1
    LD R1, STARTING_NODE_ADDRESS
    STR R1, R6, 0
    JSR DFS
    LDR R0, R6, 0
    ADD R6, R6, 3
    HALT

    STACK_PTR .fill xF000
    STARTING_NODE_ADDRESS .fill x6110
    VISITED_VECTOR_ADDR .fill x4199 ;; stores the address of the visited vector.

;;  SET_VISITED Pseudocode

;; Parameter: The address of the node
;; Updates the visited vector to mark the given node as visited

;;  SET_VISITED(addr node) {
;;      visited = mem[mem[VISITED_VECTOR_ADDR]];
;;      data = mem[node];
;;      mask = 1;
;;      while (data > 0) {
;;          mask = mask + mask;
;;          data--;
;;      }
;;      mem[mem[VISITED_VECTOR_ADDR]] = (visited | mask); //Hint: Use DeMorgan's Law!
;;  }

SET_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the SET_VISITED subroutine here!
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

LDI R2, VISITED_VECTOR_ADDR
LDR R3, R1, 0
AND R4, R4, 0
ADD R4, R4, 1

ADD R3, R3, 0
BRnz SET_ND
SET_WHILE   ADD R4, R4, R4
            ADD R3, R3, -1
            BRp SET_WHILE

SET_ND      NOT R2, R2
            NOT R4, R4
            AND R2, R4, R2
            NOT R2, R2
            STI R2, VISITED_VECTOR_ADDR
            
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


;;  IS_VISITED Pseudocode

;; Parameter: The address of the node
;; Returns: 1 if the node has been visited, 0 if it has not been visited

;;  IS_VISITED(addr node) {
;;       visited = mem[mem[VISITED_VECTOR_ADDR]];
;;       data = mem[node];
;;       mask = 1;
;;       while (data > 0) {
;;           mask = mask + mask;
;;           data--;
;;       }
;;       return (visited & mask) != 0;
;;   }

IS_VISITED ;; Do not change this label! Treat this as like the name of the function in a function header
;; Code your implementation for the IS_VISITED subroutine here!
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

LDI R2, VISITED_VECTOR_ADDR
AND R4, R4, 0
ADD R4, R4, 1
LDR R3, R1, 0

BRnz IS_ND
IS_WHILE    ADD R4, R4, R4
            ADD R3, R3, -1
            BRp IS_WHILE

IS_ND       AND R4, R2, R4
            BRz NOT_VISIT
            AND R2, R2, 0
            ADD R2, R2, 1
            STR R2, R5, 3
            BR IS_EN


NOT_VISIT   AND R2, R2, 0
            STR R2, R5, 3
        
IS_EN
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



;;  DFS Pseudocode (see PDF for explanation and examples)

;; Parameters: The address of the starting (or current) node, the data of the target node
;; Returns: the address of the node (if the node is found), 0 if the node is not found

;;  DFS(addr node, int target) {
;;        SET_VISITED(node);
;;        if (mem[node] == target) {
;;           return node;
;;        }
;;        result = 0;
;;        for (i = node + 1; mem[i] != 0 && result == 0; i++) {
;;            if (! IS_VISITED(mem[i])) {
;;                result = DFS(mem[i], target);
;;            }
;;        }       
;;        return result;
;;  }

DFS ;; Do not change this label! Treat this as like the name of the function in a function header
    ;; Code your implementation for the DFS subroutine here!
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
    
    ADD R6, R6, -1
    STR R0, R6, 0
    JSR SET_VISITED
    ADD R6, R6, 1
    
    LDR R2, R0, 0
    NOT R2, R2
    ADD R2, R2, 1
    ADD R2, R2, R1
    BRnp NOT_NODE
    STR R0, R5, 3
    BR DFS_FIN
    
NOT_NODE    AND R2, R2, 0
            ADD R3, R0, 1
FOR         LDR R4, R3, 0
            BRz END_FOR
            ADD R2, R2, 0
            BRnp END_FOR
            
            ADD R6, R6, -1
            STR R4, R6, 0
            JSR IS_VISITED
            LDR R0, R6, 0
            ADD R6, R6, 2
            
            ADD R0, R0, 0
            BRnp VISITED
            
            ADD R6, R6, -2
            STR R4, R6, 0
            STR R1, R6, 1
            JSR DFS
            LDR R2, R6, 0
            ADD R6, R6, 3
VISITED            
            ADD R3, R3, 1
            BR FOR
END_FOR    STR R2, R5, 3
        
DFS_FIN LDR R4, R5, -5      ;Restore R0
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

;; Assuming the graphs starting node (1) is at address x6100, here's how the graph (see below and in the PDF) is represented in memory
;;
;;         0      3
;;          \   / | \
;;            4   1 - 2 
;;             \ /    |
;;              5  -  6
;;

.orig x4199
    .fill 0 ;; visited set will be at address x4199, initialized to 0
.end

.orig x6110         ;; node 1 itself lives here at x6110
    .fill 1         ;; node.data (1)
    .fill x6120     ;; node 2 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address   
    .fill 0
.end

.orig x6120	        ;; node 2 itself lives here at x6120
    .fill 2         ;; node.data (2)
    .fill x6110     ;; node 1 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6130	        ;; node 3 itself lives here at x6130
    .fill 3         ;; node.data (3)
    .fill x6110     ;; node 1 lives at this address
    .fill x6120     ;; node 2 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6140	        ;; node 4 itself lives here at x6140
    .fill 4         ;; node.data (4)
    .fill x6100     ;; node 0 lives at this address
    .fill x6130     ;; node 3 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end

.orig x6100         ;; node 0 itself lives here at x6000
    .fill 0         ;; node.data (0)
    .fill x6140     ;; node 4 lives at this address
    .fill 0
.end

.orig x6150	        ;; node 5 itself lives here at x6150
    .fill 5         ;; node.data (5)
    .fill x6110     ;; node 1 lives at this address
    .fill x6140     ;; node 4 lives at this address
    .fill x6160     ;; node 6 lives at this address
    .fill 0
.end

.orig x6160	        ;; node 6 itself lives here at x6160
    .fill 6         ;; node.data (6)
    .fill x6120     ;; node 2 lives at this address
    .fill x6150     ;; node 5 lives at this address
    .fill 0
.end
 


