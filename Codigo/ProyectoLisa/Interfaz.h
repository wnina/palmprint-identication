#pragma once

#include "Imagen.h"
#include "AgAlgoritmoGenetico.h"
#include "RedNeuronal.h"

#include <sstream>
#include <string>
#include <algorithm>
#include <map>

using namespace std;

vector<float> vectCPrueba; // Almancenara el patron 
int contadorImagenes=0; // Cuantas imagenes se estan usando
int contadorImagenesPatr=0; // Cuantas imagenes se estan usando
int rpta;
float valorRpta;//Valor Obtenido
int p=0;


namespace ProyectoLisa {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Interfaz
	/// </summary>
	public ref class Interfaz : public System::Windows::Forms::Form
	{


	System::String  ^ rutaImagenCompleta;

	public:
		Interfaz(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			
		}
		
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Interfaz()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  seleccionarImagen;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::PictureBox^  imagenCuadro;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  valorObtenido;


	private: System::Windows::Forms::Label^  nombreImagen;


	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  procesarImagen;
	private: System::Windows::Forms::Button^  identificarImagen;


	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::PictureBox^  imagenDectPuntas;
	private: System::Windows::Forms::TextBox^  coordenadaY;





	private: System::Windows::Forms::TextBox^  coordenadaX;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape1;



	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::PictureBox^  imagenRoi;
	private: System::Windows::Forms::TextBox^  salidaRpta;

	private: System::Windows::Forms::Button^  visualizarResultados;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Interfaz::typeid));
			this->seleccionarImagen = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->imagenCuadro = (gcnew System::Windows::Forms::PictureBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->valorObtenido = (gcnew System::Windows::Forms::TextBox());
			this->nombreImagen = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->imagenRoi = (gcnew System::Windows::Forms::PictureBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->coordenadaY = (gcnew System::Windows::Forms::TextBox());
			this->coordenadaX = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->imagenDectPuntas = (gcnew System::Windows::Forms::PictureBox());
			this->procesarImagen = (gcnew System::Windows::Forms::Button());
			this->identificarImagen = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->salidaRpta = (gcnew System::Windows::Forms::TextBox());
			this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->visualizarResultados = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenCuadro))->BeginInit();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenRoi))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenDectPuntas))->BeginInit();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// seleccionarImagen
			// 
			this->seleccionarImagen->Location = System::Drawing::Point(275, 27);
			this->seleccionarImagen->Name = L"seleccionarImagen";
			this->seleccionarImagen->Size = System::Drawing::Size(75, 25);
			this->seleccionarImagen->TabIndex = 0;
			this->seleccionarImagen->Text = L"Seleccionar";
			this->seleccionarImagen->UseVisualStyleBackColor = true;
			this->seleccionarImagen->Click += gcnew System::EventHandler(this, &Interfaz::seleccionarImagen_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(16, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(85, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Nombre Imagen:";
			this->label1->Click += gcnew System::EventHandler(this, &Interfaz::label1_Click);
			// 
			// imagenCuadro
			// 
			this->imagenCuadro->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->imagenCuadro->Location = System::Drawing::Point(49, 122);
			this->imagenCuadro->Name = L"imagenCuadro";
			this->imagenCuadro->Size = System::Drawing::Size(331, 270);
			this->imagenCuadro->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->imagenCuadro->TabIndex = 2;
			this->imagenCuadro->TabStop = false;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(16, 22);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(82, 13);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Le pertenece a:";
			// 
			// valorObtenido
			// 
			this->valorObtenido->Location = System::Drawing::Point(126, 19);
			this->valorObtenido->Name = L"valorObtenido";
			this->valorObtenido->Size = System::Drawing::Size(80, 20);
			this->valorObtenido->TabIndex = 4;
			this->valorObtenido->TextChanged += gcnew System::EventHandler(this, &Interfaz::textBox1_TextChanged);
			// 
			// nombreImagen
			// 
			this->nombreImagen->AutoSize = true;
			this->nombreImagen->Location = System::Drawing::Point(119, 33);
			this->nombreImagen->Name = L"nombreImagen";
			this->nombreImagen->Size = System::Drawing::Size(124, 13);
			this->nombreImagen->TabIndex = 5;
			this->nombreImagen->Text = L".......................................";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->nombreImagen);
			this->groupBox1->Controls->Add(this->seleccionarImagen);
			this->groupBox1->Location = System::Drawing::Point(30, 29);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(365, 75);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Imagen Palma Mano";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &Interfaz::groupBox1_Enter);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->imagenRoi);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Controls->Add(this->pictureBox3);
			this->groupBox2->Controls->Add(this->coordenadaY);
			this->groupBox2->Controls->Add(this->coordenadaX);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->imagenDectPuntas);
			this->groupBox2->Controls->Add(this->procesarImagen);
			this->groupBox2->Location = System::Drawing::Point(423, 29);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(376, 189);
			this->groupBox2->TabIndex = 8;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Procesado";
			// 
			// imagenRoi
			// 
			this->imagenRoi->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->imagenRoi->Location = System::Drawing::Point(222, 57);
			this->imagenRoi->Name = L"imagenRoi";
			this->imagenRoi->Size = System::Drawing::Size(134, 100);
			this->imagenRoi->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->imagenRoi->TabIndex = 18;
			this->imagenRoi->TabStop = false;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(230, 164);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(108, 13);
			this->label7->TabIndex = 17;
			this->label7->Text = L"ROI (128 x128 pixels)";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(51, 162);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(92, 13);
			this->label6->TabIndex = 16;
			this->label6->Text = L"Detección Curvas";
			// 
			// pictureBox3
			// 
			this->pictureBox3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox3.Image")));
			this->pictureBox3->Location = System::Drawing::Point(180, 98);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(26, 15);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox3->TabIndex = 15;
			this->pictureBox3->TabStop = false;
			// 
			// coordenadaY
			// 
			this->coordenadaY->Location = System::Drawing::Point(222, 23);
			this->coordenadaY->Name = L"coordenadaY";
			this->coordenadaY->Size = System::Drawing::Size(38, 20);
			this->coordenadaY->TabIndex = 14;
			// 
			// coordenadaX
			// 
			this->coordenadaX->Location = System::Drawing::Point(156, 23);
			this->coordenadaX->Name = L"coordenadaX";
			this->coordenadaX->Size = System::Drawing::Size(36, 20);
			this->coordenadaX->TabIndex = 13;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(198, 26);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(17, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Y:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(136, 26);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(17, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"X:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(14, 26);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(116, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Puntos del Centro ROI:";
			// 
			// imagenDectPuntas
			// 
			this->imagenDectPuntas->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->imagenDectPuntas->Location = System::Drawing::Point(22, 57);
			this->imagenDectPuntas->Name = L"imagenDectPuntas";
			this->imagenDectPuntas->Size = System::Drawing::Size(147, 100);
			this->imagenDectPuntas->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->imagenDectPuntas->TabIndex = 8;
			this->imagenDectPuntas->TabStop = false;
			// 
			// procesarImagen
			// 
			this->procesarImagen->Enabled = false;
			this->procesarImagen->Location = System::Drawing::Point(295, 20);
			this->procesarImagen->Name = L"procesarImagen";
			this->procesarImagen->Size = System::Drawing::Size(72, 23);
			this->procesarImagen->TabIndex = 6;
			this->procesarImagen->Text = L"Procesar";
			this->procesarImagen->UseVisualStyleBackColor = true;
			this->procesarImagen->Click += gcnew System::EventHandler(this, &Interfaz::procesaImagen_Click);
			// 
			// identificarImagen
			// 
			this->identificarImagen->Enabled = false;
			this->identificarImagen->Location = System::Drawing::Point(295, 19);
			this->identificarImagen->Name = L"identificarImagen";
			this->identificarImagen->Size = System::Drawing::Size(75, 23);
			this->identificarImagen->TabIndex = 5;
			this->identificarImagen->Text = L"Identificar";
			this->identificarImagen->UseVisualStyleBackColor = true;
			this->identificarImagen->Click += gcnew System::EventHandler(this, &Interfaz::identificarImagen_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label8);
			this->groupBox3->Controls->Add(this->valorObtenido);
			this->groupBox3->Controls->Add(this->identificarImagen);
			this->groupBox3->Location = System::Drawing::Point(423, 224);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(376, 122);
			this->groupBox3->TabIndex = 9;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Identificación";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(30, 22);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(77, 13);
			this->label8->TabIndex = 6;
			this->label8->Text = L"Valor Obtenido";
			this->label8->Click += gcnew System::EventHandler(this, &Interfaz::label8_Click);
			// 
			// salidaRpta
			// 
			this->salidaRpta->Location = System::Drawing::Point(117, 20);
			this->salidaRpta->Name = L"salidaRpta";
			this->salidaRpta->Size = System::Drawing::Size(159, 20);
			this->salidaRpta->TabIndex = 7;
			// 
			// shapeContainer1
			// 
			this->shapeContainer1->Location = System::Drawing::Point(0, 0);
			this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(1) {this->lineShape1});
			this->shapeContainer1->Size = System::Drawing::Size(842, 444);
			this->shapeContainer1->TabIndex = 6;
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape1
			// 
			this->lineShape1->BorderColor = System::Drawing::SystemColors::ControlDarkDark;
			this->lineShape1->BorderStyle = System::Drawing::Drawing2D::DashStyle::Dot;
			this->lineShape1->Cursor = System::Windows::Forms::Cursors::Default;
			this->lineShape1->Name = L"lineShape1";
			this->lineShape1->X1 = 408;
			this->lineShape1->X2 = 408;
			this->lineShape1->Y1 = 26;
			this->lineShape1->Y2 = 418;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->salidaRpta);
			this->groupBox4->Controls->Add(this->visualizarResultados);
			this->groupBox4->Controls->Add(this->label2);
			this->groupBox4->Location = System::Drawing::Point(423, 352);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(376, 51);
			this->groupBox4->TabIndex = 11;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"Visualizar Resultados";
			// 
			// visualizarResultados
			// 
			this->visualizarResultados->Enabled = false;
			this->visualizarResultados->Location = System::Drawing::Point(295, 17);
			this->visualizarResultados->Name = L"visualizarResultados";
			this->visualizarResultados->Size = System::Drawing::Size(73, 23);
			this->visualizarResultados->TabIndex = 5;
			this->visualizarResultados->Text = L"Visualizar";
			this->visualizarResultados->UseVisualStyleBackColor = true;
			this->visualizarResultados->Click += gcnew System::EventHandler(this, &Interfaz::visualizarResultados_Click);
			// 
			// Interfaz
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(842, 444);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->imagenCuadro);
			this->Controls->Add(this->shapeContainer1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Interfaz";
			this->Text = L"Modelo Sistema Hibrido para Identifiación de Personas  basado Palma de Mano";
			this->Load += gcnew System::EventHandler(this, &Interfaz::Interfaz_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenCuadro))->EndInit();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenRoi))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->imagenDectPuntas))->EndInit();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Interfaz_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void seleccionarImagen_Click(System::Object^  sender, System::EventArgs^  e) {

				 //Se crea un objeto de la clase OpenFileDialo para poner buscar la imagen 
				 OpenFileDialog^ dlg = gcnew OpenFileDialog();

				 dlg->Title = "Abrir Imagen";
				 //Filtramos el contenido que vamos a seleccionar
				 dlg->Filter = "bmp archivos (*.bmp)|*.bmp|Todos Archivos (*.*)|*.*" ;

				 //Se verifica que cuadro dialogo es correcto
				 if ( dlg->ShowDialog() ==  System::Windows::Forms::DialogResult::OK)
				 {
					 imagenCuadro->Image = gcnew Bitmap( dlg->OpenFile());
					 nombreImagen->Text = dlg->SafeFileName;
					 rutaImagenCompleta = dlg->FileName;

				 }
				 procesarImagen->Enabled=true;

			 }
	
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void pictureBox3_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label8_Click(System::Object^  sender, System::EventArgs^  e) {
			 }

	private: System::Void procesaImagen_Click(System::Object^  sender, System::EventArgs^  e) {

				 // Procesamos la imagen que fue seleccionada por el usuario

				 using System::Runtime::InteropServices::Marshal;
				 System::IntPtr pointer = Marshal::StringToHGlobalAnsi(rutaImagenCompleta);
				 char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
				 std::string returnString(charPointer, rutaImagenCompleta->Length);
				 Marshal::FreeHGlobal(pointer);

				// string url = "imagenes/PalmadelaMano/"+returnString;				
				 //string url = "imagenes/PalmadelaMano/"+returnString;				
				 stringstream stream4;
 				 string nroPatron;

				 stream4<<contadorImagenesPatr;
			     stream4>>nroPatron;
				  string patron = nroPatron+"patron"+returnString;
				 const char * url1 = returnString.c_str();
				 Imagen *objImagen1 = new Imagen(url1,patron);

				 objImagen1->ProcesarImagen();

				 IplImage* image =objImagen1->RetornarimagenDeteccionDedos();


				 imagenDectPuntas->Image = gcnew Bitmap(image->width,image->height,image->widthStep,
					 System::Drawing:: Imaging::PixelFormat::Format8bppIndexed,(System::IntPtr)image->imageData); 

				 IplImage* image2 =objImagen1->RetornarimagenSobelROI();

				 //Guardamos su vector caracteristica

				 vector<float> in= objImagen1->RetornarvectorCaract();
				 vectCPrueba.push_back(1); // Se pone 1 a la bias
				 for(unsigned k=0;k<in.size();++k) {vectCPrueba.push_back(in[k]);}  
			

				 imagenRoi->Image = gcnew Bitmap(image2->width,image2->height,image2->widthStep,
					 System::Drawing:: Imaging::PixelFormat::Format24bppRgb,(System::IntPtr)image2->imageData); 

				 //Se establecen lo punto del centro del ROI

				 coordenadaX->Text = objImagen1->RetornarpuntoROI()[0].x.ToString();
				 coordenadaY->Text = objImagen1->RetornarpuntoROI()[0].y.ToString();
				 contadorImagenesPatr++;

				 identificarImagen->Enabled=true;

			 }	 

	private: System::Void identificarImagen_Click(System::Object^  sender, System::EventArgs^  e) {

					RedNeuronal *red;
					vector<int> info;

					info.push_back(81); //Capa Entrada
					info.push_back(81);   //Capa Oculta era 40
					info.push_back(50); //Capa Salida 50 
				 
					vector<float> er; // Almacenara el error del patron 
					map<float,int> w;
					int cantPalmas=50; 

					 if(p == -1){
				    			
					 //Formato de la imagen a leer
					 string form;
					 string form2;

					 int nroTomas=4;
					 int indImagenes=0;

					 vector<pair<vector<float>,vector<int> > > p;

					  //------------------------------Lectura de las manos------------------------------------------//

					 for(int i=1;i<=cantPalmas;i++)
					 {
						 for(int j=1;j<=nroTomas;++j)
						 {

							 stringstream stream;
							 stringstream stream2;
 							 string nroImagen;
							 string nroToma;
							 stringstream stream3;
							 string nro;

							 if(i<10)  stream<<"00"<<i;		 
							 else if(i<100 && i>=10)  stream<<"0"<<i;
							 else if(i>=100)  stream<<i; // Cuanto son mayores de 101
						 
							 stream>>nroImagen;
					 
							 stream2<<"0"<<j;
							 stream2>>nroToma;

							 stream3<<contadorImagenes;
							 stream3>>nro;

	 						 form = "imagenes/PalmadelaMano/PolyU_"+nroImagen+"_F_"+nroToma+".bmp";
							 form2 = nro+"PolyU_"+nroImagen+"_F_"+nroToma+".bmp";
							 Imagen *objImagen = new Imagen(form.c_str(),form2);
						 
							 contadorImagenes++;
							 objImagen->ProcesarImagen();
							
							 vector<float> vectC=objImagen->RetornarvectorCaract(); //Extra el vector Caracteristica

							 // Se establacen los datos para la red  neuronal
							 vector<float> in1;
							 pair<vector<float>,vector<int> > p1;
 
							 // Se copian los datos del vector caracteristica de 80 componentes al variable 
							 // que se era usada para la entrada de mi red neuronal
						
							 in1.push_back(1); // Se pone 1 a la bias
							 for(unsigned k=0;k<vectC.size();++k) {in1.push_back(vectC[k]);}  

							 vector<int> sa1(cantPalmas,0);						 
							 sa1[indImagenes]=1;  
							 p1.first=in1; p1.second=sa1;
						 
							 //Añade el patron 
							 p.push_back(p1); 	
						 }
				 		 indImagenes++;
					 }
				 
					 
					 red=new RedNeuronal(info);
					 
					 
					 //------------Algoritmos Genetico parte hibrida--------------------//
					 // Con 100 individuos cada poblacion
					 AgAlgoritmoGenetico ag(100,info,red);
					 int nroGeneraciones=100;

					 //Se agregan todos los patrones
					 for(int i=0;i<p.size();++i){ ag.agregarPatron(p[i]); }		  
					 AgCromosoma *pcro=NULL; // Se crea para poder almacenar el individuo o cromosoma mas optimo

					 // Iteracciones de la generaciones 
					 for(int i=0;i<nroGeneraciones;i++){ pcro=ag.nextGeneracion();}
					 red->setPesos(pcro->retonarData());	

					//------- Entrenamieto con la red Neuronal------------------// 
					 int nroVecesEntrenaRed=1000;
					 for(int i=0;i<nroVecesEntrenaRed;i++) {red->entrenar(p);}
					 red->retornarPesos(); 
                 	 
					 
					 er=red->evaluar(vectCPrueba); 
				 
					 for(unsigned i=0;i<er.size();++i){ w[er[i]] = i ;	}

					 //Escribimos el resultado en .txt
					 ofstream archivoResultado; 
					 string salida = "resultado";
					 archivoResultado.open("archivosPixelROI/"+salida + ".txt");

					for(int i = 0; i<er.size(); ++i) { archivoResultado<<er[i]<<" ";}

					sort(er.begin(),er.end()); // Se ordena para obtener el mayoy y saber que personas es
					rpta =(w[er[er.size()-1]])+1;
				 }

				else{	
					 red=new RedNeuronal(info);		 

					//Leemos .txt que tiene entrenado ya los pesos
					ifstream pesosOptimos,pesosOptimos2;
					vector<float**> Pesos;
					for(unsigned int m=0;m<(info.size()-1);m++){
						float **pesos=new float*[info[m]];
						for(int i=0;i<info[m];i++){
							pesos[i]=new float[info[m+1]];
						}
						Pesos.push_back(pesos);
					 }
		  
					pesosOptimos.open("archivosPixelROI/pesosRed.txt");
					for(int i=0;i<info[0];i++){
						for(int j=0;j<info[1];j++){
							stringstream valor;
							string output;
							float valorf; 
							pesosOptimos>>output;
							valor<<output;
							valor>>valorf; 
							(Pesos[0])[i][j]=valorf; 
						}
					}
					
					pesosOptimos2.open("archivosPixelROI/pesosRed2.txt");
					for(int i=0;i<info[1];i++){
						for(int j=0;j<info[2];j++){
							stringstream valor;
							string output;
							float valorf; 
							pesosOptimos2>>output;	
							valor<<output;
							valor>>valorf; 
							(Pesos[1])[i][j]=valorf; 
						}
					}

					pesosOptimos.close();
					pesosOptimos2.close();

					red->setPesos(Pesos);
					er=red->evaluar(vectCPrueba);

					
					 for(unsigned i=0;i<er.size();++i){ w[er[i]] = i ;	}

					 //Escribimos el resultado en .txt
					 ofstream archivoResultado; 
					 string salida = "resultado";
					 archivoResultado.open("archivosPixelROI/"+salida + ".txt");

					 for(int i = 0; i<er.size(); ++i) { archivoResultado<<er[i]<<" ";}

					 sort(er.begin(),er.end()); // Se ordena para obtener el mayoy y saber que personas es
					 rpta =(w[er[er.size()-1]])+1; // Obtener el indice

	   			     valorObtenido->Text=er[er.size()-1].ToString();//Obtener el valor obtenido de la RN						 
					 valorRpta=er[er.size()-1];
				 }
				 vectCPrueba.clear();
				 er.clear();
				 w.clear();
				 p++;
				 info.clear();
				 visualizarResultados->Enabled=true;
				 identificarImagen->Enabled=false;
			 }

	private: System::Void visualizarResultados_Click(System::Object^  sender, System::EventArgs^  e) {
			///Aqui se verifica la manos que no estan registradas en la base de datos
			  if(valorRpta<0.6){salidaRpta->Text="No se encuentra Registrado";}
			  else{
				salidaRpta->Text=rpta.ToString();
			  }
		 }
};
}