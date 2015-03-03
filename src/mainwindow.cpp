////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#include "../include/mainwindow.h"
#include "../include/createcharmodeldlg.h"

#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(triggered()),
        this, SLOT(OnNewModel()));
    connect(ui->actionOpen, SIGNAL(triggered()),
        this, SLOT(OnOpenModel()));
    connect(ui->actionClose, SIGNAL(triggered()),
        this, SLOT(OnCloseModel()));
    connect(ui->actionExit, SIGNAL(triggered()),
        this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnNewModel()
{
    CreateCharModelDlg dlg(this);

    if(QDialog::Accepted == dlg.exec())
    {
        if(dlg.SaveOnClose())
        {
            dlg.GetModel().Save(dlg.GetPath());
        }
        if(dlg.OpenOnClose())
        {
            m_sFilePath.clear();
            if(dlg.SaveOnClose())
            {
                m_sFilePath = dlg.GetPath();
            }
            setWindowTitle(m_sFilePath);
            ui->openGLWidget->SetModel(dlg.GetModel());
        }
    }
}

void MainWindow::OnOpenModel()
{
    const QString filtersAll(QString::fromUtf8(
        "Model files (*.mff);;All files (*.*)"));
    QString filterSel(QString::fromUtf8(
        "Model files (*.mff)"));
    const QString sFilePath = QFileDialog::getOpenFileName(this,
        QString::fromUtf8("Open 3D model"),
        qApp->applicationDirPath(),
        filtersAll, &filterSel);
    if(sFilePath.isEmpty())
    {
        return;
    }

    Model3D model;
    if(model.Load(sFilePath))
    {
        OnCloseModel();

        m_sFilePath = sFilePath;

        setWindowTitle(m_sFilePath);
        ui->openGLWidget->SetModel(model);
    }
}

void MainWindow::OnCloseModel()
{
    Model3D model;
    ui->openGLWidget->SetModel(model);

    m_sFilePath.clear();
    setWindowTitle(QString());
}
