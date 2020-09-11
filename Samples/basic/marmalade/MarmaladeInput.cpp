#include "MarmaladeInput.h"

Rocket::Core::Context* MarmaladeInput::context = NULL;

// Sets the context to send input events to.
void MarmaladeInput::SetContext(Rocket::Core::Context* _context)
{
	context = _context;
	
}
