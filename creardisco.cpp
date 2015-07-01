#include "creardisco.h"
#include "ui_creardisco.h"

CrearDisco::CrearDisco(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CrearDisco)
{
    ui->setupUi(this);
}

CrearDisco::~CrearDisco()
{
    delete ui;
}

void CrearDisco::on_slider_peso_valueChanged(int value)
{
    QString peso = QString::number(value * 512);
    ui->lineEdit_pesoBloque->setText(peso);
}

void CrearDisco::on_lineEdit_pesoBloque_textChanged(const QString &arg1)
{
    //IGNORAR
}

void CrearDisco::on_btnCrearDisco_clicked()
{
    string nombre = ui->lineEditNombreDisco->text().toStdString();
    int cant_bloques = ui->lineEditCantBloques->text().toInt();
    int peso_bloques = ui->lineEdit_pesoBloque->text().toInt();
    string particion = ui->lineEditParticion->text().toStdString();
    char letra = particion[0];

    if(nombre == "" || cant_bloques == 0 || peso_bloques == 0 || particion == "")
    {
        cout << "No se pudo crear disco" << endl;
        this->hide();
    }

    else
    {
        Disk disk;
        disk.createDisk(nombre, cant_bloques, peso_bloques, letra);
        cout << "Disco ha sido creado" << endl;
        this->hide();
    }
}
