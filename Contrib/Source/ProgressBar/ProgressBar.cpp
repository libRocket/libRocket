#include <Rocket/ProgressBar.h>
#include <Rocket/ProgressBar/ElementProgressBar.h>
#include <Rocket/Core/ElementInstancerGeneric.h>
#include <Rocket/Core/Factory.h>
#include <Rocket/Core/StyleSheetSpecification.h>
#include <Rocket/Core/XMLParser.h>
#include <Rocket/Core/Plugin.h>
#include <Rocket/Core/Core.h>

namespace Rocket {
namespace ProgressBar {

// Registers the custom element instancers.
void RegisterElementInstancers()
{
    Core::ElementInstancer* instancer = new Core::ElementInstancerGeneric< ElementProgressBar >();
    Core::Factory::RegisterElementInstancer("progressbar", instancer);
    instancer->RemoveReference();
}

void RegisterXMLNodeHandlers()
{
}

static bool initialised = false;

class ProgressBarPlugin : public Rocket::Core::Plugin
{
public:
	void OnShutdown()
	{
		initialised = false;
		delete this;
	}

	int GetEventClasses()
	{
		return Rocket::Core::Plugin::EVT_BASIC;
	}
};

void Initialise()
{
	// Prevent double initialisation
	if (!initialised)
	{
		//Core::StyleSheetSpecification::RegisterProperty("min-rows", "0", false, false).AddParser("number");

		// Register the element instancers for our custom elements.
		RegisterElementInstancers();

		// Register the XML node handlers for our elements that require special parsing.
		RegisterXMLNodeHandlers();

		// Register the controls plugin, so we'll be notified on Shutdown
		Rocket::Core::RegisterPlugin(new ProgressBarPlugin());

		initialised = true;
	}
}

}
}
