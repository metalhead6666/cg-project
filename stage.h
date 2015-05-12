#ifndef STAGE_H
#define STAGE_H

#define WSCREEN 1280
#define HSCREEN 720

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

class Stage{
public:
    Stage();
    ~Stage();

    GLvoid start_stage();
    GLvoid display();
    GLvoid key_pressed(unsigned char key);
    GLvoid key_not_pressed(unsigned char key);
    GLvoid special_key_pressed(GLint key);
    GLvoid special_key_not_pressed(GLint key);
    GLvoid mouse_motion(GLint x, GLint y);
    GLvoid click_mouse(GLint button, GLint state);
};

#endif // STAGE_H
