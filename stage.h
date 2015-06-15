#ifndef STAGE_H
#define STAGE_H

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include "RgbImage.h"

//#define DEBUG_MODELVIEW
//#define DEBUG_SPHERE
//#define DEBUG_PLAYER_LEFT
//#define DEBUG_PLAYER_RIGHT
//#define DEBUG_POWERUP_TOUCH
//#define GOD_MODE

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

#define MAX_SCORE 10

struct observer{
    GLdouble x, y, z;
};

enum{
    RACKET_FASTER = 0,
    RACKET_LARGER = 1,
    NO_PLAYER = 0,
    PLAYER_LEFT = 1,
    PLAYER_RIGHT = 2
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
    GLvoid Timer_powerups(GLint value);
    static void static_timer_powerups(GLint value);

private:
    /* functions */
    GLvoid init();
    GLvoid draw_world();    
    GLvoid draw_board();
    GLvoid draw_character();
    GLvoid draw_powerup();
    GLvoid keyboard();
    GLvoid writeText(char *text, GLdouble posX, GLdouble posY);
    GLvoid writePoints();
    GLvoid writeEnd();
    GLvoid loadTextures();
    GLdouble randomGenerator(GLdouble min, GLdouble max);
    GLint randomIntGenerator(GLint min, GLint max);
    GLvoid draw_wall(GLint type);

    /* variables */
    GLsizei wScreen, hScreen;
    GLboolean pause_game;

    GLdouble angle, radium;
    GLdouble fov, aspect, near_p, far_p;
    GLdouble width, height, length;
    GLdouble board_width, board_length, board_height;
    GLdouble racket_width, racket_length_left, racket_length_right, racket_height;
    GLdouble left_racket_move, right_racket_move;
    GLdouble ball_going_down;
    GLdouble actual_speed_x, actual_speed_z;
    GLdouble ball_speed_x, ball_speed_z, ball_pos_x, ball_pos_z;
    GLfloat matrix_top[4][4], matrix_bottom[4][4];    
    GLfloat matrix_player_left[4][4], matrix_player_right[4][4];
    GLfloat matrix_ball[4][4], matrix_powerup[4][4];
    GLboolean observer_position;
    GLint ball_type;
    GLboolean box_close;
    struct observer obs_begin;
    struct observer obs_end;

    GLint timer_value;
    GLint player_one_points, player_two_points;

    GLboolean front, back, left, right, up_arrow, down_arrow;

    /*Textures variables*/
    GLuint texture_player[3];
    GLuint texture_ball[3];
    GLuint texture_wall, texture_floor, texture_environment;
    GLuint tex;
    GLuint texture_powerup;
    RgbImage imag;

    /* racket speed */
    GLdouble racket_speed_left, racket_speed_right;

    /* ball rotate */
    GLdouble ballRotate;

    /* rotate world */
    GLdouble tempRotateX, tempRotateY, tempRotateZ;

    /* powerup type */
    GLint powerup_type;
    GLint player_powerup;
    GLboolean can_create_powerup;
    GLdouble powerup_x, powerup_z;

    /* end game */
    GLboolean exit_game;
};

#endif // STAGE_H
