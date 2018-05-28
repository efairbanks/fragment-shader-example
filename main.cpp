#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#endif
int programId;
char* loadFile(char* file_name){
  char *file_contents;
  long input_file_size;
  FILE *input_file = fopen(file_name, "rb");
  fseek(input_file, 0, SEEK_END);
  input_file_size = ftell(input_file);
  rewind(input_file);
  file_contents = (char*)malloc((input_file_size + 1) * (sizeof(char)));
  fread(file_contents, sizeof(char), input_file_size, input_file);
  fclose(input_file);
  file_contents[input_file_size] = 0;
  return file_contents;
}
int uniform_resolution;
int uniform_time;
double time=0;
void reshape(int width, int height){
  if(uniform_resolution!=-1) glUniform2f(uniform_resolution,width,height);
  glViewport(0,0,width,height);
}
void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(programId);
  glRecti(-1,-1,1,1);
  //glUseProgram(0);
  glutSwapBuffers();
}
void timer(int i){
  glutPostRedisplay();
  time+=i*0.001;
  if(uniform_time!=-1) glUniform1f(uniform_time,time);
  glutTimerFunc(i,timer,i);
}
int main(int argc,char** argv){
  char* f=loadFile("simple.fragment");
  int flen=strlen(f);
  // --- //
  printf("%s\n\n",f);
  glutInit(&argc, argv);
  int windowID=glutCreateWindow("single triangle");
  glClearColor(0,0,0,1);
  // --- //
  programId = glCreateProgram();
  if(!programId) printf("glCreateProgram Error\n");
  int fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
  if(!fragmentHandle) printf("glCreateShader Error\n");
  glShaderSource(fragmentHandle,1,&f,NULL);
  glCompileShader(fragmentHandle);
  glAttachShader(programId, fragmentHandle);
  glLinkProgram(programId);
  glUseProgram(programId);
  uniform_resolution=glGetUniformLocation(programId,"resolution");
  uniform_time=glGetUniformLocation(programId,"time");
  // --- //
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  timer(1000/60);
  glutMainLoop(); 
  return 0;
}
