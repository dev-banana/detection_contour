#include "mainWindow.hpp"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv);

	MainWindow window;

	app->run(window) ;

	return 0 ; 
}