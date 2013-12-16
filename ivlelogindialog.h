#ifndef IVLELOGINDIALOG_H
#define IVLELOGINDIALOG_H

#include <QDialog>

namespace Ui {
class IVLELoginDialog;
}

class IVLELoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IVLELoginDialog(QWidget *parent = 0);
    ~IVLELoginDialog();

private:
    Ui::IVLELoginDialog *ui;
};

#endif // IVLELOGINDIALOG_H
