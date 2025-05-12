//include libraries: 
#include <stdio.h>


//Creating ram: 
int** ram;

void ram_initialize(){
ram = malloc(2048 * sizeof(int*));
for (int i=0;i<2048;i++){
  ram[i]=malloc(32*sizeof(int));
}
for (int j=0; j<2048;j++){
  for(int k=0;k<32;k++){
    ram[j][k]=0;
  }
}
}


void init(){
  ram_initialize();
}


int main(){

  init();
  printf("Memory: %d",ram[200][5]);


  return 0;

}