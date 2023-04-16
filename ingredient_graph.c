#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1024

#define MAT_SIZE 10 // Use the small graph
// #define MAT_SIZE 400 // Use full-size graph

double AdjMat[MAT_SIZE][MAT_SIZE];
char ingredients[MAT_SIZE][MAX_STR_LEN];

// 2 additional datatype for substitue
typedef struct ingredient_appear
{
  int related_times;
  int x;
} ingredient_occurence;

typedef struct ingre_occ_list
{
  ingredient_occurence ingredient;
  struct ingre_occ_list *next;
} ingre_node;

typedef struct intNode_struct
{
  int x;
  struct intNode_struct *next;
} intNode;

intNode *insertInt(intNode *h, int x)
{
  // Inserts the int 'x' into the linked list
  // whose head is at the address in 'h'

  intNode *newNode;

  newNode = (intNode *)calloc(1, sizeof(intNode));
  newNode->x = x;
  newNode->next = NULL;

  if (h != NULL)
    newNode->next = h;

  return newNode;
}

int searchInt(intNode *h, int x)
{
  // Search the linked list for the value 'x', if
  // found, it returns 1, else it returns 0
  while (h != NULL)
  {
    if (h->x == x)
      return 1;
    h = h->next;
  }
  return 0;
}

ingre_node *delete_ingre(ingre_node *h)
{
  // Delete list and free memory
  ingre_node *q;
  if (h == NULL)
    return NULL;

  while (h != NULL)
  {
    q = h->next;
    free(h);
    h = q;
  }

  return NULL;
}

intNode *deleteList(intNode *h)
{
  // Delete list and free memory
  intNode *q;
  if (h == NULL)
    return NULL;

  while (h != NULL)
  {
    q = h->next;
    free(h);
    h = q;
  }

  return NULL;
}

// Read the adjacency matrix from file, and read the ingredient
// names
void load_ingredients(void)
{
  // Loads the adjacency matrix and ingredients list from file,
  // N must be either 10 (small graph), or 400 (full-size).

  FILE *f;
  int n = MAT_SIZE;

  if (n == 10)
    f = fopen("AdjMat_small.dat", "rb");
  else
    f = fopen("AdjMat_full.dat", "rb");
  if (f == NULL)
  {
    printf("Can not open file with adjacency matrix. Please make sure it's in this directory\n");
    return;
  }
  fread(&AdjMat[0][0], n * n * sizeof(double), 1, f);
  fclose(f);

  if (n == 10)
    f = fopen("Ingredient_names_small.txt", "r");
  else
    f = fopen("Ingredient_names_full.txt", "r");
  if (f == NULL)
  {
    printf("Can not open file with ingredient names. Please make sure it's in this directory\n");
    return;
  }
  printf("Reading ingredient names. Available ingredient are:\n");
  for (int i = 0; i < n; i++)
  {
    fgets(&ingredients[i][0], MAX_STR_LEN, f);
    for (int j = 0; j < strlen(ingredients[i]); j++)
      if (ingredients[i][j] == 10 || ingredients[i][j] == 13)
        ingredients[i][j] = 0;
    printf("%s\n", ingredients[i]);
  }
  fclose(f);
}

void print_ingredients(intNode *h)
{
  /*
   * This function prints out all the ingredients corresponding
   * to the ingredient indexes stored in the linked list
   * whose head is 'h'.
   */

  intNode *p = NULL;
  if (h != NULL)
  {
    p = h;
    while (p != NULL)
    {
      printf("%s\n", ingredients[p->x]);
      p = p->next;
    }
  }
  else
  {
    printf("The head is NULL!\n");
    return;
  }
}

int ingredient_index(char source_ingredient[MAX_STR_LEN])
{
  /*
   * This function looks into the array with ingredient
   * names for one that matches the requested
   * 'source_ingredient'. If a match is found, it
   * returns the index at which the matching ingredient
   * name was found.
   *
   * If the 'source_ingredient' is *not* in the ingredients
   * array, the function returns -1
   */


  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(ingredients[i], source_ingredient) == 0)
    {
      return i;
    }
  }
  return -1;
}

void related_ingredients(char source_ingredient[MAX_STR_LEN])
{
  /*
   * This function prints out all the ingredients related
   * to the source ingredient.
   */

  int index = ingredient_index(source_ingredient);
  if (index != -1)
  {
    for (int i = 0; i < MAT_SIZE; i++)
    {
      if (AdjMat[index][i] > 0)
      {
        printf("%s\n", ingredients[i]);
      }
    }
  }
  else
  {
    printf("No such ingredient found!\n");
  }
}

// helper function for related_k_dist
intNode *insert_no_duplicate(intNode *h, int index)
{
  intNode *p = NULL;
  p = h;
  int flag = 0;
  while (p != NULL)
  {
    if (index == p->x)
    {
      flag = 1;
    }
    p = p->next;
  }
  if (flag == 0)
  {
    h = insertInt(h, index);
  }

  return h;
}

intNode *related_k_dist(intNode *h, char source_ingredient[MAX_STR_LEN], int k, int dist)
{
  /*
   * This function determines the ingredients related to the
   * specified source ingredient by a distance of at most k.
   * k >= 1
   *
   * (for k=1, this produces the same list as related_ingredients)
   *
   * For instance, if k=2, the list should contain all the
   * ingredients who are neighbours of source_ingredient[]
   * as well as the ingredients that are neighbours of
   * the neighbours (distance=2).
   *
   * If k=3, then we want source_ingredients' neighbours, its
   * neighbours' neighbours, and its neighbours' neighbours'
   * neighbours. And so on.
   *
   */

  if (dist == k)
  {
    return h;
  }

  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (AdjMat[ingredient_index(source_ingredient)][i] > 0)
    {
      h = insert_no_duplicate(h, i);
    }
  }

  for (int j = MAT_SIZE - 1; j >= 0; j--)
  {
    if (AdjMat[ingredient_index(source_ingredient)][j] > 0)
    {
      h = related_k_dist(h, ingredients[j], k, dist + 1);
    }
  }

  return h;
}

intNode *related_with_restrictions(char source_ingredient[MAX_STR_LEN], char avoid[MAX_STR_LEN], int k_source, int k_avoid)
{
  /*
   * This function returns a linked list that contains the indexes of
   * all ingredients related to source_ingredient[] with a distance
   * of at most k_source.
   */

  intNode *related_head = NULL;
  intNode *not_related_head = NULL;
  intNode *new_list = NULL;
  intNode *p = NULL, *q = NULL;

  related_head = related_k_dist(related_head, source_ingredient, k_source, 0);
  not_related_head = related_k_dist(not_related_head, avoid, k_avoid, 0);
  not_related_head = insert_no_duplicate(not_related_head, ingredient_index(avoid));
  q = related_head;

  while (q != NULL)
  {
    int flag = 0;
    p = not_related_head;
    while (p != NULL)
    {
      if (q->x == p->x)
      {
        flag = flag + 1;
        break;
      }
      p = p->next;
    }
    if (flag == 0)
    {
      new_list = insert_no_duplicate(new_list, q->x);
    }
    q = q->next;
  }

  return new_list;
}

// helper functions

ingre_node *insert_ingre(ingre_node *head, int x, int related_times)
{

  int flag = 0;

  ingre_node *q = head;
  while (q != NULL)
  {
    if (q->ingredient.x == x)
    {
      flag++;
      q->ingredient.related_times = q->ingredient.related_times + related_times;
    }
    q = q->next;
  }

  if (flag == 0)
  {
    ingre_node *p = NULL;

    p = (ingre_node *)calloc(1, sizeof(ingre_node));
    if (p == NULL)
    {
      printf("No memory left!\n");
      return head;
    }
    p->ingredient.x = x;
    p->ingredient.related_times = related_times;
    p->next = head;
    return p;
  }

  else
  {
    return head;
  }
}

ingre_node *related_k_ingre(ingre_node *head, char source_ingredient[MAX_STR_LEN], int k, int dist)
{
  if (dist == k)
  {
    return head;
  }

  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (AdjMat[ingredient_index(source_ingredient)][i] > 0)
    {
      head = insert_ingre(head, i, AdjMat[ingredient_index(source_ingredient)][i]);
    }
  }

  for (int j = MAT_SIZE - 1; j >= 0; j--)
  {
    if (AdjMat[ingredient_index(source_ingredient)][j] > 0)
    {
      head = related_k_ingre(head, ingredients[j], k, dist + 1);
    }
  }

  return head;
}

void substitute_ingredient(char recipe[10][MAX_STR_LEN], char to_change[MAX_STR_LEN])
{
  /*
   * This function takes a recipe, and replaces the one in 'to_change' by the most compatible
   * ingredient from the graph (By most compatible, it means the ingredient that appears most often with
   * the remaining ingredients in the recipe)
   */

  if (strlen(to_change) == 0)
  {
    printf("Please type in the name of the food that is going to be substituted!\n");
    return;
  }

  ingre_node *p = NULL;

  int flag = 0;
  for (int i = 0; i < 10; i++)
  {
    if (strcmp(recipe[i], to_change) == 0)
    {
      flag = 1;
    }
  }
  if (flag == 0)
  {
    printf("The ingredient to change is not found!\n");
    return;
  }

  for (int i = 0; i < 10; i++)
  {
    if ((strcmp(recipe[i], to_change) != 0) && (strlen(recipe[i]) != 0))
    {
      p = related_k_ingre(p, recipe[i], 1, 0);
    }
  }

  ingre_node *ptr = p;
  // Set the to_change one to -1
  while (ptr != NULL)
  {
    if (strcmp(ingredients[ptr->ingredient.x], to_change) == 0)
    {
      ptr->ingredient.related_times = -1;
    }
    ptr = ptr->next;
  }

  ingre_node *pt = NULL;
  ingre_node *m = NULL;
  int temp_times = 0;
  int temp_x = 0;
  if (p != NULL)
  {
    for (pt = p; pt->next != NULL; pt = pt->next)
    {
      for (m = pt->next; m != NULL; m = m->next)
      {
        if (pt->ingredient.related_times < m->ingredient.related_times)
        {
          temp_times = pt->ingredient.related_times;
          temp_x = pt->ingredient.x;

          pt->ingredient.related_times = m->ingredient.related_times;
          pt->ingredient.x = m->ingredient.x;

          m->ingredient.related_times = temp_times;
          m->ingredient.x = temp_x;
        }
      }
    }
  }
  else
  {
    printf("There is no other element in the recipe list! So the recipe list is left as it is!\n\n");
    return;
  }

  char ingred[MAX_STR_LEN] = "";

  int flagger = 0;
  int flagg = 1;
  int times = 0;
  ingre_node *checker = p;
  while (flagg == 1 && checker != NULL)
  {
    for (int i = 0; i < 10; i++)
    {
      if (strcmp(ingredients[checker->ingredient.x], recipe[i]) == 0)
      {
        flagger++;
      }
    }
    if (flagger == 0)
    {
      if (times > 0)
      {
        printf("The most compatible ingredient is already in the recipe, we substituted with the next most compatible one!\n\n");
      }
      strcpy(ingred, ingredients[checker->ingredient.x]);
      flagg = 0;
      break;
    }
    else
    {
      flagger = 0;
      checker = checker->next;
      times++;
    }
  }

  for (int x = 0; x < 10; x++)
  {
    if (strcmp(recipe[x], to_change) == 0)
    {
      if (strlen(ingred) == 0)
      {
        printf("All the available most popular ingredients are already in the recipe! The to_change ingredient is left there!\n\n");
      }
      else
      {
        strcpy(recipe[x], ingred);
      }
    }
  }

  delete_ingre(p);
}
