#include "stage.h"

Stage *s_stage;

Stage::Stage(){
    srand(time(NULL));
    this->init();
}

Stage::~Stage(){

}

GLvoid Stage::init(){
    this->wScreen = 1280;
    this->hScreen = 720;

    this->pause_game = false;

    this->width = 10.0;
    this->height = 2.0;
    this->length = 5.0;

    this->board_width = 8.0;
    this->board_length = 4.0;
    this->board_height = 1.0;

    this->racket_width = 0.05;
    this->racket_length_left = 1.0;
    this->racket_length_right = 1.0;
    this->racket_height = 0.5;
    this->left_racket_move = 0.0;
    this->right_racket_move = 0.0;

#ifdef GOD_MODE
    this->ball_going_down = 0.75;
    this->observer_position = false;
    this->timer_value = -1;
#else
    this->ball_going_down = 1.75;
    this->observer_position = true;
    this->timer_value = 3;
#endif

    this->actual_speed_x = 0.03;
    this->actual_speed_z = 0.03;
    this->ball_pos_x = 0.0;
    this->ball_pos_z = 0.0;
    this->ball_speed_x = actual_speed_x;
    this->ball_speed_z = actual_speed_z;

    this->angle = 0.0;
    this->radium = 5.0;

    this->fov = 70.0;
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;
    this->near_p = 1.0;
    this->far_p = 1000.0;

    this->obs_begin.x = 0.0;
    this->obs_begin.y = 10.0;
    this->obs_begin.z = 0.0;

    this->obs_end.x = this->obs_begin.x;
    this->obs_end.y = 0.0;
    this->obs_end.z = this->obs_begin.z;

    this->front = false;
    this->back = false;
    this->left = false;
    this->right = false;

    this->player_one_points = 0;
    this->player_two_points = 0;

    this->racket_speed_left = 0.3;
    this->racket_speed_right = 0.3;

    this->ballRotate = 0.0;

    this->tempRotateX = 0.0;
    this->tempRotateY = 0.0;
    this->tempRotateZ = 0.0;

    this->powerup_type = -1;
    this->player_powerup = NO_PLAYER;
    this->can_create_powerup = false;
    this->powerup_x = -10.0;
    this->powerup_z = -10.0;

    this->exit_game = false;

    s_stage = this;
}

GLvoid Stage::start_stage(){
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(this->wScreen, this->hScreen);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - this->wScreen) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - this->hScreen) / 2);
    glutCreateWindow("Crappy name "); /* TODO: change this name */

    glClearColor(BLACK);
    this->loadTextures();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

GLvoid Stage::display(){
    char *aux;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    this->wScreen = glutGet(GLUT_WINDOW_WIDTH);
    this->hScreen = glutGet(GLUT_WINDOW_HEIGHT);
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;

    glViewport(0, 0, this->wScreen, this->hScreen);

    if(this->timer_value == -1){
        writePoints();
    }

    else{
        aux = (char *)calloc(1, sizeof(char) * 2);
        sprintf(aux, "%d", timer_value);
        writeText(aux, 2.0, 1.5);
        free(aux);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(this->fov, this->aspect, this->near_p, this->far_p);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(observer_position){
        gluLookAt(5.0, 1.0, 0.0, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 1.0, 0.0);
    }

    else{
        gluLookAt(this->obs_begin.x, this->obs_begin.y, this->obs_begin.z, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 0.0, 1.0);
    }

    glPushMatrix();

    if(this->exit_game){
        writeEnd();

        glRotated(tempRotateX, 1.0, 0.0, 0.0);
        glRotated(tempRotateY, 0.0, 1.0, 0.0);
        glRotated(tempRotateZ, 0.0, 0.0, 1.0);
    }

    else if(this->pause_game){
        aux = (char *)calloc(1, sizeof(char) * 2);
        strcpy(aux, "PAUSED");
        writeText(aux, 1.0, 1.0);
        free(aux);

        glRotated(tempRotateX, 1.0, 0.0, 0.0);
        glRotated(tempRotateY, 0.0, 1.0, 0.0);
        glRotated(tempRotateZ, 0.0, 0.0, 1.0);
    }

    else{
        if(this->timer_value == -1){
            this->keyboard();
        }
    }

    this->draw_world();
    this->draw_board();
    this->draw_character();

    if(this->can_create_powerup){
        this->draw_powerup();
    }

    glPopMatrix();
    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
    glEnable(GL_CULL_FACE);
    glPushMatrix();
        //glColor4d(WHITE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_environment);
        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Bottom
                glTexCoord2f(0.0f, 0.0f);
                glVertex3d(-width, 0.0, -length);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3d(width, 0.0, -length);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3d(width, 0.0, length);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3d(-width, 0.0, length);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_environment);
        glCullFace(GL_BACK);
        glPushMatrix();
            //glColor4d(BLUE);
            glBegin(GL_QUADS);
                //Up
                glTexCoord2f(0.0f, 0.0f);
                glVertex3d(-width, height, -length);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3d(width, height, -length);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3d(width, height, length);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3d(-width, height, length);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glCullFace(GL_FRONT);
        glPushMatrix();
            glColor4d(BROWN);
            glBegin(GL_QUADS);
                //Left
                glVertex3d(-width, 0.0, -length);
                glVertex3d(-width, 0.0, length);
                glVertex3d(-width, height, length);
                glVertex3d(-width, height, -length);
            glEnd();
        glPopMatrix();

        glCullFace(GL_BACK);
        glPushMatrix();
            glColor4d(GREEN);
            glBegin(GL_QUADS);
                //Right
                glVertex3d(width, 0.0, -length);
                glVertex3d(width, 0.0, length);
                glVertex3d(width, height, length);
                glVertex3d(width, height, -length);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glColor4d(ORANGE);
            glBegin(GL_QUADS);
                //Front
                glVertex3d(-width, 0.0, length);
                glVertex3d(-width, height, length);
                glVertex3d(width, height, length);
                glVertex3d(width, 0.0, length);
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

GLvoid Stage::draw_board(){
    glPushMatrix();
        //glColor4d(BLUE);
        if(this->pause_game){
            glDisable(GL_CULL_FACE);
        }

        else{
           glEnable(GL_CULL_FACE);
        }

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_floor);
        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Bottom
                glTexCoord2f(0.0, 0.0); glVertex3d(-board_width, 0.5, -board_length);
                glTexCoord2f(10.0, 0.0); glVertex3d(board_width, 0.5, -board_length);
                glTexCoord2f(10.0, 10.0); glVertex3d(board_width, 0.5, board_length);
                glTexCoord2f(0.0, 10.0); glVertex3d(-board_width, 0.5, board_length);
            glEnd();
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    
        //RIGHT RACKET
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_player[0]);
        glCullFace(GL_FRONT);
        glPushMatrix();
            //glColor4d(GREEN);
            glTranslated(-board_width, 0.0, right_racket_move);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, -racket_length_right);
                    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.0, racket_length_right);
                    glTexCoord2f(1.0, 1.0); glVertex3d(0.0, board_height, racket_length_right);
                    glTexCoord2f(0.0, 1.0); glVertex3d(0.0, board_height, -racket_length_right);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_player_right[0][0]);
#ifdef DEBUG_PLAYER_RIGHT
            printf("[PLAYER RIGHT] %f %f %f\n", matrix_player_right[3][0], matrix_player_right[3][1], matrix_player_right[3][2]);
#endif
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    
        //LEFT RACKET
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_player[0]);
        glCullFace(GL_BACK);
        glPushMatrix();
            //glColor4d(BROWN);
            glTranslated(board_width, 0.0, left_racket_move);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, -racket_length_left);
                    glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.0, racket_length_left);
                    glTexCoord2f(1.0, 1.0); glVertex3d(0.0, board_height, racket_length_left);
                    glTexCoord2f(0.0, 1.0); glVertex3d(0.0, board_height, -racket_length_left);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_player_left[0][0]);
#ifdef DEBUG_PLAYER_LEFT
            printf("[PLAYER LEFT] %f %f %f\n", matrix_player_left[3][0], matrix_player_left[3][1], matrix_player_left[3][2]);
#endif
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_wall);
        glPushMatrix();
            //glColor4d(ORANGE);
                glBegin(GL_QUADS);
                    //Top
                    glTexCoord2f(0.0, 0.0); glVertex3d(-board_width, 0.5, board_length);
                    glTexCoord2f(10.0, 0.0); glVertex3d(-board_width, board_height, board_length);
                    glTexCoord2f(10.0, 10.0); glVertex3d(board_width, board_height, board_length);
                    glTexCoord2f(0.0, 10.0); glVertex3d(board_width, 0.5, board_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_top[0][0]);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_wall);
        glCullFace(GL_FRONT);
        glPushMatrix();
            //glColor4d(ORANGE);
                glBegin(GL_QUADS);
                    //Bottom
                    glTexCoord2f(0.0, 0.0); glVertex3d(-board_width, 0.5, -board_length);
                    glTexCoord2f(10.0, 0.0); glVertex3d(-board_width, board_height, -board_length);
                    glTexCoord2f(10.0, 10.0); glVertex3d(board_width, board_height, -board_length);
                    glTexCoord2f(0.0, 10.0); glVertex3d(board_width, 0.5, -board_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_bottom[0][0]);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

GLvoid Stage::draw_character(){
    GLUquadricObj* s;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ball[0]);
    glPushMatrix();
        s = gluNewQuadric();
        gluQuadricDrawStyle(s, GLU_FILL);
        gluQuadricNormals(s, GLU_SMOOTH);
        gluQuadricTexture(s, GL_TRUE);
        glTranslated(ball_pos_x, ball_going_down, ball_pos_z);
        ballRotate += 2;
        glRotated(ballRotate, 0.0, 1.0, 0.0);
        gluSphere(s, 0.25, 25, 25);
        gluDeleteQuadric(s);
        glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_ball[0][0]);
    #ifdef DEBUG_SPHERE
        printf("[SPHERE] %lf %lf %lf\n", matrix_ball[3][0], matrix_ball[3][1], matrix_ball[3][2]);
    #endif
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

GLvoid Stage::draw_powerup(){
    GLUquadricObj* s;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_powerup);
    glPushMatrix();
        //glColor4d(BLACK);
        s = gluNewQuadric();
        gluQuadricDrawStyle(s, GLU_FILL);
        gluQuadricNormals(s, GLU_SMOOTH);
        gluQuadricTexture(s, GL_TRUE);

        if(this->powerup_x == -10.0){
            this->powerup_x = this->randomGenerator(-7.0, 7.0);
            this->powerup_z = this->randomGenerator(-3.0, 3.0);
        }

        glTranslated(this->powerup_x, ball_going_down, this->powerup_z);

        gluSphere(s, 0.25, 25, 25);
        gluDeleteQuadric(s);
        glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_powerup[0][0]);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

GLvoid Stage::loadTextures(){
    //*********BALL*********
    //Wood
    glGenTextures(1, &texture_ball[0]);
    glBindTexture(GL_TEXTURE_2D, texture_ball[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Ball/ball0.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
    //Glass
    glGenTextures(1, &texture_ball[1]);
    glBindTexture(GL_TEXTURE_2D, texture_ball[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Ball/ball1.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //Rock
    glGenTextures(1, &texture_ball[2]);
    glBindTexture(GL_TEXTURE_2D, texture_ball[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Ball/ball2.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //*********FLOOR*********
    glGenTextures(1, &texture_floor);
    glBindTexture(GL_TEXTURE_2D, texture_floor);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("ProjectTextures/Floor/floor2.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //*********WALLS*********
    glGenTextures(1, &texture_wall);
    glBindTexture(GL_TEXTURE_2D, texture_wall);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    imag.LoadBmpFile("ProjectTextures/Wall/brick2.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //*********PLAYERS*********

    //Wood
    glGenTextures(1, &texture_player[0]);
    glBindTexture(GL_TEXTURE_2D, texture_player[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Player/player0.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //Glass
    glGenTextures(1, &texture_player[1]);
    glBindTexture(GL_TEXTURE_2D, texture_player[1]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Player/player1.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //Rock
    glGenTextures(1, &texture_player[2]);
    glBindTexture(GL_TEXTURE_2D, texture_player[2]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Player/player2.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());


    //*********ENVIRONMENT*********

    glGenTextures(1, &texture_environment);
    glBindTexture(GL_TEXTURE_2D, texture_environment);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Environment/environment.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //*********POWERUP*********
    glGenTextures(1, &texture_powerup);
    glBindTexture(GL_TEXTURE_2D, texture_powerup);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Powerup/powerup.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());
}

GLvoid Stage::keyboard(){
    if(this->front){
        if(left_racket_move < 2.75){
            this->left_racket_move += this->racket_speed_left;
        }
    }

    if(this->back){
        if(left_racket_move > -2.75){
            this->left_racket_move -= this->racket_speed_left;
        }
    }

    if(this->down_arrow){
        if(right_racket_move > -2.75){
            this->right_racket_move -= this->racket_speed_right;
        }
    }

    if(this->up_arrow){
        if(right_racket_move < 2.75){
            this->right_racket_move += this->racket_speed_right;
        }
    }

    glutPostRedisplay();
}

GLvoid Stage::key_pressed(unsigned char key){
    switch(key){
    case 'w':
    case 'W':
        this->front = true;
        break;

    case 's':
    case 'S':
        this->back = true;
        break;

    case 'p':
    case 'P':
        if(!this->exit_game){
            this->pause_game = !this->pause_game;
        }

        break;

    case 'n':
    case 'N':
        if(this->exit_game){
            exit(0);
        }

        break;

    case 'y':
    case 'Y':
        if(this->exit_game){
            this->init();
        }

        break;

    case 27:
        exit(0);

    default:
        break;
    }

    glutPostRedisplay();
}

GLvoid Stage::key_not_pressed(unsigned char key){
    switch(key){
    case 'w':
    case 'W':
        this->front = false;
        break;

    case 's':
    case 'S':
        this->back = false;
        break;

    default:
        break;
    }
}

GLvoid Stage::special_key_pressed(GLint key){
    switch(key){
    case GLUT_KEY_DOWN:
        this->down_arrow = true;
        break;

    case GLUT_KEY_UP:
        this->up_arrow = true;
        break;

    default:
        break;
    }
}

GLvoid Stage::special_key_not_pressed(GLint key){
    switch(key){
    case GLUT_KEY_DOWN:
        this->down_arrow = false;
        break;

    case GLUT_KEY_UP:
        this->up_arrow = false;
        break;

    default:
        break;
    }
}

GLvoid Stage::writeText(char *text, GLdouble posX, GLdouble posY){
    const char *c;

    glColor4d(PINK);
    glPushMatrix();
        glRasterPos2d(posX, posY);

        for(c = text; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    glPopMatrix();
}

GLvoid Stage::writeEnd(){
    const char *c;
    char *text;

    text = (char *)calloc(1, sizeof(char) * 17);

    glColor4d(PINK);
    glPushMatrix();
        glRasterPos2d(1.0, 1.0);

        if(this->player_one_points == MAX_SCORE){
            strcpy(text, "Player One Wins!");
        }

        else{
            strcpy(text, "Player Two Wins!");
        }

        for(c = text; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }

        strcpy(text, "Continue? (y/n)");
        glTranslated(0.0, 0.0, -0.5);
        glRasterPos2d(1.0, 1.0);

        for(c = text; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    glPopMatrix();

    free(text);
}

GLvoid Stage::writePoints(){
    const char *c;
    char *one, *two;

    one = (char *)calloc(1, sizeof(char) * 22);
    two = (char *)calloc(1, sizeof(char) * 22);
    sprintf(one, "Player One Score: %d", this->player_one_points);
    sprintf(two, "Player Two Score: %d", this->player_two_points);

    glColor4d(WHITE);
    glPushMatrix();
        glTranslated(0.0, 0.0, 4.12);
        glRasterPos2d(8.0, 1.0);

        for(c = one; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }

        glRasterPos2d(-4.55, 1.0);

        for(c = two; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    glPopMatrix();

    free(one);
    free(two);
}

GLvoid Stage::Timer_ball_going_down(GLint value){
    if(this->pause_game || this->exit_game){
        ++this->tempRotateX;
        ++this->tempRotateY;
        ++this->tempRotateZ;
        glutPostRedisplay();
        glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
    }

    else{
        if(this->player_one_points == MAX_SCORE || this->player_two_points == MAX_SCORE){
            this->exit_game = true;
            glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
        }

        this->tempRotateX = this->tempRotateY = this->tempRotateZ = 0.0;

        if(observer_position){
            timer_value = value;

            if(value == 0){
                observer_position = false;
                timer_value = -1;
                glutPostRedisplay();
                glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
            }

            else if(ball_going_down > 0.75){
                ball_going_down -= 0.01;
                glutPostRedisplay();
                glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
            }

            else{
                glutPostRedisplay();
                glutTimerFunc(1000, &Stage::static_timer_ball_going_down, value - 1);
            }
        }

        else{
    #ifdef DEBUG_MODELVIEW
            printf("[DEBUG] %f %f\n", matrix_ball[3][0], matrix_player_left[3][0]);
    #endif
            if((matrix_ball[3][0] - 0.25 <= matrix_player_left[3][0] &&
               (matrix_ball[3][1] <= matrix_player_left[3][1] + 1.1 &&
                matrix_ball[3][1] >= matrix_player_left[3][1] - 1.1))){

                this->ball_speed_x *= -1.0;
                this->ball_pos_x -= 0.25;
            }

            else if((matrix_ball[3][0] + 0.25 >= matrix_player_right[3][0] &&
                    (matrix_ball[3][1] <= matrix_player_right[3][1] + 1.1 &&
                     matrix_ball[3][1] >= matrix_player_right[3][1] - 1.1))){

                this->ball_speed_x *= -1.0;
                this->ball_pos_x += 0.25;
            }

            if(this->ball_pos_z + 0.25 >= 4.0 || this->ball_pos_z - 0.25 <= -4.0){
                this->ball_speed_z *= -1.0;
            }

            if(matrix_ball[3][0] - 0.25 <= -8.75){
                this->ball_speed_x *= -1.0;
                ++this->player_two_points;

                this->ball_pos_z = randomGenerator(-3.0, 3.0);
                this->ball_speed_z = randomGenerator(-actual_speed_z, actual_speed_z);

                this->ball_pos_x = -ball_speed_x;
                this->ball_pos_z -= ball_speed_z;
            }

            else if(matrix_ball[3][0] + 0.25 >= 8.75){
                this->ball_speed_x *= -1.0;
                ++this->player_one_points;

                this->ball_pos_z = randomGenerator(-3.0, 3.0);
                this->ball_speed_z = randomGenerator(-actual_speed_z, actual_speed_z);

                this->ball_pos_x = -ball_speed_x;
                this->ball_pos_z -= ball_speed_z;
            }

            if(this->powerup_type != -1 && this->can_create_powerup &&
               sqrt(pow(this->matrix_ball[3][0] - this->matrix_powerup[3][0] - 0.1, 2) +
                    pow(this->matrix_ball[3][2] - this->matrix_powerup[3][2] - 0.1, 2)) < 0.3f){

#ifdef DEBUG_POWERUP_TOUCH
                printf("[POWERUP] Touched!\n");
#endif

                switch(this->powerup_type){
                case RACKET_FASTER:
                    if(this->ball_speed_x > 0.0){
                        this->player_powerup = PLAYER_RIGHT;
                        this->racket_speed_right += 0.2;
                    }

                    else{
                        this->player_powerup = PLAYER_LEFT;
                        this->racket_speed_left += 0.2;
                    }

                    break;

                case RACKET_LARGER:
                    if(this->ball_speed_x > 0.0){
                        this->player_powerup = PLAYER_RIGHT;
                        this->racket_length_right += 0.2;
                    }

                    else{
                        this->player_powerup = PLAYER_LEFT;
                        this->racket_length_left += 0.2;
                    }

                    break;

                case RACKET_FRAGILE:
                    break;

                case BALL_FASTER:
                    this->ball_speed_x += 0.05;
                    this->player_powerup = PLAYER_RIGHT;
                    break;

                case BALL_FRAGILE:
                    break;

                case ONE_MORE_BALL:
                    break;

                default:
                    break;
                }

                this->can_create_powerup = false;
                this->powerup_x = -10.0;
                this->powerup_z = -10.0;
            }

            this->ball_pos_x += ball_speed_x;
            this->ball_pos_z += ball_speed_z;
            glutPostRedisplay();
            glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
        }
    }
}

void Stage::static_timer_ball_going_down(GLint value){
    s_stage->Timer_ball_going_down(value);
}

GLvoid Stage::Timer_powerups(GLint value){
    GLint random_value;

    if(this->pause_game || this->exit_game){
        glutTimerFunc(1000, &Stage::static_timer_powerups, value);
    }

    else{
        if(this->powerup_type == -1){
            random_value = this->randomIntGenerator(0, 39);
            random_value = 1;

            if(random_value < 6){
                this->powerup_type = random_value;
                this->can_create_powerup = true;
                glutPostRedisplay();
                glutTimerFunc(5000, &Stage::static_timer_powerups, value);
            }

            else{
                this->can_create_powerup = false;
                glutTimerFunc(1000, &Stage::static_timer_powerups, value);
            }
        }

        else{
            switch(this->powerup_type){
            case RACKET_FASTER:
                if(this->player_powerup == PLAYER_RIGHT){
                    this->racket_speed_right -= 0.2;
                }

                else if(this->player_powerup == PLAYER_LEFT){
                    this->racket_speed_left -= 0.2;
                }

                break;

            case RACKET_LARGER:
                if(this->player_powerup == PLAYER_RIGHT){
                    this->racket_length_right -= 0.2;
                }

                else if(this->player_powerup == PLAYER_LEFT){
                    this->racket_length_left -= 0.2;
                }

                break;

            case RACKET_FRAGILE:
                break;

            case BALL_FASTER:
                if(this->player_powerup != NO_PLAYER){
                    this->ball_speed_x -= 0.05;
                }

                break;

            case BALL_FRAGILE:
                break;

            case ONE_MORE_BALL:
                break;

            default:
                break;
            }

            this->powerup_type = -1;
            this->can_create_powerup = false;
            this->player_powerup = NO_PLAYER;
            this->powerup_x = -10.0;
            this->powerup_z = -10.0;
            glutPostRedisplay();
            glutTimerFunc(1000, &Stage::static_timer_powerups, value);
        }
    }
}

void Stage::static_timer_powerups(GLint value){
    s_stage->Timer_powerups(value);
}

GLdouble Stage::randomGenerator(GLdouble min, GLdouble max){
    GLdouble f = (GLdouble)rand() / RAND_MAX;

    return min + f * (max - min);
}

GLint Stage::randomIntGenerator(GLint min, GLint max){
    return rand() % (max - min + 1) + min;
}
