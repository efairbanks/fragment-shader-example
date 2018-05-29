#include <stdio.h>
#include "frag.h"

int main(int argc,char** argv){
  Frag::Init(argc,argv,NULL,NULL,NULL);
  // --- //
  Frag::Load("simple.frag");
  // --- //
  printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
  // --- //
  Frag::EventLoop();
  return 0;
}
