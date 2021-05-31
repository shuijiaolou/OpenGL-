#include <GL/glut.h>

#include <stdio.h>

#include <stdlib.h>

#include <vector>
#include <math.h>
using namespace std;
const GLint nNumPoints = 4;

const int N = 4;

int FX = 45;
int FY = 60;
struct Point { //Point结构体
	int x, y;
};

Point pt[4]; //存放点信息的数组，计算曲线时用到

vector<Point> vpt; //存放点的向量，输入数据时用到

bool bDraw; //是否绘制曲线，false则确定没有开始绘制曲线

int nInput; //点的数量

bool mouseLeftIsDown = false; //是否点击鼠标左键

bool mouseRightIsDown = false; //是否点击鼠标右键

int caculateSquareDistance(int x, int y, Point b) //计算鼠标与点的位置
{
	return (x - b.x) * (x - b.x) + (y - b.y);
}

void ControlPoint(vector<Point> vpt) //绘制目标点

{

	glPointSize(5); //点的大小

	for (int i = 0; i < vpt.size(); i++)

	{

		glBegin(GL_POINTS);

		glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(vpt[i].x, vpt[i].y); //红色

		glEnd();

	}

}
void setline(Point p1, Point p2) {
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();
}
void ReControlPoint(int x, int y) //绘制修改的目标点

{

	glPointSize(5); //点的大小
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 0.0f); glVertex2i(x, y); 
	glEnd();
}

void PolylineGL(Point* pt, int num) //绘制曲线

{

	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);
	//glColor3f(1.0f, 0, 0);
	glPointSize(15.0f);
	
	glVertex2i(pt[0].x, pt[0].y);
	glVertex2i(pt[1].x, pt[1].y);
	glVertex2i(pt[2].x, pt[2].y);
	glVertex2i(pt[3].x, pt[3].y);
	glEnd();
	double x, y;
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	
	float Px, Py;
	for (double t = 0; t <= 1; t += 0.01) { //微分，计算100个小点组成曲线

		Px = pt[0].x * pow((1 - t), 3) + 3 * pt[1].x * t * (1 - t) * (1 - t) + 3 * pt[2].x * t * t * (1 - t) + pt[3].x * t * t * t;//x方向曲线坐标
		Py = pt[0].y * pow((1 - t), 3) + 3 * pt[1].y * t * (1 - t) * (1 - t) + 3 * pt[2].y * t * t * (1 - t) + pt[3].y * t * t * t;//y方向曲线坐标
		glVertex2d(Px, Py);
	}

	glEnd();
	//glFlush();
	//glutSwapBuffers();

}

void myDisplay()

{

	glClear(GL_COLOR_BUFFER_BIT); //清空颜色

	glColor3f(1.0f, 0.0f, 0.0f);

	if (vpt.size() > 0) {

		ControlPoint(vpt); //绘制鼠标点击的点

	}

	if (bDraw)

	{
		glClear(GL_COLOR_BUFFER_BIT); //清空颜色
		glColor3f(1.0f, 0.0f, 0.0f);
		PolylineGL(pt, 4); //render curve

	}

	/*if (bDraw == true && nInput == 4) {

		glColor3f(1.0f, 1.0f, 0.0f);
		PolylineGL(pt, 4);
	}*/

	glFlush();
	glutSwapBuffers();

}

void Init()

{

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glShadeModel(GL_SMOOTH);

	

}

void Reshape(int w, int h) //适应窗口

{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void mouse(int button, int state, int x, int y) //鼠标点击事件，
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseLeftIsDown = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mouseLeftIsDown = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		mouseRightIsDown = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		mouseRightIsDown = false;
	}
	
	switch (button)

	{

	case GLUT_LEFT_BUTTON:

		if (state == GLUT_DOWN)

		{

			if (nInput == 0)

			{

				pt[0].x = x;

				pt[0].y = y;

				nInput = 1;//第一个点

				vpt.clear();//清空栈

				vpt.push_back(pt[0]); //将vpt的数据发送到pt点的数组之中

				bDraw = false; //对于点击数量不足4，全部不允许绘制曲线

				glutPostRedisplay();//重新构图

			}

			else if (nInput == 1) //第二个点

			{

				pt[1].x = x;

				pt[1].y = y;

				vpt.push_back(pt[1]);

				nInput = 2;

				glutPostRedisplay();//重新构图

			}

			else if (nInput == 2)//第三个点

			{

				pt[2].x = x;

				pt[2].y = y;

				vpt.push_back(pt[2]);

				nInput = 3;

				glutPostRedisplay();

			}

			else if (nInput == 3)//第四个点

			{

				pt[3].x = x;

				pt[3].y = y;

				bDraw = true;

				vpt.push_back(pt[3]);

				nInput = 4;

				glutPostRedisplay();

			}
			else if (nInput == 4)//第四个点
			{
				ReControlPoint(x, y); //绘制鼠标点击的点//pt[0].x =x; pt[0].y = y;

			}
		}

		break;

	default:

		break;

	}

}
void motion(int x, int y)       //移动点
{
	if (mouseLeftIsDown)        //左键移动控制点
	{

		if (caculateSquareDistance(x, y, pt[0]) < 40)     //防止鼠标移动过快点位无法及时读取，经测试，40为一个较适合的值
		{
			pt[0].x = x;
			pt[0].y = y;
		}
		else if (caculateSquareDistance(x, y, pt[1]) < 40)
		{
			pt[1].x = x;
			pt[1].y = y;
		}
		else if (caculateSquareDistance(x, y, pt[2]) < 40)
		{
			pt[2].x = x;
			pt[2].y = y;
		}
		else if (caculateSquareDistance(x, y, pt[3]) < 40)
		{
			pt[3].x = x;
			pt[3].y = y;
		}
	}
	
	glFlush();
	glutSwapBuffers();

	glutPostRedisplay();        //重新构图
}

GLfloat ctrlPoints[nNumPoints][N][3] =
{
	{
		{-4.0f, 0.0f, 4.0f},
		{-2.0f, 4.0f, 4.0f},
		{4.0f, 0.0f, 4.0f},
		{8.0f, -4.0f, 4.0f}
	},
	{
		{-4.0f, 0.0f, 0.0f},
		{-2.0f, 4.0f, 0.0f},
		{4.0f, 0.0f, 0.0f},
		{8.0f, -4.0f, 0.0f}
	},
	{
		{-4.0f, 0.0f, -4.0f},
		{-2.0f, 4.0f, -4.0f},
		{4.0f, 0.0f, -4.0f},
		{8.0f, -4.0f, -4.0f}
	},
	{
		{-4.0f, 0.0f, -8.0f},
		{-2.0f, 4.0f, -8.0f},
		{4.0f, 0.0f, -8.0f},
		{8.0f, -4.0f, -8.0f}
	}
};


int cnt = 0;

void DrawPoints(void)
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);
	for (int i = 0; i < nNumPoints; i++)
		for (int j = 0; j < N; j++)
			glVertex3fv(ctrlPoints[i][j]);
	glEnd();
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMap2f(GL_MAP2_VERTEX_3,0.0f,10.0f,3,3,0.0f,10.f,9,3,&ctrlPoints[0][0][0]);

	glEnable(GL_MAP2_VERTEX_3);

	glMapGrid2f(10, 0.0f, 10.0f,
		10, 0.0f, 10.0f);

	glEvalMesh2(GL_LINE, 0, 10, 0, 10);

	DrawPoints();

	glPopMatrix();

	glutSwapBuffers();
}

void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
}


void ChangeSize(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 10.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'd':
		glRotatef(-10.0, 0.0, 1.0, 0.0);
		break;
	case 'a':
		glRotatef(10.0, 0.0, 1.0, 0.0);
		break;
	case 'w':
		glRotatef(-10.0, 0.0, 0.0, 1.0);
		break;
	case 's':
		glRotatef(10.0, 0.0, 0.0, 1.0);
		break;
	}

	glutPostRedisplay();
}

static void idle(void)
{
	glutPostRedisplay();   //重新构图
}
void input()
{
	int flag = 0;
	int t = 1;
	
	while (t)
	{
		printf("请选择要实现的效果：\n");
			scanf_s("%d", &flag);
		if (flag == 1)
		{
			glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
			glutCreateWindow("Bezier曲线");//绘图窗口名

			Init();                   //自定义的启动函数，在这里更改代码行里的提示信息

			glutDisplayFunc(myDisplay);//以下是应用自定义的显示函数等等

			glutReshapeFunc(Reshape);

			glutMouseFunc(mouse);

			glutMotionFunc(motion);
			glutMainLoop();        //启动glut主循环
		}
		else if (flag == 2)
		{
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
			glutCreateWindow("Bezier 曲面");
			Init();
			glutReshapeFunc(ChangeSize);
			glutDisplayFunc(RenderScene);
			glutKeyboardFunc(key);
			glutIdleFunc(idle);
			SetupRC();
			glutMainLoop();        //启动glut主循环
		}
		else if (flag == 3)
		{
			t = 0;
		}
		else {
			printf("请输入正确的选项！\n");
			input();
		}
	}
}
int main(int argc, char* argv[])

{
	glutInit(&argc, argv);     //启动
	glutInitWindowPosition(argc, argc);
	glutInitWindowSize(800, 600);
	printf("1.贝赛斯曲线\n2.贝赛斯曲面\n3.退出\n");
	input();
	return 0;

}