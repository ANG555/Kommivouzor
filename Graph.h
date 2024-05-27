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

const int maxSize = 10;                                                      //������������ ����� ������(���� ������ ��������� ���� � ���������)
extern int wight;	                                                         //������ ���� (OpenGL)
extern int hight;															 //������ ���� (OpenGL)
struct vertCoord{int x, y;};		                                         //��������� ��������� ������

class Graph
{
private:
	vector<int> vertList;							//������ ������
	int adjMatrix[maxSize][maxSize];		        //������� ���������
public:
	Graph();
	~Graph();
	bool IsEmpty();																		//�������� �� �������
	bool IsFull();																		//�������� �� ������� 
	int GetVertText(int i) { return vertList[i]; }
	vector<int> GetVertList() { return  vertList; }
	int GetAdjMatrixElem(int i, int j) { return adjMatrix[i][j]; }
	void SetEdgeZero(int i, int j) { adjMatrix[i][j] = 0; adjMatrix[j][i] = 0; }
	int GetAmountVerts() { return vertList.size(); }
	int GetVertPos(const int& vertex);
	int get_amount_edges(int choice);
	void InsertVertex(const int& vertex);													//���������� ����� ������� 
	void InsertEdge(const int& vertex1, const int& vertex2, int choice, int weigth);		//���������� ������ �����
	void Print();
	void EraseLastVert();
	void EraseEdge(const int& vertex1, const int& vertex2);
	void draw_graph();
};
extern Graph graph; //�������� ������� ������ Graph

// ������� ��� ������� ������ ������������
void print_matrix(int** matrix);		                                         //����� ������� �������
bool f_or_t(int** matrix);                                               //��������: ����� �� ������ ������ ������������
int** matr_0_v_1();	                                                         //��������� �������, ������� 0 �� -1
int* min_el(int* line, int n);	                                         //����� ������������ �������� � ������ �������
int** reduct_matrix(int** oldmatrix);											 //�������� �������
int** rabita_viz_0(int** oldmatrix);												 //�������� ���� � ���������� �������
void print_result();                                                             //����� ���������� ������� ������ ������������

// ��������� ������
void button1();                                                              //������ "Make New Graph"
void button2();                                                              //������ "Salesman Task"
void button3();                                                              //������ "Print Matrix In Console"
void button4();                                                              //������ "Add Vertex"
void button5();                                                              //������ "Delete Vertex"
void button7();                                                              //������ "Delete Edge"                                                     
void button6();                                                              //������ "Add Edge"
                                                     
// ��������� ��������� ����� (OpenGL)
void draw_krug(int x, int y, int R);                                            //��������� ����� 
void draw_krug2(int x, int y, int R);                                    //��������� �����, ���� ������ � ���� �������
void draw_zifri(int nom, int x1, int y1);                                          //��������� ���� �����
void draw_rebro(int text, int x0, int y0, int x1, int y1);                         //��������� ����� 
void draw_vertex(int n);                                                          //��������� ������� 
void make_graph();                                                                //�������� �����
void make_graph_baza();															 //�������� �������� �����
void drav_vce_krugi(int i, int n);													 //��������� ��������� ��� ������ �����

// ������� - ��������
int mous_vertexs_tab(int x, int y);                                  //��������: ������ � ���� ������� ��� ���
void y_n(int x, int y);                                 //��������: ������ � ���� ������ ��� ���

// ������� ������������ ��������� ����
void dvisenir_mous(int x, int y);                                    //������������ �������� ������� ����
void tab_mouse(int button, int state, int x, int y);            //������� ����������� ������� ����

// �����������
void mashtab(int w, int h);                                      //�������� �� ���������������
void draw();                                                  //�������� �� ����������� 
