#include <stdlib.h>
#include <stdio.h>

#include "code/list.h"
#include "code/map.h"

int main(int argc, char** argv) {
      printf("Hello, World!\n");
      char *a= "date", *b= "age", *c= "birthday";
      int x= 5282022, y= 18, z= 7222003;
      Map m;
      initMap(&m, sizeof(char*), sizeof(int), 0);
      map_add(&m, &a, &x);
      map_add(&m, &b, &y);
      map_add(&m, &c, &z);
      map_printIntString(&m);

      int g;
      map_get(&m, &c, &g);
      printf("\n%d\n", g);
      return EXIT_SUCCESS;
}