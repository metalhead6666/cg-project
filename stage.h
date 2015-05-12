#ifndef STAGE_H
#define STAGE_H

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
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

#define WSCREEN 1280
#define HSCREEN 720

#define PI 3.141529

struct observer{
    GLdouble x, y, z;
};

class Stage{
public:
    Stage();
    ~Stage();

    GLvoid start_stage();
    GLvoid display();
    GLvoid draw_world();
    GLvoid key_pressed(unsigned char key);
    GLvoid key_not_pressed(unsigned char key);
    GLvoid special_key_pressed(GLint key);
    GLvoid special_key_not_pressed(GLint key);
    GLvoid mouse_motion(GLint x, GLint y);
    GLvoid click_mouse(GLint button, GLint state);

private:
    struct observer obs_begin;
    struct observer obs_end;
    GLdouble a_vision, r_vision, inc_vision;
    GLdouble fov, aspect, near, far;
};

#endif // STAGE_H
