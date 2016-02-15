#include "Model.h"
#include "View.h"
#include "Controller.h"

void main()
{
	Model * theModel = new Model();
	View * theView = new View(theModel);
	Controller * theController = new Controller(theModel, theView);

	theController->RunGameLoop();

	if (theController != NULL)
	{
		delete theController;
		theController = NULL;
	}

	if (theView != NULL)
	{
		delete theView;
		theView = NULL;
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}
	
	_CrtDumpMemoryLeaks();
}