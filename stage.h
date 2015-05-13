#ifndef STAGE_H
#define STAGE_H

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <iostream>
#include <cmath>

/* colors */
#define BLUE			 0.0, 0.0, 1.0, 1.0
#define RED				 1.0, 0.0, 0.0, 1.0
#define YELLOW			 1.0, 1.0, 0.0, 1.0
#define GREEN			 0.0, 0.8, 0.0, 1.0
#define ORANGE			 1.0, 0.5, 0.1, 1.0
#define BROWN			 0.3, 0.2, 0.0, 1.0
#define WHITE			 1.0, 1.0, 1.0, 1.0
#define BLACK			 0.0, 0.0, 0.0, 1.0

/* math values */
#define PI 3.141529

/* debug defines */
#define DEBUG_CAMERA_ROTATION

struct observer{
    GLdouble x, y, z;
};

class Stage{
public:
    /* constructor */
    Stage();
    /* destructor */
    ~Stage();

    /* functions */
    GLvoid start_stage();
    GLvoid display();
    GLvoid key_pressed(unsigned char key);
    GLvoid key_not_pressed(unsigned char key);
    GLvoid special_key_pressed(GLint key);
    GLvoid special_key_not_pressed(GLint key);

private:
    /* functions */
    GLvoid draw_world();
    GLvoid draw_character();
    GLvoid keyboard();

    /* variables */
    GLsizei wScreen, hScreen;

    GLdouble angle, radium;
    GLdouble fov, aspect, near, far;
    GLdouble width, height, length;
    struct observer obs_begin;
    struct observer obs_end;

    GLboolean front, back, left, right, left_camera, right_camera;
};

#endif // STAGE_H
