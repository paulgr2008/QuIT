#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QtWidgets>

namespace Ui {
class LineWidget;
}

class LineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LineWidget(QLabel* label,
                        QComboBox* box,
                        QWidget *parent = 0);
    ~LineWidget();

private:
    Ui::LineWidget *ui;
};

#endif // LINEWIDGET_H
