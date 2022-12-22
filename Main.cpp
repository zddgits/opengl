#include "MyTexMgr.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "MyTexMgr.h"
#include <GL/glut.h>
#define  GLUT_WHEEL_UP 3           //定义滚轮操作
#define  GLUT_WHEEL_DOWN 4

using namespace std;

static float _angle = 30.0f;
static float _cameraAngle = 0.0f;
static int oldX;
static int oldY;
static int X = 400;
static int Y = 240;
static int flag = 33;
static float _rotX=0;
static float _rotY=0;
static float xyz[3]={1,1,1};
static float r=200;

static GLUquadric*	_earth;
static MyTexMgr		_texLoader;


//键盘操作
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);break;
		//按键+为我旋转加速
		case '+':
			flag -= 3;
			break;
		//按键+为我旋转减速
		case '-':
			flag += 3;
			break;
		case 'w':
			Y += 20;
			break;
		case 'a':
			X -= 20;
			 break;
		case 's':
			Y -= 20;
			 break;
		case 'd':
			X += 20;
			break;
	}
}

//光照
void initLight()
{
	GLfloat	position[] = { -1.0,1.0,1.0,0};
	GLfloat	ambientLight[]={0.4,0.4,0.4,1};
	GLfloat diffuseLight[]={0.8,0.8,0.8,1};
	GLfloat specularLight[]={0.8,0.8,0.8,1};
	GLfloat posLight[]={400,240,1,1};
	//设置照明模型参数
	//全局环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);
	//环境光
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	//位置
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	//漫射光
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	//反射光
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);
	//指定材质
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,128);
}

//定义D建模光照材质
void initRendering() {
	//创建二次曲面对象
	_earth=gluNewQuadric();
	//纹理装载（指定bmp位图位置）
	_texLoader.loadTex(L"earth",L"D:/download/earth.bmp");
	

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	initLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

//删除建模
void deinitRendering()
{
	gluDeleteQuadric(_earth);
}

//窗口调整调用
void handleResize(int w, int h) {
	if (h==0)
	{
		h=1;
	}	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,w,0,h,-200,200);
	glMatrixMode(GL_MODELVIEW);
}

//画地球
void drawEarth()
{
	static bool loaded=false;
	//纹理绑定到目标
	glBindTexture(GL_TEXTURE_2D,_texLoader.getTex(L"earth"));
	if (!loaded)
	{	
		//纹理坐标自动生成	
		glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		//表面生成纹理坐标
		gluQuadricDrawStyle(_earth,GL_FILL);
		gluQuadricNormals(_earth,GLU_SMOOTH);
		//生成纹理坐标
		gluQuadricTexture(_earth,GL_TRUE);
		
	}
	//生成球体
	glPushMatrix();
	{	
		glEnable(GL_TEXTURE_2D);
		glRotatef(-90,1,0,0);
		gluSphere(_earth,r,100,100);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();
	
}

//绘制D场景
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); //切换到绘图透视图
	glLoadIdentity(); //重置绘图透视图

	glPushMatrix();
	//实现缩放
	glScalef(xyz[0],xyz[1],xyz[2]);
	//此时需要把地球往后方移动防止放大的时候中间出现黑色圆圈
	glTranslatef(X,Y,-200);
	//实现拖动旋转
	glRotatef(_rotX/100,0,1,0);
	glRotatef(-_rotY/100,1,0,0);
	//实现自动旋转
	glRotatef(_angle,0,1,0);
	drawEarth();
	glPopMatrix();

	glutSwapBuffers();
}

//实现自动旋转效果
void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	//刷新
	glutPostRedisplay(); 
	
	//初始值为30fps
	glutTimerFunc(flag, update, 0);
}

//处理鼠标点击旋转事件
//处理鼠标点击滑动事件
void handleMotion(int x,int y)
{
	int rx=x-oldX;
	int ry=480-y-oldY;
	_rotX+=rx;
	_rotY+=ry;
	//重画
	glutPostRedisplay();
}

//处理鼠标事件
void handleMouse(int button,int state,int x,int y)
{	
	//控制旋转
	static bool done=false;
	if (button==GLUT_LEFT_BUTTON)
	{	
		oldX=x;
		oldY=480-y;
	}	
	//放大
	if (button == GLUT_WHEEL_UP) {
		xyz[0] += 0.1; xyz[1] += 0.1; xyz[2] += 0.1;
		glutPostRedisplay();
	}
	//缩小
	if (button == GLUT_WHEEL_DOWN) {
		if (xyz[0] > 0.4) {
			xyz[0] -= 0.1; xyz[1] -= 0.1; xyz[2] -= 0.1;
		}
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	//初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 480);
	
	//创建窗口
	glutCreateWindow("地球仪");
	initRendering();
	
	//设置用户操作
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutMouseFunc(handleMouse);
	
	//运动效果
	glutTimerFunc(flag, update, 0); 
	
	glutMainLoop();
	//退出时删除建模
	deinitRendering();
	return 0;
}
