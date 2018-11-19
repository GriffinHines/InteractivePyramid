#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods );
void InitializePolygon(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, float vertex[] );
void DrawPolygon(GLfloat vertex[]);
void Translate(float vertex[]);
void Scale(float vertex[]);
void RotationCenter();

GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;
GLfloat xCenter = halfScreenWidth;
GLfloat yCenter = halfScreenHeight;

//cube: 72 vertices, lines 213 and 240
/*
 float vertex[] = {
 0, 1, 1,
 1, 1, 1,
 1, -1, 1,
 -1, -1, 1,
 
 -1, 1, -1,
 1, 1, -1,
 1, -1, -1,
 -1, -1, -1,
 
 -1, 1, 1,
 -1, 1, -1,
 -1, -1, -1,
 -1, -1, 1,
 
 1, 1, 1,
 1, 1, -1,
 1, -1, -1,
 1, -1, 1,
 
 -1, 1, 1,
 -1, 1, -1,
 1, 1, -1,
 1, 1, 1,
 
 -1, -1, 1,
 -1, -1, -1,
 1, -1, -1,
 1, -1, 1
 };
 */


//pyramid: 48 vertices, lines 213 and 240
float vertex[] = {
    //base
    -1, 0, -1,
    1, 0, -1,
    1, 0, 1,
    -1, 0, 1,
    
    //front
    -1, 0, 1,
    0, 1, 0,
    1, 0, 1,
    
    //right
    1, 0, 1,
    0, 1, 0,
    1, 0, -1,
    
    //back
    1, 0, -1,
    0, 1, 0,
    -1, 0, -1,
    
    //left
    -1, 0, -1,
    0, 1, 0,
    -1, 0, 1
    
};


int main( void )
{
    GLFWwindow *window;
    
    // Initialize the library
    if ( !glfwInit( ) )
    {
        return -1;
    }
    
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Pyramid", NULL, NULL );
    
    glfwSetKeyCallback( window, keyCallback );
    glfwSetInputMode( window, GLFW_STICKY_KEYS, 1 );
    
    
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    if ( !window )
    {
        glfwTerminate( );
        return -1;
    }
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    glViewport( 0.0f, 0.0f, screenWidth, screenHeight ); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode( GL_PROJECTION ); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity( ); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho( 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000 ); // essentially set coordinate system
    glMatrixMode( GL_MODELVIEW ); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity( ); // same as above comment
     
    InitializePolygon(halfScreenWidth, halfScreenHeight, -500, vertex);
    
    // Loop until the user closes the window
    while ( !glfwWindowShouldClose( window ) )
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        // Render OpenGL here
        
        glPushMatrix( );
        glTranslatef( xCenter, yCenter, -500 );
        glRotatef( rotationX, 1, 0, 0 );
        glRotatef( rotationY, 0, 1, 0 );
        glTranslatef( -xCenter, -yCenter, 500 );
        DrawPolygon(vertex);
        
        glPopMatrix();
        
        
        // Swap front and back buffers
        glfwSwapBuffers( window );
        
        // Poll for and process events
        glfwPollEvents( );
    }
    
    glfwTerminate( );
    
    return 0;
}



void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
    //std::cout << key << std::endl;
    
    const GLfloat rotationSpeed = 10;
    
    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if ( action == GLFW_PRESS || action == GLFW_REPEAT )
    {
        switch ( key )
        {
            case GLFW_KEY_UP:
                rotationX -= rotationSpeed;
                break;
            case GLFW_KEY_DOWN:
                rotationX += rotationSpeed;
                break;
            case GLFW_KEY_RIGHT:
                rotationY += rotationSpeed;
                break;
            case GLFW_KEY_LEFT:
                rotationY -= rotationSpeed;
                break;
            case GLFW_KEY_T:
                Translate(vertex);
                break;
            case GLFW_KEY_S:
                Scale(vertex);
                break;
            case GLFW_KEY_C:
                RotationCenter();
                break;
            case GLFW_KEY_M:
                Translate(vertex);
                Scale(vertex);
                RotationCenter();
                break;
        }
        
        
    }
}

void InitializePolygon(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, float vertex[])
{
    //count number of elements in vertex
    int count = 0;
    while(vertex[count])
    {
        count++;
    }
    //count = 16*3 for Pyramid, 24*3 for cube
    count = 16*3;
    //j keeps track of weather vertex[i] is an x, y, or z value
    int j = 0;
    //modify vertex values to fit to screen
    for(int i=0; i<count; i++){
        if(j==0)
            vertex[i] = 50*vertex[i]+centerPosX;
        if(j==1)
            vertex[i] = 50*vertex[i]+centerPosY;
        if(j==2)
            vertex[i] = 50*vertex[i]+centerPosZ;
        if(j==3){
            j = 0;
            vertex[i] = 50*vertex[i]+centerPosX;
        }
        j++;
    }
}
void DrawPolygon(float vertex[]){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, vertex );
    //16 for pyramid, 24 for cube
    glDrawArrays( GL_QUADS, 0, 16 );
    
    glDisableClientState( GL_VERTEX_ARRAY );
}
void Translate(float vertex[]){
    int x, y ,z;
    int j=0;
    std::cout << "Translate: input x value" << std::endl;
    std::cin >> x;
    std::cout << "Translate: input y value" << std::endl;
    std::cin >> y;
    std::cout << "Translate: input z value" << std::endl;
    std::cin >> z;
    
    for(int i=0; i<48; i++){
        if(j==0)
            vertex[i] += x;
        if(j==1)
            vertex[i] += y;
        if(j==2)
            vertex[i] += z;
        if(j==3){
            j = 0;
            vertex[i] += x;
        }
    j++;
    }
    return;
}
void Scale(float vertex[]){
    GLfloat centerPosX = halfScreenWidth;
    GLfloat centerPosY = halfScreenHeight;
    GLfloat centerPosZ = -500;
    
    float x, y ,z, xCenter, yCenter, zCenter;
    int j=0;
    std::cout << "Scale: input x value" << std::endl;
    std::cin >> x;
    std::cout << "Scale: input y value" << std::endl;
    std::cin >> y;
    std::cout << "Scale: input z value" << std::endl;
    std::cin >> z;
    std::cout << "Scale: input center of scale on the x axis" << std::endl;
    std::cin >> xCenter;
    std::cout << "Scale: input center of scale on the y axis" << std::endl;
    std::cin >> yCenter;
    std::cout << "Scale: input center of scale on the z axis" << std::endl;
    std::cin >> zCenter;
    
    int k=0;
    for(int i=0; i<48; i++){
        if(k==0)
            vertex[i] += xCenter;
        if(k==1)
            vertex[i] += yCenter;
        if(k==2)
            vertex[i] += zCenter;
        if(k==3){
            k = 0;
            vertex[i] += xCenter;
        }
        k++;
    }
    
    for(int i=0; i<48; i++){
        if(j==0)
            vertex[i] = (vertex[i] - centerPosX)*x + centerPosX;
        if(j==1)
            vertex[i] = (vertex[i] - centerPosY)*y + centerPosY;
        if(j==2)
            vertex[i] = (vertex[i] - centerPosZ)*z + centerPosZ;
        if(j==3){
            j = 0;
            vertex[i] = (vertex[i] - centerPosX)*x + centerPosX;
        }
        j++;
    }
    
    int l=0;
    for(int i=0; i<48; i++){
        if(l==0)
            vertex[i] -= xCenter;
        if(l==1)
            vertex[i] -= yCenter;
        if(l==2)
            vertex[i] -= zCenter;
        if(l==3){
            l = 0;
            vertex[i] -= xCenter;
        }
        l++;
    }
    return;
}
void RotationCenter(){
    std::cout << "Input center of rotation x value: " << std::endl;
    std::cin >> xCenter;
    std::cout << "Input center of rotation y value: " << std::endl;
    std::cin >> yCenter;
}
