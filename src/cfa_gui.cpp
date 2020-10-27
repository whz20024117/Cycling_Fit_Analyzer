#include "cfa_gui_plot.hpp"
#include <QtWidgets/QApplication>
#include "ui_cfa_gui_window.h"


CfaGUI::CfaGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CfaGUI)
{
    ui->setupUi(this);

}

CfaGUI::~CfaGUI()
{
    delete ui;
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CfaGUI w;
    w.show();

    return a.exec();
}
