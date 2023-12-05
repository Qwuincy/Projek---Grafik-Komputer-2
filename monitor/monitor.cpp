#define _USE_MATH_DEFINES 

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// Global variables for adjusting the monitor screen
GLfloat screenWidth = 2.0f;  // Initial width of the screen
GLfloat screenHeight = 1.2f; // Initial height of the screen
GLfloat screenDepth = 0.2f;  // Initial depth of the screen
GLfloat screenXRotation = 0.0f; // Initial rotation around the X-axis
GLfloat screenYRotation = 0.0f; // Initial rotation around the Y-axis

GLfloat rotationAngle = 0.0f; // Initial rotation angle for animation

// Texture ID for the screen texture
GLuint screenTextureID = 0;

// Function to load a simple checkerboard texture
void loadCheckerboardTexture() {
    const int texWidth = 256;
    const int texHeight = 256;
    GLubyte texData[texWidth * texHeight * 3];

    for (int i = 0; i < texWidth; ++i) {
        for (int j = 0; j < texHeight; ++j) {
            GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            texData[(i * texHeight + j) * 3] = c;
            texData[(i * texHeight + j) * 3 + 1] = c;
            texData[(i * texHeight + j) * 3 + 2] = c;
        }
    }

    glGenTextures(1, &screenTextureID);
    glBindTexture(GL_TEXTURE_2D, screenTextureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

    // Set texture parameters (you can adjust these)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Function to draw the 3D monitor and stand
void drawMonitor() {
    glPushMatrix();
    glRotatef(screenYRotation, 0.0f, 1.0f, 0.0f); // Apply Y-axis rotation
    glRotatef(screenXRotation, 1.0f, 0.0f, 0.0f); // Apply X-axis rotation

    // Enable texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, screenTextureID);

    // Draw the monitor screen as a 3D cube with texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-screenWidth / 2, -screenHeight / 2, -screenDepth / 1.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(screenWidth / 2, -screenHeight / 2, -screenDepth / 1.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(screenWidth / 2, screenHeight / 2, -screenDepth / 1.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-screenWidth / 2, screenHeight / 2, -screenDepth / 1.0);
    glEnd();

    // Draw the monitor screen as a cube without texture
    glColor3f(0.1f, 0.1f, 0.f); // Black color for the screen
    glTranslatef(0.0f, 0.0f, -screenDepth / 2); // Move to the center of the screen
    glScalef(2.0f, 1.2, 0.2); // Scale the cube
    glutSolidCube(1.0f); // Draw a cube as the screen

    // Disable texture mapping
    glDisable(GL_TEXTURE_2D);

    // Draw the monitor stand
    glColor3f(0.2f, 0.2f, 0.2f); // Gray color for the stand
    glTranslatef(0.0f, -(screenHeight / 2.2) - 0.0f, -screenDepth / 2); // Position the stand below the screen
    glScalef(0.2f, 0.2f, 0.5f); // Scale the stand
    glutSolidCube(0.9f); // Draw a cube as the stand

    // Draw the flat stand
    glColor3f(0.2f, 0.2f, 0.2f); // Gray color for the stand
    glPushMatrix();
    glTranslatef(0.0f, -(screenHeight / 2.2) - 0.0f, 0.0f); // Position the stand below the screen
    glScalef(1.0f, 0.1f, 1.5f); // Scale the stand
    glutSolidCube(2.6f); // Draw a cube as the flat stand
    glPopMatrix(); // Restore the original transformation matrix
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set the background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Set the camera position and view
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Rotate the monitor during animation
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

    // Draw the 3D monitor
    drawMonitor();

    glutSwapBuffers();
}

// Animation callback function
void animate(int value) {
    rotationAngle += 1.0f; // Increment the rotation angle
    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, animate, 0); // Set the next animation frame
}

// Reshape callback function
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard callback function for adjusting parameters
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'i':
        screenHeight += 0.1f; // Increase screen height
        break;
    case 'k':
        screenHeight -= 0.1f; // Decrease screen height
        break;
    case 'j':
        screenWidth -= 0.1f; // Decrease screen width
        break;
    case 'l':
        screenWidth += 0.1f; // Increase screen width
        break;
    case 'z':
        screenDepth -= 0.1f; // Decrease screen depth
        break;
    case 'x':
        screenDepth += 0.1f; // Increase screen depth
        break;
    case 'w':
        screenXRotation += 5.0f; // Rotate screen upward
        break;
    case 's':
        screenXRotation -= 5.0f; // Rotate screen downward
        break;
    case 'a':
        screenYRotation -= 5.0f; // Rotate screen left
        break;
    case 'd':
        screenYRotation += 5.0f; // Rotate screen right
        break;
    case 27: // Escape key
        exit(0);
        break;
    }

    glutPostRedisplay(); // Request a redraw
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("3D Monitor");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glEnable(GL_DEPTH_TEST);

    // Load the checkerboard texture
    loadCheckerboardTexture();

    // Start the animation
    glutTimerFunc(0, animate, 0);

    glutMainLoop();
    return 0;
}