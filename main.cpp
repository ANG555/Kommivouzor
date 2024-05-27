#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Graph.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include"MyMenu.h"
#include<Windows.h>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace sf;


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru");
	RenderWindow win(VideoMode(1280, 720), L"�����������");// �������� ����
    win.setFramerateLimit(60);
    //������ �� ����
    Music music;
    music.openFromFile("music.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(2.f);

    // �������� ������
    Image icon;
    icon.loadFromFile("Image/icon1.jpg");
    win.setIcon(2048, 2048, icon.getPixelsPtr());

    win.setMouseCursorVisible(false);// ���������� ��������� �������

    // �������� ����������� ����
    Texture textureFon;
    textureFon.loadFromFile("Image/fon.jpg");
    RectangleShape fon(Vector2f(1280, 720));
    fon.setTexture(&textureFon);

    RectangleShape fon2(Vector2f(1280, 720));
    fon2.setTexture(&textureFon);
    fon2.setPosition(Vector2f(1280, 0));

    CircleShape octagon(80, 8);
    octagon.setFillColor(Color(500, 500, 500));
    octagon.setRadius(250);
    octagon.setPosition(370, 90);

    // �������� ������
    Font font;
    if (!font.loadFromFile("font/myFont.otf")) return 5;

    // �������� ������ � ��������� ������
    String f1 = L"�����������";// �������� ����������� ������
    Text name_flovers1;
    name_flovers1.setFont(font);
    name_flovers1.setFillColor(Color(15, 15, 15));
    name_flovers1.setString(f1);
    name_flovers1.setCharacterSize(40);
    name_flovers1.setPosition(460, 260);

    //�������� ����
    vector<String> name_menu{ L"������ ������", L"�����" };// �������� ������� ����
    game::MyMenu mymenu(win, 625, 320, 35, 35, name_menu);// ������ ���� (..., x, y, ������ ������, ������ ������� ����� ��������, ...)
    mymenu.setColorTextMenu(Color(200, 200, 200), Color(15, 15, 15), Color::Black);// ��������� ����� ����������� ����
    mymenu.AlignMenu(2); //������������ ������� ����

    Vector2f pos;// ���������� ��������
    Clock clock;    // ������
    float time;

    // �������� ���� 
    while (win.isOpen())
    {
        // ��������� �������
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //��� �������� ����� �������
                win.close();
            if (event.type == Event::KeyReleased)
            {
                // ������� ����� ������� ����
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // �����
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // ����
                if (event.key.code == Keyboard::Return)                         // ����
                {
                    // ��������� �� ��������� ����� ����
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:
                        glutInit(&argc, argv);
                        make_graph_baza();
                        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
                        glutInitWindowSize(wight, hight);
                        glutCreateWindow("Graph");
                        //glutFullScreen(); //�� ���� �����
                        glutDisplayFunc(draw);
                        glutReshapeFunc(mashtab);
                        glutMouseFunc(tab_mouse);
                        glutPassiveMotionFunc(dvisenir_mous);
                        glutMainLoop();  
                        break;
                    case 1:win.close(); break;
                    default:break;
                    }
                }
            }
        }
        // ���������� �������
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // �������� ����
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// ������� ������
        win.draw(fon);// ��������� ��������� ����
        win.draw(fon2);
        win.draw(octagon);
        mymenu.draw();
        win.draw(name_flovers1);
        win.display();// ���������� ������
    }
    return 0;
}
