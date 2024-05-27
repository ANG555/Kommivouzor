#include "Graph.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Graph.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
using namespace std;
using namespace sf;
int wight = 1280;						// Определяем стартовые размеры окна 
int hight = 720;
bool* Mouse_Vert = new bool[maxSize];	//массив меток (курсор в зоне вершины или нет)
bool stand_vid = false;					//отвечает за перестроение графа в стандартный вид (все вершины расположены по кругу)
bool Vert_Move = false;					//отвечает за перемещение вершин 
int R;									//радиус вершин графа
int Button_Flag;						//если курсор в зоне кнопки, принимает значение номера кнопки (подсветка кнопок при наведении)
int Moving_Vertex;						//номер перемещаемой вершины
int CursorX;							//позиция курсора по Х
int CursorY;							//позиция курсора по У
vertCoord vertC[maxSize];			//массив структур (координаты вершин), +2 чтобы не вылетало искл., при заполнении до maxSize
Graph graph;
vector<pair<int, int>> Way;
vector<int> New_Way;
//Конструктор, заполнение матрицы смежности 0 и меток Mouse_Vert 	
Graph::Graph() 	                      
{
	for (int i = 0; i < maxSize; i++)
	{
		for (int j = 0; j < maxSize; j++)
		{
			adjMatrix[i][j] = 0;
		}
		Mouse_Vert[i] = false;
	}
}
//деструктор
Graph::~Graph()
{ }
//получение номера вершины в векторе вершин
int Graph::GetVertPos(const int& vertex) 
{
	for (size_t i = 0; i < vertList.size(); i++)
	{
		if (vertList[i] == vertex)
			return i;
	}
	return -1;
}
//проверка на пустоту
bool Graph::IsEmpty() 
{
	if (vertList.size() != 0)
		return false;
	else
		return true;
}
//проверка на полноту 
bool Graph::IsFull() 
{
	return (vertList.size() == maxSize);
}
//добавление новой вершины 
void Graph::InsertVertex(const int& vertex) 
{
	if (!IsFull())
		vertList.push_back(vertex);
	else
	{
		cout << "Граф уже заполнен. Невозможно добавить новую вершину. " << endl;
		return;
	}
}
//добавление нового ребра
void Graph::InsertEdge(const int& vertex1, const int& vertex2, int choice, int weight)
{
	if (weight < 1)
	{
		cout << "\nДанная величина веса некорректна\n";
		return;
	}
	if (GetVertPos(vertex1) != (-1) && GetVertPos(vertex2) != (-1))//если вершины есть в графе
	{
		int vertPos1 = GetVertPos(vertex1);	//находим позиции вершин
		int vertPos2 = GetVertPos(vertex2);
		if (adjMatrix[vertPos1][vertPos2] != 0 && adjMatrix[vertPos2][vertPos1] != 0)//если между ними уже есть ребро
		{
			cout << "Ребро между вершинами уже есть" << endl;
			return;
		}
		else//иначе добавляем ребро
		{
			adjMatrix[vertPos1][vertPos2] = weight;
			if (choice == 2)
			{
				adjMatrix[vertPos2][vertPos1] = weight;
			}
		}
	}
	else
	{
		cout << "Обеих вершин (или одной из них) нет в графе " << endl;
		return;
	}
}
//вывод матрицы смежности на экран
void Graph::Print()
{
	if (!IsEmpty())
	{
		cout << "Матрица смежности графа: " << endl;
		cout << "-\t";
		for (int i = 0; i < vertList.size(); ++i)
		{
			cout << vertList[i] << "\t";
		}
		cout << endl;
		for (int i = 0; i < vertList.size(); ++i)
		{
			cout << vertList[i] << "\t";
			for (int j = 0; j < vertList.size(); ++j)
			{
				cout << adjMatrix[i][j] << "\t";
			}
			cout << endl;
		}
	}
	else
		cout << "\nГраф пуст\n" << endl;
}
//удаление последней вершины 
void Graph::EraseLastVert(){
	if (IsEmpty())
	{
		cout << "\nГраф пуст\n";
		return;
	}
	int n = vertList.size();
	for (int j = 0; j < n; j++)
	{
		adjMatrix[n - 1][j] = 0;
		adjMatrix[j][n - 1] = 0;
	}

	vertList.pop_back();
}
//удаление ребра
void Graph::EraseEdge(const int& vertex1, const int& vertex2) 
{
	if (GetVertPos(vertex1) != (-1) && GetVertPos(vertex2) != (-1))//если обе вершины есть в графе
	{
		int vertPos1 = GetVertPos(vertex1);//находим позиции вершин
		int vertPos2 = GetVertPos(vertex2);
		if (adjMatrix[vertPos1][vertPos2] == 0)	//если между вершинами ребра нет
		{
			cout << "Ребра между данными вершинами нет\n" << endl;
			return;
		}
		else//иначе удаляем ребро (зануляем соответствуюшую ячейку в матрице смежности)
		{
			adjMatrix[vertPos1][vertPos2] = 0;
			adjMatrix[vertPos2][vertPos1] = 0;
		}
	}
	else  //если хотя бы одной вершины в графе нет
	{
		cout << "Обеих вершин (или одной из них) нет в графе " << endl;
		return;
	}
}
//получение количества ребер в графе
int Graph::get_amount_edges(int choice) 
{
	int numb = 0;
	if (!IsEmpty())
	{
		for (int i = 0; i < vertList.size(); ++i)
		{
			for (int j = 0; j < vertList.size(); ++j)
			{
				if (adjMatrix[i][j] != 0)
					numb++;
			}
		}
	}
	if (choice == 1)
		return numb;
	else
		return numb / 2;
}
//рисование графа
void Graph::draw_graph()
{
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
	{
		if (!stand_vid)
			drav_vce_krugi(i, n);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int a = adjMatrix[i][j];
			if (a != 0)
				draw_rebro(a, vertC[i].x, vertC[i].y, vertC[j].x, vertC[j].y);
			if (a == adjMatrix[j][i] && a != 0)
				draw_rebro(a, vertC[j].x, vertC[j].y, vertC[i].x, vertC[i].y);
		}
	}
	draw_vertex(n);
	glutPostRedisplay();
}
//создается матрица, заменяя 0 на -1
int** matr_0_v_1()
{
	int n = graph.GetAmountVerts();
	int** matrix = new int* [n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int elem = graph.GetAdjMatrixElem(i, j);
			if (elem == 0 or i == j)
				matrix[i][j] = -1;
			else
				matrix[i][j] = elem;
		}
	}
	cout << "Начальная матрица: \n";
	print_matrix(matrix);
	return matrix;
}
//поиск минимального элемента в строке матрицы
int* min_el(int* line, int n)
{
	int min = 1000000;
	for (int j = 0; j < n; j++)
		if (line[j] >= 0 && line[j] < min)
			min = line[j];
	for (int j = 0; j < n; j++)
		if (line[j] >= 0)
			line[j] -= min;
	return line;
}
//вывод текущей матрицы
void print_matrix(int** matrix)
{
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << setw(4) << matrix[i][j];
		cout << endl;
	}
}
//редуцированние матрицы
int** reduct_matrix(int** oldmatrix)
{
	int** matrix = oldmatrix;
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
		matrix[i] = min_el(matrix[i], n);
	for (int i = 0; i < n; i++)
	{
		int min = 1000000;
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] >= 0 && matrix[j][i] < min)
				min = matrix[j][i];
		}
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] >= 0)
				matrix[j][i] -= min;
		}
	}
	cout << "\nРедуцированная матрица: \n";
	print_matrix(matrix);
	return matrix;
}
//Удаление нуля с наибольшей оценкой
int** rabita_viz_0(int** oldmatrix)
{
	int n = graph.GetAmountVerts();
	int** matrix = reduct_matrix(oldmatrix);
	int max = -1;
	int line, column;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == 0)
			{
				int minLine = 1000000;
				int minColumn = 1000000;
				for (int k = 0; k < n; k++)
				{
					if (matrix[i][k] != -1 && k != j && matrix[i][k] < minLine)
						minLine = matrix[i][k];
				}
				for (int k = 0; k < n; k++)
				{
					if (matrix[k][j] != -1 && k != i && matrix[k][j] < minColumn)
						minColumn = matrix[k][j];
				}
				if (max < minColumn + minLine)
				{
					max = minColumn + minLine;
					line = i;
					column = j;
				}
			}
		}
	}

	pair<int, int> p;
	p.first = line + 1;
	p.second = column + 1;
	Way.push_back(p);

	matrix[line][column] = -1;
	matrix[column][line] = -1;

	for (int i = 0; i < n; i++)
	{
		matrix[line][i] = -1;
		matrix[i][column] = -1;
	}
	cout << endl;
	cout << "Матрица после удаления 0 с наибольшей оценкой: \n";
	print_matrix(matrix);
	cout << "\nПромежуточные отрезки путей: ";
	for (int i = 0; i < Way.size(); i++)
		cout << Way[i].first << " -> " << Way[i].second << "   ";
	cout << endl;
	return matrix;
}
//Вывод результатов на консоль
void print_result()
{
	int second = Way[0].second;
	int i = 2;
	New_Way.push_back(Way[0].first);
	New_Way.push_back(Way[0].second);
	while (i != graph.GetAmountVerts() + 1)
		for (int j = 1; j < graph.GetAmountVerts(); j++)
			if (Way[j].first == second)
			{
				second = Way[j].second;
				New_Way.push_back(second);
				i++;
			}
	cout << "Ответ: ";
	for (int i = 0; i < New_Way.size(); i++)
	{
		cout << New_Way[i];
		if (i != New_Way.size() - 1)
			cout << " -> ";
	}
	int sum = 0;
	for (int i = 0; i < Way.size(); i++)
	{
		int line = Way[i].first - 1;
		int column = Way[i].second - 1;
		sum += graph.GetAdjMatrixElem(line, column);
	}
	cout << "\nS = " << sum << endl;;
}
// Рисование кнопок 
void button1()
{
	if (Button_Flag == 1)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - 20);
	glVertex2i(50, hight - hight / 7);
	glVertex2i(wight / 7, hight - hight / 7);
	glVertex2i(wight / 7, hight - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 20);
	glVertex2i(50, hight - hight / 7);
	glVertex2i(wight / 7, hight - hight / 7);
	glVertex2i(wight / 7, hight - 20);
	glEnd();
	string n1 = "MAKE NEW GRAPH";
	glRasterPos2i(wight / 20, 0.92 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
	glRasterPos2i(wight / 20, (0.92 * hight) - 15);
}
void button2()
{
	if (Button_Flag == 2)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - hight / 7 - 20);
	glVertex2i(50, hight - 2 * (hight / 7));
	glVertex2i(wight / 7, hight - 2 * (hight / 7));
	glVertex2i(wight / 7, hight - hight / 7 - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - hight / 7 - 20);
	glVertex2i(50, hight - 2 * (hight / 7));
	glVertex2i(wight / 7, hight - 2 * (hight / 7));
	glVertex2i(wight / 7, hight - hight / 7 - 20);
	glEnd();
	string n1 = "\"SALESMAN\" TASK";
	glRasterPos2i(wight / 20, 0.77 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
}
void button3()
{
	if (Button_Flag == 3)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - 2 * (hight / 7) - 20);
	glVertex2i(50, hight - 3 * (hight / 7));
	glVertex2i(wight / 7, hight - 3 * (hight / 7));
	glVertex2i(wight / 7, hight - 2 * (hight / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 2 * (hight / 7) - 20);
	glVertex2i(50, hight - 3 * (hight / 7));
	glVertex2i(wight / 7, hight - 3 * (hight / 7));
	glVertex2i(wight / 7, hight - 2 * (hight / 7) - 20);
	glEnd();
	string n1 = "PRINT MATRIX";
	string n2 = "IN CONSOLE";
	glRasterPos2i(wight / 20, 0.63 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
	glRasterPos2i(wight / 20, (0.63 * hight) - 15);
	for (int i = 0; i < n2.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n2[i]);
}
void button4()
{
	if (Button_Flag == 4)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);

	glBegin(GL_QUADS);
	glVertex2i(50, hight - 3 * (hight / 7) - 20);
	glVertex2i(50, hight - 4 * (hight / 7));
	glVertex2i(wight / 7, hight - 4 * (hight / 7));
	glVertex2i(wight / 7, hight - 3 * (hight / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 3 * (hight / 7) - 20);
	glVertex2i(50, hight - 4 * (hight / 7));
	glVertex2i(wight / 7, hight - 4 * (hight / 7));
	glVertex2i(wight / 7, hight - 3 * (hight / 7) - 20);
	glEnd();
	string n1 = "ADD VERTEX";
	glRasterPos2i(wight / 20, 0.48 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);

}
void button5()
{
	if (Button_Flag == 5)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - 4 * (hight / 7) - 20);
	glVertex2i(50, hight - 5 * (hight / 7));
	glVertex2i(wight / 7, hight - 5 * (hight / 7));
	glVertex2i(wight / 7, hight - 4 * (hight / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 4 * (hight / 7) - 20);
	glVertex2i(50, hight - 5 * (hight / 7));
	glVertex2i(wight / 7, hight - 5 * (hight / 7));
	glVertex2i(wight / 7, hight - 4 * (hight / 7) - 20);
	glEnd();
	string n1 = "DELETE VERTEX";
	glRasterPos2i(wight / 20, 0.34 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
}
void button6()
{
	if (Button_Flag == 6)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - 5 * (hight / 7) - 20);
	glVertex2i(50, hight - 6 * (hight / 7));
	glVertex2i(wight / 7, hight - 6 * (hight / 7));
	glVertex2i(wight / 7, hight - 5 * (hight / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 5 * (hight / 7) - 20);
	glVertex2i(50, hight - 6 * (hight / 7));
	glVertex2i(wight / 7, hight - 6 * (hight / 7));
	glVertex2i(wight / 7, hight - 5 * (hight / 7) - 20);
	glEnd();
	string n1 = "ADD EDGE";
	glRasterPos2i(wight / 20, 0.2 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
}
void button7()
{
	if (Button_Flag == 7)
		glColor3f(0.9, 0.9, 0.9);
	else
		glColor3f(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glVertex2i(50, hight - 6 * (hight / 7) - 20);
	glVertex2i(50, 10);
	glVertex2i(wight / 7, 10);
	glVertex2i(wight / 7, hight - 6 * (hight / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, hight - 6 * (hight / 7) - 20);
	glVertex2i(50, 10);
	glVertex2i(wight / 7, 10);
	glVertex2i(wight / 7, hight - 6 * (hight / 7) - 20);
	glEnd();
	string n1 = "DELETE EDGE";
	glRasterPos2i(wight / 20, 0.061 * hight);
	for (int i = 0; i < n1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, n1[i]);
}
//Рисование круга
void draw_krug(int x, int y, int R)            
{
	glColor3f(0.6f,0.6f, 0.6f);
	float x1, y1;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * cos(theta) + y;
		x1 = R * sin(theta) + x;;
		glVertex2f(x1, y1);
	}
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	float x2, y2;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * cos(theta) + y;
		x2 = R * sin(theta) + x;
		glVertex2f(x2, y2);
	}
	glEnd();
}
//Рисование выбранного круга 
void draw_krug2(int x, int y, int R)   
{
	glColor3f(0.9, 0.9, 0.9);
	float x1, y1;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * cos(theta) + y;
		x1 = R * sin(theta) + x;;
		glVertex2f(x1, y1);
	}
	glEnd();
	glColor3f(0.0, 0.392, 0.0);
	float x2, y2;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * cos(theta) + y;
		x2 = R * sin(theta) + x;
		glVertex2f(x2, y2);
	}
	glEnd();
}
//Рисование веса ребра
void draw_zifri(int text, int x1, int y1)	
{
	glColor3f(0.0, 0.0, 0.0);
	GLvoid* font = GLUT_BITMAP_HELVETICA_18;
	string s = to_string(text);
	glRasterPos2i(x1 - 5, y1 - 5);
	for (size_t j = 0; j < s.length(); j++)
		glutBitmapCharacter(font, s[j]);
}
//Рисование ребра ориентированного взвешенного графа
void draw_rebro(int text, int x0, int y0, int x1, int y1) 
{
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();
	draw_zifri(text, (x0 + x1) / 2 + 10, (y0 + y1) / 2 + 10);

	float vx = x0 - x1;
	float vy = y0 - y1;
	float s = 1.0f / sqrt(vx * vx + vy * vy);
	vx *= s;
	vy *= s;
	x1 = x1 + R * vx;
	y1 = y1 + R * vy;

	glColor3i(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x1 + 10 * (vx + vy), y1 + 10 * (vy - vx));
	glVertex2f(x1 + 10 * (vx - vy), y1 + 10 * (vy + vx));
	glEnd();
}
//Pисование вершины (круг + значение)
void draw_vertex(int n)		                           
{
	for (int i = 0; i < n; i++)
	{
		if (Mouse_Vert[i])
			draw_krug2(vertC[i].x, vertC[i].y, R);
		else
			draw_krug(vertC[i].x, vertC[i].y, R);
		draw_zifri(i + 1, vertC[i].x, vertC[i].y);
	}

}
//Расстановка координат вершин по кругу радиуса R и установка радиуса вершин R
void drav_vce_krugi(int i, int n)			               
{
	int R_;
	int x0 = wight / 2;
	int y0 = hight / 2;
	if (wight > hight)
	{
		R = 5 * (hight / 13) / n;
		R_ = hight / 2 - R - 10;
	}
	else
	{
		R = 5 * (wight / 13) / n;
		R_ = wight / 2 - R - 10;
	}
	float theta = 2.0f * 3.1415926f * i / n;
	int y1 = R_ * cos(theta) + y0;
	int x1 = R_ * sin(theta) + x0;
	vertC[i].x = x1;
	vertC[i].y = y1;
}
//Создание графа 
void make_graph()
{
	stand_vid = false;
	int amountVerts, amountEdges, sourceVertex, targetVertex, edgeWeight, choice;
	cout << "Какой граф? \n1) Ориентированный\n2) Неориентированный\n3) Смешанный\n";
	cin >> choice;
	cout << "Введите количество вершин в графе: "; cin >> amountVerts;
	cout << "Введите количество ребер в графе: "; cin >> amountEdges;
	cout << endl;
	for (int i = 1; i <= amountVerts; i++) {

		graph.InsertVertex(i);
	}
	if (choice == 3)
	{
		cout << "Сколько ребер являются неориентированными? ";
		int unEdges;
		cin >> unEdges;
		amountEdges -= unEdges;
		cout << "Вводите данные по неориентированным ребрам:\n ";
		for (int i = 0; i < unEdges; ++i)
		{
			cout << "Исходная вершина: ";
			cin >> sourceVertex;
			cout << "Конечная вершина: ";
			cin >> targetVertex;
			cout << "Вес ребра: ";
			cin >> edgeWeight;
			cout << endl;
			int choice3 = 2;
			//graph.Insert(choice);
			graph.InsertEdge(sourceVertex, targetVertex, choice3, edgeWeight);
		}
		choice = 1;
		cout << "Вводите данные по ориентированным ребрам:\n ";
	}
	for (int i = 0; i < amountEdges; i++)
	{
		cout << "Исходная вершина: "; cin >> sourceVertex;
		cout << "Конечная вершина: "; cin >> targetVertex;
		cout << "Вес ребра: "; cin >> edgeWeight;
		graph.InsertEdge(sourceVertex, targetVertex, choice, edgeWeight);
	}
	cout << endl;
	graph.Print();
}
//сщздание базового графа
void make_graph_baza()
{
	graph.InsertVertex(1);
	graph.InsertVertex(2);
	graph.InsertVertex(3);
	graph.InsertVertex(4);
	graph.InsertVertex(5);
	graph.InsertVertex(6);

	graph.InsertEdge(1, 3, 2, 7);//не ориентированные ребра
	graph.InsertEdge(3, 5, 2, 12);
	graph.InsertEdge(5, 6, 2, 16);
	graph.InsertEdge(6, 4, 2, 14);

	graph.InsertEdge(1, 5, 1, 19);//ориентированные ребра
	graph.InsertEdge(5, 4, 1, 31);
	graph.InsertEdge(4, 1, 1, 20);
	graph.InsertEdge(2, 6, 1, 24);
	graph.InsertEdge(3, 2, 1, 28);
}
//Проверка: корректна ли задача Коммивояжера
bool f_or_t(int** matrix)
{
	if (graph.IsEmpty())
		return false;
	for (int i = 0; i < graph.GetAmountVerts(); i++)
	{
		int cnt = 0;
		for (int j = 0; j < graph.GetAmountVerts(); j++)
		{
			if (matrix[i][j] > 0)
				cnt++;
		}
		if (cnt < 1)
			return false;
	}
	return true;
}
 //Проверка: курсор в зоне вершины или нет
int mous_vertexs_tab(int x, int y)                   
{
	for (int i = 0; i < graph.GetAmountVerts(); i++)
		if (pow(x - vertC[i].x, 2) + pow(y - vertC[i].y, 2) <= pow(R, 2))
			return i;
	return -1;
}
 //Проверка: курсор в зоне кнопки или нет
void y_n(int x, int y)                   
{
	if (x > 50 && x < wight / 7 && y < (hight - 20) && y >(hight - hight / 7))
		Button_Flag = 1;
	else if (x > 50 && x < wight / 7 && y < (hight - hight / 7 - 20) && y > hight - 2 * (hight / 7))
		Button_Flag = 2;
	else if (x > 50 && x < wight / 7 && y < hight - 2 * (hight / 7) - 20 && y > hight - 3 * (hight / 7))
		Button_Flag = 3;
	else if (x > 50 && x < wight / 7 && y > hight - 4 * (hight / 7) && y < hight - 3 * (hight / 7) - 20)
		Button_Flag = 4;
	else if (x > 50 && x < wight / 7 && y > hight - 5 * (hight / 7) && y < hight - 4 * (hight / 7) - 20)
		Button_Flag = 5;
	else if (x > 50 && x < wight / 7 && y > hight - 6 * (hight / 7) && y < hight - 5 * (hight / 7) - 20)
		Button_Flag = 6;
	else if (x > 50 && x < wight / 7 && y > 10 && y < hight - 6 * (hight / 7) - 20)
		Button_Flag = 7;
	else
		Button_Flag = 0;
}
//Отслеживание движения курсора и возможность двигать вершину
void dvisenir_mous(int x, int y)                      
{
	y = hight - y;
	CursorX = x;
	CursorY = y;
	int i = mous_vertexs_tab(x, y);
	if (i != -1)
		Mouse_Vert[i] = true;
	else
		for (int j = 0; j < graph.GetAmountVerts(); j++)
			Mouse_Vert[j] = false;
	if (Vert_Move)
	{
		vertC[Moving_Vertex].x = CursorX;
		vertC[Moving_Vertex].y = CursorY;
	}
	y_n(x, y);
	glutPostRedisplay();
}
//регистрация нажатий мыши
void tab_mouse(int button, int state, int x, int y) 
{
	int j = mous_vertexs_tab(x, hight - y);
	if (Vert_Move)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			Vert_Move = false;
			return;
		}
	}

	if (j != -1)
	{
		stand_vid = true;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			Vert_Move = true;
			Moving_Vertex = j;
			return;
		}
	}

	if (button == 3 || button == 4)
	{
		stand_vid = true;
		if (button == 3)
		{
			if (R != 300)
				R++;
		}
		else if (button == 4)
		{
			if (R != 15)
				R--;
		}
	}

	if (x >= 50 and x <= (wight / 7) and y >= 20 and y <= (hight / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			New_Way.clear();
			Way.clear();
			Graph New;
			graph = New;
			make_graph();
			return;

		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			stand_vid = false;
			return;
		}
	}
	if (x >= 50 and x <= (wight / 7) and y >= ((hight / 7) + 20) and y <= 2 * (hight / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			New_Way.clear();
			Way.clear();
			cout << "\nЗадача Коммивояжера:\n";
			int** matrix = matr_0_v_1();
			bool checker = f_or_t(matrix);
			if (!checker)
			{
				cout << "\nЗадача Коммивояжера для данного графа некорректна\n\n";
				return;
			}
			int n = graph.GetAmountVerts();
			while (Way.size() < n)
				matrix = rabita_viz_0(matrix);
			cout << endl;
			print_result();
			return;
		}
	}
	if (x >= 50 and x <= (wight / 7) and y >= (2 * (hight / 7) + 20) and y <= 3 * (hight / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			graph.Print();
			return;
		}
	}
	if (x >= 50 && x <= wight / 7 && y <= 4 * (hight / 7) && y >= 3 * (hight / 7) + 20)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			int cur = graph.GetAmountVerts();
			graph.InsertVertex(cur + 1);
			vertC[cur].x = wight / 2;
			vertC[cur].y = hight / 2;
			return;
		}
	}
	if (x >= 50 && x <= wight / 7 && y <= 5 * (hight / 7) && y >= 4 * (hight / 7) + 20)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			graph.EraseLastVert();
			int cur = graph.GetAmountVerts();
			return;
		}
	}
	if (x >= 50 && x <= wight / 7 && y <= 6 * (hight / 7) && y >= 5 * (hight / 7) + 20)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			int sourceVertex, targetVertex, edgeWeight, vertNum, choice;
			cout << "Количество добавляемых ребер: "; cin >> vertNum;
			for (int i = 0; i < vertNum; i++)
			{
				cout << "Какое ребро(1 - ориентированное, 2 - неориентированное)?";
				cin >> choice;
				cout << "Исходная вершина: "; cin >> sourceVertex;
				cout << "Конечная вершина: "; cin >> targetVertex;
				cout << "Вес ребра: "; cin >> edgeWeight;
				graph.InsertEdge(sourceVertex, targetVertex, choice, edgeWeight);
			}
			return;
		}
	}
	if (x > 50 && x < wight / 7 && y <= hight - 10 && y >= 6 * (hight / 7) + 20)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			int sourceVertex, targetVertex;
			cout << "Исходная вершина: "; cin >> sourceVertex;
			cout << "Конечная вершина: "; cin >> targetVertex;
			graph.EraseEdge(sourceVertex, targetVertex);
			return;
		}
	}
}
//отвечает за масштабирование
void mashtab(int w, int h)
{
	wight = w;
	hight = h;
	glViewport(0, 0, (GLsizei)wight, (GLsizei)hight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)wight, 0, (GLdouble)hight);
	glutPostRedisplay();
}
//Изображение
void draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, wight, 0, hight);
	glViewport(0, 0, wight, hight);
	glClearColor(0.2, 0.2, 0.2, 0.2);
	glClear(GL_COLOR_BUFFER_BIT);
	button1();
	button2();
	button3();
	button4();
	button5();
	button6();
	button7();
	graph.draw_graph();
	glutSwapBuffers();
}
