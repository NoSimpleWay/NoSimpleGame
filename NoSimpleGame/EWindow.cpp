#include "EWindow.h"

GLFWwindow* EWindow::main_window = NULL;
EWindowGame* EWindow::window_game = NULL;
std::vector<EWindow*> EWindow::window_list;








unsigned int EWindow::transformLoc;

int EWindow::mouse_x = 0;
int EWindow::mouse_y = 0;

bool EWindow::LMB = false;
bool EWindow::RMB = false;

std::vector <float> EWindow::time_process_value;
std::vector <std::string> EWindow::time_process_name;

bool EWindow::system_button_release = true;

std::chrono::time_point<std::chrono::high_resolution_clock> EWindow::start, EWindow::stop;
//std::pair<float, std::string> EWindow::time_process;

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

void EWindow::draw_interface(float _d)
{
}

void EWindow::add_time_process(std::string _name)
{
	stop = std::chrono::high_resolution_clock::now();

	time_process_value.push_back(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / 1000.0f);
	time_process_name.push_back(_name);

	start = std::chrono::high_resolution_clock::now();
}

EWindow::EWindow()
{
}

EWindow::~EWindow()
{
}
