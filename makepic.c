#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void makeimage(){
  int x = open("image.ppm",O_CREAT,O_WRONLY,O_TRUNC);
  char* header = "p3\n255 10 255\n";
  write(x,header,sizeof(header));
  int counter;
  int counter2;
  while(counter < 10){
    while(counter2 < 255){
    }
    counter ++;
  }
}

int main(){
  makeimage();
  return 0;
}
