#ifndef CfaGUI_H
#define CfaGUI_H

#include <QtWidgets/QMainWindow>
#include <QtCore>

namespace Ui {
class CfaGUI;
}

class CfaGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit CfaGUI(QWidget *parent = 0);
    ~CfaGUI();

private:
    Ui::CfaGUI *ui;
};

#endif // CfaGUI_H


#include <vector>
#include <stdexcept>

#include "qcustomplot/qcustomplot.h"
