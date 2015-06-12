#include "stage.h"

Stage *s_stage;

Stage::Stage(){
    this->wScreen = 1280;
    this->hScreen = 720;

    this->width = 10.0;
    this->height = 2.0;
    this->length = 5.0;
    
    this->board_width = 8.0;
    this->board_length = 4.0;
    this->board_height = 1.0;
    
    this->racket_width = 0.05;
    this->racket_length = 1.0;
    this->racket_height = 0.5;
    this->left_racket_move = 0.0;
    this->right_racket_move = 0.0;
    this->ball_going_down = 1.75;
    this->observer_position = true;
    
    this->ball_pos_x = 0.0;
    this->ball_pos_z = 0.0;
    this->ball_speed_x = 0.03;
    this->ball_speed_z = 0.03;

    this->angle = 0.0;
    this->radium = 5.0;

    this->fov = 70.0;
    this->aspect = (GLdouble)this->wScreen / (GLdouble)this->hScreen;
    this->near = 1.0;
    this->far = 1000.0;

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

    s_stage = this;
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

    if(observer_position){
        gluLookAt(5.0, 1.0, 0.0, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 1.0, 0.0);
    }

    else{
        gluLookAt(this->obs_begin.x, this->obs_begin.y, this->obs_begin.z, this->obs_end.x, this->obs_end.y, this->obs_end.z, 0.0, 0.0, 1.0);
    }

    this->draw_world();
    this->draw_board();
    this->draw_character();
    this->keyboard();

    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
    glPushMatrix();
        glColor4d(WHITE);
        //glTranslated(0.0, -0.7, 1.0);

        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Bottom
                glVertex3d(-width, 0.0, -length);
                glVertex3d(width, 0.0, -length);
                glVertex3d(width, 0.0, length);
                glVertex3d(-width, 0.0, length);
            glEnd();
        glPopMatrix();

        glCullFace(GL_BACK);
        glPushMatrix();
            glColor4d(BLUE);
            glBegin(GL_QUADS);
                //Up
                glVertex3d(-width, height, -length);
                glVertex3d(width, height, -length);
                glVertex3d(width, height, length);
                glVertex3d(-width, height, length);
            glEnd();
        glPopMatrix();

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
        glColor4d(BLUE);
        //glTranslated(0.0, -0.7, 1.0);
    
        glCullFace(GL_FRONT);
        glPushMatrix();
            glBegin(GL_QUADS);
                //Bottom
                glVertex3d(-board_width, 0.5, -board_length);
                glVertex3d(board_width, 0.5, -board_length);
                glVertex3d(board_width, 0.5, board_length);
                glVertex3d(-board_width, 0.5, board_length);
            glEnd();
        glPopMatrix();
    
    
        //RIGHT RACKET
        glCullFace(GL_FRONT);
        glPushMatrix();
            glColor4d(GREEN);
            glTranslatef(0.0, 0.0, right_racket_move);
                glBegin(GL_QUADS);
                    glVertex3d(-board_width, 0.0, -racket_length);
                    glVertex3d(-board_width, 0.0, racket_length);
                    glVertex3d(-board_width, board_height, racket_length);
                    glVertex3d(-board_width, board_height, -racket_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX,&matrix_player_right[0][0]);
            matrix_player_right[3][0] = 8;
            //printf("[PLAYER RIGHT] %lf %lf %lf\n",matrix_player_right[3][0],matrix_player_right[3][1],matrix_player_right[3][2]);
        glPopMatrix();
    
        //LEFT RACKET
        glCullFace(GL_BACK);
        glPushMatrix();
            glColor4d(BROWN);
            glTranslatef(0.0, 0.0, left_racket_move);
                glBegin(GL_QUADS);
                    glVertex3d(board_width, 0.0, -racket_length);
                    glVertex3d(board_width, 0.0, racket_length);
                    glVertex3d(board_width, board_height, racket_length);
                    glVertex3d(board_width, board_height, -racket_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX,&matrix_player_left[0][0]);
            matrix_player_left[3][0] = -8;
            //printf("[PLAYER LEFT] %lf %lf %lf\n",matrix_player_left[3][0],matrix_player_left[3][1],matrix_player_left[3][2]);
        glPopMatrix();
    
        glPushMatrix();
            glColor4d(ORANGE);
                glBegin(GL_QUADS);
                    //Top
                    glVertex3d(-board_width, 0.5, board_length);
                    glVertex3d(-board_width, board_height, board_length);
                    glVertex3d(board_width, board_height, board_length);
                    glVertex3d(board_width, 0.5, board_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX,&matrix_top[0][0]);
        glPopMatrix();
    
        glCullFace(GL_FRONT);
        glPushMatrix();
            glColor4d(RED);
                glBegin(GL_QUADS);
                    //Bottom
                    glVertex3d(-board_width, 0.5, -board_length);
                    glVertex3d(-board_width, board_height, -board_length);
                    glVertex3d(board_width, board_height, -board_length);
                    glVertex3d(board_width, 0.5, -board_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX,&matrix_bottom[0][0]);
        glPopMatrix();
    glPopMatrix();
}

GLvoid Stage::draw_character(){
    glPushMatrix();
    glColor4d(WHITE);
    //glRotated(this->radium * cos(this->angle), 0.0, 0.0, 1.0);
    glTranslatef(ball_pos_x, ball_going_down, ball_pos_z);
    glutSolidSphere(0.25, 25, 25);
    glGetFloatv(GL_MODELVIEW_MATRIX,&matrix_ball[0][0]);
    printf("[SPHERE] %lf %lf %lf\n",matrix_ball[3][0],matrix_ball[3][1],matrix_ball[3][2]);
    glPopMatrix();
}

GLvoid Stage::keyboard(){
    if(this->front){
        if(left_racket_move < 2.75){
            this->left_racket_move += 0.3;
        }
    }

    if(this->back){
        if(left_racket_move > -2.75){
            this->left_racket_move -= 0.3;
        }
    }

    if(this->down_arrow){
        if(right_racket_move > -2.75){
            this->right_racket_move -= 0.3;
        }
    }

    if(this->up_arrow){
        if(right_racket_move < 2.75){
            this->right_racket_move += 0.3;
        }
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

GLvoid Stage::writeText(char *text){
    /*const char *c;

    glColor4d(BLACK);
    glPushMatrix();
        glTranslated(2.0, 1.5, 0.0);
        glRasterPos2f(0.0, 0.0);

        for(c = text; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    glPopMatrix();
    
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0,1280,0,720,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
        glLoadIdentity();
        glRasterPos2f(0.0,0.0);
        printf("TEXT: %s\n",text);
        for(c = text; *c != '\0'; ++c){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);*/
    
}

GLvoid Stage::Timer_ball_going_down(GLint value){
    if (!observer_position){
        //printf("[DEBUG]%lf %lf\n",matrix_ball[3][0],matrix_player_left[3][0]);
        if((matrix_ball[3][0]-0.25 <= matrix_player_left[3][0] && ((matrix_ball[3][1]<=matrix_player_left[3][1]+1) && (matrix_ball[3][1]>=matrix_player_left[3][1]-1)))
           || (matrix_ball[3][0]+0.25 >= matrix_player_right[3][0] && ((matrix_ball[3][1]<=matrix_player_right[3][1]+1) && (matrix_ball[3][1]>=matrix_player_right[3][1]-1)))){
            ball_speed_x *= -1.0;
        }
        
        if((this->ball_pos_z+0.25)>=4.0 || (this->ball_pos_z-0.25)<=-4.0){
            ball_speed_z *= -1.0;
        }
        
        this->ball_pos_x += ball_speed_x;
        this->ball_pos_z += ball_speed_z;
        glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
    }
    
    else{
        char *aux;

        if(!value){
            aux = (char *)calloc(1, sizeof(char) * 4);
            strcpy(aux, "Go!");
            writeText(aux);
            free(aux);
            observer_position = false;
            glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
        }

        if(ball_going_down > 0.75){
            ball_going_down -= 0.01;
            glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
        }

        else{
            aux = (char *)calloc(1, sizeof(char) * 2);
            sprintf(aux, "%d", value);
            writeText(aux);
            free(aux);
            glutTimerFunc(1000, &Stage::static_timer_ball_going_down, value - 1);
        }
    }
}

void Stage::static_timer_ball_going_down(GLint value){
    s_stage->Timer_ball_going_down(value);
}

GLboolean Stage::getObserverPosition(){
    return this->observer_position;
}
