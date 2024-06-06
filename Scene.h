// Scene.h
#ifndef Scene_h
#define Scene_h

#include "Shape.h"
#include "Disk.h"
#include "Cross.h"
#include <vector>
#include<string>

namespace CrossGame
{

    const int M = 3, N = 3; // ������� ����� �� ������� ����

    // �������� ���� ���, ���� ����������� �������� �����,
    // ��������� ������� ���, ������������ ��������,
    // � ����� ������ ������� ���� GLUT
    class Scene
    {
        std::vector<Shape*> shapes; // "�������" ����� ��������� �� �������� ���
        int button;           // ������ ���� (-1 - �� ���������, 0 - ���, 2 - �����)
        float angleX, angleY; // �������� ��� �������� ����� 
        float mouseX, mouseY; // ������ ����������
        float width, height;  // ������ ����
        float distZ;          // ������� �� �� Z �� �����
        bool finish;          // ������ ����, �� ��� ���������
        float xStep, zStep;   // ������� �� ��������
        int time;             // �������� ��� � ��������

    public:
        bool Turn;             //����� ��� ������
        int page;              //�� ��� �� ������� ����� ��� �������� ����� ���
        bool stalemate;        //���� 

        struct Player
        {
            std::string name;
            int victory = 0;
        }player1, player2;      //��������� ���� �������

    public:
        Scene(float xStep, float zStep);
        ~Scene();
        void on_paint();
        void on_size(int width, int height);
        void on_mouse(int button, int state, int x, int y);
        void on_motion(int x, int y);
        void on_special(int key, int x, int y);
        void on_keyboard(unsigned char key, int x, int y);
        void on_timer(int value);
    private:
        void initialize();
        bool findNearest(int x, int z, int& x1, int& z1);
        int CountElement();
        float allocX(int i);
        float allocZ(int i);
        int GetType(int x, int y);
        bool TestVictory(int t);
    };

}

#endif