#include "tl04.h"
#include <stdio.h>

ArrayList *students;
int initial_capacity = 5;

/*
 * @brief Initialize the ArrayList with the initial capacity
 * DO NOT MODIFY THIS FUNCTION -- (for reference purposes only - will not affect autograder)
 */ 
int initialize_arraylist(void)
{
    students = malloc(sizeof(ArrayList));
    if (students == NULL)
    {
        return FAILURE;
    }
    students->backing_array = malloc(sizeof(Student) * initial_capacity);
    if (students->backing_array == NULL)
    {
        free(students);
        return FAILURE;
    }
    students->current_size = 0;
    students->capacity = initial_capacity;
    return SUCCESS;
}

/*
 * @brief Create a student with the given name and gpa
 *
 * @param output where the student will be stored [out variable]
 * @param name The name of the student
 * @param gpa The gpa of the student
 * @return int representing if the Student's creation was successful
 * 	- SUCCESS if the student was created successfully
 * 	- FAILURE if any parameters are invalid or malloc fails
 */
int create_student(Student *output, const char *name, const float gpa) {
  if (output == NULL || name == NULL) {
    return FAILURE; 
  }

  if (gpa < 0 || gpa > 4)  {
    return FAILURE; 
  }

  Student* new_student = (Student*)malloc(sizeof(Student));
  if (new_student == NULL) {
    return FAILURE; 
  }

  new_student->name = malloc(strlen(name) + 1);
  if (new_student->name == NULL) {
    free(new_student); 
    return FAILURE; 
  }
  strcpy(new_student->name, name);

  new_student->gpa = gpa;

  *output = *new_student;  

  free(new_student);

  return SUCCESS;
}



/*
 * @brief Add the student with the given parameters to the front of the Arraylist, ensuring there is enough space in the backing array
 * Usage of create_student() is recommended, but not required.
 *
 * @param name The name of the student
 * @param gpa The gpa of the student
 * @return int representing if the Student was added successfully
 * 	- SUCCESS if the student was added successfully
 * 	- FAILURE if any parameters are invalid, malloc fails or the students arraylist is NULL
 */
int add_student_to_front(const char *name, const float gpa) {
  if (students == NULL || name == NULL) {
    return FAILURE;  
  }
  if (gpa < 0 || gpa > 4.0) {
    return FAILURE;
  }

  if (students->current_size >= students->capacity) {

    int new_capacity = students->capacity + 1; 
    Student* new_backing_array = (Student*)realloc(students->backing_array, sizeof(Student) * new_capacity);
    if (new_backing_array == NULL) {
      return FAILURE; 
    }
    students->backing_array = new_backing_array;
    students->capacity = new_capacity;
  }


  for (int i = students->current_size - 1; i >= 0; i--) {
    students->backing_array[i + 1] = students->backing_array[i];
  }

  int create_result = create_student(&students->backing_array[0], name, gpa);
  if (create_result == FAILURE) {
    return FAILURE; 
  }
  students->current_size++;
  return SUCCESS;
  
}



/*
 * @brief Remove a student from the Arraylist and leaving a blank space at the end
 * @param name the name of the student to remove
 * @return int representing if the Student was removed successfully
 * 	- SUCCESS if the student was removed successfully
 * 	- FAILURE if any parameters are invalid, the student is not found or the students arraylist is NULL
 */
int remove_student_by_name(const char *name) {
  if (students == NULL || name == NULL) {
    return FAILURE;
  }

  int student_index = -1;
  for (int i = 0; i < students->current_size; i++) {
    if (strcmp(students->backing_array[i].name, name) == 0) {
      student_index = i;
      break;
    }
  }


  if (student_index == -1) {
    return FAILURE; 
  }

 
  free(students->backing_array[student_index].name);


  for (int i = student_index; i < students->current_size - 1; i++) {
    students->backing_array[i] = students->backing_array[i + 1];
  }

  students->current_size--;

  return SUCCESS;
}