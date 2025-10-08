#ifndef IMPORTSTUDENTCOURSESDIALOG_H
#define IMPORTSTUDENTCOURSESDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ImportStudentCoursesDialog;
}

class ImportStudentCoursesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportStudentCoursesDialog(QWidget *parent = nullptr);
    ~ImportStudentCoursesDialog();
    
    QString getFilePath() const;

private slots:
    void onBrowseClicked();

private:
    Ui::ImportStudentCoursesDialog *ui;
};

#endif // IMPORTSTUDENTCOURSESDIALOG_H
