#include "main.h"

int main(int argc, char **argv){
    glutInit(&argc, argv);

    stage.start_stage();

    glutDisplayFunc(display);
    glutKeyboardFunc(key_pressed);
    glutKeyboardUpFunc(key_not_pressed);
    glutSpecialFunc(special_key_pressed);
    glutSpecialUpFunc(special_key_not_pressed);
    glutMouseFunc(click_mouse);
    glutPassiveMotionFunc(mouse_motion);

    glutMainLoop();

    return 0;
}

GLvoid display(){
    stage.display();
}

GLvoid key_pressed(unsigned char key, GLint x, GLint y){
    (void)x;
    (void)y;
    stage.key_pressed(key);
}

GLvoid key_not_pressed(unsigned char key, GLint x, GLint y){
    (void)x;
    (void)y;
    stage.key_not_pressed(key);
}

GLvoid special_key_pressed(GLint key, GLint x, GLint y){
    (void)x;
    (void)y;
    stage.special_key_pressed(key);
}

GLvoid special_key_not_pressed(GLint key, GLint x, GLint y){
    (void)x;
    (void)y;
    stage.special_key_not_pressed(key);
}

GLvoid mouse_motion(GLint x, GLint y){
    stage.mouse_motion(x, y);
}

GLvoid click_mouse(GLint button, GLint state, GLint x, GLint y){
    (void)x;
    (void)y;
    stage.click_mouse(button, state);
}
