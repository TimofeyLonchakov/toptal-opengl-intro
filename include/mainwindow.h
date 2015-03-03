#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void OnNewModel();
    void OnOpenModel();
    void OnCloseModel();

private:
    QString m_sFilePath;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
