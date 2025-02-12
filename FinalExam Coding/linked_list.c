#include "linked_list.h"

/** insert_at_end -----------------------------------------------------------------------------
 * @brief Create a new Node, initialize its data with the value parameter,
 *        and add the node to the end of the linked list.
 *
 * Implementation constraints:
 * - If the list argument is NULL, this function should return FAILURE.
 * - If malloc returns NULL, this function should return FAILURE and leave the list unchanged.
 * - If list->head is NULL, this function should add the new node as 
 *       the only element in the list and return SUCCESS.
 * - No additional header files are needed. All required header files 
 *       are provided in linked_list.h.
 *
 * @example list: [1, 2], value: 3
 *    if no malloc failure:
 *        list-after: [1, 2, 3], return SUCCESS
 *    --- or ---
 *    if malloc failure:
 *        list-after: [1, 2], return FAILURE
 *
 * @example list: [], value: 7
 *    if no malloc failure:
 *        list-after: [7], return SUCCESS
 *    --- or ---
 *    if malloc failure:
 *        list-after: [], return FAILURE
 *
 * @example list: NULL, value: 1
 *    return FAILURE
 *
 * @param list Pointer to the linked list structure in which value is to be inserted.
 *             The first list node is pointed to by list->head.
 * @param value The data value for the node to be inserted.
 * @return SUCCESS (0) upon successful insertion or FAILURE (-1) otherwise
 * --------------------------------------------------------------------------------------------
*/
int insert_at_end(LinkedList *list, int value) {
  if (list == NULL) {
    return FAILURE;
  }
  Node* new_node = (Node*)malloc(sizeof(Node));
  if (new_node == NULL) {
    return FAILURE;
  }
  new_node->data = value;
  new_node->next = NULL;  
  if (list->head == NULL) {
    list->head = new_node;
    return SUCCESS;
  }
  Node* current = list->head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_node;
  return SUCCESS;
}

/** get_element_at ----------------------------------------------------------------------------
 * @brief Finds the element at the provided index (under zero-indexing).
 *
 * Implementation constraints:
 * - If the list argument is NULL, this function should return FAILURE.
 * - If the index argument is out of bounds, this function should also return FAILURE.
 * - No additional header files are needed. All required header files 
 *       are provided in linked_list.h.
 *
 * @example get_element_at([1, 3, 4, 5, 6, 9], 0) == 1
 * @example get_element_at([1, 3, 4, 5, 6, 9], 4) == 6
 * @example get_element_at([1, 3, 4, 5, 6, 9], 9) == FAILURE
 * @example get_element_at(NULL, 0) == FAILURE
 * 
 * @param list Pointer to the linked list structure to search.
 * @param index The index to search (under zero-indexing).
 * @return the value at that index, or FAILURE (-1) if any above error occurs
 * --------------------------------------------------------------------------------------------
*/
int get_element_at(LinkedList *list, int index) {
  if (list == NULL || index < 0) {
    return FAILURE;
  }
  Node* current = list->head;
  int counter = 0;
  while (current != NULL && counter < index) {
    current = current->next;
    counter++;
  }
  if (current == NULL) {
    return FAILURE;
  }
  return current->data;
}