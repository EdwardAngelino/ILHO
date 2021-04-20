#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTableView>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void leeimprimetabla(QStringList &nom, QVector<QVector<double>> &dat, QStandardItemModel *modelo, QString filename);

    //void leematrizglpk(QVector<QVector<double>> dunc,int *iv,int *jv,double *val);

    QStringList nomdxunc;
    QStringList nomdxatt;
    QStringList nomtofut;

    QVector<QVector<double>> datdxunc;
    QVector<QVector<double>> datdxatt;
    QVector<QVector<double>> dattofut;
    QVector<QVector<double>> dattoatt;

    QStandardItemModel *modelounc;
    QStandardItemModel *modeloatt;
    QStandardItemModel *modelofut;
    QStandardItemModel *modelotoatt;


    bool leidounc;
    bool leidoatt;
    bool leidofut;

private slots:
    void on_toolButton_3_clicked();

    void on_toolButton_clicked();

    void on_toolButton_4_clicked();

    void on_toolButton_2_clicked();

    void onDataChangedunc(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void onDataChangedatt(const QModelIndex& topLeft, const QModelIndex& bottomRight);

    void onDataChangedtofut(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
