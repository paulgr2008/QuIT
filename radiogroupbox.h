#ifndef RADIOGROUPBOX_H
#define RADIOGROUPBOX_H

#include <QGroupBox>

namespace Ui {
class RadioGroupBox;
}

class RadioGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    explicit RadioGroupBox(QWidget *parent = 0);
    ~RadioGroupBox();

protected:
     void paintEvent(QPaintEvent*);
};

#endif // RADIOGROUPBOX_H
