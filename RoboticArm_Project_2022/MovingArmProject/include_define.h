#ifndef INCLUDE_DEFINE_H
#define INCLUDE_DEFINE_H

#define Inertia_Jz        0.0576 //[m²kg]
#define mass_arm_prop    0.220 //[kg]
#define distOG            0.48     //[m]
#define L_ARM            0.65       //[m]
#define gravity            9.81     //[m/s²]
#define MY_PI            (3.1415926535897932384626433832795)

#define SAMPLE_TIME_25ms 0.025 //mwm? 20 ms is hardcoded in the controller - what is this for?
#define SAMPLE_TIME_50ms 0.05

#define MIN_THRUST         0.0
#define MAX_THRUST         3.0

#define MIN_THETA         0.0
#define MAX_THETA         (0.9 * MY_PI) // 0.9 = coefficient

// files "view_fly_arm.h" and "model_openGL.h"
#define VIEW_FLY_ARM_WIDTH    1520
#define VIEW_FLY_ARM_HEIGHT    780

// origine en haut à gauche
#define VIEW_OPENGL_X        0
#define VIEW_OPENGL_Y        24
#define VIEW_OPENGL_WIDTH    1000 //OPENGL_WIDTH
#define VIEW_OPENGL_HEIGHT    1100 //OPENGL_HEIGHT

// origine: en bas à gauche
#define OPENGL_X        0
#define OPENGL_Y (VIEW_OPENGL_HEIGHT - OPENGL_HEIGHT)
#define OPENGL_WIDTH    720
#define OPENGL_HEIGHT    720

#define PANEL_LOCATION_LEFT                726
#define PANEL_WIDTH                        778
#define PANEL_HEIGHT                    180
#define PANEL_THETA_TOP VIEW_OPENGL_Y
#define PANEL_CONSOLE_TOP                648
#define PANEL_CONSOLE_WIDTH OPENGL_WIDTH

#define MARGE_MESURE 9
#define MARGE_TITLE     4
#define MARGE_VALUE     52
#define MARGE_TIME     29

#define CASCADE_CONTROLLER true
#define LEADLAG_CONTROLLER false

// définir les hils mode
#define HILS_MODE_PC_CONTROLLER            0
#define HILS_MODE_1_                    1
#define HILS_MODE_REAL_ANGLE            2
#define HILS_MODE_3_                    3
#define HILS_MODE_MANUAL_THRUST_COMMAND 4

// définir les ports utilisables
#define PORT_USB 0
#define PORT_COM 1

// définir les parties pour les données à enregistrer
#define DONNEES_DEBUT                0
#define DONNEES_TIMER                1
#define DONNEES_ARMPROPCONTROLLER    2
#define DONNEES_ARMPROPSIMULATOR    3
#define DONNEES_GRAPH_1            4
#define DONNEES_GRAPH_2            5
#define DONNEES_FIN                6

//#include <windows.h>
//#include <cstdio>            // Header File For Standard Input/Output
//#include <gl\gl.h>            // Header File For The OpenGL32 Library
//#include <gl\glu.h>            // Header File For The GLu32 Library
//#include <gl\GLAUX.h>        // Header File For The Glaux Library
#include <cstring>
#include <cmath>
#include <assert.h>

#endif
