/**
 * @file my_string.c
 * @author YOUR NAME HERE
 * @brief Your implementation of the famous string.h library functions!
 *
 * NOTE: NO ARRAY NOTATION IS ALLOWED IN THIS FILE
 *
 * @date 2024-03-xx
 */

#include "my_string.h"

/* Note about UNUSED_PARAM
*
* UNUSED_PARAM is used to avoid compiler warnings and errors regarding unused function
* parameters prior to implementing the function. Once you begin implementing this
* function, you can delete the UNUSED_PARAM lines.
*/

/**
 * @brief Calculate the length of a string
 *
 * @param s a constant C string
 * @return size_t the number of characters in the passed in string
 */
size_t my_strlen(const char *s)
{
    UNUSED_PARAM(s);

    size_t count = 0;
    int i = 0;
    while (*(s + i) != '\0') {
        count = count + 1;
        i = i + 1;
    }
    return count;
}

/**
 * @brief Compare two strings
 *
 * @param s1 First string to be compared
 * @param s2 Second string to be compared
 * @param n First (at most) n bytes to be compared
 * @return int representing the difference between the strings:
 *          - 0 if the strings are equal
 *          - arbitrary positive number if s1 > s2
 *          - arbitrary negative number if s1 < s2
 */
int my_strncmp(const char *s1, const char *s2, size_t n)
{
    UNUSED_PARAM(s1);
    UNUSED_PARAM(s2);
    UNUSED_PARAM(n);

if (s1 == NULL || s2 == NULL) {
    return -1; // Handle null pointers (common practice for string functions)
  }

  for (size_t i = 0; i < n; i++) {
    char ch1 = *s1++; 
    char ch2 = *s2++; 

    if (ch1 == '\0' || ch2 == '\0') { 
      return ch1 - ch2; 
    }

    if (ch1 != ch2) {
      return ch1 - ch2; 
    }
  }

  return 0;
}

/**
 * @brief Copy a string
 *
 * @param dest The destination buffer
 * @param src The source to copy from
 * @param n maximum number of bytes to copy
 * @return char* a pointer same as dest
 */
char *my_strncpy(char *dest, const char *src, size_t n)
{
    UNUSED_PARAM(dest);
    UNUSED_PARAM(src);
    UNUSED_PARAM(n);

    size_t i = 0;
    while ((i < n) && (*(src + i) != '\0')) {
        *(dest + i) = *(src + i);
        i++;
    }
    while (i < n) {
        *(dest + i) = '\0';
        i++;
    }
    return dest;
}

/**
 * @brief Concatenates two strings and stores the result
 * in the destination string
 *
 * @param dest The destination string
 * @param src The source string
 * @param n The maximum number of bytes (or characters) from src to concatenate
 * @return char* a pointer same as dest
 */
char *my_strncat(char *dest, const char *src, size_t n)
{
    UNUSED_PARAM(dest);
    UNUSED_PARAM(src);
    UNUSED_PARAM(n);
    size_t	i = my_strlen(dest);
    size_t	count = 0;

    while (*(src + count) && (count < n)) {
        *(dest + (i + count)) = *(src + count);
        count = count + 1;
    }
    *(dest + (i + count)) = '\0';
    return (dest);

}

/**
 * @brief Copies the character c into the first n
 * bytes of memory starting at *str
 *
 * @param str The pointer to the block of memory to fill
 * @param c The character to fill in memory
 * @param n The number of bytes of memory to fill
 * @return char* a pointer same as str
 */
void *my_memset(void *str, int c, size_t n)
{
    UNUSED_PARAM(str);
    UNUSED_PARAM(c);
    UNUSED_PARAM(n);
    unsigned char val = (unsigned char)c;
    for (size_t i = 0; i < n; i++) {
        *((unsigned char*)str + i) = val;
    }
    return(str);

}

/**
 * @brief Checks whether the string is a palindrome
 * (i.e., reads the same forwards and backwards)
 * assuming that the case of letters is irrelevant.
 * 
 * @param str The pointer to the string
 * @return 1 if the string is a palindrome,
 * or 0 if the string is not
*/
int is_palindrome_ignore_case(const char *str) 
{
    UNUSED_PARAM(str);
if (str == NULL) {
    return 0; 
  }

  int len = my_strlen(str);
  int left = 0, right = len - 1;

  while (left < right) {
    char ch1 = (*(str + left));  
    char ch2 = (*(str + right)); 
    if ((ch1 >= 'A') && (ch1 <= 'Z')) {
      ch1 = ch1 - 'A' + 'a'; 
    }
    if ((ch2 >= 'A') && (ch2 <= 'Z')) {
      ch2 = ch2 - 'A' + 'a'; 
    }

    if (ch1 != ch2) {
      return 0; 
    }

    left++;
    right--;
  }

  return 1;

}

/**
 * @brief Apply a Caesar shift to each character
 * of the provided string in place.
 * 
 * @param str The pointer to the string
 * @param shift The amount to shift by
*/
void caesar_shift(char *str, int shift) 
{
    UNUSED_PARAM(str);
    UNUSED_PARAM(shift);
if (str == NULL || shift == 0) {
    return; 
  }

  int len = my_strlen(str);
  for (int i = 0; i < len; i++) {
    char ch = *(str + i);
    if ((ch >= 'A') && (ch <= 'Z')) {
      ch = (ch - 'A' + shift) % 26 + 'A';
    } else if ((ch >= 'a') && (ch <= 'z')) {
      ch = (ch - 'a' + shift) % 26 + 'a';
    }
    *(str + i) = ch;
  }
}

/**
 * @brief Mutate the string in-place to
 * remove duplicate characters that appear
 * consecutively in the string.
 * 
 * @param str The pointer to the string
*/
void deduplicate_str(char *str) 
{
    UNUSED_PARAM(str);
    if (str == NULL) {
        return; 
    }

    char *write_ptr = str;
    char prev = '\0'; 

    while (*str != '\0') {
        char current = *str;

        if (current != prev) {
            *write_ptr++ = current; 
            prev = current;              
        }

        str++; 
    }

    *write_ptr = '\0'; 
}

/**
 * @brief Swap the position of
 * two strings in memory.
 * 
 * @param s1 The first string
 * @param s2 The second string
*/
void swap_strings(char **s1, char **s2) 
{
    UNUSED_PARAM(s1);
    UNUSED_PARAM(s2);
    char *temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}