#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) : QWidget(parent), ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    Disk disk;
   //disk.createDisk("Prueba", 100000, 4096, 'D');
   disk.openDisk("Prueba D.disk");
   //disk.loadSong("Thunderstruck.mp3");
   //disk.readSuperBlock("Prueba D.disk");
   //disk.buscarCancion("Thunderstruck.mp3");
   disk.readBitMap();
//   Lista<int>* lista = disk.getBloqueDisponible(8220835);
//   cout << "Posicion del primer bloque libre: " << lista->obtenerValor(2006) << endl;
}

MainMenu::~MainMenu()
{
    delete ui;
}
