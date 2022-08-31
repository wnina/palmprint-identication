#include "Interfaz.h"


using namespace System;
using namespace System ::Windows::Forms;


[STAThread]
//void main(array<String^>^ arg) {
int main(array<System::String^>^ arg) {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	ProyectoLisa::Interfaz form;
	Application::Run(%form);
	
	return 0;
}