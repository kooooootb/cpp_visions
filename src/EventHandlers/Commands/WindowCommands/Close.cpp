#include "Close.h"

Close::Close(WindowHolder &window) : WindowCommand(window)
{}

void Close::execute() {
    m_window.close();
}
