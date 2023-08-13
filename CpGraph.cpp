#include "CpGraph.h"

int CpMain()
{
    QApplication app();

    // Create a QPushButton (a button widget)
    QPushButton button("Click me!");

    // Set up a connection to handle the button click event
    QObject::connect(&button, &QPushButton::clicked, [&]() {
        button.setText("Hello, Qt!");
    });

    // Show the button and run the application event loop
    button.show();
    return app.exec();
}

