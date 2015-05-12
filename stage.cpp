#include "stage.h"

Stage::Stage(){

}

Stage::~Stage(){

}

GLvoid Stage::start_stage(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WSCREEN, HSCREEN);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WSCREEN) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - HSCREEN) / 2);
    glutCreateWindow("Crappy name ");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glShadeModel(GL_SMOOTH);
}

GLvoid Stage::display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
}

GLvoid Stage::key_pressed(unsigned char key){
    switch(key){
    case 27:
        exit(0);

    default:
        break;
    }
}

GLvoid Stage::key_not_pressed(unsigned char key){
    switch(key){
    default:
        break;
    }
}

GLvoid Stage::special_key_pressed(GLint key){
    switch(key){
    default:
        break;
    }
}

GLvoid Stage::special_key_not_pressed(GLint key){
    switch(key){
    default:
        break;
    }
}

GLvoid Stage::mouse_motion(GLint x, GLint y){

}

GLvoid Stage::click_mouse(GLint button, GLint state){

}
