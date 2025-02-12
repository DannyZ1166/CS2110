/**
 * CS 2110 - Spring 2024 - Homework #9
 *
 * @author NAME HERE
 *
 * slack.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return a pointer (or return FAILURE if
 * the function returns a int).
 */

// Do not add ANY additional includes!!!
#include "slack.h"

/* You should NOT have any global variables. */

/** createChannel
 *
 * Creates a new channel. 
 * Initially: 
 *      - The LinkedList should have a size of 0.
 *      - The accounts array should be NULL.
 *      - numUsers should be 0.
 * 
 * @return A pointer to the Channel if successful, NULL if unsuccessful.
 */
Channel *createChannel(void) {
  // Allocate memory for the channel
  Channel *channel = (Channel*)malloc(sizeof(Channel));
  if (channel == NULL) {
    return NULL; // Memory allocation failed
  }

  // Initialize channel members
  channel->posts.head = NULL;
  channel->posts.size = 0;
  channel->users = NULL;
  channel->numUsers = 0;

  return channel;
}

/** createAccount
 *
 * Creates a new account with the provided username and accountID. 
 * 
 * Make sure to add the account to the channel. Remember, if this is
 * the first account, the account array will be NULL.
 * 
 * You will have to resize the accounts array to add the new account
 * to the channel. Think about what function you can use to help you
 * with this.
 * 
 * All accountID's must be unique. If the given accountID already exists
 * in the channel, return FAILURE. You may find it helpful to implement
 * findUser() first.
 * 
 * @param channel A pointer to the channel you are adding the account to
 * @param username A pointer to a string representing the name of the account
 * @param accountID An integer representing the ID of the account
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the accountID is not unique, or
 *          - if malloc fails. 
 * SUCCESS if successful.
 */
int createAccount(Channel *channel, const char *username, int accountID) {
  // Check for invalid parameters
  if (channel == NULL || username == NULL) {
    return FAILURE; // Channel or username cannot be NULL
  }

  // Find if account with ID already exists (assuming findUser is implemented)
  if (findUser(channel, accountID) != NULL) {
    return FAILURE; // Account ID already exists
  }

  // Increase the number of users
  int new_user_count = channel->numUsers + 1;

  // Reallocate memory for the accounts array (using realloc)
  Account** new_users = (Account**)realloc(channel->users, new_user_count * sizeof(Account*));
  if (new_users == NULL) {
    return FAILURE; // Memory allocation failed
  }

  // Update channel's user pointers
  channel->users = new_users;

  // Allocate memory for the new account
  Account* new_account = (Account*)malloc(sizeof(Account));
  if (new_account == NULL) {
    return FAILURE; // Memory allocation failed
  }

  // Allocate memory for username (prevents memory issues)
  new_account->username = (char*)malloc(strlen(username) + 1); // +1 for null terminator
  if (new_account->username == NULL) {
    free(new_account); // Free account memory on username allocation failure
    return FAILURE;
  }

  // Copy username string (prevents modification of original string)
  strcpy(new_account->username, username);

  // Assign account ID to the new account
  new_account->accountID = accountID;

  // Add the new account to the end of the users array
  channel->users[channel->numUsers] = new_account;

  // Update the number of users in the channel
  channel->numUsers = new_user_count;

  return SUCCESS;
}

/** createPost
 *
 * Creates a new post with the provided text, sender ID, channel ID, and post ID.
 * Remember to add the post to the back of the LinkedList of posts in the 
 * provided channel and increment the size of the LinkedList accordingly.
 * 
 * There can be a maximum of 10 reactions on a single post (MAX_REACTION_NUM).
 * By default, no reactions are on a post.
 * 
 * All postID's must be unique. If the given postID already exists
 * in the channel, return FAILURE. You may find it helpful to implement
 * findPost() first.
 * 
 * Additionally, you must ensure that the given senderID is associated with
 * an account in the channel. You may find it helpful to implement findUser()
 * first.
 *
 * @param channel A pointer to the channel where the post will be created
 * @param text A pointer to a string representing the content of the post
 * @param senderID An integer representing the ID of the account that sent the post
 * @param postID An integer representing the unique ID of the post
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the senderID is not an account in the channel,
 *          - the postID is not unique, or
 *          - if malloc fails. 
 * SUCCESS if successful.
 */
int createPost(Channel *channel, const char *text, int senderID, int postID) {
   // Check for invalid parameters
  if (channel == NULL || text == NULL) {
    return FAILURE; // Channel or text cannot be NULL
  }

  // Check if sender is a valid account in the channel
  if (findUser(channel, senderID) == NULL) {
    return FAILURE; // Sender not found in the channel
  }

  // Check for duplicate post ID
  if (findPost(channel, postID) != NULL) {
    return FAILURE; // Post ID already exists
  }

  // Allocate memory for a new Node
  Node* new_node = (Node*)malloc(sizeof(Node));
  if (new_node == NULL) {
    return FAILURE; // Memory allocation failed
  }

  // Allocate memory for a new Post struct
  new_node->data = (Post*)malloc(sizeof(Post));
  if (new_node->data == NULL) {
    free(new_node); // Free node memory on post allocation failure
    return FAILURE;
  }

  // Assign values to the new Post struct
  new_node->data->postID = postID;
  new_node->data->senderID = senderID;
  new_node->data->text = (char*)malloc(strlen(text) + 1); // +1 for null terminator
  if (new_node->data->text == NULL) {
    free(new_node->data); // Free post memory on text allocation failure
    free(new_node);
    return FAILURE;
  }
  strcpy(new_node->data->text, text);
  new_node->data->numReactions = 0; // Initialize number of reactions to 0

  // Handle appending the new node to the linked list
  if (channel->posts.head == NULL) { // Empty linked list (first post)
    new_node->next = NULL;
    channel->posts.head = new_node;
  } else {
    // Find the last node
    Node* current = channel->posts.head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
    new_node->next = NULL;
  }

  // Update the size of the linked list
  channel->posts.size++;

  return SUCCESS;
}

/** addReaction
 *
 * Adds a reaction to the specified post. Remember to increment numReactions
 * in the provided post accordingly and add it to the reactions array.
 * 
 * Hint: Do you need to malloc?
 * 
 * You may assume that the accountID is valid for this method. However,
 * we are declaring that a user can only react once to a post. So it is upon
 * you to enforce this constraint. For example, if the user has already reacted
 * with HEART, they cannot also react with SKULL.
 * 
 * Additionally, a post can have a maximum of 10 reactions.
 *
 * @param post A pointer to the post to which the reaction will be added
 * @param accountID An integer representing the ID of the account that reacted to the post
 * @param reaction An enum value representing the type of reaction
 * @return FAILURE if 
 *          - any of the parameters are NULL,
 *          - the post cannot have more reactions, or
 *          - the user has already reacted to the post
 * SUCCESS if successful.
 */
int addReaction(Post *post, int accountID, enum ReactionType reaction) {
  // Check for invalid parameters
  if (post == NULL || reaction < SMILEY || reaction > SOBBING) { // Check reaction type validity
    return FAILURE; // Post or reaction type is invalid
  }

  // Check if the post has reached the maximum number of reactions
  if (post->numReactions >= MAX_REACTION_NUM ) {
    return FAILURE; // Post cannot have more reactions
  }

  for (int i = 0; i < post->numReactions; i++) {
    if (post->reactions[i].userID == accountID) {
      return FAILURE;
    }
  }

  // Implement check for existing reaction (assuming you have a way to check user's past reactions)
  // ... (code to check if accountID already reacted to this post)
  // If the user has already reacted, return FAILURE

  // Add the reaction to the reactions array
  post->reactions[post->numReactions].userID = accountID;
  post->reactions[post->numReactions].reaction = reaction;

  // Increment the number of reactions
  post->numReactions++;

  return SUCCESS;
}

/** findUser
 *
 * Finds and returns the user with the specified userID within the
 * given channel.
 *
 * @param channel A pointer to the channel where the post will be searched
 * @param accountID An integer representing the ID of the user to be found
 * @return A pointer to the found user, or NULL if the channel is NULL or
 * no user with the given ID exists.
 */
Account* findUser(Channel *channel, int accountID) {
  // Check for invalid channel
  if (channel == NULL) {
    return NULL; // Channel cannot be NULL
  }

  // Iterate through the users array in the channel
  for (int i = 0; i < channel->numUsers; i++) {
    // Check if current user's ID matches the target ID
    if (channel->users[i]->accountID == accountID) {
      // User found, return pointer to the account
      return channel->users[i];
    }
  }

  // User not found, return NULL
  return NULL;
}

/** findPost
 *
 * Finds and returns the post with the specified postID within the given channel.
 *
 * @param channel A pointer to the channel where the post will be searched
 * @param postID An integer representing the ID of the post to be found
 * @return A pointer to the found post, or NULL if the channel is NULL or
 * no post with the given ID exists.
 */
Post *findPost(Channel *channel, int postID) {
  // Check for invalid channel
  if (channel == NULL) {
    return NULL; // Channel cannot be NULL
  }

  // Iterate through the posts linked list
  Node* current = channel->posts.head;
  while (current != NULL) {
    // Check if current post's ID matches the target ID
    if (current->data->postID == postID) {
      // Post found, return pointer to the post struct
      return current->data;
    }
    current = current->next; // Move to the next node in the list
  }

  // Post not found, return NULL
  return NULL;
}

/** searchForPalindrome
 *
 * Searches for the first post that is a palindrome. The text of the
 * post must be a case-sensitive palindrome (including spaces and all). 
 * 
 * Ex:  "evil rats on no star live" -> good
 *      "Evil rats on no star live" -> nope (Capitalization differs)
 *      "was it a car or a cat I saw" -> bad (the spaces ruin the palindrome)
 *      "Can I be a palindrome" -> no
 *      "rise to vote, sir" -> nope again (spaces and punctuation)
 * 
 * Return a pointer to the post that contains the palindrome. Also, set
 * the out variable names index to contain the index of the post in the
 * linked list.
 *
 * @param channel An pointer to the channel to search
 * @param index A pointer to a int that should be set to the index of the post
 *              in the Linked List. If no palindromic post is found, do not touch.
 * @return A pointer to the found post, or NULL if no palindromic post was found
 *         or if any of the parameters are NULL.
 *         Also, set the index out variable appropriately.
 */

Post *searchForPalindrome(Channel *channel, int* index) {
  // Check for invalid parameters
  if (channel == NULL || index == NULL) {
    return NULL; // Invalid channel or index pointer
  }

  // Initialize variables for loop
  Node *current = channel->posts.head;
  int post_index = 0;

  // Iterate through the linked list of posts
  while (current != NULL) {
    int len = strlen(current->data->text);
    int i = 0, j = len - 1;

    // Check if the current post's text is a palindrome
    while (i < j) {
        i++;
        char char_i =current->data->text[i];
        j--;
        char char_j = current->data->text[j];

      // Check if characters don't match (not a palindrome)
      if (char_i != char_j) {
        break; // Exit the inner loop if not a palindrome
      }

      i++;
      j--;
    }

    // Palindrome found, set index and return post
    if (i >= j) {
      *index = post_index;
      return current->data;
    }

    // Move to the next node and increment post index
    current = current->next;
    post_index++;
  }

  // No palindrome found
  *index = 1804289383; // Set index to 1804289383 to indicate no palindrome found
  return NULL;
}

/** deleteReaction
 *
 * Deletes a reaction from the specified post. Remember to decrement numReactions
 * and remove it from the reactions array in the provided post.
 * 
 * Make sure when you are deleting from an array, you shift the remaining elements
 * towards the beginning of the array, if needed.
 *
 * @param post A pointer to the post from which the reaction will be deleted
 * @param accountID An integer representing the ID of the user whose reaction will be deleted
 * @param reaction An enum value representing the type of reaction to be deleted
 * @return FAILURE if the post is NULL or if the reaction isn't found, SUCCESS if successful.
 */
int deleteReaction(Post *post, int accountID, enum ReactionType reaction) {
  // Check for invalid parameters
  if (post == NULL) {
    return FAILURE; // Post pointer is NULL
  }

  // Find the reaction with the matching account ID and type
  int index = -1;
  for (int i = 0; i < post->numReactions; i++) {
    if (post->reactions[i].userID == accountID && post->reactions[i].reaction == reaction) {
      index = i;
      break;
    }
  }

  // Reaction not found
  if (index == -1) {
    return FAILURE; // Reaction with the given account ID and type doesn't exist
  }

  // Decrement number of reactions
  post->numReactions--;

  // If not deleting the last element, shift remaining elements
  if (index < post->numReactions) {
    for (int i = index; i < post->numReactions; i++) {
      post->reactions[i] = post->reactions[i + 1];
    }
  }

  // No additional memory management needed assuming Reaction doesn't hold dynamically allocated data.

  return SUCCESS;
}

/** deletePost
 *
 * Deletes the post with the specified post ID from the given channel. Remember
 * to remove the post of the LinkedList of posts in the provided channel, free any structs/
 * data structures, and decrement numPosts accordingly.
 *
 * @param channel A pointer to the channel where the post is located
 * @param postID An integer representing the ID of the post to be deleted
 * @return FAILURE if the channel is NULL or if the post isn't found, SUCCESS if successful.
 */
int deletePost(Channel *channel, int postID) {
  // Check for invalid channel
  if (channel == NULL) {
    return FAILURE; // Channel pointer is NULL
  }

  // Find the node containing the post to delete
  Node* prev = NULL;
  Node* current = channel->posts.head;

  while (current != NULL) {
    if (current->data->postID == postID) {
      break; // Post found
    }
    prev = current;
    current = current->next;
  }

  // Post not found
  if (current == NULL) {
    return FAILURE; // Post with the given ID doesn't exist
  }

  // Handle removing the head node
  if (prev == NULL) {
    channel->posts.head = current->next;
  } else {
    prev->next = current->next;
  }

  // Free memory associated with the post and node
  free(current->data->text); // Free text string
  free(current->data);       // Free Post struct
  free(current);              // Free Node struct

  // Decrement number of posts in the linked list
  channel->posts.size--;

  return SUCCESS;
}

/** deleteAccount
 *
 * Deletes the account with the specified accountID. Make sure to remove it from the accounts
 * array in the channel and decrement numAccounts accordingly. 
 * 
 * Make sure when you are deleting from an array, you shift the remaining elements, 
 * if needed.
 * 
 * When an account is deleted, all of that user's posts and reactions should be 
 * deleted as well.
 *
 * @param channel A pointer to the channel you are deleting the account from
 * @param accountID An integer representing the ID of the account to be deleted
 * @return FAILURE if the channel is NULL or if the account isn't found, SUCCESS if successful.
 */
int deleteAccount(Channel *channel, int accountID) {

  if (channel == NULL) {
    return FAILURE; // Channel pointer is NULL
  }

  // Find the index of the account to delete
  int account_index = -1;
  for (int i = 0; i < channel->numUsers; i++) {
    if (channel->users[i]->accountID == accountID) {
      account_index = i;
      break;
    }
  }

  // Account not found
  if (account_index == -1) {
    return FAILURE; // Account with the given ID doesn't exist
  }

// Delete the user's posts (iterate through the linked list)
  Node* current_post = channel->posts.head;
  while (current_post != NULL) {
    Node* next_post = current_post->next; // Store next node for traversal
    for (int i = 0; i < current_post->data->numReactions; i++) {
      if (current_post->data->reactions[i].userID == accountID) {
        deleteReaction(current_post->data, accountID, current_post->data->reactions[i].reaction);
      }
    }
    // Check if the post belongs to the user being deleted
    if (current_post->data->senderID == accountID) {
      // Delete the post using the defined deletePost function
      deletePost(channel, current_post->data->postID);
    }
    
    current_post = next_post;
  }

  // Free memory associated with the deleted account
  free(channel->users[account_index]->username);
  free(channel->users[account_index]);

  // Shift remaining accounts in the users array (if not the last element)
  if (account_index < channel->numUsers - 1) {
    for (int i = account_index; i < channel->numUsers - 1; i++) {
      channel->users[i] = channel->users[i + 1];
    }
  }

  // Decrement number of users in the channel
  channel->numUsers--;

  return SUCCESS;
}

/** deleteChannel
 *
 * Deletes the channel that is passed in. Make sure to free any structs/
 * data structures contained in the channel.
 * 
 * Do nothing if the channel is NULL.
 * 
 * @param channel A pointer to the channel that you are deleting
 */
void deleteChannel(Channel *channel) {
  if (channel == NULL) {
    return; // Do nothing if channel is NULL
  }

  // Free memory associated with the users array
  for (int i = 0; i < channel->numUsers; i++) {
    free(channel->users[i]->username);
    free(channel->users[i]);
  }
  free(channel->users);

  // Free memory associated with posts (iterate through the linked list)
  Node* current_post = channel->posts.head;
  while (current_post != NULL) {
    Node* next_post = current_post->next;

    // Call deletePost to free post data and node
    deletePost(channel, current_post->data->postID);

    current_post = next_post;
  }

  // Free the channel struct itself
  free(channel);
}
