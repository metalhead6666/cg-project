#include "stage.h"

Stage::Stage(){
    this->wScreen = 1280;
    this->hScreen = 720;

    this->width = 1.0;
    this->height = 1.0;
    this->length = 1.0;

    this->fov = 70.0;
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;
    this->near = 1.0;
    this->far = 1000.0;

    this->r_vision = 3.0;
    this->a_vision = 0.5 * PI;
    this->inc_vision = 0.04;

    this->obs_begin.x = 0.0;
    this->obs_begin.y = 0.0;
    this->obs_begin.z = 0.0;

    this->obs_end.x = this->obs_begin.x;
    this->obs_end.y = this->obs_begin.y;
    this->obs_end.z = -5.0;
}

Stage::~Stage(){

}

GLvoid Stage::start_stage(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(this->wScreen, this->hScreen);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - this->wScreen) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - this->hScreen) / 2);
    glutCreateWindow("Crappy name ");

    glClearColor(BLACK);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

GLvoid Stage::display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->wScreen = glutGet(GLUT_WINDOW_WIDTH);
    this->hScreen = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, this->wScreen, this->hScreen);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->fov, this->aspect, this->near, this->far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(this->obs_begin.x, this->obs_begin.y, this->obs_begin.z, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 1.0, 0.0);

    this->draw_world();

    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
    glPushMatrix();
        glColor4d(WHITE);
        glTranslated(0.0, 0.0, -5.0);
        //glScaled(2.0, 2.0, 2.0);

        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Bottom
                glVertex3d(-width, 0.0, -length);
                glVertex3d(width, 0.0, -length);
                glVertex3d(width, 0.0, 1.0);
                glVertex3d(-width, 0.0, 1.0);
            glEnd();
        glPopMatrix();

        glCullFace(GL_BACK);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Up
                glVertex3d(-width, height, -length);
                glVertex3d(width, height, -length);
                glVertex3d(width, height, 1.0);
                glVertex3d(-width, height, 1.0);
            glEnd();
        glPopMatrix();

        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Left
                glVertex3d(-width, 0.0, -length);
                glVertex3d(-width, 0.0, 1.0);
                glVertex3d(-width, height, 1.0);
                glVertex3d(-width, height, -length);
            glEnd();
        glPopMatrix();

        glCullFace(GL_BACK);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Right
                glVertex3d(width, 0.0, -length);
                glVertex3d(width, 0.0, 1.0);
                glVertex3d(width, height, 1.0);
                glVertex3d(width, height, -length);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glBegin(GL_QUADS);
                //Front
                glVertex3d(-width, 0.0, 1.0);
                glVertex3d(-width, height, 1.0);
                glVertex3d(width, height, 1.0);
                glVertex3d(width, 0.0, 1.0);
            glEnd();
        glPopMatrix();

        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Back
                glVertex3d(-width, 0.0, -length);
                glVertex3d(-width, height, -length);
                glVertex3d(width, height, -length);
                glVertex3d(width, 0.0, -length);
            glEnd();
        glPopMatrix();
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
    (void)x;
    (void)y;
}

GLvoid Stage::click_mouse(GLint button, GLint state){
    (void)button;
    (void)state;
}
