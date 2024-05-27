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
	RenderWindow win(VideoMode(1280, 720), L"Комивоязжер");// Создание окна
    win.setFramerateLimit(60);
    //Музыка на фоне
    Music music;
    music.openFromFile("music.ogg");
    music.play();
    music.setLoop(true);
    music.setVolume(2.f);

    // Загрузка иконки
    Image icon;
    icon.loadFromFile("Image/icon1.jpg");
    win.setIcon(2048, 2048, icon.getPixelsPtr());

    win.setMouseCursorVisible(false);// Отключение видимости курсора

    // Создание движущегося фона
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

    // Загрузка шрифта
    Font font;
    if (!font.loadFromFile("font/myFont.otf")) return 5;

    // Создание текста с названием экрана
    String f1 = L"КОМИВОЯЗЖЕР";// Название конкретного цветка
    Text name_flovers1;
    name_flovers1.setFont(font);
    name_flovers1.setFillColor(Color(15, 15, 15));
    name_flovers1.setString(f1);
    name_flovers1.setCharacterSize(40);
    name_flovers1.setPosition(460, 260);

    //Основное меню
    vector<String> name_menu{ L"НАЧАТЬ РАБОТУ", L"ВЫЙТИ" };// Название пунктов меню
    game::MyMenu mymenu(win, 625, 320, 35, 35, name_menu);// Объект меню (..., x, y, размер текста, размер отступа между пунктами, ...)
    mymenu.setColorTextMenu(Color(200, 200, 200), Color(15, 15, 15), Color::Black);// Установка цвета отображения меню
    mymenu.AlignMenu(2); //Выравнивание пунктов меню

    Vector2f pos;// Координаты объектов
    Clock clock;    // Таймер
    float time;

    // Основной цикл 
    while (win.isOpen())
    {
        // Обработка событий
        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) //Для закрытия через крестик
                win.close();
            if (event.type == Event::KeyReleased)
            {
                // События выбра пунктов меню
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }        // вверх
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }    // вниз
                if (event.key.code == Keyboard::Return)                         // ввод
                {
                    // Переходим на выбранный пункт меню
                    switch (mymenu.getSelectedMenuNumber())
                    {
                    case 0:
                        glutInit(&argc, argv);
                        make_graph_baza();
                        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
                        glutInitWindowSize(wight, hight);
                        glutCreateWindow("Graph");
                        //glutFullScreen(); //во весь экрвн
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
        // Обновление таймера
        time = clock.getElapsedTime().asMicroseconds();
        time = time / 15000;
        clock.restart();

        // Движение фона
        fon.move(-0.2 * time, 0);
        pos = fon.getPosition();
        if (pos.x < -1280) fon.setPosition(1280, pos.y);

        fon2.move(-0.2 * time, 0);
        pos = fon2.getPosition();
        if (pos.x < -1280) fon2.setPosition(1280, pos.y);

        win.clear();// Очистка экрана
        win.draw(fon);// Отрисовка элементов игры
        win.draw(fon2);
        win.draw(octagon);
        mymenu.draw();
        win.draw(name_flovers1);
        win.display();// Обновление экрана
    }
    return 0;
}
