#include "Application.h"


// TODO: implementer Select
// TODO: ajouter un vrai canvas a l'interieur de la fenetre
// TODO: Optimize Fill algorithm
// TODO: Antialiasing
// TODO: faire un iterator pour acceder au nom de Brush/Shape/DrawingModeManager Manager




int main()
{
	Application* app = Application::GetInstance();
	
	app->loop();
	
	return 0;
}