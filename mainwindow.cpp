#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pbGo, SIGNAL(clicked()), this, SLOT(ProcessSpirograph()));
    connect(ui->pbIncGran, SIGNAL(clicked()), ui->spwSpirograph, SLOT(IncreaseGranularity()));
    connect(ui->pbDecGran, SIGNAL(clicked()), ui->spwSpirograph, SLOT(DecreaseGranularity()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ProcessSpirograph()
{
    ui->spwSpirograph->ClearMirrors();

    if(ui->cbMirror1->isChecked() && ui->sbMirror1_ratio->value() > 0)
        ui->spwSpirograph->AddMirror(0, ui->sbMirror1_ratio->value());

    if(ui->cbMirror2->isChecked() && ui->sbMirror2_ratio->value() > 0)
        ui->spwSpirograph->AddMirror(0, ui->sbMirror2_ratio->value());

    if(ui->cbMirror3->isChecked() && ui->sbMirror3_ratio->value() > 0)
        ui->spwSpirograph->AddMirror(0, ui->sbMirror3_ratio->value());

    if(ui->cbAnimate->isChecked())
        ui->spwSpirograph->AnimatePeriod();
    else
        ui->spwSpirograph->NoAnimateUpdate();
}
