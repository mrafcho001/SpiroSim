#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QListWidgetItem;
class QMirrorValues;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void AddMirror();
    void RemoveMirror();
    void ProcessSpirograph();
    void ChangeMirrorSelection(QListWidgetItem* newItem, QListWidgetItem*);
    
    void ChangeRatio(int newRatio);
    void ChangeOffset(double newOffset);

private:
    QString mirrorName(int ratio, double offset);
    Ui::MainWindow *ui;
    QListWidgetItem *currMirrorItem;
    QMirrorValues *currMirrorValues;
};

#endif // MAINWINDOW_H
