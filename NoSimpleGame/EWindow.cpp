#include "EWindow.h"

GLFWwindow* EWindow::main_window = NULL;
EWindowGame* EWindow::window_game = NULL;
std::vector<EWindow*> EWindow::window_list;

int EWindow::SCR_WIDTH		= 1000;
int EWindow::SCR_HEIGHT		= 800;

Batcher* EWindow::batch = NULL;

glm::mat4 EWindow::matrix_transform;
Shader* EWindow::ourShader = NULL;

float EWindow::correction_x = 0.1f;
float EWindow::correction_y = 0.1f;

unsigned int EWindow::transformLoc;

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
