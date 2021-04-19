#include "UselessPch.h"

#include "Useless/System/w32/Application.h"

namespace Useless {

/*! Default OnExit handler (does completely nothing)
 *  It's "static virtual" method of Application
 *  You may define your own version of this handler
 */
void Application::OnExit() {}

};//namespace Useless
