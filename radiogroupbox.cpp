#include "radiogroupbox.h"
#include "ui_radiogroupbox.h"
#include <QStylePainter>
RadioGroupBox::RadioGroupBox(QWidget *parent) :
    QGroupBox(parent)
{

}

void RadioGroupBox::paintEvent(QPaintEvent *)
{
       QStylePainter paint( this );
       QStyleOptionGroupBox option;
       initStyleOption( &option );
       // don't remove the original check box control, as we want to keep
       // it as a placeholder
   //  option.subControls &= ~QStyle::SC_GroupBoxCheckBox;
       paint.drawComplexControl( QStyle::CC_GroupBox, option );

       // re-use the style option, it contians enough info to make sure the
       // button is correctly checked
       option.rect = style()->subControlRect( QStyle::CC_GroupBox, &option,
               QStyle::SC_GroupBoxCheckBox, this );

       // now erase the checkbox
       paint.save();
       QPixmap px( option.rect.width(), option.rect.height() );
       px.fill( this, option.rect.left(), option.rect.top() );
       QBrush brush( px );
       paint.fillRect( option.rect, brush );
       paint.restore();

       // and replace it with a radio button
       paint.drawPrimitive( QStyle::PE_IndicatorRadioButton, option );
}

RadioGroupBox::~RadioGroupBox()
{

}
