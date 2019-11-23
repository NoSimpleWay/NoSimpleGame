#include "EWindow.h"

GLFWwindow* EWindow::main_window = NULL;

int EWindow::SCR_WIDTH		= 1000;
int EWindow::SCR_HEIGHT		= 800;

Batcher* EWindow::batch = NULL;

void EWindow::default_update(float _d)
{
}

void EWindow::update(float _d)
{
}

void EWindow::default_draw(float _d)
{
}

void EWindow::draw(float _d)
{
}

EWindow::EWindow()
{
}
