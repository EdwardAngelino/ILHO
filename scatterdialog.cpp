#include "scatterdialog.h"
#include "ui_scatterdialog.h"
#include "dialog.h"

scatterDialog::scatterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scatterDialog)
{
    ui->setupUi(this);
    Dialog *w = qobject_cast<Dialog*>(parent);
    pw = w;  // mejor lo asigno global y publico

    //qDebug() << pw->nomdxunc.count();
    //llenar combos
    ui->cmbopc->clear();

    ui->cmbopc->addItem("Incertidumbre");
    ui->cmbopc->addItem("Atributo");

    ui->cmb_X->clear(); ui->cmb_Y->clear();
    on_cmbopc_currentIndexChanged(0);

}

scatterDialog::~scatterDialog()
{
    delete ui;
}

void scatterDialog::graficar(QVector<QVector<double> > datnod, QVector<QVector<double> > datfut, QStringList nombres, int cax, int cay)
{
    int nnod= datnod.count();
    int nfut = datfut.count();
    int ncampos = nombres.count();
    //datx.at(n).at(cax);
    //daty.at(n).at(cay);

    QString cad = (opc) ?  "Interpolaciones" : "Futuros" ;
    QString cad2 = (opc) ?  "Atributos" : "Incertidumbres" ;

    qDebug() << "dibujando" << cad << cax << cay;


   //limpiar el grafico
    for( int g=0; g < ui->customPlot->graphCount(); g++ ){
        ui->customPlot->graph(g)->data().data()->clear();

        //ui->customPlot->graph(g)->data().data()->
    }
    for( int g=0; g < ui->customPlot->graphCount(); g++ ) ui->customPlot->legend->clear();

   ui->customPlot->xAxis->setRange(0,0.001);
   ui->customPlot->yAxis->setRange(0,0.001);

    // graficar los nodos
    QPen pen;
    pen.setColor(QColor(255, 0, 0, 127));
    ui->customPlot->addGraph();
    ui->customPlot->graph()->setPen(pen);
    ui->customPlot->graph()->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
    ui->customPlot->graph()->setName(cad2);

    // generate data:
      QVector<double> x(nnod), y(nnod);
      double minx=100000000, maxx=-100000000, miny=100000000, maxy=-100000000;
      for (int j=0; j<nnod; ++j)
      {
        x[j] = datnod.at(j).at(cax); if(maxx < x[j]) maxx = x[j]; if(minx > x[j]) minx = x[j];
        y[j] = datnod.at(j).at(cay); if(maxy < y[j]) maxy = y[j]; if(miny > y[j]) miny = y[j];
      }
      ui->customPlot->graph()->setData(x, y);

      // graficar futuros
    if( ui->checkBox->isChecked()){
      QPen pen2;
      pen2.setColor(QColor(0, 0, 255, 127));
      ui->customPlot->addGraph();
      ui->customPlot->graph()->setName(cad);
      ui->customPlot->graph()->setPen(pen2);
      ui->customPlot->graph()->setLineStyle(QCPGraph::lsNone);
      ui->customPlot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

       QVector<double> xx(nfut), yy(nfut);
      // generate data:
        for (int j=0; j<nfut; ++j)
        {
          xx[j] = datfut.at(j).at(cax);
          yy[j] = datfut.at(j).at(cay);
        }
        ui->customPlot->graph()->setData(xx, yy);

     }

      if(ncampos > 0) ui->customPlot->xAxis->setLabel(nombres.at(cax));
      if(ncampos > 0) ui->customPlot->yAxis->setLabel(nombres.at(cay));

      ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 10));
      ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 10));
      ui->customPlot->graph()->rescaleAxes(true);
      ui->customPlot->xAxis->setRange(minx-(maxx-minx)*0.1, maxx+(maxx-minx)*0.1);
      ui->customPlot->yAxis->setRange(miny-(maxy-miny)*0.1, maxy+(maxy-miny)*0.1);

      ui->customPlot->legend->setFont(QFont(QFont().family(), 10));
      ui->customPlot->legend->setVisible(true);
      ui->customPlot->legend->setBrush(QColor(255, 255, 255, 150));
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);  // ubicacion de la leyenda


      ui->customPlot->replot();



}

void scatterDialog::on_buttonBox_accepted()
{
    close();
}

void scatterDialog::on_cmbopc_currentIndexChanged(int index)
{
    cmbact = false;
    ui->cmb_X->clear(); ui->cmb_Y->clear();
    opc = index; //1: grafico de atributos 0: grafico de incertidumbres
    if(index) {
        foreach(auto nom, pw->nomdxatt) { ui->cmb_X->addItem(nom);ui->cmb_Y->addItem(nom);}
        graficar(pw->datdxatt,pw->dattoatt,pw->nomdxatt, 0,0); }
    else  {
        foreach(auto nom, pw->nomdxunc) { ui->cmb_X->addItem(nom);ui->cmb_Y->addItem(nom);}
        graficar(pw->datdxunc,pw->dattofut,pw->nomdxunc,0,0); }
    cmbact = true;

}

void scatterDialog::on_cmb_X_currentIndexChanged(int index)
{   if (cmbact && index != -1)  // cuando llena aveces pone -1 al incializar
        if(opc) graficar(pw->datdxatt,pw->dattoatt,pw->nomdxatt,index,ui->cmb_Y->currentIndex());
        else graficar(pw->datdxunc,pw->dattofut,pw->nomdxunc,index,ui->cmb_Y->currentIndex());
}

void scatterDialog::on_cmb_Y_currentIndexChanged(int index)
{
    if (cmbact && index != -1)  // cuando llena aveces pone -1 al incializar
        if(opc) graficar(pw->datdxatt,pw->dattoatt,pw->nomdxatt,ui->cmb_X->currentIndex(),index);
        else graficar(pw->datdxunc,pw->dattofut,pw->nomdxunc,ui->cmb_X->currentIndex(),index);

}

void scatterDialog::on_checkBox_stateChanged(int arg1)
{
  if(ui->cmb_X->currentIndex() !=-1 && ui->cmb_Y->currentIndex() != -1  )
    if(opc) graficar(pw->datdxatt,pw->dattoatt,pw->nomdxatt,ui->cmb_X->currentIndex(),ui->cmb_Y->currentIndex());
    else graficar(pw->datdxunc,pw->dattofut,pw->nomdxunc,ui->cmb_X->currentIndex(),ui->cmb_Y->currentIndex());
}

void scatterDialog::on_checkBox_clicked()
{
    qDebug() << ui->checkBox->isChecked();
}
