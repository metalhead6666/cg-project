#include "stage.h"

Stage::Stage(){
    this->fov = 70.0;
    this->aspect = (GLdouble)(WSCREEN / HSCREEN);
    this->near = 1.0;
    this->far = 1000.0;

    this->r_vision = 3.0;
    this->a_vision = 0.5 * PI;
    this->inc_vision = 0.04;

    this->obs_begin.x = 0.0;
    this->obs_begin.y = 1.2;
    this->obs_begin.z = -1.0;

    this->obs_end.x = this->obs_begin.x - this->r_vision * cos(this->a_vision);
    this->obs_end.y = this->obs_begin.y;
    this->obs_end.z = this->obs_begin.z - this->r_vision * sin(this->a_vision);
}

Stage::~Stage(){

}

GLvoid Stage::start_stage(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WSCREEN, HSCREEN);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - WSCREEN) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - HSCREEN) / 2);
    glutCreateWindow("Crappy name ");

    glClearColor(BLACK);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->fov, this->aspect, this->near, this->far);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

GLvoid Stage::display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(this->obs_begin.x, this->obs_begin.y, this->obs_begin.z, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 1.0, 0.0);
    glViewport(0, 0, WSCREEN, HSCREEN);

    this->draw_world();

    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
    glPushMatrix();
        glColor4d(WHITE);
        glBegin(GL_POLYGON);

        glEnd();
    glPopMatrix();
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
