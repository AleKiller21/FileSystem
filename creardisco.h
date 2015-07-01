#ifndef CREARDISCO_H
#define CREARDISCO_H

#include <QMainWindow>
#include "Disk.h"

namespace Ui {
    class CrearDisco;
}

class CrearDisco : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrearDisco(QWidget *parent = 0);
    ~CrearDisco();

private slots:
    void on_slider_peso_valueChanged(int value);

    void on_lineEdit_pesoBloque_textChanged(const QString &arg1);

    void on_btnCrearDisco_clicked();

private:
    Ui::CrearDisco *ui;
};

#endif // CREARDISCO_H
