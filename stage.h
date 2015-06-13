#ifndef STAGE_H
#define STAGE_H

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

//#define DEBUG_MODELVIEW
//#define DEBUG_SPHERE
//#define DEBUG_PLAYER_LEFT
//#define DEBUG_PLAYER_RIGHT

/* colors */
#define BLUE			 0.0, 0.0, 1.0, 1.0
#define RED				 1.0, 0.0, 0.0, 1.0
#define YELLOW			 1.0, 1.0, 0.0, 1.0
#define GREEN			 0.0, 0.8, 0.0, 1.0
#define ORANGE			 1.0, 0.5, 0.1, 1.0
#define BROWN			 0.3, 0.2, 0.0, 1.0
#define WHITE			 1.0, 1.0, 1.0, 1.0
#define BLACK			 0.0, 0.0, 0.0, 1.0
#define PINK             0.737255, 0.560784, 0.560784, 1.0

/* math values */
#define PI 3.141529

struct observer{
    GLdouble x, y, z;
};

class Stage{
public:
    /* constructor */
    Stage();
    /* destructor */
    ~Stage();

    /* functions */
    GLvoid start_stage();
    GLvoid display();
    GLvoid key_pressed(unsigned char key);
    GLvoid key_not_pressed(unsigned char key);
    GLvoid special_key_pressed(GLint key);
    GLvoid special_key_not_pressed(GLint key);
    GLvoid Timer_ball_going_down(GLint value);
    static void static_timer_ball_going_down(GLint value);

private:
    /* functions */
    GLvoid draw_world();
    GLvoid draw_character();
    GLvoid keyboard();
    GLvoid draw_board();
    GLvoid writeText(char *text);
    GLvoid writePoints();
    GLdouble randomGenerator(GLdouble min, GLdouble max);

    /* variables */
    GLsizei wScreen, hScreen;

    GLdouble angle, radium;
    GLdouble fov, aspect, near, far;
    GLdouble width, height, length;
    GLdouble board_width, board_length, board_height;
    GLdouble racket_width, racket_length, racket_height;
    GLdouble left_racket_move, right_racket_move;
    GLdouble ball_going_down;
    GLdouble actual_speed_x, actual_speed_z;
    GLdouble ball_speed_x, ball_speed_z, ball_pos_x, ball_pos_z;
    GLfloat matrix_top[4][4], matrix_bottom[4][4];    
    GLfloat matrix_player_left[4][4], matrix_player_right[4][4];
    GLfloat matrix_ball[4][4];
    GLboolean observer_position;
    struct observer obs_begin;
    struct observer obs_end;

    GLint timer_value;
    GLint player_one_points, player_two_points;

    GLboolean front, back, left, right, up_arrow, down_arrow;    
};

#endif // STAGE_H
