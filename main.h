#ifndef MAIN_H
#define MAIN_H

#include "stage.h"

Stage stage;

GLvoid display();
GLvoid key_pressed(unsigned char key, GLint x, GLint y);
GLvoid key_not_pressed(unsigned char key, GLint x, GLint y);
GLvoid special_key_pressed(GLint key, GLint x, GLint y);
GLvoid special_key_not_pressed(GLint key, GLint x, GLint y);
GLvoid mouse_motion(GLint x, GLint y);
GLvoid click_mouse(GLint button, GLint state, GLint x, GLint y);

#endif // MAIN_H

