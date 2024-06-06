#include <gl/glut.h>
#include "Cross.h"
#include "utils.h"


namespace CrossGame
{

    void Cross::draw()
    {
        // ���������� ������������ ��������:
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, getAmbiColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, getDiffColor());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, getSpecColor());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, GraphUtils::shininess);
        // ����� ������� ������� � ����
        // (���������� ����� ������� ������� ��� ���������� ������������):
        glPushMatrix();
        glTranslatef(getXCenter(), getYCenter() + getYSize() / 2, getZCenter());
        
        glRotatef(90, 1, 0, 0);
        GLUquadricObj* quadricObj = gluNewQuadric();

        float s = getXSize(); //�����'����� ��������� �����
        float sW = s / 2; //������� �� ������
        float sH = s / 10; //������ �� ������
        float sG = s / 7;//�������


        glRotatef(45, 0, 0, 1); //��������� ������� �� 45 ������� 

        for (int i = 0; i < 2; i++) // � ���� �������� ��� ��� � ���������  ������� ����������� � �������� 90 ������� �� � ������ �������
        {
            glBegin(GL_QUADS);
            //����� ������� 1
            glNormal3d(0, 1, 0);
            glVertex3d(-sW, sG, sH);
            glVertex3d(sW, sG, sH);
            glVertex3d(sW, sG, -sH);
            glVertex3d(-sW, sG, -sH);
            //����� ������� 2
            glNormal3d(0, -1, 0);
            glVertex3d(-sW, -sG, sH);
            glVertex3d(sW, -sG, sH);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(-sW, -sG, -sH);

            //������ � ����� 3
            glNormal3d(0, 0, -1);
            glVertex3d(-sW, -sG, -sH);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(sW, sG, -sH);
            glVertex3d(-sW, sG, -sH);

            //������ front
            glNormal3d(1, 0, 0);
            glVertex3d(sW, -sG, -sH);
            glVertex3d(sW, -sG, sH);
            glVertex3d(sW, sG, sH);
            glVertex3d(sW, sG, -sH);


            //������ back
            glNormal3d(-1, 0, 0);
            glVertex3d(-sW, -sG, -sH);
            glVertex3d(-sW, -sG, sH);
            glVertex3d(-sW, sG, sH);
            glVertex3d(-sW, sG, -sH);
            glEnd();
            glRotatef(90, 0, 0, 1); //��������� �� 90 ������� ��� �������� �� ���� �����������
        }

        gluDeleteQuadric(quadricObj);
        // ³��������� ������� ������� � �����:
        glPopMatrix();
    }

}
