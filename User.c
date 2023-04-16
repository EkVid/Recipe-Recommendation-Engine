#include <stdio.h>
#include <stdlib.h>
#include "ingredient_graph.c"

void show_ingre(char ingredients[MAT_SIZE][MAX_STR_LEN])
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    printf("%s: %d\n", ingredients[i], i);
  }
}

void print_matrix(double AdjMatrix[MAT_SIZE][MAT_SIZE], int row, int col)
{
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      printf("%f, ", AdjMatrix[i][j]);
    }
    printf("\n");
  }
}

void print_ingre(ingre_node *h)
{
  ingre_node *p = NULL;
  if (h != NULL)
  {
    p = h;
    while (p != NULL)
    {
      printf("%s, %d\n", ingredients[p->ingredient.x], p->ingredient.related_times);
      p = p->next;
    }
  }
  else
  {
    return;
  }
}

int count_nodes(intNode *h)
{
  intNode *p = h;
  int count = 0;
  while (p != NULL)
  {
    count++;
    p = p->next;
  }
  return count;
}

int main()
{

  intNode *head = NULL;
  char recipe[10][MAX_STR_LEN] = {"quinoa",
                                  "turkey",
                                  "artichoke hearts",
                                  "soymilk",
                                  "medium shrimp",
                                  "gruyere cheese",
                                  "sunflower seeds",
                                  "",
                                  "",
                                  ""};

  // Load the ingredients graph - you control which size
  // you want by commenting/uncommenting the corresponding
  // #define at the top of 'ingredient_graph.c'
  load_ingredients();
  printf("------- The available ingredients should appear above this line -------\n");
  // Call related ingredients
  printf("\n ****** Related ingredients for quinoa:\n");
  related_ingredients("quinoa"); 
  printf("\n ****** Related to 'quinoa' k-dist, k=1 - Updtaed\n");
  head = related_k_dist(head, "quinoa", 1, 0);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related to 'quinoa' k-dist, k=2 - Updated\n");
  head = related_k_dist(head, "quinoa", 2, 0);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related to 'artichoke hearts' k-dist, k=1 - Updated\n");
  head = related_k_dist(head, "artichoke hearts", 1, 0);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related to 'tomato juice' k-dist, k=1 - Updated\n");
  head = related_k_dist(head, "tomato juice", 1, 0);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related to 'tomato juice' k-dist, k=2 - Updated\n");
  head = related_k_dist(head, "tomato juice", 2, 0);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related with restrictions, 'quinoa', 'artichoke hearts', k_source=1, k_dest=1 - Updated;\n");
  head = related_with_restrictions("quinoa", "artichoke hearts", 1, 1);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related with restrictions, 'quinoa', 'tomato juice', k_source=2, k_dest=1 - Updated;\n");
  head = related_with_restrictions("quinoa", "tomato juice", 2, 1);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** Related with restrictions, 'quinoa', 'tomato juice', k_source=2, k_dest=2 - Updated;\n");
  head = related_with_restrictions("quinoa", "tomato juice", 2, 1);
  print_ingredients(head);
  head = deleteList(head);

  printf("\n ****** substituting ingredient 'quinoa' in recipe\n");

  substitute_ingredient(recipe, "quinoa");
  for (int i = 0; i < 10; i++)
  {
    if (strlen(recipe[i]) > 0)
    {
      printf("%s\n", recipe[i]);
    }
  }
  return 0;
}
