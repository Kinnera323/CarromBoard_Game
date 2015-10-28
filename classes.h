#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)



class Coin{
    public:
        float x;
        float y;
        float velx;
        float vely;
        float v;
        float rad;
        float r;float g;float b;
        float mass;
        int avail;
        int type;
        Coin(){
        	vely = 0.0f;
        	velx = 0.0f;
            v = 0.0f;
        	rad = 0.08f;
            avail = 1;
            mass = 1.0f;
        }
        void SetColor(float red,float green,float blue) {
        	r = red;
        	g = green ;
        	b = blue ;

        }
        void SetMass(float c){
            mass = c;
        }
        void SetPosition(float newx, float newy){
        	x = newx;
        	y = newy;
        }
        void SetRadius(float newradius){
        	rad = newradius;
        }
        void SetType(int value){
            type =value;
        }
        void MakeCircle() {
        	glPushMatrix();
    		glTranslatef(x, y, 0.0f);
    		glColor3f(r, g, b);
        	glBegin(GL_TRIANGLE_FAN);
   			for(int i=0 ; i<360 ; i++) {
       			glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
   			}
   			glEnd();
   		    glPopMatrix();

   		}
}coins[10];

class Mouse
{
    public:
    float x,y;
    GLdouble a,b,c;
    float d, angle;
    
 
    void windowCood(float p,float q)
    {
        x = p;
        y = q;
        GLdouble model[16];
        GLdouble  proj[16];  
        GLint  view[4];
        glGetDoublev(GL_MODELVIEW_MATRIX, model);
        glGetDoublev(GL_PROJECTION_MATRIX, proj);
        glGetIntegerv(GL_VIEWPORT, view);
        gluProject(-x,  y,  8.0f,  model,  proj,  view,  &a,  &b,  &c);
        //printf("this:%lf %lf %lf\n",a,b,c);
    }
};


