#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbGo, SIGNAL(clicked()), this, SLOT(ProcessSpirograph()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ProcessSpirograph()
{
    ui->spwSpirograph->ClearMirrors();

    if(ui->cbMirror1->isChecked())
        ui->spwSpirograph->AddMirror(0, ui->sbMirror1_RPM->value());

    if(ui->cbMirror2->isChecked())
        ui->spwSpirograph->AddMirror(0, ui->sbMirror2_RPM->value());

    if(ui->cbMirror3->isChecked())
        ui->spwSpirograph->AddMirror(0, ui->sbMirror3_RPM->value());

    ui->spwSpirograph->AnimatePeriod();
}
