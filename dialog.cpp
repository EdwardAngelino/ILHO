#include "dialog.h"
#include "ui_dialog.h"
#include <QFileDialog>
#include <QStandardItemModel>
#include <QTextStream>
#include <QDebug>
#include "glpk.h"
#include <QProgressDialog>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);



    ui->toolButton_3->setEnabled(false); //no calcular hasta que se lea bie todos los cuadros

    //fijando los modelos de datos de las tablas al contro
    modelounc = new QStandardItemModel(); ui->dxuncView->setModel(modelounc);
    modeloatt= new QStandardItemModel();  ui->dxattView->setModel(modeloatt);
    modelofut = new QStandardItemModel(); ui->tofutView->setModel(modelofut);
    modelotoatt = new QStandardItemModel(); ui->toattView->setModel(modelotoatt);


    leidounc=false;
    leidoatt=false;
    leidofut=false;

    // controlar la edicion de los QTableView
    connect(ui->dxuncView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(onDataChangedunc(const QModelIndex&, const QModelIndex&)));
    connect(ui->dxattView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(onDataChangedatt(const QModelIndex&, const QModelIndex&)));
    connect(ui->tofutView->model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(onDataChangedtofut(const QModelIndex&, const QModelIndex&)));

    QDir::setCurrent(qApp->applicationDirPath());

    QHeaderView *verticalHeader = ui->dxuncView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(16);

    verticalHeader = ui->dxattView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(16);

    verticalHeader = ui->tofutView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(16);

    verticalHeader = ui->toattView->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(16);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::leeimprimetabla(QStringList &nom, QVector<QVector<double> > &dat,
                             QStandardItemModel *modelo, QString filename)
{

    modelo->clear();
    dat.clear();
    nom.clear();
    QFile file(filename);
    QTextStream in(&file);

    if(file.open(QFile::ReadOnly | QFile::Text)){
        QString line = in.readLine();
        QStringList listt;
        nom = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        modelo->setHorizontalHeaderLabels(nom);
        //qDebug() << nom.at(0);
        int r = 0;
        while (!in.atEnd()) {
            listt.clear();
            line = in.readLine();
            listt = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            QVector<double> datosfila;
            for(int k = 0 ; k < nom.count() ; k++) {
                datosfila.append(listt.at(k).toFloat());
                QStandardItem *item = new QStandardItem(listt.at(k));
                item->setTextAlignment(Qt::AlignRight); item->setEditable(true);
                modelo->setItem(r,k,item);
            }
            dat.append(datosfila);
            r++;
        }
    }       // else
}



/*
void Dialog::leematrizglpk(QVector<QVector<double> > dunc, int *iv, int *jv, double *val)
{

    for(int i=1, k=1 ; i <= datdxunc.count(); i++)
        for(int j =1 ; j <= datdxunc.begin()->count(); j++){
            iv[k] = i; jv[k]=j; val[k]=dunc.at(i-1).at(j-1);
            k++;
        }
}*/

void Dialog::on_toolButton_clicked()
{
    //abrir dxunc
    leidounc = false;
    const QString fileName = QFileDialog::getOpenFileName(this,tr("Archivo de Incertidumbres")); //falta filtros
        if (!fileName.isEmpty() ) {
            ui->nomdxunc->setText(fileName);
            leeimprimetabla(nomdxunc,datdxunc,modelounc,fileName);
            leidounc = true;

            if(leidounc && leidoatt && leidofut) ui->toolButton_3->setEnabled(true);
        }
}

void Dialog::on_toolButton_4_clicked()
{
  //DXatt
    leidoatt = false;
    const QString fileName = QFileDialog::getOpenFileName(this,tr("Archivo de Atributos")); //falta filtros
        if (!fileName.isEmpty() ) {
            ui->nomdxatt->setText(fileName);
            leeimprimetabla(nomdxatt,datdxatt,modeloatt,fileName);
            leidoatt = true;
            if(leidounc && leidoatt && leidofut) ui->toolButton_3->setEnabled(true);
        }
}

void Dialog::on_toolButton_2_clicked()
{
    //tofut
    leidofut = false;
    const QString fileName = QFileDialog::getOpenFileName(this,tr("Archivo de Futuros")); //falta filtros
        if (!fileName.isEmpty() ) {
            ui->nomtofut->setText(fileName);
            leeimprimetabla(nomtofut,dattofut,modelofut,fileName);
            leidofut = true;
            if(leidounc && leidoatt && leidofut) ui->toolButton_3->setEnabled(true);
        }

}

void Dialog::onDataChangedunc(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (leidounc) {  //para que no se active cuando esta leyendo los datos.
        //qDebug() << "se modifico dxunc de " << datdxunc.at(topLeft.row()).at(topLeft.column()) << "a " << topLeft.data().toFloat();
        datdxunc[topLeft.row()][topLeft.column()] = topLeft.data().toFloat();
       //qDebug() << "nuevo" <<  datdxunc.at(topLeft.row()).at(topLeft.column());
    }

}

void Dialog::onDataChangedatt(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (leidoatt) {
        //qDebug() << "se modifico dxatt de " << datdxatt.at(topLeft.row()).at(topLeft.column()) << "a " << topLeft.data().toFloat();
        datdxatt[topLeft.row()][topLeft.column()] = topLeft.data().toFloat();
    }
}

void Dialog::onDataChangedtofut(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (leidofut) {
        //qDebug() << "se modifico tofut de " << dattofut.at(topLeft.row()).at(topLeft.column()) << "a " << topLeft.data().toFloat();
        dattofut[topLeft.row()][topLeft.column()] = topLeft.data().toFloat();
    }
}

void Dialog::on_toolButton_3_clicked()
{
    //calcular

    qDebug() << "numero de incert : " << datdxunc.at(0).count() << datdxunc.count() << "registros";
    qDebug() << "numero de atributos : " << datdxatt.at(0).count() << datdxatt.count() << "registros";
    qDebug() << "numero de futuros : " << dattofut.at(0).count() << dattofut.count() << "registros";

    glp_prob *lp;
    // (n+1) x m : n = numero de incertidumbres , m = numero de nodos.
    int m = datdxunc.count();
    int n = datdxunc.at(0).count();
    int tot = (1+n)*m+1; //joder si que faltaba 1

    //qDebug() << m << n << tot;

    int ii[tot];
    int jj[tot];
    double ar[tot], z, x[m];

  //lectura de la matriz para el problema


  for(int i = 1,k=1 ; i <= m; i++)
        for(int j = 1 ; j <= n; j++){
            jj[k] = i;
            ii[k] = j;
            ar[k] = datdxunc.at(i-1).at(j-1);
            k++;
        }

  for(int i=1, k=n*m+1 ; i <= m; i++){
    ii[k] = n+1;
    jj[k] = i;
    ar[k] = 1.0;
    k++;
  }


  //crear el problema

    lp = glp_create_prob();
    glp_set_prob_name(lp, "Interpolacion");
    glp_set_obj_dir(lp, GLP_MIN);

    //añadiendo los xi
    glp_add_cols(lp, m);        //numero de nodos. xi
    for(int i = 1 ; i <= m ; i++){
        QString var = "x"+ QString::number(i);
        glp_set_col_name(lp, i, var.toStdString().c_str());
        glp_set_col_bnds(lp, i, GLP_LO, 0.0, 0.0);
//      glp_set_obj_coef(lp, i, datdxatt.at(i-1).at(1));
    }


    // añadiendo las filas y nombres de las unc
    glp_add_rows(lp, n+1);
    for(int i = 1 ; i <= n ; i++) {
        glp_set_row_name(lp, i, nomdxunc.at(i-1).toStdString().c_str()); //nomdxatt.at(i).toStdString());
  //    glp_set_row_bnds(lp, i, GLP_FX, dattofut.at(0).at(i-1), dattofut.at(0).at(i-1));
    }


    glp_set_row_name(lp,n+1,"suma_uno");  // la ultima fila sera de suma(xi) = 1
    glp_set_row_bnds(lp, n+1, GLP_FX, 1.0, 1.0);

    glp_load_matrix(lp, (1+n)*m , ii, jj, ar); //se puede subir antes

    int pr=1;
    dattoatt.clear();


    QFile fileout(ui->nomtoatt->text());
    QTextStream out(&fileout);
    fileout.open(QFile::WriteOnly | QFile::Text);

    modelotoatt->setHorizontalHeaderLabels(nomdxatt);
    foreach(auto n, nomdxatt) out << n << "\t";

    int numTasks = 100;
    QProgressDialog progress("Interpolacion de futuros en progreso...", "Cancel", 0, numTasks, this);
    progress.setWindowModality(Qt::WindowModal);

    glp_term_out(GLP_OFF);

    for(int f =0; f < dattofut.count(); f++ ){
        for(int i = 1 ; i <= n ; i++)
            glp_set_row_bnds(lp, i, GLP_FX, dattofut.at(f).at(i-1), dattofut.at(f).at(i-1)); // prueba para el segundo

        out << "\n";

        QVector<double> toatttemp;
        for(int t=0 ; t < datdxatt.at(0).count() ; t++ ){
             for(int i = 1 ; i <= m ; i++)
                    glp_set_obj_coef(lp, i, datdxatt.at(i-1).at(t));  // para el primero

              glp_simplex(lp, NULL);
              z = glp_get_obj_val(lp);  // valor para TOATT de el futuro evaluado y atributo

              out << z << "\t";
            /*
              qDebug() << "Valor objetivo(att=" << t+1 << ",fut=" << f+1 <<  ") =" << z << glp_get_status(lp); //4 es infeasible
              qDebug() << "x1"  << glp_get_col_prim(lp, 1);
              qDebug() << "x2"  << glp_get_col_prim(lp, 2);
              qDebug() << "x3"  << glp_get_col_prim(lp, 3);
              qDebug() << "x4"  << glp_get_col_prim(lp, 4);
            */
              QString celltemp = QString::number(z,'f',2) + QString((glp_get_status(lp) ==4) ? " *":"  ");
              QStandardItem *item = new QStandardItem(celltemp);
              item->setTextAlignment(Qt::AlignRight);
              modelotoatt->setItem(f,t,item);

              //QString probnomfile = "ProblemaLP" + QString::number(pr++) + ".txt";
              //glp_write_lp(lp,NULL,probnomfile.toStdString().c_str());

              toatttemp.append(z);
          }

        out << QString((glp_get_status(lp) ==4) ? "*":"");
        dattoatt.append(toatttemp);
        progress.setValue(f/dattofut.count()*numTasks);
        if (progress.wasCanceled())      break;
       }
    progress.setValue(numTasks);
    glp_delete_prob(lp);
}
