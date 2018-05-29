#ifndef FRAG_H
#define FRAG_H

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

class Frag{
  public:
    static int _uniform_resolution;
    static int _uniform_time;
    static double _time;
    // --- //
    static char _ERROR_BUF[2048];
    // --- //
    static int _window_id;
    static int _program_id;
    static int _fragment_id;
    static void (*_reshape)(int,int);
    static void (*_display)();
    static void (*_timer)(int);
    static void _default_reshape(int width, int height){
      if(_uniform_resolution!=-1) glUniform2f(_uniform_resolution,width,height);
      glViewport(0,0,width,height);
    }
    static void _default_display(void){
      glClear(GL_COLOR_BUFFER_BIT);
      glUseProgram(_program_id);
      glRecti(-1,-1,1,1);
      glutSwapBuffers();
    }
    static void _default_timer(int i){
      glutPostRedisplay();
      _time+=i*0.001;
      if(_uniform_time!=-1) glUniform1f(_uniform_time,_time);
      glutTimerFunc(i,_timer,i);
    }
    static void Init(int argc,char** argv,void (*reshape)(int,int),void (*display)(),void (*timer)(int)){
      _reshape=reshape==NULL?_default_reshape:reshape;
      _display=display==NULL?_default_display:display;
      _timer=timer==NULL?_default_timer:timer;
      glutInit(&argc, argv);
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
      _window_id=glutCreateWindow("#justfragmentshaderthings");
      glClearColor(0,0,0,1);
      _program_id = glCreateProgram();
      if(!_program_id) printf("glCreateProgram Error\n");
      _uniform_resolution=-1;
      _uniform_time=-1;
      glutReshapeFunc(_reshape);
      glutDisplayFunc(_display);
      _timer(1000/60);
    }
    static void Load(char* file_name){
      char *f;
      long input_file_size;
      FILE *input_file = fopen(file_name, "rb");
      fseek(input_file, 0, SEEK_END);
      input_file_size = ftell(input_file);
      rewind(input_file);
      f = (char*)malloc((input_file_size + 1) * (sizeof(char)));
      fread(f, sizeof(char), input_file_size, input_file);
      fclose(input_file);
      f[input_file_size] = 0;
      int flen=strlen(f);
      int _fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
      if(!_fragment_id) printf("glCreateShader Error\n");
      glShaderSource(_fragment_id,1,&f,NULL);
      glCompileShader(_fragment_id);
      // --- //
      GLint success = 0;
      glGetShaderiv(_fragment_id, GL_COMPILE_STATUS, &success);
      printf("success: %d\n",success);
      GLint logSize = 0;
      glGetShaderiv(_fragment_id, GL_INFO_LOG_LENGTH, &logSize);
      memset(_ERROR_BUF,0,2048*sizeof(char));
      glGetShaderInfoLog(_fragment_id,logSize,NULL,_ERROR_BUF);
      printf("ERROR: %s",_ERROR_BUF);
      // --- //
      glAttachShader(_program_id, _fragment_id);
      glLinkProgram(_program_id);
      glUseProgram(_program_id);
      _uniform_resolution=Frag::GetUniformID("resolution");
      _uniform_time=Frag::GetUniformID("time");
      free(f);
    }
    static int GetUniformID(char* name){
      return glGetUniformLocation(_program_id,name);
    }
    static void EventLoop(){glutMainLoop();}
};
int Frag::_uniform_resolution;
int Frag::_uniform_time;
double Frag::_time;
// --- //
char Frag::_ERROR_BUF[2048];
// --- //
int Frag::_window_id;
int Frag::_program_id;
int Frag::_fragment_id;
void (*Frag::_reshape)(int,int);
void (*Frag::_display)();
void (*Frag::_timer)(int);

#endif
