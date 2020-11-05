#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = nullptr);
    ~ProgressDialog();

    void setUp(qint64 size);
    void progress(qint64 size);
    void tearDown();

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
