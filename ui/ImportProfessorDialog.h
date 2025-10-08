#ifndef IMPORTPROFESSORDIALOG_H
#define IMPORTPROFESSORDIALOG_H

#include <QDialog>

namespace Ui {
class ImportProfessorDialog;
}

class ImportProfessorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ImportProfessorDialog(QWidget* parent = nullptr);
    ~ImportProfessorDialog();

    QString getFilePath() const;

private slots:
    void onBrowseClicked();

private:
    Ui::ImportProfessorDialog *ui;
};

#endif // IMPORTPROFESSORDIALOG_H
