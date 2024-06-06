// Scene.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <gl/glut.h>
#include <stdio.h>
#include "Scene.h"
#include "utils.h"
#include "Board.h"
#include <iostream>

namespace CrossGame
{
    using namespace GraphUtils;

    
    Scene::Scene(float xStep, float zStep)
    {
        this->xStep = xStep;
        this->zStep = zStep;
        // ��������� ����������� ��������� ����� ������ ����:
        initialize();
    }

    Scene::~Scene()
    {
        // ��������� �� ������:
        for (int i = 0; i < shapes.size(); i++)
        {
            delete shapes[i];
        }
    }


    // ������������� ������� ������ fields � ���������� x
    float Scene::allocX(int i)
    {
        return  xStep * i - (N - 1) * xStep / 2;
    }

    // ������������� ������� ������ fields � ���������� z
    float Scene::allocZ(int i)
    {
        return  zStep * i - (M - 1) * zStep / 2;
    }



    // ����������� ����� (���������� ��������, � ���� � ������ ���������� ���):
    void Scene::initialize()
    {
        stalemate = false; //���������� ����
        Turn = true; //������ �������� ��������
        shapes.clear();

        float* col1D = diffLightBlue;
        float* col1A = ambiLightBlue;
        float* col1S = specLightBlue;


        // ������ �����  
        shapes.push_back(new Board(0.0f, -0.0345f, 0.0f, N * xStep, 0.07f, M * xStep, diffOrange, ambiOrange, ambiOrange));


        //������� ��
        shapes.push_back(new Board(0.2f, 0.04f, 0.0f, 0.05f, 0.07f, M * xStep, col1D, col1A, col1S));
        shapes.push_back(new Board(-0.2f, 0.04f, 0.0f, 0.05f, 0.07f, M * xStep, col1D, col1A, col1S));

        shapes.push_back(new Board(0.0f, 0.03f, 0.2f, N * xStep, 0.07f, 0.05f, col1D, col1A, col1S));
        shapes.push_back(new Board(0.0f, 0.03f, -0.2f, N * xStep, 0.07f, 0.05f, col1D, col1A, col1S));


        

        distZ = -2;
        angleX = -10;
        angleY = 30;
        time = 0;
        finish = false;

    }

    // ����� �������, �������� �� ������� ������� ����:
    bool Scene::findNearest(int x, int y, int& x1, int& z1)
    {
        int viewport[4];
        int iMin = -1, jMin = -1;
        double mvMatrix[16], projMatrix[16];
        double minDist = 2000;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
            {

                // ����� x, y, z ������� �������:
                double wx = allocX(j);
                double wy = 0.1;
                double wz = allocZ(i);

                // ���������� ����� viewport �������� ������� ���������:
                glGetIntegerv(GL_VIEWPORT, viewport);

                // ���������� ������ ��������� ���������:
                glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
                glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

                // ����� x, y, z ����������, �� �����������:
                double dx, dy, dz;

                // �������� ���������� �����, �� ��� ������������ �������:
                gluProject(wx, wy, wz, mvMatrix, projMatrix, viewport, &dx, &dy, &dz);
                dy = viewport[3] - dy - 1; // dy ��������� ������������
                double d = (x - dx) * (x - dx) + (y - dy) * (y - dy); // ������� ������
                if (d < minDist) // ������� ������ �������
                {
                    minDist = d;
                    iMin = i;
                    jMin = j;
                }
            }
        }
        if (minDist < 1000) // ������� ��������� �������
        {
            x1 = jMin;
            z1 = iMin;
            return true;
        }
        else
        {
            return false;
        }
    }

    // ���������� ��䳿, ���'����� � �������������� ����
    void Scene::on_paint()
    {
        char text[128]; // ����� �������, 
        // ���������� ������ ������� �������� �� ����� ���:
        if (finish)
        {
            std::string win = "";

            if (!stalemate) // �� ͳ���
            {
                if (Turn) win = player1.name; else win = player2.name;
                sprintf(text, "Game over. Time: %d sec. player: %s won   F2 - Restart game   Esc - Exit", time, win.c_str());
            }
            else //ͳ���
                sprintf(text, "Game over. Time: %d sec.  Draw   F2 - Restart game   Esc - Exit", time);


        }
        else
        {
            sprintf(text, "F2 - Restart game   Esc - Exit              Time: %d sec.", time);
        }
        // ������������ ������� ��������� ����, ��� ���� �������� ��� ����:
        glViewport(0, 0, (GLsizei)width, (GLsizei)height);

        // ���������� ��������� �������� � ������� �����:
        float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // ���� �������� ��������� 
        float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // ���� ��������� ��������� 
        float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };// ���� ������������ �����������
        float lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };// ������������ ������� �����

        // ������������ ��������� ������� �����:
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

        // ��������� ��������� ���� ��� ��������:

        if (stalemate) //ͳ���
        {
            glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.1, (GLclampf)0);
        }
        else
            if (finish) // ��� ��������� � ����� ������
            {
                glClearColor((GLclampf)0.1, (GLclampf)0.1, (GLclampf)0.3, (GLclampf)0);
            }
            else
            {
                glClearColor((GLclampf)0.0, (GLclampf)0.0, (GLclampf)0.0, (GLclampf)0);
            }

        // ������� ������:
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // ��� ����������� ������, ����� ��������������� ������������ ��������:
        glOrtho(0, width, 0, height, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1, 1, 0); // ������ �����
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, text, 10, 10);



        //���� ��������� ���� � ���������� �����
        char Plt1[256];
        char Plt2[256];
        sprintf(Plt1, "%s X: %d", player1.name.c_str(), player1.victory);
        sprintf(Plt2, "%s O: %d", player2.name.c_str(), player2.victory);
        glColor3f(1, 0.7, 0.9);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, Plt1, 10, height - 30);
        glColor3f(0.7, 0.5, 1);
        drawString(GLUT_BITMAP_TIMES_ROMAN_24, Plt2, 10, height - 65);

        glPopMatrix();

        // �������� ����� ������ � �������� ��������:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // ������ ������� ����� �������� � ����������� ������ ���������, 
        // 60 - ��� �������� � �������� �� �� �,
        // width/height - ��� �������� ������ �� x,
        // 1 � 100 - ������� �� ����������� �� ������ �������� �� ������:
        gluPerspective(60, width / height, 1, 100);

        // �������� ����� ������ � ������� ��������:
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0, 0, distZ);	// ������ � ������� ��������� ��������� �� distZ, 

        glRotatef(angleX, 0.0f, 1.0f, 0.0f);  // ���� ���������� �� �� Oy
        glRotatef(angleY, 1.0f, 0.0f, 0.0f);  // ���� ���������� �� �� Ox
        glEnable(GL_DEPTH_TEST);	// �������� ����� ������� (��� �������� ��������� ������ ����������)

        // �������� ����� ��� ��������� ���������:
        glEnable(GL_LIGHTING);

        // ������ ������� ����� "0 (�� ���� ���� �� 8), ����� ���� ������ � "����":
        glEnable(GL_LIGHT0);

        // ������� �� ������:
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i]->draw();
        }

        // �������� ���, �� ��������:
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        if (page < 2) //������������� ���� ����� ������� ����� ����
        {
            //���������� ��������� ������ ������ �����.
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            // ��� ����������� ������, ����� ��������������� ������������ ��������:
            glOrtho(0, 1, 0, 1, -1, 1);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glColor3f(1, 1, 0); // ������ �����


            if (page == 0) //������� 0
            {
                char text1[128]; // ����� �������,
                // ���������� ������ ������� �������� �� ����� ���:
                sprintf(text1, "Enter a name Player 1, (only EN) Next : Press enter");
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, text1, 0.01, 0.8f);
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, player1.name.c_str(), 0.01, 0.7f);
            }



            if (page == 1) //������� 1
            {
                char text1[128]; // ����� �������,
                // ���������� ������ ������� �������� �� ����� ���:
                sprintf(text1, "Enter a name Player 2, (only EN) Next : Press enter");
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, text1, 0.01, 0.8f);
                drawString(GLUT_BITMAP_TIMES_ROMAN_24, player2.name.c_str(), 0.01, 0.7f);
            }
        }


        glFlush();
        // �������� ����:
        glutSwapBuffers(); // ����������� ������
    }

    // ���������� ��䳿, ���'����� � ����� ������ ���� 
    void Scene::on_size(int width, int height)
    {
        this->width = width;
        if (height == 0)
            height = 1;
        this->height = height;
    }


    //������� ��� ��������� ���� �������� �������� �������� �� ������ ��� �� ���������
    int Scene::GetType(int x, int y)
    {

        float Xob = allocX(x);
        float Yob = allocX(y);
        for (int i = 0; i < shapes.size(); i++) //�������� �� ��� ������
        {
            if (shapes[i]->type == 1 || shapes[i]->type == 2) //������� ����� �������� � ������
            {

                if (shapes[i]->getXCenter() == Xob && shapes[i]->getZCenter() == Yob) //���� �������� ��'��� � ������� ����������� �� ��������� ���� ���
                {

                    return shapes[i]->type;
                }

            }
        }

        return 0;
    }

    //�������� ���� �� �������� �� ���� ������
    bool Scene::TestVictory(int t)
    {
        //����� �� ����������
        for (int i = 0; i < 3; i++)
            if (GetType(0, i) == t && GetType(1, i) == t && GetType(2, i) == t) return true;

        //����� �� ��������
        for (int i = 0; i < 3; i++)
            if (GetType(i, 0) == t && GetType(i, 1) == t && GetType(i, 2) == t) return true;

        //����� �������� ���� �� �����
        if (GetType(0, 0) == t && GetType(1, 1) == t && GetType(2, 2) == t) return true;

        //���� �������� � ����� �� ���
        if (GetType(2, 0) == t && GetType(1, 1) == t && GetType(0, 2) == t) return true;

        //����� ����� �������� �� ��������
        return false;
    }

    //ϳ�������� ������� �������� � ������.
    int Scene::CountElement()
    {
        int count = 0;

        for (int i = 0; i < shapes.size(); i++) //�������� �� �� ������
        {
            if (shapes[i]->type == 1 || shapes[i]->type == 2) //������� ����� �������� � ������
            {

                count++; //��������� ����
            }
        }

        return count;
    }


    // ���������� ����, ���'������ � ����������� ������ ����
    void Scene::on_mouse(int button, int state, int x, int y)
    {

        // �������� ������ ���������� ����:
        mouseX = x;
        mouseY = y;
        if ((state == GLUT_UP)) // ������ ������
        {
            this->button = -1;  // ���� ������ �� ���������
            return;
        }
        this->button = button;  // �������� ���������� ��� ������
        if (finish)
        {
            return;
        }



        int X, Y; //���� ��� ��������� ���������� ������ ���� �������
        
        if (button == 0 && findNearest(mouseX, mouseY, X, Y))
        {

            float CorectHeight = 0.02;

            //�������� ���������� ���� ���� ������� ������� ����� ��'���
            float Xob = allocX(X);
            float Yob = allocX(Y);

            if (!(bool)GetType(X, Y)) //���� �� ������� � ���� �� ���� �� ����� ��� ������ ��������
            {
                if (Turn)
                {
                    shapes.push_back(new Cross(Xob, 0.00 - CorectHeight, Yob, 0.3, 0.1, 0.3, diffGreen, ambiRed, specRed));
                    shapes[shapes.size() - 1]->type = 1; //������ ��� ��������
                }
                else
                {
                    shapes.push_back(new Disk(Xob, 0.05 - CorectHeight, Yob, 0.3, 0.05, 0.3, diffGreen, ambiGreen, specGreen, 0.05));
                    shapes[shapes.size() - 1]->type = 2; //������ ��� ��������
                }
                Turn = !Turn; //������� �����

            }


            if (TestVictory(1)) { player1.victory++;  Turn = true; finish = true; } //��������� �� ������ ������� ������
            else
                if (TestVictory(2)) { player2.victory++;  Turn = false;  finish = true; } //��������� �� ������ ������� ������
                else
                    if (CountElement() >= 3 * 3) { stalemate = true;  finish = true; } //���� �� ���� ������� ��������� ��  ���� �� ������ �� ����

        }
    }

    // ���������� ����, ���'������ � ������������ ���� � ���������� �������
    void Scene::on_motion(int x, int y)
    {
        switch (button)
        {

        case 2: // ����� ������ - ��������� �����
            angleX += x - mouseX;
            angleY += y - mouseY;
            mouseX = x;
            mouseY = y;
            break;
        }
    }

    // ���������� ����, ���'������ � ����������� �������������� ����� � ������ 
    void Scene::on_special(int key, int x, int y)
    {
        switch (key) {
        case GLUT_KEY_UP:   // ����������
            if (distZ > -1.7)
            {
                break;
            }
            distZ += 0.1;
            break;
        case GLUT_KEY_DOWN: // ���������
            distZ -= 0.1;
            break;
        case GLUT_KEY_F2:   // ���� ���
            initialize();
            break;
        }
    }

    //�������� � ���������
    void Scene::on_keyboard(unsigned char key, int x, int y)
    {

        if (key == 13 && (page == 0 || page == 1))
        {

            if (player2.name.empty() && page == 1)   {
                MessageBox(NULL, L"Empty username 2", L"Tic-tac-toe game", MB_OK);
                return;
            }


            if (player1.name.empty()) {
                MessageBox(NULL, L"Empty username 1", L"Tic-tac-toe game", MB_OK);
                return;
            }

            page++;
            return;
        }


        std::string* t = &player1.name;
        if (page == 1) t = &player2.name;


        if (page == 0 || page == 1)
        {

            if (key == 8) //��������� ������ ���� ��������� backspace 
            {
                if (t->size() > 0)
                    t->erase(t->begin() + t->size() - 1);
            }
            else
                t->push_back(key);
        }
    }


    int tick = 0; // ��������, �������� ����� ��������� ���� 25 ��

    // ���������� ��䳿 �� �������
    void Scene::on_timer(int value)
    {
        tick++;
        if (tick >= 40) // ���������� �������� �������
        {
            if (!finish)// ������� �����������, ���� ��� �� ��������
            {
                time++;
            }
            tick = 0;   // ������� ��������
        }
        on_paint();     // ��������� ������������� ����
    }

}