#include <GLUT/glut.h>
#include <complex.h>
#include <math.h>
#include <stdio.h>

#define CONVERGENCE 1000
#define SPLIT_Z  900
#define R_START -2.2
#define R_END   0.5
#define I_START -1.35
#define I_END   1.35

#define VIEW_X_MAX 1.0 
#define VIEW_X_MIN -1.0
#define VIEW_Y_MAX 1.0
#define VIEW_Y_MIN -1.0

#define X_MIN 0.0
#define Y_MIN 0.0

double 
trans_x(double x, double x_min, double x_max)
{
    return (VIEW_X_MAX - VIEW_X_MIN) * (x-x_min) / (x_max - x_min)  + VIEW_X_MIN;
}

double 
trans_y(double y, double y_min, double y_max)
{
    return (VIEW_Y_MAX - VIEW_Y_MIN) * (y-y_min) / (y_max - y_min)  + VIEW_Y_MIN;
}

void
set_rgb(int c, double *r, double *g, double *b)
{
    
    *r = (c/1000)/10.0;
    *g = ((c%100)/10)/10.0;
    *b = (c%10)/10.0;
}


/* Normalized Iteration Count Algorithm */
/* color is choiced from color table depend on iteration
   iteration         red       green     blue
   mod 60 == 0       1.0       0.0-1.0   0
   mod 60 == 10      1.0-0.0   1.0       0
   mod 60 == 20      0         1.0       0.0-1.0
   mod 60 == 30      0         1.0-0.0   1.0
   mod 60 == 40      0.0-1.0   0         1.0
   mod 60 == 50      1.0       0         1.0-0.0
*/
void
set_rgb2(int c, double complex z)
{
    double n, r, g, b;
    int mod;

    n = c + 1 - (log(log(cabs(z)))) / log(2.0);
    mod = (((int)n/10)*10)%60;

    if( mod == 0) {
        r = 1.0;
        g = (n - (((int)n/60)*60 + 0)) / 10;
        b = 0;
    } else if ( mod == 10) {
        r = 1.0 - (n - (((int)n/60)*60 + 10)) / 10;
        g = 1.0;
        b = 0;
    } else if ( mod == 20) {
        r = 0;
        g = 1.0;
        b = (n - (((int)n/60)*60 + 20)) / 10;
    } else if ( mod == 30) {
        r = 0;
        g = 1.0-(n - (((int)n/60)*60 + 30)) / 10;
        b = 1.0;
    } else if ( mod == 40) {
        r = (n - (((int)n/60)*60 + 40)) / 10;
        g = 0;
        b = 1.0;
    } else if ( mod == 50) {
        r = 1.0;
        g = 0;
        b = 1.0-(n - (((int)n/60)*60 + 50)) / 10;
    }
    glColor3d(r, g, b);
}


void
mandelbrot(void)
{
    int x, y, i;
    double p, q, r, g, b;
    double d_real, d_imag;
    double complex z, z_next, c;
    
    d_real = (R_END-R_START)/SPLIT_Z;
    d_imag = (I_END-I_START)/SPLIT_Z;

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for (x=0; x<SPLIT_Z; x++) {
        for (y=0; y<SPLIT_Z; y++) {
            c = (x*d_real + R_START) + (y*d_imag + I_START)*I;
            z = 0.0 + 0.0*I;

            /* escape time algorithm */
            for (i=0; i<CONVERGENCE; i++) {
                z_next = cpow(z, 2.0) + c;

                if (cabs(z_next) > 4.0) {
                    //set_rgb(i, &r, &g, &b);
                    set_rgb2(i, z_next);
                    p = trans_x(x, X_MIN, SPLIT_Z);
                    q = trans_y(y, Y_MIN, SPLIT_Z);

                    glVertex2d(p, q);
                    break;
                }
                z = z_next;
            }
        }
    }
    glEnd(); 
    glFlush();
}

void 
init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0); 
}

int
main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(900, 900);
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(mandelbrot);
    init();
    glutMainLoop();
    return 0;
}
