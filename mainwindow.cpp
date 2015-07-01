#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tagScene = new QGraphicsScene();
    this->superBlockScene = new QGraphicsScene();
    this->player = new QMediaPlayer(this);
    this->tagScene->setSceneRect(0, 0, ui->graphicsViewTags->width(), ui->graphicsViewTags->height());
    this->superBlockScene->setSceneRect(0, 0, ui->graphicsViewSuperBlock->width(), 500);
    this->cant_bloques_previo = 0;
    this->pos_bloque_actual = 0;
    ui->graphicsViewTags->setScene(this->tagScene);
    ui->graphicsViewSuperBlock->setScene(this->superBlockScene);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::on_positionchanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::on_durationchanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Disk_triggered()
{
    //Crear instancia de disco y llamar a la funcion CreateDisk
    CrearDisco* newDisco = new CrearDisco();
    newDisco->show();
}

void MainWindow::on_actionOpen_Disk_triggered()
{
    QString disco = QFileDialog::getOpenFileName(this, "Seleccione un disco", "", "*.disk");
    if(disco == "")
        return;
    QFileInfo file(disco);
    QString name = file.baseName();
    name.append(".disk");
    Lista<string>* canciones = disk.openDisk(name.toStdString());
    this->cant_bloques_previo = disk.super.used_blocks;

    if(canciones != NULL)
    {
        for(int i = 0; i < canciones->getCantidad(); i++)
        {
            string cancion = canciones->obtenerValor(i);
            ui->cmbSongs->addItem(cancion.c_str());
        }
    }

    enableButtons();
    ui->tableWidget->setRowCount(disk.super.cant_block / ui->tableWidget->columnCount());
    loadTable();
}

void MainWindow::on_actionDelete_Disk_triggered()
{
    QString disco = QFileDialog::getOpenFileName(this, "Seleccione disco virtual para borrar", "", "*.disk");
    QFileInfo file(disco);
    QString name = file.baseName();
    name.append(".disk");

    int number = remove(name.toStdString().c_str());

    if(number == 0)
    {
        cout << "El disco virtual ha sido borrado" << endl;
    }

    else
    {
        cout << "No se pudo borrar el disco virtual" << endl;
    }
}

void MainWindow::enableButtons()
{
    ui->btnAgregar->setEnabled(true);
    ui->btnBorrar->setEnabled(true);
    ui->btnExportar->setEnabled(true);
    ui->btnInfo->setEnabled(true);
    ui->btnMostrarBitmap->setEnabled(true);
    ui->btnPlay->setEnabled(true);
    ui->cmbSongs->setEnabled(true);
    ui->horizontalSliderPos->setEnabled(true);
    ui->graphicsViewSuperBlock->setEnabled(true);
    ui->graphicsViewTags->setEnabled(true);
    ui->tableWidget->setEnabled(true);
}

void MainWindow::loadTable()
{
    //Lista<int>* peso_canciones = pesoCanciones();
    int inumber = 0;

    for(int i = 0; i < (disk.super.cant_block / ui->tableWidget->columnCount()); i++)
    {
        for(int x = 0; x < ui->tableWidget->columnCount(); x++)
        {
            ui->tableWidget->setItem(i, x, new QTableWidgetItem(QString::number(pos_bloque_actual)));
            if(pos_bloque_actual < disk.super.block_size)
            {
                ui->tableWidget->item(i, x)->setBackground(Qt::blue);
            }

            else if(pos_bloque_actual < disk.super.inicioInodeTable)
            {
                ui->tableWidget->item(i, x)->setBackground(Qt::green);
            }

            else if(pos_bloque_actual < disk.super.inicioInode)
            {
                ui->tableWidget->item(i, x)->setBackground(Qt::red);
            }

            else if(pos_bloque_actual < disk.super.inicioDataBlocks)
            {
                ui->tableWidget->item(i, x)->setBackground(Qt::yellow);
            }

//            else if((peso_canciones != NULL) &&   pos_bloque_actual < (disk.super.inicioDataBlocks + peso_canciones->obtenerValor(inumber)))
//            {
//                ui->tableWidget->item(i, x)->setBackground(Qt::darkBlue);
//            }

            else
            {
                inumber++;
            }

//            if(pos_bloque_actual == (disk.super.used_blocks * disk.super.block_size))
//            {
//                cout << pos_bloque_actual << endl;
//                return;
//            }

//            if(peso_canciones != NULL &&  inumber >= peso_canciones->getCantidad())
//                return;

            pos_bloque_actual+= 4096;
        }
    }
}

Lista<int>*MainWindow::pesoCanciones()
{
    if(disk.super.canciones_cargadas == 0)
        return NULL;

    Lista<int>* peso_canciones = new Lista<int>();

    for(int i = 0; i < disk.super.num_inodos; i++)
    {
        if(disk.inodeOnTable[i].inumber == -1)
            return peso_canciones;

        Inode inode;
        int pos = disk.super.inicioInode + (sizeof(Inode) * i);
        disk.file.seekg(pos, ios::beg);
        disk.file.read((char*)&inode, sizeof(Inode));
        peso_canciones->agregar(inode.size);
    }

    return peso_canciones;
}

void MainWindow::on_btnAgregar_clicked()
{
    QString song_path = QFileDialog::getOpenFileName(this, "Seleccione una cancion", "", "*.mp3");
    QFileInfo song(song_path);
    QString song_name = song.baseName();
    if(song_path == "")
        return;
    string name = disk.loadSong(song_path.toStdString());
    if(name == "NO")
    {
        cout << "La cancion no cabe en el disco";
        return;
    }
    cout << song_path.toStdString() << endl;

    ui->cmbSongs->addItem(name.c_str());
    int bloques_agregados = disk.super.used_blocks - this->cant_bloques_previo;
    this->cant_bloques_previo = disk.super.used_blocks;

    //loadTable();
}

void MainWindow::on_btnExportar_clicked()
{
    QString cancion = ui->cmbSongs->currentText();
    if(cancion == "")
        return;
    disk.buscarCancion(cancion.toStdString());
}

void MainWindow::on_btnPlay_clicked()
{
    if(disk.super.canciones_cargadas == 0)
        return;

    QString cancion = ui->cmbSongs->currentText();
    player->setMedia(QUrl::fromLocalFile(cancion));
    player->play();
    cout << player->errorString().toStdString() << endl;
}

void MainWindow::on_btnInfo_clicked()
{
    superBlockScene->clear();
    superBlockScene->update();

    int pos_y = 0;

    superBlockScene->addText("Nombre: ")->setPos(0, pos_y);
    superBlockScene->addText(disk.super.nombre)->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Peso de bloque: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.block_size))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Numero de canciones: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.canciones_cargadas))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Cantidad de bloques: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.cant_block))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Fin de los inodos: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.finInode))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Fin del InodeTable: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.finInodeTable))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Bloques libres: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.free_blocks))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Inicio de data blocks: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.inicioDataBlocks))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Inicio de los inodos: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.inicioInode))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Inicio del InodeTable: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.inicioInodeTable))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Inodos libres:: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.inodos_libres))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Nombre:: ")->setPos(0, pos_y);
    superBlockScene->addText(disk.super.nombre)->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Numero de inodos: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.num_inodos))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Particion: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.particion))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Peso del disco virtual: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.size))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Peso del bitmap (bytes): ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.size_bitmap))->setPos(200, pos_y);
    pos_y+= 25;

    superBlockScene->addText("Bloques usados: ")->setPos(0, pos_y);
    superBlockScene->addText(QString::number(disk.super.used_blocks))->setPos(200, pos_y);
    pos_y+= 25;
}

void MainWindow::on_cmbSongs_currentIndexChanged(int index)
{
    tagScene->clear();
    tagScene->update();

    int pos_y = 0;
    Lista<string>* tags = disk.showTAGS(index);
    if(tags == NULL)
        return;

    for(int i = 0; i < tags->getCantidad(); i++)
    {
        tagScene->addText(tags->obtenerValor(i).c_str())->setPos(20, pos_y);
        cout << tags->obtenerValor(i).c_str() << endl;
        pos_y+= 25;
    }

    delete tags;
}

void MainWindow::on_horizontalSliderPos_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_positionchanged(qint64 pos)
{
    ui->horizontalSliderPos->setValue(pos);
}

void MainWindow::on_durationchanged(qint64 pos)
{
    ui->horizontalSliderPos->setMaximum(pos);
}
