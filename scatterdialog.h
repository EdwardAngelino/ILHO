#ifndef SCATTERDIALOG_H
#define SCATTERDIALOG_H

#include <QDialog>
#include "dialog.h"

namespace Ui {
class scatterDialog;
}

class scatterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scatterDialog(QWidget *parent = nullptr);
    ~scatterDialog();

    Dialog *pw;
    int opc;
    bool cmbact=false;
    void graficar(QVector<QVector<double>> datnod,QVector<QVector<double>> datfut, QStringList nombres,int cax,int cay);


private slots:
    void on_buttonBox_accepted();

    void on_cmbopc_currentIndexChanged(int index);

    void on_cmb_X_currentIndexChanged(int index);

    void on_cmb_Y_currentIndexChanged(int index);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_clicked();

private:
    Ui::scatterDialog *ui;
};

#endif // SCATTERDIALOG_H
