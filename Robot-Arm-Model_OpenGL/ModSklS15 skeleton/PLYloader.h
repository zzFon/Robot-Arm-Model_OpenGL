#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <gl/glut.h>
using namespace std;

class PLYFile
{
public:
	PLYFile(string = "");
	~PLYFile();
	GLvoid print();

	// 4个用于显示和交互的回调函数
	GLboolean PLYdisplay();
	GLboolean PLYreshape(GLint, GLint);
	GLboolean PLYkey(GLubyte);
	GLboolean PLYskey(GLint);
private:
	GLboolean PLYLoad(string = "");

	string path;/**/
	//vertex information.For convinence,using vector as container
	GLint vertexNum;/*vertex number*/
	vector<GLvoid *>array;/*save information of vertex,different element in different array*/
	vector<GLsizei>count;/*number of properties each element contains*/
	vector<GLenum>propertyType;/*type of property*/
	vector<GLenum>elementType;/*type of element,must be GL_VERTEX_ARRAY/GL_NORMAL_ARRAY/GL_COLOR_ARRAY*/
	//face information
	GLint faceNum;/*face number*/
	GLuint *indices;/*index of face,indicate the index of vertex that each face contains*/
	//detail for projection translation
	GLdouble max[3];/*max of x,y,z*/
	GLdouble min[3];/*min of x,y,z*/
	GLdouble size;/*length of diagonal line对角线长度*/
	// 旋转
	GLdouble xRotAngle;
	GLdouble yRotAngle;
	// 缩放
	GLdouble scale[3];
};