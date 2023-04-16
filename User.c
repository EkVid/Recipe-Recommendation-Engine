/*
 * CSC A48 - Assignment 3 driver
 *
 * This driver is *very* different from the ones for A1 and A2.
 *
 * For the previous 2 assignments, and through the exercises
 * you have solved so far, we have been helping you develop and
 * improve your skills for testing code and verything it is
 * *correct*.
 *
 * So, this driver only does the basic functionality of
 * calling the functions that set-up a graph, and shows you
 * how to call the functions that you have to implement.
 *
 * Defining the tests you have to run on your code, and
 * determining whether the code is correct, is up to you.
 *
 * *You will NOT submit this file* - but you obviously want
 * to make sure you run sufficient and thorough testing of
 * all components of your A3 solution to avoid any problems
 * when we auto-mark it.
 *
 * Suggestions: Test for *correctness*, not output. As we
 * have explained before - you can write code that produces
 * the right output and still is completely wrong in terms of
 * what it does to solve a problem.
 *
 * So, think carefully about the correctness aspect of your
 * solution, see the post on Piazza on how to test your
 * code (it's old by now, but pinned), and remember your
 * notes for Unit 5 - the section on how to debug recursive
 * code.
 *
 * (c) 2019 - F. Estrada, M. Ahmadzadeh
 */

#include <stdio.h>
#include <stdlib.h>
#include "ingredient_graph_1008088490.c"

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
  /*
   * Just a quick demo of how to set up the graph and call
   * the functions you are supposed to implement
   */

  intNode *head = NULL;
  char recipe[10][MAX_STR_LEN] = {"quinoa",
                                  "turkey",
                                  "artichoke hearts",
                                  "soymilk",
                                  "medium shrimp",
                                  "gruyere cheese",
                                  "sunflower seeds",
                                  "fresh breadcrumbs",
                                  "",
                                  ""};

  // Load the ingredients graph - you control which size
  // you want by commenting/uncommenting the corresponding
  // #define at the top of 'ingredient_graph.c'
  load_ingredients();
  printf("------- The available ingredients should appear above this line -------\n");
  // Call related ingredients
  /*printf("\n ****** Related ingredients for quinoa:\n");
  related_ingredients("quinoa"); // Make sure this
                                 // ingredient exists*/
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

  /*substitute_ingredient(recipe, "quinoa");
  for (int i = 0; i < 10; i++)
  {
    if (strlen(recipe[i]) > 0)
    {
      printf("%s\n", recipe[i]);
    }
  }*/

  ingre_node *p = NULL;
  for (int i = 0; i < 10; i++)
  {
    if ((strcmp(recipe[i], "quinoa") != 0) && (strlen(recipe[i]) != 0))
    {
      p = related_k_ingre(p, recipe[i], 1, 0);
    }
  }

  ingre_node *ptr = p;
  // Set the to_change one to -1
  while (ptr != NULL)
  {
    if (strcmp(ingredients[ptr->ingredient.x], "quinoa") == 0)
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
  }

  print_ingre(p);

  /* The OUTPUT of my solution to this assignment for the sample calls above is:

****** Related ingredients for quinoa:
artichoke hearts
sunflower seeds

****** Related to 'quinoa' k-dist, k=1 - Updtaed
sunflower seeds
artichoke hearts

****** Related to 'quinoa' k-dist, k=2 - Updated
gruyere cheese
turkey
medium shrimp
quinoa
soymilk
sunflower seeds
artichoke hearts

****** Related to 'artichoke hearts' k-dist, k=1 - Updated
quinoa
gruyere cheese
turkey
medium shrimp

****** Related to 'tomato juice' k-dist, k=1 - Updated
fresh breadcrumbs
prepared horseradish

****** Related to 'tomato juice' k-dist, k=2 - Updated
medium shrimp
gruyere cheese
tomato juice
fresh breadcrumbs
prepared horseradish

****** Related with restrictions, 'quinoa', 'artichoke hearts', k_source=1, k_dest=1 - Updated;
sunflower seeds

****** Related with restrictions, 'quinoa', 'tomato juice', k_source=2, k_dest=1 - Updated;
artichoke hearts
sunflower seeds
soymilk
quinoa
medium shrimp
turkey
gruyere cheese

****** Related with restrictions, 'quinoa', 'tomato juice', k_source=2, k_dest=2 - Updated;
artichoke hearts
sunflower seeds
soymilk
quinoa
turkey

****** substituting ingredient 'quinoa' in recipe
fresh breadcrumbs
artichoke hearts
gruyere cheese

  You can use that as a reference, but BEWARE the fact that though I am confident my
  solution is correct *I have not had the time to thoroughly test it*
  (you should know by now what that means)

  */

  // Insert ALL of your tests below this line, make sure you test your
  // code thoroughly.
  // Verify your code's work by checking *on paper* for lists of
  // ingredients your tests ask for!
  // Time to practice those testing skills!

  return 0;
}
