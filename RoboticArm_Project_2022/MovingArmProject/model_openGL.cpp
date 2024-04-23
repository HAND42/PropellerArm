#include "StdAfx.h"
#include "model_openGL.h"

// -------- PUBLIC --------
// ++++++++++++++++ INIT
void Cmodel_openGL::init(void)
{
    this->myArmPropSimulator = ArmPropSimulator::getInstance();

    this->openGL_x = OPENGL_X;
    this->openGL_y = OPENGL_Y;
    this->openGL_width = OPENGL_WIDTH;
    this->openGL_height = OPENGL_HEIGHT;

    this->init_scene();
    this->init_opengl();
    this->render();
}

GLvoid Cmodel_openGL::openGL_width_set(GLsizei openGL_width_new)
{
    this->openGL_width = openGL_width_new;
}

GLvoid Cmodel_openGL::openGL_height_set(GLsizei openGL_height_new)
{
    this->openGL_height = openGL_height_new;
}

GLvoid Cmodel_openGL::openGL_x_set(GLsizei openGL_x_new)
{
    this->openGL_x = openGL_x_new;
}

GLvoid Cmodel_openGL::openGL_y_set(GLsizei openGL_y_new)
{
    this->openGL_y = openGL_y_new;
}

// ++++++++++++++++ RENDER
void Cmodel_openGL::render(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear screen and depth buffer
    glLoadIdentity();                                    // Reset the current modelview matrix

    glTranslatef(-0.52f, -0.4f, -3.5f);// Move left 1.5 units and into the screen 6.0

    this->draw_background();
    this->draw_base();
    this->draw_arm();
    this->draw_thrust();
    this->draw_box();
    this->draw_side_clamp();

    glDisable( GL_TEXTURE_2D );
}

// -------- PRIVATE --------
// ++++++++++++++++ CMODEL_OPENGL
Cmodel_openGL::Cmodel_openGL(void)
{
}

// ++++++++++++++++ ~CMODEL_OPENGL
Cmodel_openGL::~Cmodel_openGL(void)
{
}

// ++++++++++++++++ INIT_SCENE
void Cmodel_openGL::init_scene(void)        // Resize and initialise the gl window
{
    if (this->openGL_height == 0)                                        // Prevent A Divide By Zero By
    {
        this->openGL_height = 1;                                        // Making Height Equal One
    }
    glViewport(this->openGL_x, this->openGL_y, this->openGL_width, this->openGL_height);            
    
    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(47.0f,(GLfloat)this->openGL_width/(GLfloat)this->openGL_height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
}

// ++++++++++++++++ INIT_OPENGL
void Cmodel_openGL::init_opengl(GLvoid)                                        // All setup for opengl goes here
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);                // Black background
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glClearDepth(1.0f);                                    // Depth buffer setup
    glEnable(GL_DEPTH_TEST);                            // Enables depth testing
    glDepthFunc(GL_LEQUAL);                                // The type of depth testing to do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really nice perspective calculations
    // load textures
    this->init_texture();
}

// ++++++++++++++++ INIT_TEXTURE
void Cmodel_openGL::init_texture(void)
{
    glGenTextures(TEXTURE_NUMBER, this->texture_table);
    texture_load("texture_blackAlu.raw", TRUE, TEXTURE_BOX, 256, 256);
    texture_load("texture_wood.raw", TRUE, TEXTURE_BASE, 256, 256);
    texture_load("texture_aluminium.raw", TRUE, TEXTURE_ARM , 256, 256);
    texture_load("Dell_background2.raw", TRUE, TEXTURE_BACKGROUND, 600, 600);
//    texture_load("background-guillaume.raw", TRUE, TEXTURE_BACKGROUND, 640, 340);
//    texture_load("form-background.raw", TRUE, TEXTURE_BACKGROUND, 720, 720);
}

// ++++++++++++++++ TEXTURE_LOAD
// load a 256x256 RGB .RAW file as a texture
//void Cmodel_openGL::texture_load(GLuint* texture, const char * filename, int wrap, int texture_number, int texture_width, int texture_height)
void Cmodel_openGL::texture_load(const char * filename, int wrap, int texture_number, int texture_width, int texture_height)
{
    BYTE* data = NULL;
    FILE* file = NULL;

    // open texture data
    /* la commande suivante "file = fopen_s( filename, "rb" );" a �t� remplac� par la commande ci-dessous
    pour �viter l'avertissement C4996 concernant le risque de s�curit� de la fonction "fopen" */
    fopen_s(&file, filename, "rb" );
    if ( file != NULL )
    {
        // allocate buffer
        data =  (BYTE*)malloc( texture_width * texture_height * 3 );

        // read texture data
        fread( data, texture_width * texture_height * 3, 1, file );

        // select our current texture
        glBindTexture( GL_TEXTURE_2D, this->texture_table[texture_number]);

        // select modulate to mix texture with color for shading
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

        // when texture area is small, bilinear filter the closest MIP map
        //    glTexParameterf(, , GL_LINEAR_MIPMAP_LINEAR | GL_LINEAR_MIPMAP_NEAREST | GL_NEAREST_MIPMAP_LINEAR | GL_NEAREST_MIPMAP_NEAREST)
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
               GL_NEAREST_MIPMAP_NEAREST);
        // when texture area is large, bilinear filter the first MIP map
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        // if wrap is true, the texture wraps over at the edges (repeat)
        //       ... false, the texture ends at the edges (clamp)
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
               wrap ? (GLfloat)GL_REPEAT : (GLfloat)GL_CLAMP );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
               wrap ? (GLfloat)GL_REPEAT : (GLfloat)GL_CLAMP );

        // build our texture MIP maps
//int gluBuild2DMipmaps(GLenum target, GLint components, GLint width,GLint height, GLenum format, GLenum type,void *data);
        gluBuild2DMipmaps( GL_TEXTURE_2D, 3, texture_width,
        texture_height, GL_RGB, GL_UNSIGNED_BYTE, data );

        // free buffer
        free( data );
    
        fclose( file );
    }    
}

// ++++++++++++++++ DRAW_BOX
void Cmodel_openGL::draw_box(void)
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, this->texture_table[TEXTURE_BOX]);

    glBegin(GL_QUADS);
        //Top
        glTexCoord2f(0.0, 1.0);    glVertex3f( 0.08f, 0.5f,-0.08f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.08f, 0.5f,-0.08f);    
        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.08f, 0.5f, 0.08f);    
        glTexCoord2f(1.0, 1.0);    glVertex3f( 0.08f, 0.5f, 0.08f);

        //Left   
        glTexCoord2f(0.0, 0.0);    glVertex3f( 0.08f, 0.5f, 0.08f);
        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.08f, 0.5f, 0.08f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.08f,-0.5f, 0.08f);
        glTexCoord2f(0.0, 1.0);    glVertex3f( 0.08f,-0.5f, 0.08f);
        //Right
        glTexCoord2f(1.0, 0.0);    glVertex3f( 0.08f,-0.5f,-0.08f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.08f,-0.5f,-0.08f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.08f, 0.5f,-0.08f);
        glTexCoord2f(0.0, 0.0);    glVertex3f( 0.08f, 0.5f,-0.08f);  
        //back
        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.08f, 0.5f, 0.08f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.08f, 0.5f,-0.08f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(-0.08f,-0.5f,-0.08f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.08f,-0.5f, 0.08f);    

        //front
        glTexCoord2f(0.0, 0.0);    glVertex3f( 0.08f, 0.5f,-0.08f);      
        glTexCoord2f(1.0, 0.0);    glVertex3f( 0.08f, 0.5f, 0.08f);
        glTexCoord2f(1.0, 1.0);    glVertex3f( 0.08f,-0.5f, 0.08f);
        glTexCoord2f(0.0, 1.0); glVertex3f( 0.08f,-0.5f,-0.08f);

    glEnd();
}

// ++++++++++++++++ DRAW_BASE
void Cmodel_openGL::draw_base(void)
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, this->texture_table[TEXTURE_BASE]);

    glBegin(GL_QUADS);
        //Base
        glTexCoord2f(0.0, 1.0);    glVertex3f(-0.15f, -0.5f,  0.3f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.15f, -0.5f, -0.3f);
        glTexCoord2f(1.0, 0.0);    glVertex3f( 1.15f, -0.5f, -0.3f);
        glTexCoord2f(1.0, 1.0);    glVertex3f( 1.15f, -0.5f,  0.3f);

        glTexCoord2f(1.0, 1.0);    glVertex3f( 1.15f, -0.6f,  0.3f);
        glTexCoord2f(1.0, 0.0);    glVertex3f( 1.15f, -0.6f, -0.3f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.15f, -0.6f, -0.3f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(-0.15f, -0.6f,  0.3f);

        glTexCoord2f(0.0, 0.0);    glVertex3f( 1.15f, -0.6f, -0.3f);      
        glTexCoord2f(1.0, 0.0);    glVertex3f( 1.15f, -0.6f,  0.3f);
        glTexCoord2f(1.0, 1.0);    glVertex3f( 1.15f, -0.5f,  0.3f);      
        glTexCoord2f(0.0, 1.0); glVertex3f( 1.15f, -0.5f, -0.3f);      

        glTexCoord2f(1.0, 0.0); glVertex3f( 1.15f, -0.5f, -0.3f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.15f, -0.5f, -0.3f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.15f, -0.6f, -0.3f);      
        glTexCoord2f(0.0, 0.0); glVertex3f( 1.15f, -0.6f, -0.3f);

        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.15f, -0.5f, -0.3f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.15f, -0.5f,  0.3f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(-0.15f, -0.6f, -0.3f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(-0.15f, -0.6f,  0.3f);

        glTexCoord2f(0.0, 0.0);    glVertex3f( 1.15f, -0.5f, 0.3f);
        glTexCoord2f(1.0, 0.0);    glVertex3f( 1.15f, -0.6f, 0.3f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(-0.15f, -0.6f, 0.3f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(-0.15f, -0.5f, 0.3f);
    glEnd();

}

// ++++++++++++++++ DRAW_SIDE_CLAMP
void Cmodel_openGL::draw_side_clamp(void){
    //sideclamp (Left)
    glBegin(GL_QUADS);
        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.008f,0.35f, -0.03f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(0.008f, 0.45f, -0.03f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(0.15f,  0.45f, -0.03f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(0.15f,  0.35f, -0.03f);
        //sideclamp (Right)
        glTexCoord2f(1.0, 0.0);    glVertex3f(-0.008f,0.35f, 0.03f);
        glTexCoord2f(0.0, 0.0);    glVertex3f(0.008f, 0.45f, 0.03f);
        glTexCoord2f(1.0, 1.0);    glVertex3f(0.15f,  0.45f, 0.03f);
        glTexCoord2f(0.0, 1.0);    glVertex3f(0.15f,  0.35f, 0.03f);
    glEnd();
}

// ++++++++++++++++ DRAW_PROPELLER
void Cmodel_openGL::draw_propeller(void)
{
    glBegin(GL_QUADS); // force
        glVertex2f(0.15f, -0.2f);
        glVertex2f(0.16f, -0.2f);
        glVertex2f(0.17f, -0.36f);
        glVertex2f(0.15f, -0.36f);
    glEnd();
}

// ++++++++++++++++ DRAW_ARM
void Cmodel_openGL::draw_arm(void)
{
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, this->texture_table[TEXTURE_ARM]);

    glPushMatrix();
        glTranslatef (0.12f, 0.4f, 0.0f);
        glRotatef(((GLfloat)this->myArmPropSimulator->GetTheta() * 180.0f / (GLfloat)MY_PI), 0.0f, 0.0f, 1.0f);
        glTranslatef (-0.12f, -0.4f, 0.0f);

        glRectf(0.1f, 0.4f, 0.14f, -0.4f); // glRectf(coin sup�rieur gauche: x, y, coin inf�rieur droit: x, y)

        // ajouter propeller
        draw_propeller();
    glPopMatrix();
}

// ++++++++++++++++ DRAW_THRUST
void Cmodel_openGL::draw_thrust(void)
{
    glDisable( GL_TEXTURE_2D );

    glPushMatrix();
        glTranslatef (0.12f, 0.4f, 0.0f);
        glRotatef(((GLfloat)this->myArmPropSimulator->GetTheta() * 180.0f / (GLfloat)MY_PI), 0.0f, 0.0f, 1.0f);
        glTranslatef (-0.12f, -0.4f, 0.0f);

        // draw thrust
        glColor3f(0.21f,0.98f,0.16f);

        GLfloat position_thrust_y = -0.26f;
        // longueur pour thrust
        GLfloat Thrust_long = (GLfloat)this->myArmPropSimulator->GetPropThrust()/4.0f;

        // base
        glBegin(GL_QUADS);
            glVertex2f(0.19f, position_thrust_y); // point 0
            glVertex2f(0.19f, position_thrust_y - 0.04f); // point 1        -4
            glVertex2f(Thrust_long + 0.19f, position_thrust_y - 0.04f); // point 2    -4
            glVertex2f(Thrust_long + 0.19f, position_thrust_y); // point 6    0
        glEnd();

            // arrow
        glBegin(GL_TRIANGLES); // force
            glVertex2f(Thrust_long + 0.19f, position_thrust_y + 0.02f); // point 5    +2
            glVertex2f(Thrust_long + 0.23f, position_thrust_y - 0.02f); // point 4    -2
            glVertex2f(Thrust_long + 0.19f, position_thrust_y - 0.06f); // point 3    -6
        glEnd();

        glColor3f(1.0,1.0,1.0);
    glPopMatrix();
    glEnable( GL_TEXTURE_2D );
}

// ++++++++++++++++ DRAW_BACKGROUND
void Cmodel_openGL::draw_background(void)
{
/*
    float x_left = -10.0;
    float x_right = 30.0;
    float y_top = 0.0;
    float y_bottom = 40.0;
*/
    float x_left = -2.0;
    float x_right = 2.5;
    float y_top = -2.5;
    float y_bottom = 2.5;
    float z = -1.0;
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, this->texture_table[TEXTURE_BACKGROUND]);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);    glVertex3f(x_left, y_top, z);
        glTexCoord2f(1.0, 0.0);    glVertex3f(x_right, y_top, z);
        glTexCoord2f(1.0, 1.0);    glVertex3f(x_right, y_bottom,  z);
        glTexCoord2f(0.0, 1.0);    glVertex3f(x_left, y_bottom,  z);
    glEnd();

}
