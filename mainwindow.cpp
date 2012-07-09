#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmirrorvalues.h"
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), currMirrorItem(NULL), currMirrorValues(NULL)
{
    ui->setupUi(this);
    ui->lwMirrorList->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->dsbGranularity->setValue(ui->spwSpirograph->GetGranularity());

    connect(ui->tbAdd, SIGNAL(clicked()),
            this, SLOT(AddMirror()));
    connect(ui->tbRemove, SIGNAL(clicked()),
            this, SLOT(RemoveMirror()));
    connect(ui->lwMirrorList, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(ChangeMirrorSelection(QListWidgetItem*,QListWidgetItem*)));
    connect(ui->dsbGranularity, SIGNAL(valueChanged(double)),
            ui->spwSpirograph, SLOT(SetGranularity(double)));

    connect(ui->sbRatio, SIGNAL(valueChanged(int)),
            this, SLOT(ChangeRatio(int)));
    connect(ui->dsbOffset, SIGNAL(valueChanged(double)),
            this, SLOT(ChangeOffset(double)));

    connect(ui->pbGo, SIGNAL(clicked()),
            this, SLOT(ProcessSpirograph()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddMirror()
{
    QString str = mirrorName(ui->sbRatio->value(), ui->dsbOffset->value());
    QListWidgetItem *item = new QListWidgetItem(str, ui->lwMirrorList);

    QMirrorValues *mirror = new QMirrorValues(ui->sbRatio->value(), ui->dsbOffset->value());

    currMirrorItem = item;
    currMirrorValues = mirror;

    ui->spwSpirograph->AddMirror(mirror);
    item->setData(Qt::UserRole, QVariant::fromValue(mirror));
    item->setSelected(true);

}

void MainWindow::RemoveMirror()
{
    QList<QListWidgetItem*> items = ui->lwMirrorList->selectedItems();

    for(QList<QListWidgetItem*>::iterator itr = items.begin(); itr != items.end(); itr++)
    {
        if(currMirrorItem == *itr)
        {
            currMirrorItem = NULL;
            currMirrorValues = NULL;
        }
        ui->spwSpirograph->RemoveMirror((*itr)->data(Qt::UserRole).value<QMirrorValues*>());
        delete *itr;
    }
}

void MainWindow::ProcessSpirograph()
{
    //ui->spwSpirograph->ClearMirrors();

    if(ui->cbAnimate->isChecked())
        ui->spwSpirograph->AnimatePeriod();
    else
        ui->spwSpirograph->NoAnimateUpdate();
}

void MainWindow::ChangeMirrorSelection(QListWidgetItem *newItem, QListWidgetItem *)
{
    currMirrorItem = newItem;
    if(newItem == NULL)
        currMirrorValues = NULL;
    else
    {
        currMirrorValues = newItem->data(Qt::UserRole).value<QMirrorValues*>();
        ui->sbRatio->setValue(currMirrorValues->GetRatio());
        ui->dsbOffset->setValue(currMirrorValues->GetOffset());
    }
}

void MainWindow::ChangeRatio(int newRatio)
{
    if(currMirrorItem == NULL)
        return;

    currMirrorValues->SetRatio(newRatio);
    currMirrorItem->setText(mirrorName(currMirrorValues->GetRatio(), currMirrorValues->GetOffset()));
}

void MainWindow::ChangeOffset(double newOffset)
{
    if(currMirrorItem == NULL)
        return;

    currMirrorValues->SetOffset(newOffset);
    currMirrorItem->setText(mirrorName(currMirrorValues->GetRatio(), currMirrorValues->GetOffset()));
}

QString MainWindow::mirrorName(int ratio, double offset)
{
    QString str("Ratio: ");
    str.append(QString::number(ratio));
    str.append(", Offset: ");
    str.append(QString::number(offset));
    return str;
}
