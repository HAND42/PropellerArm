#ifndef MODEL_OPENGL_H
    #define MODEL_OPENGL_H

    #define TEXTURE_NUMBER 8
    #define TEXTURE_BOX 1
    #define TEXTURE_BASE 2
    #define TEXTURE_ARM 3
    #define TEXTURE_FIBRE_CARBONE 4
    #define TEXTURE_BACKGROUND 5

    #include "include_define.h"
    #include "model_singleton.h"
    #include "arm_prop_simulator.h"

    class Cmodel_openGL : public Singleton<Cmodel_openGL>
    {
        friend class Singleton<Cmodel_openGL>;

        public:
            void init(void);

            ArmPropSimulator* myArmPropSimulator;

            void init_scene(void);

            GLvoid openGL_width_set(GLsizei);
            GLvoid openGL_height_set(GLsizei);
            GLvoid openGL_y_set(GLsizei);
            GLvoid openGL_x_set(GLsizei);

            void render(void);

        private:
            Cmodel_openGL(void);
            ~Cmodel_openGL(void);

            GLsizei openGL_x;
            GLsizei openGL_y;
            GLsizei openGL_width;
            GLsizei openGL_height;

            GLuint texture_table[TEXTURE_NUMBER];

//        ----------------------------------------------------
//                    INIT
//        ----------------------------------------------------
            void init_opengl(void);
            void init_texture(void);

//        ----------------------------------------------------
//                    DRAW OBJECTS
            void texture_load(const char*, int, int, int, int);

            void draw_box(void);
            void draw_base(void);
            void draw_side_clamp(void);
            void draw_propeller(void);
            void draw_arm(void);
            void draw_thrust(void);
            void draw_background(void);
    };

#endif
