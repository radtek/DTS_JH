#include "DialogConfirm.h"
#include "ui_DialogConfirm.h"

DialogConfirm::DialogConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfirm)
{
    ui->setupUi(this);
}

DialogConfirm::~DialogConfirm()
{
    delete ui;
}
