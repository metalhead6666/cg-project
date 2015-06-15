#include "stage.h"

Stage *s_stage;

Stage::Stage(){
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
    this->racket_length = 1.0;
    this->racket_height = 0.5;
    this->left_racket_move = 0.0;
    this->right_racket_move = 0.0;
    /* mexer aqui caralho, 0.75/false ou 1.75/true */
    this->ball_going_down = 0.75;
    /* mexer aqui caralho true/start screen ou false/jogo em si*/
    this->observer_position = false;
    
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

    /* mexer aqui caralho, 3/true ou -1/false */
    this->timer_value = -1;
    this->player_one_points = 0;
    this->player_two_points = 0;

    this->ballRotate = 0.0;

    this->tempRotateX = 0.0;
    this->tempRotateY = 0.0;
    this->tempRotateZ = 0.0;

    srand(time(NULL));
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
    if(this->pause_game){
        glRotated(tempRotateX, 1.0, 0.0, 0.0);
        glRotated(tempRotateY, 0.0, 1.0, 0.0);
        glRotated(tempRotateZ, 0.0, 0.0, 1.0);
    }

    this->draw_world();
    this->draw_board();
    this->draw_character();
    glPopMatrix();

    if(this->pause_game){
        aux = (char *)calloc(1, sizeof(char) * 2);
        strcpy(aux, "PAUSED");
        writeText(aux, 1.0, 1.0);
        free(aux);
    }

    else{
        if(this->timer_value == -1){
            this->keyboard();
        }
    }

    glutSwapBuffers();
}

GLvoid Stage::draw_world(){
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
        //glColor4d(BLUE);
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
        glCullFace(GL_FRONT);
        glPushMatrix();
            glColor4d(GREEN);
            glTranslated(-board_width, 0.0, right_racket_move);
                glBegin(GL_QUADS);
                    glVertex3d(0.0, 0.0, -racket_length);
                    glVertex3d(0.0, 0.0, racket_length);
                    glVertex3d(0.0, board_height, racket_length);
                    glVertex3d(0.0, board_height, -racket_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_player_right[0][0]);
#ifdef DEBUG_PLAYER_RIGHT
            printf("[PLAYER RIGHT] %f %f %f\n", matrix_player_right[3][0], matrix_player_right[3][1], matrix_player_right[3][2]);
#endif
        glPopMatrix();
    
        //LEFT RACKET
        glCullFace(GL_BACK);
        glPushMatrix();
            glColor4d(BROWN);
            glTranslated(board_width, 0.0, left_racket_move);
                glBegin(GL_QUADS);
                    glVertex3d(0.0, 0.0, -racket_length);
                    glVertex3d(0.0, 0.0, racket_length);
                    glVertex3d(0.0, board_height, racket_length);
                    glVertex3d(0.0, board_height, -racket_length);
                glEnd();
            glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_player_left[0][0]);
#ifdef DEBUG_PLAYER_LEFT
            printf("[PLAYER LEFT] %f %f %f\n", matrix_player_left[3][0], matrix_player_left[3][1], matrix_player_left[3][2]);
#endif
        glPopMatrix();

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
        //glColor4d(WHITE);
        s = gluNewQuadric();
        gluQuadricDrawStyle(s, GLU_FILL);
        gluQuadricNormals(s, GLU_SMOOTH);
        gluQuadricTexture(s, GL_TRUE);
        glTranslated(ball_pos_x, ball_going_down, ball_pos_z);
        ballRotate += 2;
        glRotated(ballRotate, 0.0, 1.0, 0.0);
        //glTranslated(-ball_pos_x, -ball_going_down, -ball_pos_z);
        gluSphere(s, 0.25, 25, 25);
        //glutSolidSphere(0.25,25,25);
        gluDeleteQuadric(s);
        glGetFloatv(GL_MODELVIEW_MATRIX, &matrix_ball[0][0]);
    #ifdef DEBUG_SPHERE
        printf("[SPHERE] %lf %lf %lf\n",matrix_ball[3][0],matrix_ball[3][1],matrix_ball[3][2]);
    #endif
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

    //*********PLAYER_LEFT*********

    glGenTextures(1, &texture_left_player[0]);
    glBindTexture(GL_TEXTURE_2D, texture_left_player[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    imag.LoadBmpFile("ProjectTextures/Player/player1.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, 3, imag.GetNumCols(),
                 imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 imag.ImageData());

    //*********PLAYER_RIGHT*********


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
    case 'W':
        this->front = true;
        break;

    case 's':
    case 'S':
        this->back = true;
        break;

    case 'p':
    case 'P':
        this->pause_game = !this->pause_game;
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
    if(this->pause_game){
        ++this->tempRotateX;
        ++this->tempRotateY;
        ++this->tempRotateZ;
        glutPostRedisplay();
        glutTimerFunc(10, &Stage::static_timer_ball_going_down, value);
    }

    else{
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

GLdouble Stage::randomGenerator(GLdouble min, GLdouble max){
    GLdouble f = (GLdouble)rand() / RAND_MAX;

    return min + f * (max - min);
}
