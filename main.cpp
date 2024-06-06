// main.cpp
#define _CRT_SECURE_NO_WARNINGS

#include <gl/glut.h>
#include <iostream>
#include "Scene.h"
#include <string> 
#include <fstream>
#include <sstream>
#include <stdio.h> 

using namespace std;
using CrossGame::Scene;

Scene* scene; // вказівник на клас Scene


unsigned short num = 1;
void setScore();

void on_paint()
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_paint();
}

void on_size(int width, int height)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_size(width, height);
}

void on_mouse(int button, int state, int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_mouse(button, state, x, y);
}

void on_motion(int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_motion(x, y);
}

void on_special(int key, int x, int y)
{
    // викликаємо відповідну функцію класу Scene:
    scene->on_special(key, x, y);
}


void on_keyboard(unsigned char key, int x, int y)
{
    // обробка подій від клавіатури:
    if (key == 27) {
        setScore();
        exit(0); 
    }

    scene->on_keyboard(key, x, y);
}

void on_timer(int value)
{
    // обробка подій від таймера
    scene->on_timer(value);
    glutTimerFunc(25, on_timer, 0); // зa 25 мс викличеться ця функція
}


void getScore() {

    string str, dateStr, pl_1, pl_2;

    ifstream file("scoring.txt");//читання та виведення тексту у консоль

    if (file.good()) {

        while (getline(file, str)) {

            stringstream ss(str);

            getline(ss, str, ';');
            num = stoi(str);

            getline(ss, str, ';');
            dateStr = str;

            getline(ss, str, ';');
            pl_1 = str;

            getline(ss, str, ';');
            pl_2 = str;

            cout << num << "\t" << dateStr << "\t " << pl_1 << "\t" << pl_2 << endl;

            num++;
        }
    }

    file.close();
}


string getDateStr()
{
    time_t curtime;
    struct tm* loctime;
    char buffer[12];

    time(&curtime);
    loctime = localtime(&curtime);

    strftime(buffer, 12, "%d.%m.%Y", loctime);
    return buffer;
}


void setScore()
{
    string dateStr = getDateStr();

    ofstream of("scoring.txt", ios::app);// запис рядку у кінець файлу

    if (of.is_open())
    {
        of << num << ";" << dateStr << ";" << scene->player1.name << "=" << scene->player1.victory << ";" 
                        << scene->player2.name << "=" << scene->player2.victory << endl;
    }

    of.close();
}


int main(int argc, char* argv[])
{

    glutInit(&argc, argv);         // ініціалізуємо GLUT
    scene = new Scene(0.4, 0.4);   // створюємо об'єкт "сцена"
    glutInitWindowSize(800, 600);  // встановлюємо розміри вікна
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);// ініціалізуємо режими відображення
    glutCreateWindow("Tic-tac-toe");     // створюємо вікно
    glutDisplayFunc(on_paint);     // реєструємо функцію відображення
    glutReshapeFunc(on_size);      // реєструємо функцію обробки зміни розмірів вікна
    glutMotionFunc(on_motion);     // реєструємо функцію, яка відповідає за переміщення миші з натиснутою кнопкою
    glutMouseFunc(on_mouse);       // реєструємо функцію, яка відповідає за натискання на кнопку миші
    glutKeyboardFunc(on_keyboard); // реєструємо функцію, яка відповідає за натискання клавіш
    glutSpecialFunc(on_special);   // реєструємо функцію, яка відповідає за натискання спеціальних клавіш
    glutTimerFunc(25, on_timer, 0);// кожні 25 мс викликається ця функція
    getScore();
    glutMainLoop();                // стартуємо основний цикл обробки подій
    delete scene;                  // видаляємо об'єкт "сцена"
    return(0);
}