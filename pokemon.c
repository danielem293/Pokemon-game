#include "pokemon.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128

// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================

// --------------------------------------------------------------
// 1) Safe integer reading & String Manipulation
//    Helper functions to ensure robust user input and memory safety.
// --------------------------------------------------------------

/**
 * Removes leading and trailing whitespace characters from a string.
 * Modifies the string in-place.
 */
void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

/**
 * Custom implementation of strdup to ensure cross-platform compatibility
 * and safe memory allocation checks.
 */
char *myStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in mymyStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

/**
 * Reads an integer from stdin safely, preventing buffer overflows and handling invalid inputs.
 * Uses a buffer to read the line and strtol for conversion.
 */
int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// Dynamically reallocates memory as needed to handle inputs of any length.
// --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

// --------------------------------------------------------------
// Display Menu & Tree Traversals
// Implements various tree traversal algorithms (BFS, DFS variations).
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
    if(owner->pokedexRoot== NULL)
    {
         printf("Pokedex is empty.\n");
    }
    else
    {
        preOrderTraversal(owner->pokedexRoot);
    }
        break;
    case 3:
    if(owner->pokedexRoot== NULL)
    {
         printf("Pokedex is empty.\n");
    }
    else
    {
        inOrderTraversal(owner->pokedexRoot);
    }
        break;
    case 4:
    if(owner->pokedexRoot== NULL)
    {
         printf("Pokedex is empty.\n");
    }
    else
    {
        postOrderTraversal(owner->pokedexRoot);
    }
        break;
    case 5:
    if(owner->pokedexRoot== NULL)
    {
         printf("Pokedex is empty.\n");
    }
    else
    {
        displayAlphabetical(owner->pokedexRoot);
    }
        break;
    default:
        printf("Invalid choice.\n");
    }
}


/**
 * BFS Search Implementation.
 * Uses a manually allocated queue to traverse the BST level by level
 * to find a Pokemon by ID.
 * @param root The root of the BST.
 * @param id The ID to search for.
 * @return Pointer to the found PokemonNode or NULL.
 */
PokemonNode *searchPokemonBFS(PokemonNode *root, int id)
{
    
    if (root == NULL)
    {
        return NULL;
    }

    // Create a queue for BFS
    PokemonNode **queue = (PokemonNode **)malloc(sizeof(PokemonNode *) * 151); // Adjust size as needed
    if (queue == NULL)
    {
        fprintf(stderr, "Memory allocation failed for BFS queue.\n");
        exit(EXIT_FAILURE);
    }

    int front = 0, rear = 0;

    // Enqueue the root node
    queue[rear++] = root;

    while (front < rear)
    {
        // Dequeue a node
        PokemonNode *current = queue[front++];

        // Check if the current node's ID matches the target ID
        if (current->data->id == id)
        {
            free(queue); // Free allocated memory before returning
            return current;
        }

        // Enqueue left child if it exists
        if (current->left != NULL)
        {
            queue[rear++] = current->left;
        }

        // Enqueue right child if it exists
        if (current->right != NULL)
        {
            queue[rear++] = current->right;
        }
    }

    // Free allocated memory
    free(queue);

    // If we reach here, the ID was not found
    return NULL;

}

/**
 * Standard Breadth-First Search traversal to display nodes.
 */
void displayBFS(PokemonNode* root)
{
    if (root == NULL) 
    {
        printf("Pokedex is empty.\n");
        return;
    }
    PokemonNode **queue = (PokemonNode **)malloc(sizeof(PokemonNode *) * 1000); // Adjust size as needed
    if (queue == NULL)
    {
        fprintf(stderr, "Memory allocation failed for BFS queue.\n");
        exit(EXIT_FAILURE);
    }

    int front = 0, rear = 0;

    // Enqueue the root node
    queue[rear++] = root;

    while (front < rear)
    {
        // Dequeue a node
        PokemonNode *current = queue[front++];

        // Print the current node
        printPokemonNode(current);

        // Enqueue left child if it exists
        if (current->left != NULL)
        {
            queue[rear++] = current->left;
        }

        // Enqueue right child if it exists
        if (current->right != NULL)
        {
            queue[rear++] = current->right;
        }
    }

    // Free allocated memory
    free(queue);
    
}

// Recursive Pre-Order Traversal (Root -> Left -> Right)
void preOrderTraversal(PokemonNode* root) 
{
     if (root == NULL)
    {
        return;
    }

    // Print the current node
    printPokemonNode(root);

    // Recursively traverse the left subtree
    preOrderTraversal(root->left);

    // Recursively traverse the right subtree
    preOrderTraversal(root->right);
}

// Recursive In-Order Traversal (Left -> Root -> Right)
void inOrderTraversal(PokemonNode* root) 
{
    if (root == NULL)
    {
        return;
    }

    // Recursively traverse the left subtree
    inOrderTraversal(root->left);

    // Print the current node
    printPokemonNode(root);

    // Recursively traverse the right subtree
    inOrderTraversal(root->right);
}

// Recursive Post-Order Traversal (Left -> Right -> Root)
void postOrderTraversal(PokemonNode* root) 
{
     if (root == NULL)
    {
        return;
    }
    // Recursively traverse the left subtree
    postOrderTraversal(root->left);

    // Recursively traverse the right subtree
    postOrderTraversal(root->right);

    // Print the current node
    printPokemonNode(root);
}

/**
 * Displays Pokemon sorted alphabetically.
 * Strategy: Collects all nodes into an array using BFS, then uses QuickSort (qsort).
 */
void displayAlphabetical(PokemonNode *root)
{
    if (root == NULL)
    {
        return;
    }

    // Create a queue for BFS
    PokemonNode **queue = (PokemonNode **)malloc(sizeof(PokemonNode *) * 1000);
    if (queue == NULL)
    {
        fprintf(stderr, "Memory allocation failed for BFS queue.\n");
        exit(EXIT_FAILURE);
    }

    // Create an array to store the nodes for sorting
    PokemonNode **nodes = (PokemonNode **)malloc(sizeof(PokemonNode *) * 1000);
    if (nodes == NULL)
    {
        fprintf(stderr, "Memory allocation failed for nodes array.\n");
        free(queue);
        exit(EXIT_FAILURE);
    }

    int front = 0, rear = 0, count = 0;

    // Enqueue the root node
    queue[rear++] = root;

    while (front < rear)
    {
        // Dequeue a node
        PokemonNode *current = queue[front++];

        // Add the current node to the array for sorting
        nodes[count++] = current;

        // Enqueue left child if it exists
        if (current->left != NULL)
        {
            queue[rear++] = current->left;
        }

        // Enqueue right child if it exists
        if (current->right != NULL)
        {
            queue[rear++] = current->right;
        }
    }

    // Sort the nodes alphabetically by name
    qsort(nodes, count, sizeof(PokemonNode *), compareByNameNode);

    // Display the sorted nodes
    for (int i = 0; i < count; i++)
    {
        printPokemonNode(nodes[i]);
    }

    // Free allocated memory
    free(queue);
    free(nodes);
}

// Comparator for sorting Pokémon by their names
int compareByNameNode(const void *a, const void *b)
{
    PokemonNode *nodeA = *(PokemonNode **)a;
    PokemonNode *nodeB = *(PokemonNode **)b;
    return strcmp(nodeA->data->name, nodeB->data->name);
}


// --------------------------------------------------------------
// Sub-menu for existing Pokedex
// Handles operations for a specific owner in the circular list.
// --------------------------------------------------------------
void enterExistingPokedexMenu()
{
    if(ownerHead == NULL)
    {
        printf("No existing Pokedexes.\n");
        return;
    }

    OwnerNode* cur = ownerHead; // Initialize cur to the start of the circular list
    int i = 1;

    // list owners
    printf("\nExisting Pokedexes:\n");

    // Traverse through the circular linked list
    do {
        printf("%d. %s\n", i, cur->ownerName);
        i++;
        cur = cur->next; // Move to the next owner
    } while(cur != ownerHead);
    // you need to implement a few things here :)
    int choosingowner;
    printf("Choose a Pokedex by number: ");
    scanf("%d",&choosingowner);
    getchar();
    for(int i =1; i<choosingowner;i++)
    {
        cur = cur->next;
    }
    printf("\nEntering %s's Pokedex...\n", cur->ownerName);


    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
            addPokemon(cur);
            break;
        case 2:
            displayMenu(cur);
            break;
        case 3:
            freePokemon(cur);
            break;
        case 4:
            pokemonFight(cur);
            break;
        case 5:
            evolvePokemon(cur);
            break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}

/**
 * Adds a Pokemon to the BST.
 * Implements standard BST insertion logic:
 * - Smaller IDs go left
 * - Larger IDs go right
 * - No duplicates allowed
 */
void addPokemon(OwnerNode* owner)
{
    if (owner == NULL) 
    {
        printf("Invalid owner.\n");
        return;
    }

    int idofnewpokemon = readIntSafe("Enter ID to add: ");

    // Check if the entered ID is valid
    if (idofnewpokemon <= 0 || idofnewpokemon > 151) 
    {
        printf("Invalid Pokémon ID. Please enter a valid ID.\n");
        return;
    }

    // Create a new Pokémon node
    PokemonNode *newPokemonNode = createPokemonNode(&pokedex[idofnewpokemon - 1]);

    // If the Pokedex root is NULL, set the new node as the root
    if (owner->pokedexRoot == NULL) 
    {
        owner->pokedexRoot = newPokemonNode;
        printf("Pokemon %s (ID %d) added.\n", newPokemonNode->data->name, idofnewpokemon);
        return;
    }

    // Traverse the binary search tree to find the correct position for the new node
    PokemonNode *cur = owner->pokedexRoot;
    while (1) 
    {
        if (idofnewpokemon == cur->data->id) 
        {
            // If the Pokémon ID already exists, do not add it again
            printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", idofnewpokemon);
            free(newPokemonNode); // Free the unused new node
            return;
        } 
        else if (idofnewpokemon < cur->data->id) 
        {
            if (cur->left == NULL) 
            {
                // Insert the new Pokémon node here if left is NULL
                cur->left = newPokemonNode;
                printf("Pokemon %s (ID %d) added.\n", newPokemonNode->data->name, idofnewpokemon);
                return;
            }
            cur = cur->left;
        } 
        else 
        {
            if (cur->right == NULL) 
            {
                // Insert the new Pokémon node here if right is NULL
                cur->right = newPokemonNode;
                printf("Pokemon %s (ID %d) added.\n", newPokemonNode->data->name, idofnewpokemon);
                return;
            }
            cur = cur->right;
        }
    }
}


void freePokemon(OwnerNode* owner) 
{
    if (owner->pokedexRoot == NULL) 
    {
        printf("No Pokemon to release.\n");
        return;
    }

    int chosenid = readIntSafe("Enter Pokemon ID to release: ");

    // Use BFS to search for the Pokémon
    PokemonNode* foundPokemon = searchPokemonBFS(owner->pokedexRoot, chosenid);

    if (foundPokemon == NULL) 
    {
        printf("Pokemon with ID %d not found in the Pokedex.\n", chosenid);
        return;
    }

    // Delete the Pokémon node
    owner->pokedexRoot = deletePokemonNode(owner->pokedexRoot, chosenid);
    printf("Removing Pokemon %s (ID %d).\n", pokedex[chosenid - 1].name, chosenid);
}

// Helper: Finds the minimum value node in a BST (leftmost child)
PokemonNode* findMin(PokemonNode* node) 
{
    while (node->left != NULL) 
    {
        node = node->left;
    }
    return node;
}

/**
 * Deletes a node from the BST while maintaining BST properties.
 * Handles 3 cases:
 * 1. Node is leaf (no children).
 * 2. Node has one child.
 * 3. Node has two children (replaces with successor and deletes successor).
 */
PokemonNode* deletePokemonNode(PokemonNode* root, int id)
{
    if (root == NULL)
    {
        return root;
    }

    if (id < root->data->id)
    {
        root->left = deletePokemonNode(root->left, id);
    }
    else if (id > root->data->id)
    {
        root->right = deletePokemonNode(root->right, id);
    }
    else
    {
        // Node with the ID found
        if (root->left == NULL && root->right == NULL) // No children
        {
            freePokemonNode(root);
            return NULL;
        }
        else if (root->left == NULL) // One child (right)
        {
            PokemonNode* temp = root->right;
            freePokemonNode(root);
            return temp;
        }
        else if (root->right == NULL) // One child (left)
        {
            PokemonNode* temp = root->left;
            freePokemonNode(root);
            return temp;
        }
        else // Two children
        {
            PokemonNode* temp = findMin(root->right);
            // Deep copy the data
            PokemonData* newData = (PokemonData*)malloc(sizeof(PokemonData));
            if (!newData)
            {
                fprintf(stderr, "Memory allocation failed for PokemonData.\n");
                exit(EXIT_FAILURE);
            }
            newData->id = temp->data->id;
            char* tempName = myStrdup(temp->data->name);
            if(tempName == NULL)
            {
                printf("Memory allocation error\n");
                free(newData);
                exit(1);
            }
            newData->name = tempName;
            freePokemonNode(root); // Free old node
            root->data = newData;
            root->right = deletePokemonNode(root->right, temp->data->id);
        }
    }
    return root;
}  // this is the code


void pokemonFight(OwnerNode* owner) 
{
    if (owner->pokedexRoot == NULL) 
    {
        printf("Pokedex is empty.\n");
        return;
    }

    int id1 = readIntSafe("Enter ID of the first Pokemon: ");
    int id2 = readIntSafe("Enter ID of the second Pokemon: ");

    // Search for the Pokémon using the provided BFS search function
    PokemonNode* pokemon1 = searchPokemonBFS(owner->pokedexRoot, id1);
    PokemonNode* pokemon2 = searchPokemonBFS(owner->pokedexRoot, id2);

    if (pokemon1 == NULL || pokemon2 == NULL) 
    {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }

    // Calculate scores based on the formula: attack * 1.5 + hp * 1.2
    double score1 = pokemon1->data->attack * 1.5 + pokemon1->data->hp * 1.2;
    double score2 = pokemon2->data->attack * 1.5 + pokemon2->data->hp * 1.2;

    // Print details of the Pokémon
    printf("Pokemon 1: %s (Score = %.2f)\n", pokemon1->data->name, score1);
    printf("Pokemon 2: %s (Score = %.2f)\n", pokemon2->data->name, score2);

    // Determine the winner
    if (score1 > score2) {
        printf("%s wins!\n", pokemon1->data->name);
    } else if (score1 < score2) {
        printf("%s wins!\n", pokemon2->data->name);
    } else {
        printf("It's a tie!\n");
    }
}


void evolvePokemon(OwnerNode* owner) 
{
    if (owner->pokedexRoot == NULL) 
    {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }

    int oldID = readIntSafe("Enter ID of Pokemon to evolve: ");
    PokemonNode* pokemonToEvolve = searchPokemonBFS(owner->pokedexRoot, oldID);

    // Check if the Pokémon exists in the Pokedex
    if (pokemonToEvolve == NULL) {
        printf("No Pokemon with ID %d found.\n", oldID);
        return;
    }

    // Check if the Pokémon can evolve
    if (pokemonToEvolve->data->CAN_EVOLVE == 0) 
    {
        printf("%s (ID %d) cannot evolve.\n", pokemonToEvolve->data->name, oldID);
        return;
    }

    int newID = oldID + 1;  // ID of the evolved form
    PokemonNode* evolvedPokemon = searchPokemonBFS(owner->pokedexRoot, newID);

    // If the evolved form already exists in the Pokedex
    if (evolvedPokemon != NULL) {
        printf("Evolution ID %d (%s) already in the Pokedex. Releasing %s (ID %d).\n",
               newID, evolvedPokemon->data->name, pokemonToEvolve->data->name, oldID);

        // Release the original Pokémon
        owner->pokedexRoot = deletePokemonNode(owner->pokedexRoot, oldID);
        return;
    }

    // Proceed to evolve the Pokémon
    printf("Pokemon evolved from %s (ID %d) to %s (ID %d).\n",
        pokemonToEvolve->data->name, oldID, pokedex[newID-1].name, newID);

        evolvedPokemon = createPokemonNode(&pokedex[newID-1]);
        pokemonToEvolve->right = evolvedPokemon;
        owner->pokedexRoot = deletePokemonNode(owner->pokedexRoot, oldID);
        
        
}

// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu()
{
    int choice;
    do
    {
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            enterExistingPokedexMenu();
            break;
        case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        case 7:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}

int main()
{
    mainMenu();
    freeAllOwners();
    return 0;
}


/**
 * Creates a new Pokedex (and a new Owner) in the circular linked list.
 * Includes conflict checks for existing names.
 */
void openPokedexMenu() 
{
    printf("Your name: ");
    char *newownername = getDynamicInput();
    OwnerNode* i = ownerHead;

    if(ownerHead != NULL)
    {
        do
    {

        if(strcmp(newownername,i->ownerName)==0)
        {
            printf("Owner '%s' already exists. Not creating a new Pokedex.\n", newownername);
            free(newownername);
            return;
        }
        i = i->next;
    }while (i!= ownerHead);
    }
    
    
    
    printf("\nCOOSE YOUR STARTER:\n1. Bulbasaur\n2. Charmander\n3. Squirtle\n");
    int starterchoise = readIntSafe("your choice: ");

    if(starterchoise == 2)
    {
        starterchoise = 4;
    }
    else if(starterchoise==3)
    {
        starterchoise = 7;
    }
    else
    {
        if(starterchoise != 1)
        {
            printf("Invalid choice.\n");
            return;
        }
        
    }
    

    PokemonNode *starterNode = createPokemonNode(&pokedex[starterchoise - 1]);
    OwnerNode *newOwner = createOwner(newownername, starterNode);
    linkOwnerInCircularList(newOwner);
    printf("New Pokedex created for %s with starter %s.\n\n", newownername, pokedex[starterchoise-1].name);
    free(newownername);
    
}



void deletePokedex() 
{
    // Check if there are any owners in the list
    if (ownerHead == NULL) 
    {
        printf("No existing Pokedexes to delete.\n");
        return;
    }

    printf("\n=== Delete a Pokedex ===\n");

    // Display the list of owners
    OwnerNode *current = ownerHead;
    int count = 0;

    do 
    {
        printf("%d. %s\n", ++count, current->ownerName);
        current = current->next;
    } while (current != ownerHead);

    // Prompt the user to select an owner by number
    int choice = readIntSafe("Choose a Pokedex to delete by number: ");

    // Validate the choice
    if (choice < 1 || choice > count) 
    {
        printf("Invalid choice.\n");
        return;
    }

    // Locate the chosen owner
    current = ownerHead;
    for (int i = 1; i < choice; i++) 
    {
        current = current->next;
    }

    // Delete the selected owner's Pokedex
    printf("Deleting %s's entire Pokedex...\n", current->ownerName);

    // Handle the case where the owner to delete is the head
    if (current == ownerHead) 
    {
        if (ownerHead->next == ownerHead) // Only one owner in the list
        {
            freeOwnerNode(ownerHead);
            ownerHead = NULL;
        } 
        else 
        {
            OwnerNode *prev = ownerHead->prev;
            ownerHead = ownerHead->next;
            prev->next = ownerHead;
            ownerHead->prev = prev;
            freeOwnerNode(current);
        }
    } 
    else // For non-head owners
    {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        freeOwnerNode(current);
    }

    printf("Pokedex deleted.\n");
}



PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode) 
{
    if (root == NULL) 
    {
        // If the tree is empty, the new node becomes the root
        return newNode;
    }

    if (newNode->data->id < root->data->id) 
    {
        // Insert in the left subtree
        root->left = insertPokemonNode(root->left, newNode);
    } else if (newNode->data->id > root->data->id) 
    {
        // Insert in the right subtree
        root->right = insertPokemonNode(root->right, newNode);
    }

    // Return the unchanged root
    return root;
}

void deleteOwnerFromList(OwnerNode *target) 
{
    if(ownerHead == NULL || target == NULL)
    {
            return;
    }
    if(ownerHead == target && ownerHead->next == ownerHead)
    {
        freeOwnerNode(ownerHead);
        ownerHead = NULL;
    } else {
        OwnerNode* temp1 = target->next;
        OwnerNode* temp2 = target->prev;
        temp2->next = temp1;
        temp1->prev = temp2;
        if(target == ownerHead)
        {
            ownerHead = temp1;
        }
        freeOwnerNode(target);
    }
}

OwnerNode *findOwnerByName(const char *name) 
{
    OwnerNode *current = ownerHead;

    // Traverse the linked list to find the owner
    while (current != NULL) 
    {
        if (strcmp(current->ownerName, name) == 0) 
        {
            // Found the owner
            return current;
        }
        current = current->next;
    }

    // Owner not found
    return NULL;
}


void mergePokedexMenu() 
{

    if (ownerHead == NULL || ownerHead->next == ownerHead) 
    {
        printf("Not enough owners to merge.\n");
        return;
    }
    char *firstOwnerName = NULL, *secondOwnerName = NULL;
    printf("\n=== Merge Pokedexes ===\n");

    printf("Enter name of first owner: ");
    firstOwnerName = getDynamicInput();

    printf("Enter name of second owner: ");
    secondOwnerName = getDynamicInput();

    OwnerNode *firstOwner = findOwnerByName(firstOwnerName);
    OwnerNode *secondOwner = findOwnerByName(secondOwnerName);

    if (firstOwner == NULL || secondOwner == NULL) 
    {
        printf("One or both owners not found.\n");
        free(firstOwnerName);
        free(secondOwnerName);
        return;
    }

    printf("Merging %s and %s...\n", firstOwner->ownerName, secondOwner->ownerName);

    // Perform BFS on the second owner's Pokedex and add each node to the first owner's Pokedex
    PokemonNode **queue = (PokemonNode **)malloc(sizeof(PokemonNode *) * 1000); // Adjust size as needed
    if (!queue) 
    {
        printf("Memory allocation failed.\n");
        free(firstOwnerName);
        free(secondOwnerName);
        return;
    }

    int front = 0, rear = 0;
    queue[rear++] = secondOwner->pokedexRoot;

    while (front < rear) 
    {
        PokemonNode *current = queue[front++];

        if (current != NULL) 
        {
            // Create a new node for insertion
            PokemonNode *newNode = (PokemonNode *)malloc(sizeof(PokemonNode));
            if (!newNode) 
            {
                printf("Memory allocation failed for new node.\n");
                free(queue);
                free(firstOwnerName);
                free(secondOwnerName);
                return;
            }
            newNode->data = current->data; // Point to the same data
            newNode->left = NULL;
            newNode->right = NULL;

            firstOwner->pokedexRoot = insertPokemonNode(firstOwner->pokedexRoot, newNode);

            // Enqueue children
            queue[rear++] = current->left;
            queue[rear++] = current->right;
        }
    }

    free(queue);

    // Remove the second owner
    deleteOwnerFromList(secondOwner);

    printf("Merge completed.\n");
    printf("Owner '%s' has been removed after merging.\n", secondOwnerName);

    free(firstOwnerName);
    free(secondOwnerName);
}


void sortOwners() {
    if (ownerHead == NULL || ownerHead->next == ownerHead) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }

    OwnerNode *current = ownerHead;
    OwnerNode *index = NULL;
    char tempName[100];

    // Perform a bubble sort on the circular doubly linked list
    do {
        index = current->next;
        while (index != ownerHead) {
            // Compare the names of two owners
            if (strcmp(current->ownerName, index->ownerName) > 0) {
                // Swap the owner names
                strcpy(tempName, current->ownerName);
                strcpy(current->ownerName, index->ownerName);
                strcpy(index->ownerName, tempName);
            }
            index = index->next;
        }
        current = current->next;
    } while (current != ownerHead);  // Continue until we return to the head
    printf("Owners sorted by name.\n");
}


/**
 * Links a new OwnerNode into the circular doubly linked list.
 * Updates pointers for the new node, the previous tail, and the head.
 */
void linkOwnerInCircularList(OwnerNode *newOwner)
{
    if(ownerHead == NULL)
    {
        ownerHead = newOwner;
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
        return;
    }
    OwnerNode* lastowner = ownerHead->prev;;
    lastowner->next = newOwner;
    newOwner->prev = lastowner;

    newOwner->next = ownerHead;
    ownerHead->prev = newOwner;
    
}



PokemonNode *createPokemonNode(const PokemonData *data)
{
    PokemonNode *newpokemonnode = malloc(sizeof(PokemonNode));
    if (!newpokemonnode)
    {
        printf("Memory allocation failed for PokemonNode.\n");
        return NULL;
    }

    newpokemonnode->data = (PokemonData *)data;
    newpokemonnode->left = NULL;
    newpokemonnode->right = NULL;
    return newpokemonnode;
}




OwnerNode *createOwner(char *ownerName, PokemonNode *starter)
{
    OwnerNode *newOwner = (OwnerNode *)malloc(sizeof(OwnerNode));
    if (newOwner == NULL) 
    {
        printf("Memory allocation failed for OwnerNode.\n");
        free(newOwner);
        return NULL;
    }

    // Allocate memory for the ownerName and copy the provided name into it
    newOwner->ownerName = (char *)malloc(strlen(ownerName) + 1);
    if (newOwner->ownerName == NULL) 
    {
        printf("Memory allocation failed for ownerName.\n");
        free(newOwner);
        return NULL;
    }
    strcpy(newOwner->ownerName, ownerName);

    newOwner->pokedexRoot = starter;

    // Initialize the next pointer to NULL (assuming the list is being built)
    newOwner->pokedexRoot->right = NULL;
    newOwner->pokedexRoot->left = NULL;

    return newOwner;

}


void printOwnersCircular() 
{
    if (ownerHead == NULL) 
    {
        printf("No owners.\n");
        return;
    }

    char choice = 'a';
    while (choice != 'f' && choice != 'F' && choice != 'b' && choice != 'B') 
    {
        printf("Enter direction (F or B): ");
        scanf(" %c", &choice);
        if (choice == 'f' || choice == 'F') 
        {
            getchar();
            int numprints = readIntSafe("How many prints? ");
            OwnerNode *current = ownerHead;
            for (int i = 0; i < numprints; i++) 
            {
                printf("[%d] %s\n", i + 1, current->ownerName);
                current = current->next;  // Move to the next owner in the circular list
            }
            return;
        } 
        else if (choice == 'b' || choice == 'B') 
        {
            getchar();
            int numprints = readIntSafe("How many prints? ");
            OwnerNode *current = ownerHead;
            // Move to the last node to print in reverse order
            for (int i = 0; i < numprints; i++) 
            {
                printf("[%d] %s\n", i + 1, current->ownerName);
                current = current->prev;  // Move to the previous owner in the circular list
            }
            return;
        } 
        else 
        {
            printf("Invalid direction, must be F or B.\n");
        }
    }
}


//========================================
// Memory Management & Freeing
// Ensures no memory leaks by recursively freeing trees and lists.
//========================================

void freePokemonNode(PokemonNode* node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->data != NULL && node->data >= pokedex && node->data < pokedex + 151)
    {
        // Don't free if data points to the static pokedex array
        node->data = NULL;
    }
    else
    {
        free(node->data); // Only free dynamically allocated data
    }
    free(node);
}

void freePokemonTree(PokemonNode *root)
{
    if (root == NULL)
    {
        return;
    }
    freePokemonTree(root->left);
    freePokemonTree(root->right);
    freePokemonNode(root);
}

void freeOwnerNode(OwnerNode *owner)
{
    if (owner == NULL)
    {
        return;
    }
    freePokemonTree(owner->pokedexRoot);
    free(owner->ownerName);
    free(owner);
}

void freeAllOwners() 
{
    if (ownerHead == NULL) 
    {
        return;
    }

    OwnerNode *current = ownerHead;
    OwnerNode *nextNode;

    // Traverse the circular linked list and free each node
    do 
    {
        nextNode = current->next;
        freeOwnerNode(current);
        current = nextNode;
    } 
    while (current != ownerHead);
    // Reset the head pointer to NULL as the list is now empty
    ownerHead = NULL;
}