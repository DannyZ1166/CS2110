/**
 * @file pkmn_gym.c
 * @author Danny
 *
 * @date 2024-03-xx
 */

// DO NOT MODIFY THE INCLUDE(S) LIST
#include "pkmn_gym.h"

// DO NOT MODIFY THE GLOBAL VARIABLE(S) LIST
struct Gym gym;

// ALWAYS check the validity of the passed in arguments
// (i.e. not NULL, in the correct range, not too long)

// If any of the parameters are invalid, the function should
// do nothing and return FAILURE.



/** register_trainer
 * @brief Add a new trainer to the gym with the provided parameters.
 * @param name the name of the trainer
 * @return FAILURE if parameters invalid or the gym is full, SUCCESS otherwise
*/
int register_trainer(const char *name) {
UNUSED_PARAM(name);
  if (name == NULL) {
    return FAILURE; // Invalid parameters
  }

  int len = my_strlen(name);
  if (len >= MAX_NAME_LENGTH) {
    return FAILURE; // Name cannot be TOO LONG
  }

  // Check if gym is full
  if (gym.num_trainers == MAX_TRAINER_LENGTH) {
    return FAILURE; // Gym is full
  }

  // Check for duplicate name
  for (int i = 0; i < gym.num_trainers; i++) {
    if (my_strncmp(gym.trainers[i].name, name, MAX_NAME_LENGTH) == 0) {
      return FAILURE; // Duplicate name found
    }
  }

  // Add new trainer data to the end of the trainers array
  my_strncpy(gym.trainers[gym.num_trainers].name, name, MAX_NAME_LENGTH - 1);
  // ... initialize other trainer data ...
  gym.num_trainers++;

  return SUCCESS;
    
}

/** unregister_trainer
 * @brief Removes a trainer from the gym.
 * @param name the name of the trainer
 * @return FAILURE if parameters invalid or the trainer is not found, SUCCESS otherwise
*/
int unregister_trainer(const char *name) {
    UNUSED_PARAM(name);
      if (name == NULL) {
    return FAILURE; // Invalid parameters
  }

  int len = my_strlen(name);
  if (len >= MAX_NAME_LENGTH) {
    return FAILURE; // Name cannot be TOO LONG
  }

  // Find the trainer with the matching name
  int found_index = -1;
  for (int i = 0; i < gym.num_trainers; i++) {
    if (my_strncmp(gym.trainers[i].name, name, MAX_NAME_LENGTH) == 0) {
      found_index = i;
      break;
    }
  }

  // If no trainer found, return failure
  if (found_index == -1) {
    return FAILURE;
  }

  // If it's the last trainer, simply decrement num_trainers
  if (gym.num_trainers == 1) {
    gym.num_trainers = 0;
    return SUCCESS;
  }

  // Trainer found, need to shift remaining trainers to remove the gap
  for (int i = found_index; i < gym.num_trainers - 1; i++) {
    gym.trainers[i] = gym.trainers[i + 1];
  }
  gym.num_trainers -= 1;
    return SUCCESS;
}

/** catch_pokemon
 * @brief Creates a new Pokémon and adds it to
 * the trainer's party.
 * @param trainer pointer to the trainer who caught this Pokémon
 * @param species the species of the Pokémon caught
 * @param level the level of the Pokémon caught
 * @return FAILURE if parameters invalid or the trainer's party is full, SUCCESS otherwise
*/
int catch_pokemon(struct Trainer *trainer, const char *species, int level) {
    UNUSED_PARAM(trainer);
    UNUSED_PARAM(species);
    UNUSED_PARAM(level);
    if (trainer == NULL || species == NULL) {
    return FAILURE; // Invalid parameters
  }

  int len = my_strlen(*species);
  if (len >= MAX_NAME_LENGTH) {
    return FAILURE; // Species name too long
  }

  // Check level validity
  if (level < 1 || level > 100) {
    return FAILURE; // Invalid level
  }

  // Check if trainer's party is full (simulated by party_size)
  if (trainer->party_size == MAX_PARTY_LENGTH) {
    return FAILURE; // Party full
  }
  // Copy the name (avoid exceeding max name length)
  my_strncpy(&trainer->party[trainer->party_size].species, species, MAX_NAME_LENGTH - 1);
  // Simulate catching by incrementing party_size (or setting a boolean flag if preferred)
  trainer->party_size++;

  return SUCCESS;
}

/** release_pokemon
 * @brief Removes a Pokémon from a trainer's party.
 * @param trainer pointer to the trainer who is releasing a Pokémon
 * @param species the species of the Pokémon to release
 * @return FAILURE if parameters invalid or the Pokémon is not found in the trainer's party, SUCCESS otherwise
*/
int release_pokemon(struct Trainer *trainer, const char *species) {
    UNUSED_PARAM(trainer);
    UNUSED_PARAM(species);
    if (trainer == NULL || species == NULL) {
    return FAILURE; // Invalid parameters
  }

  int len = my_strlen(species);
  if (len >= MAX_NAME_LENGTH) {
    return FAILURE; // Species name too long
  }

  int found_index = -1;  // Index of the Pokemon to be removed (initially -1)

  // Find the Pokemon with the matching species
  for (int i = 0; i < trainer->party_size; i++) {
    if (my_strncmp(trainer->party[i].species, species, MAX_NAME_LENGTH - 1) == 0) {
      found_index = i;
      break;
    }
  }

  // If no Pokemon found, return failure
  if (found_index == -1) {
    return FAILURE;
  }

  // If it's the last Pokemon, simply decrement party_size
  if (trainer->party_size == 1) {
    trainer->party_size = 0;
    return SUCCESS;
  }

  // Pokemon found, need to shift remaining Pokemon to remove the gap
  for (int i = found_index; i < trainer->party_size - 1; i++) {
    trainer->party[i] = trainer->party[i + 1];
  }

  // Decrement party_size to reflect the removed Pokemon
  trainer->party_size--;

  return SUCCESS;
}

/** count_species
 * @brief Count the total number of Pokémon of a given species
 * out of the parties of every trainer in this gym.
 * @param species the name of the species to search for
 * @return FAILURE if parameters invalid, otherwise the number of Pokémon in the gym of the given species
*/
int count_species(const char *species) {
    UNUSED_PARAM(species);
    if (species == NULL) {
    return FAILURE; // Invalid parameters
  }

  int total_count = 0;

  // Iterate through all trainers in the gym
  for (int i = 0; i < gym.num_trainers; i++) {
    // Iterate through each Pokemon in the current trainer's party
    for (int j = 0; j < MAX_PARTY_LENGTH; j++) {
      // Check if the species match (ignoring case)
      if (my_strncmp(gym.trainers[i].party[j].species, species, MAX_NAME_LENGTH - 1) == 0) {
        // Increment the total count if a match is found
        total_count++;
      }
    }
  }

  return total_count;
}

/** trade_pokemon
 * @brief Trade two Pokémon between two trainers,
 * swapping their positions in their parties.
 * @param t0 pointer to the first trainer in the trade
 * @param party_index_0 the index of the Pokémon to trade in the first trainer's party
 * @param t1 pointer to the second trainer in the trade
 * @param party_index_1 the index of the Pokémon to trade in the second trainer's party
 * @return FAILURE if 
 *                  parameters invalid,
 *                  both Trainers are the same,
 *                  party_index_0 or party_index_1 do not point to pokemon,
 *         SUCCESS otherwise
*/
int trade_pokemon(struct Trainer *t0, int party_index_0, struct Trainer *t1, int party_index_1) {
    UNUSED_PARAM(t0);
    UNUSED_PARAM(party_index_0);
    UNUSED_PARAM(t1);
    UNUSED_PARAM(party_index_1);
    
    if (t0 == NULL || t1 == NULL || party_index_0 < 0 || party_index_1 < 0 ||
      party_index_0 >= MAX_PARTY_LENGTH || party_index_1 >= MAX_PARTY_LENGTH) {
    return FAILURE; // Invalid parameters or out-of-bounds indices
  }

  if (t0 == t1) {
    return FAILURE; // Trainers cannot trade with themselves
  }

  // Temporary storage for Pokemon being traded
  struct Pokemon temp;

  // Perform the trade by swapping Pokemon data using the temporary variable
  temp = t0->party[party_index_0];
  t0->party[party_index_0] = t1->party[party_index_1];
  t1->party[party_index_1] = temp;

  return SUCCESS;
}

/** battle_trainer
 * @brief Start a battle between two trainers.
 * 
 * The num_wins variable of the winner of this battle should
 * also be incremented to reflect the trainer's win.
 * 
 * @param challenger pointer to the trainer who initiated the battle
 * @param opponent pointer to the trainer who was initiated by the challenger
 * @return FAILURE if parameters invalid, 0 if the challenger wins, 1 if the opponent wins
*/
int battle_trainer(struct Trainer *challenger, struct Trainer *opponent) {
    UNUSED_PARAM(challenger);
    UNUSED_PARAM(opponent);
    if (challenger == NULL || opponent == NULL) {
    return FAILURE; // Invalid parameters
  }

  int challenger_wins = 0;
  int opponent_wins = 0;
  int min_party_size = opponent->party_size;
  if (challenger->party_size != opponent->party_size) {
    if (opponent->party_size > challenger->party_size) {
        min_party_size = challenger->party_size;
    } else {
        min_party_size = opponent->party_size;
    }
  }
  // Iterate through each Pokemon slot up to the minimum party size
  for (int i = 0; i < min_party_size; i++) {
    // Check if challengers have Pokemon
    if (challenger->party[i].level > 0) {
      // Opponent wins automatically if they have no Pokemon at the index
      if (opponent->party[i].level == 0) {
        opponent_wins++;
      } else {
        // Compare Pokemon levels if both have them
        if (challenger->party[i].level > opponent->party[i].level) {
          challenger_wins++;
        } else if (challenger->party[i].level < opponent->party[i].level) {
          opponent_wins++;
        }
      }
    } else {
      // Challenger has no Pokemon at the index, opponent wins automatically
      opponent_wins++;
    }
  }

  // Handle remaining matches for the trainer with more Pokemon (if any)
  if (challenger->party_size > opponent->party_size) {
    for (int i = min_party_size; i < challenger->party_size; i++) {
      if (challenger->party[i].level > 0) {
        challenger_wins++;
      }
    }
  }

  // Determine the winner based on the number of won matches
  if (challenger_wins > opponent_wins) {
    challenger->num_wins++;
    return 0; // Challenger wins
  } else if (opponent_wins > challenger_wins) {
    opponent->num_wins++;
    return 1; // Opponent wins
  } else {
    // Challenger wins in case of a draw
    opponent->num_wins++;
    printf("The battle is a draw, but the challenger loses!\n");
    return 1; // Opponent wins (due to draw rule)
  }
}

/** find_champion
 * @brief Find the CHAMPION!!! (i.e., the trainer in this gym with the most wins.)
 * If several trainers have the maximum number of wins, pick the first in the gym trainer array with
 *  the maximum wins.
 * @return a pointer to the Trainer with the most wins, NULL is the gym has no Trainers in it
*/
struct Trainer *find_champion(void) {
    if (gym.num_trainers == 0) {
    return NULL; // No trainers in the gym
  }

  int max_wins = gym.trainers[0].num_wins;  // Initialize with first trainer's wins
  int champion_index = 0;

  // Iterate through all trainers in the gym
  for (int i = 1; i < gym.num_trainers; i++) {
    if (gym.trainers[i].num_wins > max_wins) {
      max_wins = gym.trainers[i].num_wins;
      champion_index = i;
    }
  }

  // Return pointer to the champion trainer
  return &gym.trainers[champion_index];
}