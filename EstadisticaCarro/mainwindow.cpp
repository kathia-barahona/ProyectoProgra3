#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_gasfactura.h"
#include "gasfactura.h"
#include "carro.h"
#include "factura.h"
#include <sstream>
#include <QMessageBox>
#include <QList>
#include <iostream>
#include <QCloseEvent>
#include <QFileDialog>
#include <QFile>
#include <ostream>
#include <fstream>

using namespace std;
MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    Carro carro;
    QString archivo = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Files (*.kat)"));
    if(archivo!=""){
        ifstream entrada;
        entrada.open(archivo.toStdString().c_str(),ios::binary|ios::in);
        if(entrada.good())
             entrada.read(reinterpret_cast<char*>(&carro),sizeof(Carro));

        /*while(!entrada.eof() && veces <1){
            veces++;
            cout<<"hola3"<<endl;
            //entrada.read((char*)&*carro,sizeof(carro));
            //cout<<carro->getPlaca().toStdString()<<endl;
            if(!entrada.eof()){
             *    ifstream fentrada("prueba.dat",
      ios::in | ios::binary);

   fentrada.read(reinterpret_cast<char *>(&pepe2),
      sizeof(tipoRegistro));
            cout<<carro.getPlaca().toStdString()<<endl;
        }

        }*/


        entrada.close();
    }

    ui->treeWidget->setColumnCount(3);
    treeItem = new QTreeWidgetItem(ui->treeWidget);
    treeItem->setText(0,"Placa");
    treeItem->setText(1,"Marca");
    treeItem->setText(2,"Cilindraje");



}
void MainWindow:: closeEvent(QCloseEvent *event){
    QMessageBox mb;
    mb.setText("Desea guardar?");

    mb.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    mb.setDefaultButton(QMessageBox::Yes);
    int result = mb.exec();
    if(result==QMessageBox::Yes){
        QString archivo = QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Files (*.kat)"));
        if(archivo!=""){
            ofstream salida;
            salida.open(archivo.toStdString().c_str(),ios::binary|ios::app);
            for(int i=0;i<carros.size();i++){
                salida<<*carros[i];
            }

            salida.close();}


    }
    event->accept();




}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    Carro *c=new Carro(ui->lt_placa->text(),ui->lt_marca->text(),(ui->lt_cilindraje->text()).toInt(),(ui->lt_gasmax->text()).toInt());
    ui->lt_placa->setText("");
    ui->lt_marca->setText("");
    ui->lt_cilindraje->setText("");
    ui->lt_gasmax->setText("");
    carros.append(c);
    QTreeWidgetItem *treeItem2=new QTreeWidgetItem();
    treeItem2->setText(0,c->getPlaca());
    treeItem2->setText(1,c->getMarca());
    treeItem2->setText(2,QString::number(c->getCilindraje()));
    treeItem->addChild(treeItem2);
    ui->listWidget->addItem(new QListWidgetItem(c->getPlaca()));



}




void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item)
{

    int indice;
    bool bandera=false;

    for(int i=0;i<carros.size();i++){
        if(item->text(0)==carros[i]->getPlaca() && item->text(1)==carros[i]->getMarca()&&item->text(2).toInt()==carros[i]->getCilindraje()){
            indice=i;
            bandera=true;
            break;
        }
    }
    if(bandera){
        GasFactura gasfactura ;
        gasfactura.setCarro(carros[indice]);
        gasfactura.setModal(true);
        gasfactura.exec();
    }

}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int indice;
    bool bandera=false;
    for(int i=0;i<carros.size();i++){
        if(item->text()==carros[i]->getPlaca()){
            indice=i;
            bandera=true;
            break;
        }
    }
    if(bandera){
        stringstream ss;
        ss<<"Promedio km/L: "<<carros[indice]->promediokml()<<"\nPromedio km/G: "<<carros[indice]->promediokmg()
         <<"\nPromedio Lemp/km: "<<carros[indice]->promediolempkm()<<"\nPromedio Lemp/dia: "<<carros[indice]->promediolempdia();
        ui->tf_promedios->setText(ss.str().c_str());
    }
}
