#include <iostream>
#include <sstream>
#include <cmath>
#include <GL/glut.h>
#include <stdio.h>
#include "classes.h"
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

std::string scor;

float d;

// Function Declarations
void drawScene();
void drawBox(float len, int flag);
void drawBall(float rad, int flag);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key,int x,int y);

void handleMouseclick(int button, int state, int x, int y);
void StrikerUpdateWall(int value);
void Striker_Coin(int value);
void CoinUpdate(int value);
void Collisions(int value);
void score_time(int value);
void reload(int value);
//void friction(int value);
float findscale();

float acceleration = -0.001f;
//Coin Striker;
int strike = 0;
float strike_angle = 0.0f;
float len = 0.10;

int color_game = -1;
int score = 30;


float box_len_outer = 4.0f;
float box_len_inner = 3.6f;
float box_len = 2.5f;

float theta = 0.0f; 

float hole_rad = 0.14f;
float striker = 0.1f;


void SetCoins() {
    coins[9].SetColor(0.43f,0.5f,0.56f);
    coins[9].SetPosition(0.0f,-1.15f);
    coins[9].SetRadius(0.1f);
    coins[9].SetMass(2.0f);
    coins[9].SetType(3);
    coins[0].x = coins[0].y = 0.0f;
    coins[0].SetColor(0.5f,0.0f,0.0f);
    coins[0].SetPosition(0.0f,0.0f);
    coins[0].SetType(0);
    float arc = 0.25 ; 
    float angle = 360.0 / 8 ;
    int i;float x;float y;
    for(i=1; i<9 ; i++){
        if(i%2 == 1){
            coins[i].SetColor(0.0f,1.0f,1.0f);
            coins[i].SetType(2);
        }
        else if(i%2 == 0){
            coins[i].SetColor(0.0f,0.0f,0.0f);
            coins[i].SetType(1);
        }
    
        x = arc * cos(DEG2RAD((float)i * angle));
        y = arc * sin(DEG2RAD((float)i * angle));
        //printf("%f %f\n",x,y);
        coins[i].SetPosition(x,y);
    }

}
void drawText(const char *text, int length, int x, int y)
    {
        glRasterPos2f(x,y);
        //glTranslatef(0.0f,0.0f,-8.0f);
        for(int i=0;i<length;i++)
        {
             glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
        }
    
    }
int main(int argc, char **argv) {

    SetCoins();
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);


	int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("CSE251_CARROM BOARD");  // Setup the window
    initRendering();

   
    // Register callbacks
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);

    glutMouseFunc(handleMouseclick);
    glutReshapeFunc(handleResize);

    glutTimerFunc(1000, score_time , 0);
    glutTimerFunc(10, StrikerUpdateWall, 0);
    glutTimerFunc(10,CoinUpdate,0);
    glutTimerFunc(10,Collisions,0);
    glutTimerFunc(10,reload,0);
    //glutTimerFunc(10, friction, 0);
    //glutTimerFunc(10,Striker_Coin,0);

    glutMainLoop();
    return 0;

}
void score_time(int value){
    score -= 1;
     //printf("%d\n",score );
    glutTimerFunc(1000, score_time , 0);

}
void reload(int value){
    int reload_flag=0;
    {
        for(int i=0;i<9;i++){
            if(coins[i].avail==0){
                if(color_game == 1 ){
                    if(coins[i].type==0 || coins[i].type== 2)
                        reload_flag += 1;
                }
                else if(color_game == 0){
                    if(coins[i].type==0 || coins[i].type == 1)
                        reload_flag += 1;
                }
            }
        }
    }
    if(reload_flag == 5){
        SetCoins();
    }
    glutTimerFunc(10, reload, 0);
}
void StrikerUpdateWall(int value) {

    //cout << "entered " << Striker.velx <<"\n";

    if (strike ==1) {
        
    // Handle ball collisions with box
    if(coins[9].x + striker > box_len_inner/2 || coins[9].x - striker < -box_len_inner/2)
        coins[9].velx *= -1;
    if(coins[9].y + striker > box_len_inner/2 || coins[9].y - striker < -box_len_inner/2)
        coins[9].vely *= -1;
    float a1 = pow(1.67 - coins[9].x, 2);
    float a2 = pow(1.67 + coins[9].x, 2); 
    float b1 = pow(1.67 - coins[9].y, 2);
    float b2 = pow(1.67 + coins[9].y, 2);
    float dist1 = a1 + b1; 
    float dist2 = a1 + b2;
    float dist3 = a2 + b1;
    float dist4 = a2 + b2; 
    float rad_dist = pow(hole_rad- 0.1, 2); 
    int newflag=0;
    if( rad_dist >= dist1 || rad_dist >= dist2 || rad_dist >= dist3 || rad_dist >= dist4){
        coins[9].avail=0;
        score -= 5;
    }
    if(coins[9].avail==0){
    for(int i=0;i<9;i++){
            if(coins[i].vely > 0.001 || coins[i].velx > 0.001)
            {
                newflag=1;
            }
        }
        if(newflag==0){
                coins[9].avail=1;
                coins[9].x = 0;
                coins[9].y = -1.15;
                coins[9].vely =0;
                coins[9].velx = 0;
                strike = 0;
                len = 0.10f ;
                strike_angle = 0.0 ;
            }
    }
    int check=0;
    if(abs(coins[9].velx) < 0.001 && abs(coins[9].vely)<0.001){
        //printf("**\n");
        for(int i=0;i<9;i++){
            if(coins[i].vely > 0.001 || coins[i].velx > 0.001 ){
                check=1;
            }
        }
        if(check==0){
                //printf("*\n");
                coins[9].x = 0;
                coins[9].y = -1.15;
                coins[9].vely =0;
                coins[9].velx = 0;
                strike = 0;
                len = 0.10f ;
                strike_angle = 0.0 ;
            }
    }
    // Update position of ball
    coins[9].x += coins[9].velx;
    coins[9].y += coins[9].vely;
    }
    glutTimerFunc(10, StrikerUpdateWall, 0);
}
void CoinUpdate(int value){
    if(strike==1){
        
    
    for(int i=0;i<9 ;i++) {
    if(coins[i].x + coins[i].rad > box_len_inner/2 || coins[i].x - coins[i].rad < -box_len_inner/2)
        coins[i].velx *= -1;
    if(coins[i].y + coins[i].rad > box_len_inner/2 || coins[i].y - coins[i].rad < -box_len_inner/2)
        coins[i].vely *= -1;

    float a1 = pow(1.67 - coins[i].x, 2);
    float a2 = pow(1.67 + coins[i].x, 2); 
    float b1 = pow(1.67 - coins[i].y, 2);
    float b2 = pow(1.67 + coins[i].y, 2);
    float dist1 = a1 + b1; 
    float dist2 = a1 + b2;
    float dist3 = a2 + b1;
    float dist4 = a2 + b2; 
    float rad_dist = pow(hole_rad- coins[i].rad, 2); 
    if( (rad_dist >= dist1 || rad_dist >= dist2 || rad_dist >= dist3 || rad_dist >= dist4) && coins[i].avail==1){
        
            coins[i].avail = 0;
           
            if(color_game == 1){
                if(coins[i].type == 2){
                    score += 10;
                }
                else if(coins[i].type == 1){
                    score -= 5;
                }
                else if(coins[i].type ==0){
                    score += 50;
                }
            }
            if(color_game == 0){
                if(coins[i].type == 1){
                    score += 10;
                }
                else if(coins[i].type == 2){
                    score -= 5;
                }
                else if(coins[i].type == 0){
                    score += 50;
                }
            }
    }
    
    coins[i].x += coins[i].velx;
    coins[i].y += coins[i].vely;

    }
    }

    glutTimerFunc(10, CoinUpdate, 0);

}
/*void Striker_Coin(int value){
    for(int i=0;i<9 ; i++){
        float a1 = pow(coins[9].x - coins[i].x,2);
        float a2 = pow(coins[9].y - coins[i].y , 2);
        float a3 = a1 + a2 ;
        float r1 = pow (striker + coins[i].rad,2) ;
        if (a3 <= r1)
        {
            float velx1=coins[i].velx;
            float vely1=coins[i].vely;
            float velx2=coins[9].velx;
            float vely2=coins[9].vely;

        //check_coin velocity
        coins[i].velx=(4*velx2 - velx1)/3;
        coins[i].vely=(4*vely2 - vely1)/3;
        //check_striker velocity
        coins[9].velx=(2*velx1 + velx2)/3;
        coins[9].vely=(2*vely1 + vely2)/3;
        }
    }
    glutTimerFunc(10, Striker_Coin, 0);
}*/

void DrawLine(float a,float b){
        glColor3f(0.0, 0.0, 0.0);
        glPushMatrix();
        float x2= a  ;
        float y2 = b+len;
        glTranslatef(a,b,0.0f);
        glRotatef(strike_angle,  0,  0,  -4.0f);
        glTranslatef(-a,-b,0.0f);   
        glBegin(GL_LINES);
        glVertex3f(a, b, 0.0);
        glVertex3f(x2, y2, 0.0);
        glEnd();
        glPopMatrix();
}
// Function to draw objects on the screen
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    // Draw Box
    glTranslatef(0.0f, 0.0f, -6.0f);
    glColor3f(0.6f , 0.32f , 0.17f);
    drawBox(box_len_outer,1);

    glPushMatrix();
    glColor3f(1.0f,0.92f , 0.80f);
    drawBox(box_len_inner,1);
    glPopMatrix();
    

    //Four holes 
    glPushMatrix();
    glTranslatef(1.67f, 1.67f, 0.0f);
    glColor3f(0.0f,0.0f,0.0f);
    drawBall(hole_rad,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.67f, -1.67f, 0.0f);
    glColor3f(0.0f,0.0f,0.0f);
    drawBall(hole_rad,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.67f, 1.67f, 0.0f);
    glColor3f(0.0f,0.0f,0.0f);
    drawBall(hole_rad,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.67f, -1.67f, 0.0f);
    glColor3f(0.0f,0.0f,0.0f);
    drawBall(hole_rad,1);
    glPopMatrix();
    //Ending of holes code

    glPushMatrix();
    glColor3f(1.0f,0.0f , 0.0f);
    drawBox(box_len,0);
    glPopMatrix();
	
	glPushMatrix();
    glTranslatef(1.15f, 1.15f, 0.0f);
    glColor3f(1.0f,0.0f,0.0f);
    drawBall(striker,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.15f, 1.15f, 0.0f);
    glColor3f(1.0f,0.0f,0.0f);
    drawBall(striker,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(1.15f, -1.15f, 0.0f);
    glColor3f(1.0f,0.0f,0.0f);
    drawBall(striker,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-1.15f, -1.15f, 0.0f);
    glColor3f(1.0f,0.0f,0.0f);
    drawBall(striker,1);
    glPopMatrix();

    int i;
    if(coins[9].avail==1)
    coins[9].MakeCircle();
    if (strike ==0)
        DrawLine(coins[9].x,coins[9].y);
    for(i=0;i<9;i++){
    //printf("%d\n",i);
        if (coins[i].avail == 1)
            coins[i].MakeCircle();
    }

    glPushMatrix();
    ostringstream sc;
    sc << score;
    scor = "Current Score:" + sc.str();
    glColor3f(0.0f,0.0f, 0.0f);
    drawText(scor.data(), scor.size(), 2.7f, 1.0f);
    glPopMatrix();
    // Draw Ball
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 0.02f, 0.0f);
    drawBall(0.35 , 0);
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

void drawBox(float len, int flag) {
	
	if (flag==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);		
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);	
	}
    glBegin(GL_QUADS);
    glVertex2f(-len / 2, -len / 2);
    glVertex2f(len / 2, -len / 2);
    glVertex2f(len / 2, len / 2);
    glVertex2f(-len / 2, len / 2);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawBall(float rad, int flag) {
    
   if (flag==1)
   {
   glBegin(GL_TRIANGLE_FAN);
	}
	else
	{
		glBegin(GL_LINES);
	}
   for(int i=0 ; i<360 ; i++) {
       glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
   }
   glEnd();
}



// Initializing some openGL 3D rendering options
void initRendering() {

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }
    if (key == 32){
        if( coins[9].velx || coins[9].vely)
            strike = 1;
    }
    if (key == 99){
        strike_angle += 10.0f;
        coins[9].velx = coins[9].v * sin( DEG2RAD( strike_angle));
        coins[9].vely = coins[9].v * cos(DEG2RAD( strike_angle));
    }
    if(key == 97 ){
        strike_angle -= 10.0f; 
        coins[9].velx = coins[9].v * sin(DEG2RAD( strike_angle));
        coins[9].vely = coins[9].v * cos(DEG2RAD( strike_angle));
    }
    if(key == 119 && strike==0){
        //w
        color_game = 1;
    }
    if(key == 98 && strike==0){
        color_game = 0;
    }

}
void handleKeypress2(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT && strike==0)
    {
        if(coins[9].x - 0.15 > -1.15f)
            coins[9].x -= 0.15f;
        else
            coins[9].x = -1.15f;
    }
    if (key == GLUT_KEY_RIGHT && strike==0)
    {
        if( coins[9].x + 0.15f <= 1.15f)
        coins[9].x += 0.15f;
        else
            coins[9].x = 1.15f;
    }
    if( key == GLUT_KEY_UP && strike==0)
    {
        len += 0.05 ;
        coins[9].v += 0.01f; 
        coins[9].velx = coins[9].v * sin(DEG2RAD( strike_angle));
        coins[9].vely = coins[9].v * cos(DEG2RAD( strike_angle));       
    }
    if( key == GLUT_KEY_DOWN && strike==0)
    {
        if(coins[9].v >=0 ){
        len -= 0.05 ;
        coins[9].v -= 0.01f;
        coins[9].velx = coins[9].v * sin(DEG2RAD(strike_angle));
        coins[9].vely = coins[9].v * cos(DEG2RAD( strike_angle));
    }
    }
      
}
void handleMouseclick(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        
        if (button == GLUT_LEFT_BUTTON)
        {
            
            float x1 = x;
            float y1 = y;
            float x2 = coins[9].x;
 
            float y2 = coins[9].y;
            
            Mouse strikernew;
            strikernew.windowCood(coins[9].x,coins[9].y);
            x2 = strikernew.a;
            y2 = strikernew.b;
           
            d = sqrt(pow((x1-x2),2) + pow((y1-y2),2));
           
            float theta = atan2f(y1-y2,x1-x2);
            strike_angle = theta * 180 / PI;
            
        }
           
        //else if (button == GLUT_RIGHT_BUTTON)
            //theta -= 15;
    }
    if (state == GLUT_UP)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            float scale = findscale();
            strike_angle = strike_angle + 90;
            //striker.line.setLength(scale, striker.strike.d);
            //printf("%f %f\n", scale,striker.strike.d);
            //striker.startMove();
            strike = 1;
            coins[9].v = coins[9].v + (scale)*(0.005)/d;
            coins[9].velx = coins[9].v * sin(DEG2RAD(strike_angle));
            coins[9].vely = coins[9].v * cos(DEG2RAD( strike_angle));
        }
    }
}


// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Collisions(int value){
   
    if(strike==1){
       
    for(int i=0;i<10;i++)
    {
        for(int j=i+1;j<10;j++)
        {
            float a = coins[i].x - coins[j].x;
            float b = coins[i].y - coins[j].y;
            float sumrad = coins[i].rad + coins[j].rad ;
           
            float sqrrad = sumrad * sumrad ;
            float distsqr = (a * a) + (b * b);


            float a1 = coins[i].x + coins[i].velx- coins[j].x - coins[j].velx;
            float b1 = coins[i].y + coins[i].vely - coins[j].y - coins[j].vely;
            float sumrad1 = coins[i].rad + coins[j].rad ;
           
            float sqrrad1 = sumrad1 * sumrad1 ;
            float distsqr1 = (a1 * a1) + (b1 * b1);
           
            if( distsqr <= sqrrad  && distsqr1 <= sqrrad1)
            {
                
                float collision[2];
                collision[0]=a;
                collision[1]=b;
                float distance = sqrt((a*a) + (b*b)) ;
                float unor[2];   unor[0]=collision[0]/distance;  unor[1] = collision[1]/distance;
                float u1[2]; u1[0]=coins[i].velx; u1[1]=coins[i].vely;
                float u2[2]; u2[0]=coins[j].velx; u2[1]=coins[j].vely;
                float utang[2]; utang[0]=-1*unor[1]; utang[1]=unor[0];
                float u1n = unor[0] * u1[0] + unor[1] * u1[1];
                float u1t = utang[0] * u1[0] + utang[1] * u1[1];
                float u2n = unor[0] * u2[0] + unor[1] * u2[1];
                float u2t = utang[0] * u2[0] + utang[1] * u2[1];    
                float v1n = (u1n * (coins[i].mass - coins[j].mass)+ (2 * coins[j].mass * u2n))/(coins[i].mass + coins[j].mass);
                float v2n = (u2n * (coins[j].mass - coins[i].mass)+ (2 * coins[i].mass * u1n))/(coins[i].mass + coins[j].mass);
                float v1nf[2]; v1nf[0] = v1n * unor[0]; v1nf[1] = v1n * unor[1];
                float v1tf[2]; v1tf[0] = u1t * utang[0]; v1tf[1] = u1t * utang[1];
                float v2nf[2]; v2nf[0] = v2n * unor[0]; v2nf[1] = v2n * unor[1];
                float v2tf[2]; v2tf[0] = u2t * utang[0]; v2tf[1] = u2t * utang[1];
                float v1[2]; v1[0] = v1nf[0] + v1tf[0];  v1[1] = v1nf[1] + v1tf[1];
                float v2[2]; v2[0] = v2nf[0] + v2tf[0];  v2[1] = v2nf[1] + v2tf[1];
                coins[i].velx = v1[0];
                coins[i].vely = v1[1];
                coins[j].velx = v2[0];
                coins[j].vely = v2[1];
                coins[i].velx *= 0.98;
                coins[i].vely *= 0.98;
                coins[j].velx *= 0.98;
                coins[j].vely *= 0.98;
                coins[i].x += coins[i].velx;
                coins[i].y += coins[i].vely;
                coins[j].x += coins[j].velx;
                coins[j].y += coins[j].vely;

            }
        }
        coins[i].velx *= 0.98;
        coins[i].vely *= 0.98;
        /*coins[i].x += coins[i].velx;
        coins[i].y += coins[i].vely;*/
    }
    }
    glutTimerFunc(10, Collisions, 0);
}

float findscale()
{
    Mouse edge1;
    edge1.windowCood(-box_len_inner/2,-box_len_inner/2);
    Mouse edge2;
    edge2.windowCood(-box_len_inner/2,box_len_inner/2);
    float x1 = edge1.a;
    float y1 = edge1.b;
    float x2 = edge2.a;
    float y2 = edge2.b;
    float d = sqrt(pow((x1-x2),2) + pow((y1-y2),2));
    //printf("d%f\n",d);
    return d;
}

