#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
Project by Andrew Boyce
*/

void *palin(char* test)
{
  char* temp = malloc(sizeof(test));
  char* temp2 = malloc(sizeof(test));
  int i = 0, j =0, k = 0;//int variables

  while (test[i] != '\0')
    i++;
 
  while(k <= i-1 & j <= i-1){
    if(!isspace(test[k]) & ((test[k] >= 'a' & test[k] <= 'z') | (test[k] >= 'A' & test[k] <= 'Z')))//checks if it's a letter or a space
      temp[j++] = tolower(test[k++]);
    else
      k++;
    
  }
  temp[k+1] = '\0';

  
  while (temp[i] != '\0')
    i++;
  k=0;
  j = i-1;
  i=j;
  
   while(k <= j & i > -1){
    if(!isspace(temp[i]) & ((temp[i] >= 'a' & temp[i] < 'z') | (temp[i] >= 'A' & temp[i] < 'Z')))//checks if it's a letter or a space
      temp2[k++] = temp[i--];
    else
      i--;
    
  }
  temp2[k+1] = '\0';

  i=0;
 while (test[i] != '\0')
    i++;
 j = i-1;
  
  if (strcmp(temp,temp2) == 0){
    while(j > -1)
    printf("%c", test[j--]);
  printf(" <- Palindrome\n");
  }
  else{
    while(j > -1)
    printf("%c", test[j--]);
    printf(" <- not Palindrome\n");
  }
}

void *cascnvt(char *test)
{
  int k = 0, j=0, i = 0;
  while(test[k] != '\0')
    k++;
  j = k;

  while(i < j){
    if (test[i] >= 'a' & test[i] <= 'z'){
      printf("%c",toupper(test[i++]));
    }
    else if (test[i] >= 'A' & test[i] <= 'Z'){
      printf("%c",tolower(test[i++]));
    }
    else{
      printf("%c", test[i++]);
    }
  }
  printf("\n");
}


void *upper(char *test)
{
  int k = 0, j=0, i = 0;
  while(test[k] != '\0')
    k++;
  j = k;

  while(i < j){
    if (test[i] >= 'a' & test[i] <= 'z'){
      printf("%c",toupper(test[i++]));
    }
    else{
      printf("%c", test[i++]);
    }
  }
  printf("\n");
}

void *lower(char *test)
{
  int k = 0, j=0, i = 0;
  while(test[k] != '\0')
    k++;
  j = k;

  while(i < j){
    if (test[i] >= 'A' & test[i] <= 'Z'){
      printf("%c",tolower(test[i++]));
    }
    else{
      printf("%c", test[i++]);
    }
  }
  printf("\n");
}

int main()
{
  char command[50];
  printf("Please enter a command\n");
  printf("aboyce>$ ");
  fgets(command, sizeof(command), stdin);// Takes user input
  char* tCommand = strtok(command, "(");//tokenizes first part
  char*tInput = strtok(NULL, "()");//Tokenizes second part
  
  while(strcmp(tCommand, "exit\n") != 0 & strcmp(command,"exit\n") != 0)
    {
      if(strcmp(tCommand, "palin") == 0)
	{
	  palin(tInput);//void with printf's in the method.
	  printf("Please enter a command\n");
	  printf("aboyce>$ ");
	  fgets(command, sizeof(command), stdin);
	  tCommand = strtok(command, "(");
	  tInput = strtok(NULL, "()");
	}

      else if(strcmp(tCommand, "cascnvt") == 0)
	{
	  cascnvt(tInput);//prints the word converted
	  printf("Please enter a command\n");
	  printf("aboyce>$ ");
	  fgets(command, sizeof(command), stdin);
	  tCommand = strtok(command, "(");
	  tInput = strtok(NULL, ")");
	}
      else if(strcmp(tCommand, "upper") == 0)
	{
	  upper(tInput);//prints upper case
	  printf("Please enter a command\n");
	  printf("aboyce>$ ");
	  fgets(command, sizeof(command), stdin);
	  tCommand = strtok(command, "(");
	  tInput = strtok(NULL, ")");
	}
      else if(strcmp(tCommand, "lower") == 0)
	{
	  lower(tInput);//lower for all
	  printf("Please enter a command\n");
	  printf("aboyce>$ ");
	  fgets(command, sizeof(command), stdin);
	  tCommand = strtok(command, "(");
	  tInput = strtok(NULL, ")");
	}
	else
	{
	  printf("[Error]Please enter a valid command\n");
	  printf("aboyce>$ ");
	  fgets(command, sizeof(command), stdin);
	  tCommand = strtok(command, "(");
	  tInput = strtok(NULL, ")");
	}
    }
  
  return 0;
}
