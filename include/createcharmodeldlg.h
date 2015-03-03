////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#ifndef CREATECHARMODELDLG_H
#define CREATECHARMODELDLG_H

#include "model3d.h"

#include <QDialog>
#include <QFont>
#include <QImage>
#include <QLabel>

namespace Ui {
class CreateCharModelDlg;
}

class CreateCharModelDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CreateCharModelDlg(QWidget *parent = 0);
    ~CreateCharModelDlg();

public:
    Model3D& GetModel() {   return m_model; };
    bool SaveOnClose() const        {   return m_bSaveOnClose; };
    bool OpenOnClose() const        {   return m_bOpenOnClose; };
    const QString& GetPath() const  {   return m_sFilePath;     };

private slots:
    void OnSelectFont();
    void RefreshImage();
    void UpdateOkButton();
    void OnCreateModel();

private:
    QFont   m_font;
    QImage  m_image;
    QLabel* m_labelPicture;
    Model3D m_model;

private:
    bool    m_bSaveOnClose;
    bool    m_bOpenOnClose;
    QString m_sFilePath;

private:
    Ui::CreateCharModelDlg *ui;
};

#endif // CREATECHARMODELDLG_H
