#include "CpGraph.h"

int CpMain()
{
auto app = Gtk::Application::create();
        
Gtk::Window window;
window.set_default_size(600,400);

return app->run(window);
}

