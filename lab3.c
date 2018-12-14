#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>

int tagDec;
int setDec;
char* blockChar;

int btoi(char *binary) {
  return (int) strtol(binary, NULL, 2);
}

void parseAddress(char hex[], int addressLength){
  int number = (int)strtol(hex, NULL, 16);
  int count = 0;
  char binary[addressLength];
  int temp;

  //converts decimal to binary
  for(int i = addressLength; i > -1; i--){
    temp = number >> i;
    if (temp & 1){
      binary[count++] = '1';
    }
    else{
      binary[count++] = '0';
    }
  }
  
  binary[addressLength+1] = '\0';

  //parses binary for tag, set id, and block offset
  char* tagT = (char*) malloc(sizeof(char) * (addressLength-8));
  char* setID = (char*) malloc(sizeof(char) * 4);
  char* bOffset = (char*) malloc(sizeof(char) * 4); 

  int i = 0;
  //Creates a tag in decimal
  while(i < addressLength - 7)
    *(tagT + i) = binary[i++];
  tagDec = btoi(tagT);

  //Creates a set id in decimal
  int counter = 0;
  while(i < addressLength-3)
    *(setID + counter++) = binary[i++];
  counter = 0;
  setDec = btoi(setID);

  //Creates block offset in a string
  while(i < addressLength+1)
    *(bOffset + counter++) = binary[i++];
  blockChar = bOffset;

  free(tagT);
  free(setID);
  
}


int main(int argc, char *argv[]){
  
  int opt;
  int m = -1;
  int s = -1;
  int e = -1;
  int b = -1;
  char* error = "[Error] Please enter the following syntax: ./cachelab -m4-s2-e0-b1 \n";

  //Reads the Commands in proper syntax.
  while(-1 != (opt = getopt(argc, argv, "m:s:e:b:"))){
    switch(opt) {
    case 'm':
      for(int i = 0; i < strlen(optarg); i++){
	if(optarg[i] > '9' || optarg[i] < '0'){
	  printf("%s", error);
	  return 1;
	}
      }
      m = atoi(optarg);
      break;

    case 's':
      for(int i = 0; i < strlen(optarg); i++){
	if(optarg[i] > '9' || optarg[i] < '0'){
	  printf("%s", error);
	  return 1;
	}
      }
      s = atoi(optarg);
      break;

    case 'e':
      for(int i = 0; i < strlen(optarg); i++){
	if(optarg[i] > '9' || optarg[i] < '0'){
	  printf("%s", error);
	  return 1;
	}
      }
      e = atoi(optarg);
      break;

    case 'b':
      for(int i = 0; i < strlen(optarg); i++){
	if(optarg[i] > '9' || optarg[i] < '0'){
	  printf("%s", error);
	  return 1;
	}
      }
      b = atoi(optarg);
      break;
      
    default:
      printf("%s",error);
      return 1;
      break;
    }
  }

  //Raises 2 to the power of e, s, and b respectively.
  if((e > -1) & (s > -1) & (b > -1) & (m > -1)){
    s = pow(2,s);
    e = pow(2,e);
    b = pow(2,b);
  }else{
    printf("%s", error);
    return 1;
  }

  //Creates the cache struct.
  typedef struct cacheLine{
    int tag;
    int setIndex;
    char* block;
    int valid;
    int lru;
  }cacheLine;

  //creates the cache and assigns an lru counter and valid bit to each cache line.
  struct cacheLine cache[s][e];

  //Initializes every counter and valid bit to 0
  int j = 0;
  for(int i = 0; i < s; i++){
    while(j < e){
      cache[i][j].lru = 0;
      cache[i][j++].valid = 0;
    }
    j = 0;
  }

  FILE *fp;
  char str[5];
  char* filename = "address.txt";
  char offset[4];
  int hits=0;
  int misses=0;
  int missRate=0;
  int cycles=0;
  int evictions=0;
  
  //opens the input file.
  fp = fopen(filename, "r");
  if (fp == NULL){
    printf("Can't open %s",filename);
    return 1;
  }
  
  while (fgets(str,5, fp) != NULL){
    int missBool = 1;
    int hitBool = 0;
    parseAddress(str,m-s-e-b);
    for(int i = 0; i < e; i++){
      //If a hit, evict if needed unless its a perfect match
      if((cache[setDec][i].valid == 1) & (cache[setDec][i].tag == tagDec)){
	hits = hits +1;
	cycles = cycles +1;
	if(cache[setDec][i].block != blockChar)	cache[setDec][i].block = blockChar;		
	missBool = 0;
	hitBool = 1;
      }
      cache[setDec][i].lru = cache[setDec][i].lru + 1;
    }
    if(missBool = 1){
      //checks lru to see if its the biggest
      int biggest = cache[setDec][0].lru;
      for(int i = 0; i < e; i++){
	if(biggest < cache[setDec][i].lru){
	  biggest = cache[setDec][i].lru;
	}
      }
      int found = 0;
      for(int i = 0; i < e ; i++){
	if(cache[setDec][i].lru == biggest & found == 0){
	  cache[setDec][i].lru = cache[setDec][i].lru +1;
	  cache[setDec][i].valid = 1;
	  cache[setDec][i].tag = tagDec;
	  cache[setDec][i].block = blockChar;
	  evictions++;
	  found++;
	}
      }
    }
      
    int i =0;
    //Prints if a hit or miss
    if(hitBool == 1){
      i = 0;
      while(str[i] != '\n')
	printf("%c",str[i++]);
      printf(" H\n");
    }else{
      i = 0;
      while(str[i] != '\n')
	printf("%c",str[i++]);
      printf(" M\n");
      misses++;
    }
    free(blockChar);
  }

  cycles = (hits * 1) + (misses * 101);
  missRate = (int)(100 - (int)100*(((float)hits / (float)evictions)));
  //Prints hits,misses,miss rate,cycles.
  printf("[result] Hits: %d Misses: %d Miss Rate: %d%c Total running time: %d cycles\n",hits,misses,missRate,'%',cycles);
  fclose(fp); //closes file
  return 1;
}
