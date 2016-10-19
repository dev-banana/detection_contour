#include "mainWindow.hpp"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv);

  MainWindow helloworld;

  app->run(helloworld) ;

  return 0 ; 
}