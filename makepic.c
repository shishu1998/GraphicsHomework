#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int randNum(){
  int x = open("/dev/random",O_RDONLY);
  int *rand = (int*)malloc(4);
  read(x,rand,4);
  close(x);
  return rand % 255;
}

void makeimage(){
  int x = open("image.ppm",O_CREAT,O_WRONLY,O_TRUNC);
  char* header = "p3\n255 10 255\n";
  write(x,header,sizeof(header));
  char str[15];
  int counter;
  int counter2;
  while(counter2 < 10){
    while(counter < 255){
      counter ++;
    }
    counter ++;
  }
}

int main(){
  makeimage();
  return 0;
}
