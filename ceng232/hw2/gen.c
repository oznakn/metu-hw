#include <stdio.h>

void print_input(int n) {
  switch(n) {
    case 0: {
      printf("00");
      break;
    }
    case 1: {
      printf("01");
      break;
    }
    case 2: {
      printf("10");
      break;
    }
    case 3: {
      printf("11");
      break;
    }
  }
}

void print_output(int n) {
  if (n < 0) {
    printf("1 ");
    n *= -1;
  }
  else {
    printf("0 ");
  }

  switch(n) {
    case 0: {
      printf("000");
      break;
    }
    case 1: {
      printf("001");
      break;
    }
    case 2: {
      printf("010");
      break;
    }
    case 3: {
      printf("011");
      break;
    }
    case 4: {
      printf("100");
      break;
    }
    case 5: {
      printf("101");
      break;
    }
    case 6: {
      printf("110");
      break;
    }
    case 7: {
      printf("111");
      break;
    }
  }
}

void print_row(int a ,int b, int r) {
  print_input(a);
  printf(" ");
  print_input(b);
  printf(" ");
  print_output(r);
  printf("\n");
}

int main() {
  printf("+\n");
  
  for (int a = 0; a < 4; a++) {
    for (int b = 0; b < 4; b++) {
      print_row(a, b, a - b);
    }
  }
  printf("\n\n");
  
  return 0;
}
