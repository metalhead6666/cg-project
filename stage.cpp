#include "stage.h"

Stage::Stage(){
    this->wScreen = 1280;
    this->hScreen = 720;

    this->width = 2.0;
    this->height = 2.0;
    this->length = 10.0;

    this->angle = 0.0;
    this->radium = 5.0;

    this->fov = 70.0;
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;
    this->near = 1.0;
    this->far = 1000.0;

    this->obs_begin.x = 0.0;
    this->obs_begin.y = 0.0;
    this->obs_begin.z = 0.0;

    this->obs_end.x = this->obs_begin.x;
    this->obs_end.y = this->obs_begin.y;
    this->obs_end.z = -5.0;

    this->front = false;
    this->back = false;
    this->left = false;
    this->right = false;
}

Stage::~Stage(){

}

GLvoid Stage::start_stage(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(this->wScreen, this->hScreen);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - this->wScreen) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - this->hScreen) / 2);
    glutCreateWindow("Crappy name "); /* TODO: change this name */

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
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;

    glViewport(0, 0, this->wScreen, this->hScreen);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->fov, this->aspect, this->near, this->far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(this->obs_begin.x, this->obs_begin.y, this->obs_begin.z, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 1.0, 0.0);

    this->draw_world();
    this->draw_character();
    this->keyboard();

    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
    glPushMatrix();
        glColor4d(WHITE);
        glTranslated(0.0, -0.7, 1.0);

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
            glColor4d(BLUE);
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
            glColor4d(YELLOW);
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
            glColor4d(GREEN);
            glBegin(GL_QUADS);
                //Right
                glVertex3d(width, 0.0, -length);
                glVertex3d(width, 0.0, 1.0);
                glVertex3d(width, height, 1.0);
                glVertex3d(width, height, -length);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glColor4d(ORANGE);
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
            glColor4d(RED);
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

GLvoid Stage::draw_character(){
    glPushMatrix();
        glColor4d(BLACK);
        glRotated(this->radium * cos(this->angle), 0.0, 0.0, 1.0);
        glTranslated(0.0, -0.5, -1.5);
        glutSolidSphere(0.15, 25, 25);
    glPopMatrix();
}

GLvoid Stage::keyboard(){
    if(this->front){
        if(this->obs_end.z < 0.0){
            this->obs_begin.z -= 0.1;
            this->obs_end.z -= 0.1;
        }

        else{
            this->obs_begin.z += 0.1;
            this->obs_end.z += 0.1;
        }
    }

    if(this->back){
        if(this->obs_end.z < 0.0){
            this->obs_begin.z += 0.1;
            this->obs_end.z += 0.1;
        }

        else{
            this->obs_begin.z -= 0.1;
            this->obs_end.z -= 0.1;
        }
    }

    if(this->left){
        if(this->obs_end.x < 0.0){
            this->obs_begin.x -= 0.03;
            this->obs_end.x -= 0.03;
        }

        else{
            this->obs_begin.x += 0.03;
            this->obs_end.x += 0.03;
        }
    }

    if(this->right){
        if(this->obs_end.x < 0.0){
            this->obs_begin.x += 0.03;
            this->obs_end.x += 0.03;
        }

        else{
            this->obs_begin.x -= 0.03;
            this->obs_end.x -= 0.03;
        }
    }

    if(this->left_camera){
        this->angle -= 0.03;
        this->obs_end.x = this->radium * cos(this->angle);
        this->obs_end.z = this->radium * sin(this->angle);

#ifdef DEBUG_CAMERA_ROTATION
        std::cout << "LEFT: \n";
        std::cout << "OBS END - X: " << this->obs_end.x;
        std::cout << "\nOBS END - Z: " << this->obs_end.z << "\n\n";
#endif

    }

    if(this->right_camera){
        this->angle += 0.03;
        this->obs_end.x = this->radium * cos(this->angle);
        this->obs_end.z = this->radium * sin(this->angle);

#ifdef DEBUG_CAMERA_ROTATION
        std::cout << "RIGHT: \n";
        std::cout << "OBS END - X: " << this->obs_end.x;
        std::cout << "\nOBS END - Z: " << this->obs_end.z << "\n\n";
#endif
    }

    glutPostRedisplay();
}

GLvoid Stage::key_pressed(unsigned char key){
    switch(key){
    case 'w':
        this->front = true;
        break;

    case 's':
        this->back = true;
        break;

    case 'a':
        this->left = true;
        break;

    case 'd':
        this->right = true;
        break;

    case 27:
        exit(0);

    default:
        break;
    }
}

GLvoid Stage::key_not_pressed(unsigned char key){
    switch(key){
    case 'w':
        this->front = false;
        break;

    case 's':
        this->back = false;
        break;

    case 'a':
        this->left = false;
        break;

    case 'd':
        this->right = false;
        break;

    default:
        break;
    }
}

GLvoid Stage::special_key_pressed(GLint key){
    switch(key){
    case GLUT_KEY_LEFT:
        this->left_camera = true;
        break;

    case GLUT_KEY_RIGHT:
        this->right_camera = true;
        break;

    default:
        break;
    }
}

GLvoid Stage::special_key_not_pressed(GLint key){
    switch(key){
    case GLUT_KEY_LEFT:
        this->left_camera = false;
        break;

    case GLUT_KEY_RIGHT:
        this->right_camera = false;
        break;

    default:
        break;
    }
}
