#include "ImportStudentCoursesDialog.h"
#include "ui_ImportStudentCoursesDialog.h"
#include <QFileDialog>
#include <QMessageBox>

ImportStudentCoursesDialog::ImportStudentCoursesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ImportStudentCoursesDialog)
{
    ui->setupUi(this);
}

ImportStudentCoursesDialog::~ImportStudentCoursesDialog()
{
    delete ui;
}

QString ImportStudentCoursesDialog::getFilePath() const
{
    return ui->filePathEdit->text();
}

void ImportStudentCoursesDialog::onBrowseClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        QString::fromUtf8("选择学生课程CSV文件"),
        QString(),
        QString::fromUtf8("CSV文件 (*.csv);;所有文件 (*.*)")
    );
    
    if (!fileName.isEmpty()) {
        ui->filePathEdit->setText(fileName);
    }
}



