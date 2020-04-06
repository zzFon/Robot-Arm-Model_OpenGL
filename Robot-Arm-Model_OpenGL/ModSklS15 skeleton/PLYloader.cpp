#include"PLYLoader.h"

PLYFile::PLYFile(string p)
{
	path = p;
	indices = NULL;
	max[0] = 0; max[1] = 0; max[2] = 0;
	min[0] = 0; min[1] = 0; min[2] = 0;
	xRotAngle = 0;
	yRotAngle = 0;
	PLYLoad();
	size = sqrt(pow(max[0] - min[0], 2) + pow(max[1] - min[1], 2) + pow(max[2] - min[2], 2));
	scale[0] = 1;
	scale[1] = 1;
	scale[2] = 1;
}

PLYFile::~PLYFile()
{
	if (indices) delete[]indices;
	for (int i = 0; i < array.size(); i++)
		delete[]array[i];
}
/*******************************************************
 *PLYLoad
 *用于读取ply文件，存入类内的变量
 *传入参数p是文件路径，默认为空。如果构建类的时候传入了文件路径，此处可以省略。
 *返回值为1表示读取成功，反之失败。
 *******************************************************/
// 读取.ply
GLboolean PLYFile::PLYLoad(string p)
{
	fstream fopen;
	if (!p.empty())
		fopen.open(p, ios::in);
	else if (!path.empty())
		fopen.open(path, ios::in);
	else return 0;
	if (!fopen.is_open())
	{
		return 0;//open file fail!
		cout << "read file fail!" << endl;
	}
	//start to read
	string filein;
	while (!fopen.eof())
	{
		fopen >> filein;
		if (filein == "ply" || filein == "PLY")
		{	//this is the header of .ply file,add TODO here
		}
		else if (filein == "comment")
		{	//this is the comment,add TODO here
			getline(fopen, filein, '\n');
		}
		else if (filein == "format")
		{	//this is the format information,add TODO here
			getline(fopen, filein, '\n');
		}
		else if (filein == "element")
		{	//element information,only accept vertex and face.ignore user-defined element
			fopen >> filein;
			if (filein == "vertex")
			{	//read the number of vertex
				fopen >> vertexNum;
			}
			else if (filein == "face")
			{	//read the number of face 
				fopen >> faceNum;
				//ignore this and the next lines
				getline(fopen, filein, '\n');
				getline(fopen, filein, '\n');
			}
			else
			{	//user-defined element
				cout << "ignore user-defined element!" << endl;
				getline(fopen, filein, '\n');
			}
		}
		else if (filein == "property")
		{	//property,only accept xyz/nxyz/rgb
			fopen >> filein;//read in type of element
			if (filein == "char" || filein == "int8")
			{
				propertyType.push_back(GL_BYTE);
			}
			else if (filein == "uchar" || filein == "uint8")
			{
				propertyType.push_back(GL_UNSIGNED_BYTE);
			}
			else if (filein == "short" || filein == "int16")
			{
				propertyType.push_back(GL_SHORT);
			}
			else if (filein == "ushort" || filein == "uint16")
			{
				propertyType.push_back(GL_UNSIGNED_SHORT);
			}
			else if (filein == "int" || filein == "int32")
			{
				propertyType.push_back(GL_INT);
			}
			else if (filein == "uint" || filein == "uint32")
			{
				propertyType.push_back(GL_UNSIGNED_INT);
			}
			else if (filein == "float" || filein == "float32")
			{
				propertyType.push_back(GL_FLOAT);
			}
			else if (filein == "double" || filein == "float64")
			{
				propertyType.push_back(GL_DOUBLE);
			}
			else
			{	//use GLdouble as default
				cout << "Undefined property type!" << endl;
				propertyType.push_back(GL_DOUBLE);
			}
			fopen >> filein;
			if (filein == "x")
			{
				elementType.push_back(GL_VERTEX_ARRAY);
				getline(fopen, filein, '\n');//x
				getline(fopen, filein, '\n');//y
				count.push_back(2);
			}
			else if (filein == "nx")
			{
				elementType.push_back(GL_NORMAL_ARRAY);
				getline(fopen, filein, '\n');//nx
				getline(fopen, filein, '\n');//ny
				getline(fopen, filein, '\n');//nz
				count.push_back(3);
			}
			else if (filein == "red")
			{
				elementType.push_back(GL_COLOR_ARRAY);
				getline(fopen, filein, '\n');//red
				getline(fopen, filein, '\n');//green
				getline(fopen, filein, '\n');//blue
				count.push_back(3);
			}
			else if (/*filein=="y"||*/filein == "z" || filein == "w"/*||filein=="green"||filein=="blue"*/ || filein == "alpha")
			{
				count[count.size() - 1] += 1;
				propertyType.pop_back();
			}
			else
			{
				cout << "Unknown property!" << endl;
			}
		}
		else if (filein == "end_header")
		{	//header end,add TODO here
			//ckeck
			if (count.size() != propertyType.size() || count.size() != elementType.size())
			{
				cout << "the number of element collision" << endl;
				return 0;
			}
			//start to read element information
			//initialise the array
			GLvoid *subarray = NULL;
			for (int i = 0; i < propertyType.size(); i++)
			{
				switch (propertyType[i])
				{
					case GL_BYTE:subarray = new GLbyte[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_UNSIGNED_BYTE:subarray = new GLubyte[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_SHORT:subarray = new GLshort[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_UNSIGNED_SHORT:subarray = new GLushort[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_INT:subarray = new GLint[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_UNSIGNED_INT:subarray = new GLuint[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_FLOAT:subarray = new GLfloat[vertexNum*count[i]];
						array.push_back(subarray); break;
					case GL_DOUBLE:subarray = new GLdouble[vertexNum*count[i]];
						array.push_back(subarray); break;
				}
			}
			if (faceNum)
				indices = new GLuint[faceNum * 3];
			//read vertex information
			int totalelement = 0;
			for (int i = 0; i < count.size(); i++)
				totalelement += count[i];
			// 读取face
			for (int i = 0; i < vertexNum; i++)//接下来的vertexNum行都是顶点信息
			{
				for (int j = 0; j < count.size(); j++)//每行包括顶点的count.size()(<=3)种信息
					for (int k = 0; k < count[j]; k++)//每种信息包括count[j]个property
					{
						switch (propertyType[j])
						{
							case GL_BYTE: {
								GLdouble tem = 0;//(GLdouble)(((GLbyte*)(array[j]))[i*count[j]+k]);
								fopen >> tem;
								((GLbyte*)(array[j]))[i*count[j] + k] = GLbyte(tem);
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break; }
							case GL_UNSIGNED_BYTE: {
								GLint tem = 0;//(GLdouble)(((GLubyte*)(array[j]))[i*count[j]+k]);
								fopen >> tem;
								((GLubyte*)(array[j]))[i*count[j] + k] = GLubyte(tem);
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break; }
							case GL_SHORT:
								fopen >> ((GLshort*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = (GLdouble)(((GLshort*)(array[j]))[i*count[j] + k]);
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break;
							case GL_UNSIGNED_SHORT:
								fopen >> ((GLushort*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = (GLdouble)(((GLushort*)(array[j]))[i*count[j] + k]);
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break;
							case GL_INT:
								fopen >> ((GLint*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = (GLdouble)(((GLint*)(array[j]))[i*count[j] + k]);
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break;
							case GL_UNSIGNED_INT:
								fopen >> ((GLuint*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = (GLdouble)(((GLuint*)(array[j]))[i*count[j] + k]);
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break;
							case GL_FLOAT: {
								fopen >> ((GLfloat*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = (GLdouble)(((GLfloat*)(array[j]))[i*count[j] + k]);
									if (tem > max[k])max[k] = GLdouble(tem);
									if (tem < min[k])min[k] = GLdouble(tem);
								}
								break; }
							case GL_DOUBLE:
								fopen >> ((GLdouble*)(array[j]))[i*count[j] + k];
								if (elementType[j] == GL_VERTEX_ARRAY && k < 3)
								{
									GLdouble tem = ((GLdouble*)(array[j]))[i*count[j] + k];
									if (tem > max[k])max[k] = tem;
									if (tem < min[k])min[k] = tem;
								}
								break;
						}
					}
			}
			//read face information
			int front = 0, i = 0;;
			fopen >> front;
			while (front == 3 && i < faceNum)
			{
				fopen >> indices[i * 3] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
				fopen >> front;
				i++;
			}

		}
		else
		{
			continue;
		}
	}
	fopen.close();
	return 1;//finished
}
/*******************************************************
 *PLYdisplay
 *显示函数，需放在glutDisplayFunc()定义的回调函数内调用
 *返回值可以忽略
 *******************************************************/
// 绘制
GLboolean PLYFile::PLYdisplay()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	// 缩放
	glScalef(scale[0], scale[1], scale[2]);
	// 旋转
	glRotatef(xRotAngle, 1.0, 0.0, 0.0);
	glRotatef(yRotAngle, 0.0, 1.0, 0.0);
	//smooth
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	// 坐标轴绘制
	/*glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f((min[0] < 0) ? (1.4*(min[0])) : (-0.8*min[0]), 0.0, 0.0);
	glVertex3f(1.8*(max[0]), 0.0, 0.0);
	glVertex3f(0.0, (min[1] < 0) ? (1.4*(min[1])) : (-0.8*min[1]), 0.0);
	glVertex3f(0.0, 1.8*(max[1]), 0.0);
	glVertex3f(0.0, 0.0, (min[2] < 0) ? (1.4*(min[2])) : (-0.8*min[2]));
	glVertex3f(0.0, 0.0, 1.8*(max[2]));
	glEnd();

	glPushMatrix();
	glTranslatef(1.8*(max[0]), 0.0, 0.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.8*(max[1]), 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.8*(max[2]));
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glutSolidCone(0.02*size, 0.04*size, 10, 10);
	glPopMatrix();*/

	//draw object
	glColor3f(1.0, 1.0, 1.0);
	//enable and assign
	for (int i = 0; i < (elementType.size()); i++)
	{
		switch (elementType[i])
		{
			case GL_VERTEX_ARRAY:
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(count[i], propertyType[i], 0, array[i]);
				break;
			case GL_NORMAL_ARRAY:
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(propertyType[i], 0, array[i]);
				break;
			case GL_COLOR_ARRAY:
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(count[i], propertyType[i], 0, array[i]);
				break;
		}
	}
    
	// 绘制faces
	//use array
	for (int i = 0; i < faceNum; i++)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &(indices[i * 3]));
	}
	glPopMatrix();

	glutSwapBuffers();

	return 1;
}
/*******************************************************
 *PLYreshape
 *重绘函数，需放在glutReshapeFunc()定义的回调函数内调用
 *返回值可以忽略
 *******************************************************/
// 重绘的回调函数
GLboolean PLYFile::PLYreshape(GLint w, GLint h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, size * 0.05, size * 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(max[0] * 3, max[1] * 3, max[2] * 3,/*eye position, changeable*/
		0.0, 0.0, 0.0,/*a point that on the sight line. FIXED to zero point*/
		-max[0] * 3, -max[1] * 3, (pow(max[0], 2) + max[1], 2) * 3 / max[2]/*Specifies the direction of the up vector, vertical to the sight line*/);
	return 1;
}
/*******************************************************
 *PLYkey
 *普通按键处理函数，需放在glutKeyboardFunc()定义的回调函数内调用
 *返回值可以忽略
 *******************************************************/
// 缩放交互
GLboolean PLYFile::PLYkey(GLubyte key)
{
	switch (key)
	{
		case '=':
			scale[0] *= 1.1;
			scale[1] *= 1.1;
			scale[2] *= 1.1;
			break;
		case '-':
			scale[0] *= 0.9;
			scale[1] *= 0.9;
			scale[2] *= 0.9;
			break;
	}
	glutPostRedisplay();
	return 1;
}
/*******************************************************
 *PLYskey
 *特殊按键处理函数，需放在glutSpecialFunc()定义的回调函数内调用
 *返回值可以忽略
 *******************************************************/
// 旋转交互
GLboolean PLYFile::PLYskey(GLint key)
{
	switch (key)
	{
		case GLUT_KEY_UP:xRotAngle -= 5.0; break;
		case GLUT_KEY_DOWN:xRotAngle += 5.0; break;
		case GLUT_KEY_RIGHT:yRotAngle += 5.0; break;
		case GLUT_KEY_LEFT:yRotAngle -= 5.0; break;
	}
	glutPostRedisplay();
	return 1;
}
/*******************************************************
 *print
 *打印函数。将读入的问件内容显示出来。用于调试。
 *******************************************************/
// 参数打印
GLvoid PLYFile::print()
{
	cout << "PLY file" << endl;
	cout << "path:" << path << endl;
	cout << "VertexNum:" << vertexNum << endl;
	cout << "property number:" << array.size() << endl <<
		count.size() << endl <<
		propertyType.size() << endl <<
		elementType.size() << endl;
	cout << "count:" << endl;
	for (int i = 0; i < count.size(); i++)
		cout << i << " : " << count[i] << endl;
	cout << "propertyType" << endl;
	for (int i = 0; i < propertyType.size(); i++)
		cout << i << " : " << propertyType[i] << endl;
	cout << "elementType" << endl;
	for (int i = 0; i < elementType.size(); i++)
		cout << i << " : " << elementType[i] << endl;
	cout << "max:" << endl;
	cout << max[0] << '\t' << max[1] << '\t' << max[2] << '\t' << endl;
	cout << "min:" << endl;
	cout << min[0] << '\t' << min[1] << '\t' << min[2] << '\t' << endl;
	cout << "size: " << size << endl;
	cout << "faceNum:" << faceNum << endl;
}