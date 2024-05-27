#pragma once
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

const int maxSize = 10;                                                      //максимальное число вершин(если больше небольшие баги в отрисовке)
extern int wight;	                                                         //ширина окна (OpenGL)
extern int hight;															 //высота окна (OpenGL)
struct vertCoord{int x, y;};		                                         //структура координат вершин

class Graph
{
private:
	vector<int> vertList;							//вектор вершин
	int adjMatrix[maxSize][maxSize];		        //матрица смежности
public:
	Graph();
	~Graph();
	bool IsEmpty();																		//проверка на пустоту
	bool IsFull();																		//проверка на полноту 
	int GetVertText(int i) { return vertList[i]; }
	vector<int> GetVertList() { return  vertList; }
	int GetAdjMatrixElem(int i, int j) { return adjMatrix[i][j]; }
	void SetEdgeZero(int i, int j) { adjMatrix[i][j] = 0; adjMatrix[j][i] = 0; }
	int GetAmountVerts() { return vertList.size(); }
	int GetVertPos(const int& vertex);
	int get_amount_edges(int choice);
	void InsertVertex(const int& vertex);													//добавление новой вершины 
	void InsertEdge(const int& vertex1, const int& vertex2, int choice, int weigth);		//добавление нового ребра
	void Print();
	void EraseLastVert();
	void EraseEdge(const int& vertex1, const int& vertex2);
	void draw_graph();
};
extern Graph graph; //создание объекта класса Graph

// Функции для решения задачи Коммивояжера
void print_matrix(int** matrix);		                                         //вывод текущей матрицы
bool f_or_t(int** matrix);                                               //проверка: можно ли решить задачу Коммивояжера
int** matr_0_v_1();	                                                         //создается матрица, заменяя 0 на -1
int* min_el(int* line, int n);	                                         //поиск минимального элемента в строке матрицы
int** reduct_matrix(int** oldmatrix);											 //редукция матрицы
int** rabita_viz_0(int** oldmatrix);												 //Удаление нуля с наибольшей оценкой
void print_result();                                                             //вывод результата решения задачи Коммивояжера

// Рисование кнопок
void button1();                                                              //кнопка "Make New Graph"
void button2();                                                              //кнопка "Salesman Task"
void button3();                                                              //кнопка "Print Matrix In Console"
void button4();                                                              //кнопка "Add Vertex"
void button5();                                                              //кнопка "Delete Vertex"
void button7();                                                              //кнопка "Delete Edge"                                                     
void button6();                                                              //кнопка "Add Edge"
                                                     
// Рисование элементов графа (OpenGL)
void draw_krug(int x, int y, int R);                                            //рисование круга 
void draw_krug2(int x, int y, int R);                                    //рисование круга, если курсор в зоне вершины
void draw_zifri(int nom, int x1, int y1);                                          //рисование веса ребра
void draw_rebro(int text, int x0, int y0, int x1, int y1);                         //рисование ребра 
void draw_vertex(int n);                                                          //рисование вершины 
void make_graph();                                                                //создание графа
void make_graph_baza();															 //создание базового графа
void drav_vce_krugi(int i, int n);													 //установка координат для вершин графа

// Функции - проверки
int mous_vertexs_tab(int x, int y);                                  //проверка: курсор в зоне вершины или нет
void y_n(int x, int y);                                 //проверка: курсор в зоне кнопки или нет

// Функции отслеживания поведения мыши
void dvisenir_mous(int x, int y);                                    //отслеживание движения курсора мыши
void tab_mouse(int button, int state, int x, int y);            //функция регистрации нажатий мыши

// Изображение
void mashtab(int w, int h);                                      //отвечает за масштабирование
void draw();                                                  //отвечает за изображение 
