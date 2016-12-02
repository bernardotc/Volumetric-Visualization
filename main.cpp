//
//  main.cpp
//  Volumetric Visualization
//
//  Created by Bernardo Trevino on 12/2/16.
//  Copyright © 2016 bernardo. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// angle of rotation for the camera direction
float angle=0.0, angle2=0.0, angle3 = 0.0;
// actual vector representing the camera's direction
float lx=0.0f, ly=0.0f, lz=-1;
// XZ position of the camera
float x=1.0f, y=1.0f, z=5.0f;

// Size of voxel
float voxelSize = .1;

// Data size, values and helper variables
int dataXSize = 10, dataYSize = 10, dataZSize= 10;
float dataMatrix[100][100][100];
float minDataValue = 100000, maxDataValue = -100000;

// Camera distance helper variables
float maxDistanceToCamera = -100000, minDistanceToCamera = 100000;

// Helper function to get distance between a point and the camera
float distanceToCamera(float posX, float posY, float posZ) {
    return sqrtf(powf(posX - x, 2) + powf(posY - y, 2) + powf(posZ - z, 2));
}

void initDataMatrix(int value) {
    switch (value) {
        case 1:
            for (int x = 0; x < dataXSize; x++) {
                for (int y = 0; y < dataYSize; y++) {
                    for (int z = 0; z < dataZSize; z++) {
                        if (x > 2  && x < dataXSize - 3 && y > 2  && y < dataYSize - 3 && z > 2  && z < dataZSize - 3 ) {
                            dataMatrix[x][y][z] = 1;
                        } else {
                            dataMatrix[x][y][z] = 0;
                        }
                    }
                }
            }
            minDataValue = 0;
            maxDataValue = 1;
            break;
            
        case 2:
        {
            for (int x = 0; x < dataXSize; x++) {
                for (int y = 0; y < dataYSize; y++) {
                    for (int z = 0; z < dataZSize; z++) {
                        dataMatrix[x][y][z] = 0;
                    }
                }
            }
            
            for (int x = 2; x < 7; x++) {
                dataMatrix[x][2][x] = 10;
            }
            for (int x = 3; x < 7; x++) {
                dataMatrix[x][3][x] = 10;
            }
            for (int x = 4; x < 7; x++) {
                dataMatrix[x][4][x] = 10;
            }
            for (int x = 5; x < 7; x++) {
                dataMatrix[x][5][x] = 10;
            }
            for (int x = 6; x < 7; x++) {
                dataMatrix[x][6][x] = 10;
            }
            minDataValue = 0;
            maxDataValue = 10;
            break;
        }
            
        case 3:
        {
            for (int x = 0; x < dataXSize; x++) {
                for (int y = 0; y < dataYSize; y++) {
                    for (int z = 0; z < dataZSize; z++) {
                        dataMatrix[x][y][z] = rand() % 500;
                    }
                }
            }
            minDataValue = 0;
            maxDataValue = 500;
            break;
        }
        default:
            for (int x = 0; x < dataXSize; x++) {
                for (int y = 0; y < dataYSize; y++) {
                    for (int z = 0; z < dataZSize; z++) {
                        dataMatrix[x][y][z] = x * y * z;
                    }
                }
            }
            minDataValue = 0;
            maxDataValue = dataXSize * dataYSize * dataZSize;
            break;
    }
}

void changeSize(int w, int h) {
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;
    
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void setColorAndAlpha(float x, float y, float z, float colorVal) {
    float posToCamera = distanceToCamera(x, y, z);
    float alphaVal = (posToCamera - minDistanceToCamera) / (maxDistanceToCamera - minDistanceToCamera);
    
    glColor4f( 0.0, 1.0 - colorVal, 0.0 + colorVal, 0.0 + alphaVal);
}

void drawVoxel(float x, float y, float z, float data) {
    
    glTranslatef(x , y , z);
    
    float posToCamera = distanceToCamera(x, y, z);
    
    // Check positions comparing with camera.
    if (posToCamera < minDistanceToCamera) {
        minDistanceToCamera = posToCamera;
    } else if (posToCamera > maxDistanceToCamera) {
        maxDistanceToCamera = posToCamera;
    }
    
    // Calculate color and alpha values
    float colorVal = data / (maxDataValue - minDataValue);
    float alphaVal = (posToCamera - minDistanceToCamera) / (maxDistanceToCamera - minDistanceToCamera);
    
    //cout << posToCamera << "\t" << minDistanceToCamera << "\t" << maxDistanceToCamera << "\t" << alphaVal << endl;

    // Define color and alpha
    glColor4f( 0.0, 1.0 - colorVal, 0.0 + colorVal, 0.0 + alphaVal);
    
    //glColor4f(0, 0, 1, .5);
    //glutSolidCube(voxelSize);
    
    // Draw voxel
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    setColorAndAlpha(x + voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f(  voxelSize, -voxelSize, -voxelSize );
    setColorAndAlpha(x + voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f(  voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x - voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize, -voxelSize );
    glEnd();
    
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    setColorAndAlpha(x + voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f(  voxelSize, -voxelSize, voxelSize );
    setColorAndAlpha(x + voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f(  voxelSize,  voxelSize, voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize, voxelSize );
    setColorAndAlpha(x - voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize, voxelSize );
    glEnd();
    
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    setColorAndAlpha(x + voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f( voxelSize, -voxelSize, -voxelSize );
    setColorAndAlpha(x + voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f( voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x + voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f( voxelSize,  voxelSize,  voxelSize );
    setColorAndAlpha(x + voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f( voxelSize, -voxelSize,  voxelSize );
    glEnd();
    
    
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    setColorAndAlpha(x - voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize,  voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize,  voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x - voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize, -voxelSize );
    glEnd();
    
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    setColorAndAlpha(x + voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f(  voxelSize,  voxelSize,  voxelSize );
    setColorAndAlpha(x + voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f(  voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize, -voxelSize );
    setColorAndAlpha(x - voxelSize, y + voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize,  voxelSize,  voxelSize );
    glEnd();
    
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    setColorAndAlpha(x + voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f(  voxelSize, -voxelSize, -voxelSize );
    setColorAndAlpha(x + voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f(  voxelSize, -voxelSize,  voxelSize );
    setColorAndAlpha(x - voxelSize, y - voxelSize, z + voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize,  voxelSize );
    setColorAndAlpha(x - voxelSize, y - voxelSize, z - voxelSize, colorVal);
    glVertex3f( -voxelSize, -voxelSize, -voxelSize );
    glEnd();
    
    glFlush();
}

void processSpecialKeys(int key, int xx, int yy) {
    
    float movement = 0.1f;
    float movement2 = 0.025f;
    
    switch (key) {
        case GLUT_KEY_LEFT :
            x-= movement;
            break;
        case GLUT_KEY_RIGHT :
            x += movement;
            break;
        case GLUT_KEY_UP :
            y -= movement2;
            break;
        case GLUT_KEY_DOWN :
            y += movement2;
            break;
            
    }
}

static void key(unsigned char key, int x, int y)
{
    float fraction = 0.1f;
    
    switch (key) {
        case 'q':
            exit(0);
            break;
        case 'n':
            x += lx * fraction;
            z += lz * fraction;
            break;
        case 'f' :
            x -= lx * fraction;
            z -= lz * fraction;
            break;
        case 'r':
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case 't':
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case 'i':
            angle2 += 0.01f;
            lz = -cos(angle2);
            ly = sin(angle2);
            break;
        case 'o':
            angle2 -= 0.01f;
            lz = -cos(angle2);
            ly = sin(angle2);
            break;
        case 'c':
            angle3 += 0.01f;
            break;
        case 'v':
            angle3 -= 0.01f;
            break;
    }
}

void renderScene(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(	x, y, z,
              lx, ly,  lz,
              sin(angle3), cos(angle3),  0.0f);
    
    //glColor4f(0, 0, 1, .9);
    //glutSolidCube(1);
    
    for (int x = 0; x < dataXSize; x++) {
        for (int y = 0; y < dataYSize; y++) {
            for (int z = 0; z < dataZSize; z++) {
                glPushMatrix();
                drawVoxel(x * (voxelSize), y * (voxelSize), z * (voxelSize), dataMatrix[x][y][z]);
                //drawVoxel(x * (2 * voxelSize), y * (2 * voxelSize), z * (2 * voxelSize), dataMatrix[x][y][z]);
                glPopMatrix();
            }
        }
    }
    
    glutSwapBuffers();
}



int main(int argc, char **argv) {
    
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutCreateWindow("Volumetric Visualization");
    
    initDataMatrix(2);
    
    glClearColor( 0, 0, 0, 0);
    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(key);
    glutSpecialFunc(processSpecialKeys);
    
    
    // OpenGL init
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // enter GLUT event processing cycle
    glutMainLoop();
    
    return 1;
}

