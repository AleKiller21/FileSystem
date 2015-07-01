#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTableWidget>
#include <QtGui>
#include <QtCore>
#include <QMediaPlayer>
#include "creardisco.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    Disk disk;
    QGraphicsScene* tagScene;
    QGraphicsScene* superBlockScene;
    QTableWidget* table;
    QMediaPlayer* player;
    int cant_bloques_previo;
    int pos_bloque_actual;

    void enableButtons();
    void loadTable();
    Lista<int>* pesoCanciones();

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_Disk_triggered();

    void on_actionOpen_Disk_triggered();

    void on_actionDelete_Disk_triggered();

    void on_btnAgregar_clicked();

    void on_btnExportar_clicked();

    void on_btnInfo_clicked();

    void on_cmbSongs_currentIndexChanged(int index);

    void on_btnPlay_clicked();

    void on_horizontalSliderPos_sliderMoved(int position);

    void on_positionchanged(qint64 pos);

    void on_durationchanged(qint64 pos);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
