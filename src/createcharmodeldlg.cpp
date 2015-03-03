////////
// This sample is published as part of the blog article at www.toptal.com/blog
// Visit www.toptal.com/blog and subscribe to our newsletter to read great posts
////////

#include "../include/createcharmodeldlg.h"
#include "../include/model3d.h"
#include "../include/model_creator.h"

#include "../include/model2d_processing.h"
#include "ui_createcharmodeldlg.h"

#include <QApplication>
#include <QFontDialog>
#include <QBitmap>
#include <QFileDialog>

CreateCharModelDlg::CreateCharModelDlg(QWidget *parent) :
    QDialog(parent),
    m_font(qApp->font()),
    m_labelPicture(0),
    m_bSaveOnClose(true),
    m_bOpenOnClose(false),
    ui(new Ui::CreateCharModelDlg)
{
    ui->setupUi(this);

    ui->chkSave->setChecked(true);

    m_labelPicture = new QLabel(ui->scrollArea);
    ui->scrollArea->setWidget(m_labelPicture);

    connect(ui->pbSelectFont, SIGNAL(clicked()),
             this, SLOT(OnSelectFont()));
    connect(ui->leText, SIGNAL(textChanged(const QString&)),
             this, SLOT(RefreshImage()));
    connect(ui->cbHeight, SIGNAL(currentIndexChanged(int)),
             this, SLOT(RefreshImage()));

    connect(ui->chkSave, SIGNAL(stateChanged(int)), this,
        SLOT(UpdateOkButton()));
    connect(ui->chkOpen, SIGNAL(stateChanged(int)), this,
        SLOT(UpdateOkButton()));

    connect(ui->pbCreateModel, SIGNAL(clicked()),
             this, SLOT(OnCreateModel()));
    connect(ui->pbClose, SIGNAL(clicked()),
             this, SLOT(reject()));

    RefreshImage();
}

CreateCharModelDlg::~CreateCharModelDlg()
{
    delete ui;
}

void CreateCharModelDlg::OnSelectFont()
{
    QFontDialog dlgFont(this);

    if(QDialog::Accepted == dlgFont.exec())
    {
        m_font = dlgFont.currentFont();
        RefreshImage();
    }
}

void CreateCharModelDlg::RefreshImage()
{
    ui->labelFontSelected->setText(
        m_font.family() +
        QString::fromUtf8(", %1").arg(m_font.pointSize()));

    QString sText(ui->leText->text().trimmed());
    if(sText.isEmpty())
    {
        m_labelPicture->setPixmap(QPixmap());
        m_image = QImage();
        UpdateOkButton();
        return;
    }

    unsigned short dimension = ui->cbHeight->currentText().toUShort();

    Model2DProcessing::Create2DImage(m_image, sText, m_font, dimension);

    QPixmap pixmap;
    pixmap.convertFromImage(m_image, Qt::MonoOnly);
    pixmap.setMask(pixmap.createMaskFromColor(QColor(Qt::white), Qt::MaskInColor));
    m_labelPicture->setPixmap(pixmap);

    UpdateOkButton();
}

void CreateCharModelDlg::UpdateOkButton()
{
    ui->pbCreateModel->setEnabled((m_image.width() > 0) && (m_image.height() > 0) &&
        (ui->chkSave->isChecked() || ui->chkOpen->isChecked()));
    m_bSaveOnClose = ui->chkSave->isChecked();
    m_bOpenOnClose = ui->chkOpen->isChecked();
}

void CreateCharModelDlg::OnCreateModel()
{
    if((m_image.width() <= 0) || (m_image.height() <= 0) ||
        (!ui->chkSave->isChecked() && !ui->chkOpen->isChecked()))
    {
        UpdateOkButton();
        return;
    }

    if(ui->chkSave->isChecked())
    {
        const QString filtersAll(QString::fromUtf8(
            "Model files (*.mff);;All files (*.*)"));
        QString filterSel(QString::fromUtf8(
            "Model files (*.mff)"));
        m_sFilePath = QFileDialog::getSaveFileName(this,
            QString::fromUtf8("Save 3D model"),
            qApp->applicationDirPath(),
            filtersAll, &filterSel);
        if(m_sFilePath.isEmpty())
        {
            return;
        }
    }

    ModelCreator creator(m_image, m_model);
    creator.Create3DModel();

    accept();
}
