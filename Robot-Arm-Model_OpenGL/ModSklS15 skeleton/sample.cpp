// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include <FL/gl.h>
#include <gl/glut.h>
#include"PLYLoader.h"

#include "modelerglobals.h"
#include <cstdlib>

#define  D 12
float BestX[D + 10];

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) : ModelerView(x,y,w,h,label) { }

    virtual void draw();// overrides draw(), see code ahead of main()
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel
int t = -1;
int direction = 1;
int cycle = 100;
void SampleModel::draw()
{
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5,-3,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);

	glPushMatrix();// PUSH
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));// M_translation

	    // Base
		glPushMatrix();// PUSH
		glTranslated(-1.5, -3, -1);// M_translation
		glScaled(3, 0.6, 2);// M_scale
		drawBox(1, 1, 1);// M_box
		glPopMatrix();// POP

	    // Support
	    glTranslated(0,-3,0);// M_translation
		glPushMatrix();// PUSH
		//glRotated(VAL(ROTATE), 0, 1, 0);// M_rotation
		glRotated(-90, 1, 0, 0);// M_rotation
		drawCylinder(3*VAL(HEIGHT_SUPPORT),0.2,0.2);// M_cylinder
		if(VAL(drawOBB_flag) == 1)
		  drawOBB(0,0,0,0.4,0.4, 3 * VAL(HEIGHT_SUPPORT),0,0,0);
		//drawOBB(0, 0, 0, BestX[1], BestX[1], BestX[2], 0, 0, 0);

		// JointA
		glTranslated(0,0, 3*VAL(HEIGHT_SUPPORT));// M_tranlation
		drawSphere(0.3);// M_sphere
		glPushMatrix();
		glTranslated(0,0,-0.3);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.6004, 0.6004, 0.6004, 0, 0, 0);
		glPopMatrix();

		// ArmA
		if(t == -1)
			glRotated(VAL(ROTATE_ARMA), 0, 1, 0);
		else if(0<=t && t <=cycle)
			glRotated(VAL(ROTATE_ARMA)+(120.0f- VAL(ROTATE_ARMA))/cycle*t, 0, 1, 0);// M_rotation
		else if (cycle<t)
			glRotated(VAL(ROTATE_ARMA) + (120.0f - VAL(ROTATE_ARMA)), 0, 1, 0);// M_rotation
		drawCylinder(2*VAL(HEIGHT_ARMA), 0.2, 0.2);// M_cylinder
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.4, 0.4, 1.0086*2* VAL(HEIGHT_SUPPORT), 0, 0, 0);

		// JointB
		glTranslated(0,0,2*VAL(HEIGHT_ARMA));// M_translation
		drawSphere(0.3);// M_sphere
		glPushMatrix();
		glTranslated(0, 0, -0.3);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.6, 0.6, 0.6, 0, 0, 0);
		glPopMatrix();

		// ArmB
		if (t == -1)
			glRotated(VAL(ROTATE_ARMB), 1, 0, 0);
		else if (0 <= t && t <= cycle)
			glRotated(VAL(ROTATE_ARMB)+(90.0f- VAL(ROTATE_ARMB))/cycle*t, 1, 0, 0);// M_rotation
		else if (cycle < t)
			glRotated(VAL(ROTATE_ARMB) + (90.0f - VAL(ROTATE_ARMB)), 1, 0, 0);// M_rotation
		drawCylinder(0.8*VAL(HEIGHT_ARMB), 0.15, 0.15);// M_cylinder
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.3029, 0.3029, 0.8163 * VAL(HEIGHT_SUPPORT), 0, 0, 0);

		// Pin
		if(-1 <= t && t <= cycle)
			glRotated(VAL(ROTATE_PIN), 0, 0, 1);
		else if (cycle < t && t <= 2*cycle)
			glRotated(VAL(ROTATE_PIN) + (120.f - VAL(ROTATE_PIN)) / (cycle)*(t - cycle), 0, 0, 1);
		else if (2*cycle < t)
			glRotated(VAL(ROTATE_PIN) + (120.f - VAL(ROTATE_PIN)), 0, 0, 1);
		glTranslated(0, 0, 0.8*VAL(HEIGHT_ARMB));
		drawCylinder(0.3*VAL(HEIGHT_PIN), 0.05, 0.05);// M_cylinder
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.1482, 0.1482, 0.5361 * VAL(HEIGHT_SUPPORT), 0, 0, 0);

		// connecter
		glTranslated(0, 0, 0.3*VAL(HEIGHT_PIN));
		drawSphere(0.08);

        // upper-claw1
		glPushMatrix();
		if(t == -1)
			glRotated(VAL(CLAW), 0, 1, 0);
		else if(0 <= t && t <= cycle)
			glRotated(VAL(CLAW)+(85.0f-VAL(CLAW))/(cycle)*t,0,1,0);
		else if (cycle < t && t <= 2 * cycle)
			glRotated(85.0f, 0, 1, 0);
		else if(2*cycle < t)
			glRotated(85.0f+(60.0f-85.0f)/(0.5*cycle)*(t-2*cycle), 0, 1, 0);
		drawCylinder(0.3*VAL(CLAW_SIZE),0.05,0.05);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.1674, 0.1674, 0.3874*VAL(CLAW_SIZE), 0, 0, 0);

		// lower-claw1
		glTranslated(0, 0, 0.3*VAL(CLAW_SIZE));
		drawSphere(0.08);
		glRotated(-90, 0, 1, 0);
		drawCylinder(0.3*VAL(CLAW_SIZE), 0.05, 0.05);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.1674, 0.1674, 0.3874*VAL(CLAW_SIZE), 0, 0, 0);

		// upper-claw2
		glPopMatrix();
		if (t == -1)
			glRotated(-VAL(CLAW), 0, 1, 0);
		else if (0 <= t && t <= cycle)
			glRotated(-(VAL(CLAW) + (85.0f - VAL(CLAW)) / (cycle)*t), 0, 1, 0);
		else if (cycle < t && t <= 2 * cycle)
			glRotated(-85.0f, 0, 1, 0);
		else if (2 * cycle < t)
			glRotated(-(85.0f + (60.0f - 85.0f) / (0.5*cycle)*(t - 2 * cycle)), 0, 1, 0);
		drawCylinder(0.3*VAL(CLAW_SIZE), 0.05, 0.05);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.1674, 0.1674, 0.3874*VAL(CLAW_SIZE), 0, 0, 0);

		// lower-claw2
		glTranslated(0, 0, 0.3*VAL(CLAW_SIZE));
		drawSphere(0.08);
		glRotated(90, 0, 1, 0);
		drawCylinder(0.3*VAL(CLAW_SIZE), 0.05, 0.05);
		if (VAL(drawOBB_flag) == 1)
			drawOBB(0, 0, 0, 0.1674, 0.1674, 0.3874*VAL(CLAW_SIZE), 0, 0, 0);

		glPopMatrix();// POP
	glPopMatrix();// POP

	if (t == 0)
	{
		direction = 1;
		Sleep(800);
		/*for (int i = 1; i <= 100000000/10*8; i++)
		{
			i = i;
		}*/
	}
	else if (t == 2.5*cycle)
	{
		direction = -1;
		//Sleep(800);
		/*for (int i = 1; i <= 100000000 / 10 * 8; i++)
		{
			i = i;
		}*/
	}

	if (ModelerApplication::Instance()->m_animating == true)// Animation Enabled
	{
		if (t == -1)
			t = 0;
		t = t + direction;
	}
	else if (ModelerApplication::Instance()->m_animating == false)// Animation Disabled
		t = -1;
}

void SA();
GLvoid init();
GLvoid display();
GLvoid reshape(GLint, GLint);
GLvoid keyboard(GLubyte, GLint, GLint);
GLvoid specialKey(GLint, GLint, GLint);
int main(int argc, char** argv)
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	//                               stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];// contains the inf to make a control
	// get controls from ModerlerControl
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[HEIGHT_SUPPORT] = ModelerControl("Support Length (scale)", 1, 2.5, 0.1f, 1);
    controls[HEIGHT_ARMA] = ModelerControl("Arm_A Length (scale)", 1, 2.5, 0.1f, 1);
	controls[HEIGHT_ARMB] = ModelerControl("Arm_B Length (scale)", 1, 2.5, 0.1f, 1);
	controls[HEIGHT_PIN] = ModelerControl("Pin Length (scale)", 1, 2.5, 0.1f, 1);
	controls[CLAW_SIZE] = ModelerControl("Claw Size", 1, 2.5, 0.1f, 1.8f);
	controls[ROTATE_ARMA] = ModelerControl("Rotation of Arm_A", -135, 135, 1, 45);
	controls[ROTATE_ARMB] = ModelerControl("Rotation of Arm_B", -135, 135, 1, 0);
	controls[ROTATE_PIN] = ModelerControl("Rotation of Pin", -180, 180, 1, -30);
	controls[CLAW] = ModelerControl("Claw", 46, 100, 1, 50);
	controls[drawOBB_flag] = ModelerControl("Draw OBB", 0, 1, 1, 0);

	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKey);
	glutMainLoop();*/

	// create the sliders
	ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}

GLvoid init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);//渐变模式。GL_FLAT:单色模式
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float tight(float a1,float h1,float a2,float a3,float h3,float a4,float a5,float h5,float a6,float h6,float a7,float h7)
{
	
	float V_Support = 3.14 * 0.2*0.2 * 3;               float V_OBB_Support = a1*a1*h1;
	float V_JointA = (4.0 / 3.0)*3.14*0.3*0.3*0.3;      float V_OBB_JointA = a2*a2*a2;
	float V_ArmA = 3.14 * 0.2*0.2 * 1;                  float V_OBB_ArmA = a3*a3*h3;
	float V_JointB = (4.0 / 3.0)*3.14*0.3*0.3*0.3;      float V_OBB_JointB = a4*a4*a4;
	float V_ArmB = 3.14 * 0.15*0.15*0.8;                float V_OBB_ArmB = a5*a5*h5;
	float V_Pin = 3.14 * 0.05*0.05*0.3;                 float V_OBB_Pin = a6*a6*h6;
	float V_Claw = 4 * 3.14*0.05*0.05*0.3;              float V_OBB_Claw = 4 * a7*a7*h7;
	float V_model = V_Support + V_JointA + V_ArmA + V_JointB + V_ArmB + V_Pin + V_Claw;
	float V_OBB = V_OBB_Support + V_OBB_JointA + V_OBB_ArmA + V_OBB_JointB + V_OBB_ArmB + V_OBB_Pin + V_OBB_Claw;
	float output = V_OBB - V_model;
	return output;
}

void SA()
{
	float Xx[] = {0, 0.2 * 2, 3, 0.3 * 2, 0.2 * 2, 1, 0.3 * 2, 0.15 * 2, 0.8, 0.05 * 2, 0.3, 0.05 * 2, 0.3 };
	float Xs[] = {0, 0.4 * 2, 6, 0.6 * 2, 0.4 * 2, 2, 0.6 * 2, 0.30 * 2, 1.6, 0.10 * 2, 0.6, 0.10 * 2, 0.6};

	int L = 20; 
	float K = 0.998; 
	float S = 0.01; 
	float T = 100; 
	float YZ = 1e-8;

	float PreX[D+10];
	float PreBestX[D+10];
	for (int i = 1; i <= D; i++)
	{
		PreX[i] = Xx[i] + (rand()%1000/1000.0) * (Xs[i] - Xx[i]);
		PreBestX[i] = PreX[i];
	}
	for (int i = 1; i <= D; i++)
	{
		PreX[i] = Xx[i] + (rand() % 1000 / 1000.0) * (Xs[i] - Xx[i]);
		BestX[i] = PreX[i];
	}

	float delta = abs(tight(BestX[1], BestX[2], BestX[3], BestX[4], BestX[5], BestX[6], BestX[7], BestX[8], BestX[9], BestX[10], BestX[11], BestX[12]) - tight(PreBestX[1], PreBestX[2], PreBestX[3], PreBestX[4], PreBestX[5], PreBestX[6], PreBestX[7], PreBestX[8], PreBestX[9], PreBestX[10], PreBestX[11], PreBestX[12]));
	float NextX[D + 1];
	while ((delta > YZ) && (T > 0.01))
	{
		T = K * T;
		for (int i = 1; i <= L; i++)
		{
			for (int j = 1; j <= D; j++)
			{
				NextX[j] = PreX[j] + S * ((2 * (rand() % 1000 / 1000.0) - 1)*(Xs[j] - Xx[j]));
				if (NextX[j] > Xs[j])
					NextX[j] = Xs[j];
				if (NextX[j] < Xx[j])
					NextX[j] = Xx[j];
			}

			if (tight(NextX[1], NextX[2], NextX[3], NextX[4], NextX[5], NextX[6], NextX[7], NextX[8], NextX[9], NextX[10], NextX[11], NextX[12]) < tight(BestX[1], BestX[2], BestX[3], BestX[4], BestX[5], BestX[6], BestX[7], BestX[8], BestX[9], BestX[10], BestX[11], BestX[12]))
			{
				PreBestX[1] = BestX[1];
				PreBestX[2] = BestX[2];
				PreBestX[3] = BestX[3];
				PreBestX[4] = BestX[4];
				PreBestX[5] = BestX[5];
				PreBestX[6] = BestX[6];
				PreBestX[7] = BestX[7];
				PreBestX[8] = BestX[8];
				PreBestX[9] = BestX[9];
				PreBestX[10] = BestX[10];
				PreBestX[11] = BestX[11];
				PreBestX[12] = BestX[12];

				BestX[1] = NextX[1];
				BestX[2] = NextX[2];
				BestX[3] = NextX[3];
				BestX[4] = NextX[4];
				BestX[5] = NextX[5];
				BestX[6] = NextX[6];
				BestX[7] = NextX[7];
				BestX[8] = NextX[8];
				BestX[9] = NextX[9];
				BestX[10] = NextX[10];
				BestX[11] = NextX[11];
				BestX[12] = NextX[12];
			}
			if (tight(NextX[1], NextX[2], NextX[3], NextX[4], NextX[5], NextX[6], NextX[7], NextX[8], NextX[9], NextX[10], NextX[11], NextX[12]) < tight(PreX[1], PreX[2], PreX[3], PreX[4], PreX[5], PreX[6], PreX[7], PreX[8], PreX[9], PreX[10], PreX[11], PreX[12]))
			{
				PreX[1] = NextX[1];
				PreX[2] = NextX[2];
				PreX[3] = NextX[3];
				PreX[4] = NextX[4];
				PreX[5] = NextX[5];
				PreX[6] = NextX[6];
				PreX[7] = NextX[7];
				PreX[8] = NextX[8];
				PreX[9] = NextX[9];
				PreX[10] = NextX[10];
				PreX[11] = NextX[11];
				PreX[12] = NextX[12];
			}
			else
			{
				float changer = -1 * (tight(NextX[1], NextX[2], NextX[3], NextX[4], NextX[5], NextX[6], NextX[7], NextX[8], NextX[9], NextX[10], NextX[11], NextX[12]) - tight(PreX[1], PreX[2], PreX[3], PreX[4], PreX[5], PreX[6], PreX[7], PreX[8], PreX[9], PreX[10], PreX[11], PreX[12])) / T;
				float p1 = exp(changer);
				if (p1 > (rand() % 1000 / 1000.0))
				{
					PreX[1] = NextX[1];
					PreX[2] = NextX[2];
					PreX[3] = NextX[3];
					PreX[4] = NextX[4];
					PreX[5] = NextX[5];
					PreX[6] = NextX[6];
					PreX[7] = NextX[7];
					PreX[8] = NextX[8];
					PreX[9] = NextX[9];
					PreX[10] = NextX[10];
					PreX[11] = NextX[11];
					PreX[12] = NextX[12];
				}
			}
		}
		delta = abs(tight(BestX[1], BestX[2], BestX[3], BestX[4], BestX[5], BestX[6], BestX[7], BestX[8], BestX[9], BestX[10], BestX[11], BestX[12]) - tight(PreBestX[1], PreBestX[2], PreBestX[3], PreBestX[4], PreBestX[5], PreBestX[6], PreBestX[7], PreBestX[8], PreBestX[9], PreBestX[10], PreBestX[11], PreBestX[12]));
	}
}
