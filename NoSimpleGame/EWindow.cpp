#include "EWindow.h"

GLFWwindow* EWindow::main_window = NULL;
EWindowGame* EWindow::window_game = NULL;
std::vector<EWindow*> EWindow::window_list;








unsigned int EWindow::transformLoc;

int EWindow::mouse_x = 0;
int EWindow::mouse_y = 0;

bool EWindow::LMB = false;
bool EWindow::RMB = false;

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

EWindow::~EWindow()
{
}
