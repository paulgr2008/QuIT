#include "linestab.h"
#include "globalvars.h"
#define CA_TOOLS
 #include "line.h"
 Q_DECLARE_METATYPE(line_type_descr_t)

/* switches */
#define SW_DK               10
#define SW_P_ON_2           4
#define SW_ME               0
#define SW_TO               5
#define SW_HT               5
#define SW_SA               5
#define SW_UNUSED           0
#define SW_P_4_0            14
#define SW_ALL_ON           15
#define SW_INITIAL_STATE    -1
#define ON                  1
#define OFF                 0

void sortDescrList(QList<int> dev);
bool comparatorLineTypeDescr(int t1, int t2);
void insertItemToLineCombo(QList<int> dev, QComboBox* box);
enum SwichRaws {P_ROW,Q_ROW,R_ROW,S_ROW};
LinesTab::LinesTab(QMap<QString,QString> *_commandBuffer,int _devType , int _numbHVAC,
                   QWidget *parent) : QWidget(parent)
{
    /* initialize inner variables */
    commandBuffer = _commandBuffer;
    devType = _devType;
    numbHVAC =_numbHVAC;
    int _nHVACLines[]={8,5,5,4};
    nHVACLines = _nHVACLines [_devType];
    isHandlingLines = false;
    /* create layouts and containers */
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGroupBox* groupBoxLinesPage = new QGroupBox;
    groupBoxLinesPage->setAutoFillBackground(true);
    groupBoxLinesPage->setObjectName("groupBoxLinesPage");

    QHBoxLayout *mainLayoutWithColumns = new QHBoxLayout;
    QVBoxLayout* column1 = new QVBoxLayout;
    QVBoxLayout* column2 = new QVBoxLayout;

     /* Dynamicaly set line properties */
    for (int i=1; i<=nHVACLines; i++)
    {
        /* Create line frame */
        QGroupBox* lineBox;
        lineBox = new QGroupBox(this);
        lineBox->setAutoFillBackground(true);
        lineBox->setObjectName(QString("line %1").arg(i));
        lineBox->setProperty("lineFrame",true);
        lineBox->setMinimumWidth((this->width())*4.6/9);
        lineBox->setMaximumHeight(35);

        /* Create line label */
        QLabel* labelLine=new QLabel(QString("Line %1").arg(i));


        QFont font ( "Tahoma",9 );
        font.setBold(true);
        labelLine->setFont( font );
        labelLine->setStyleSheet( "color: #ffffff; padding-left:10px" );
        labelLine->setFixedWidth(80);

        /* Create line comboBox */
        QComboBox* comboBoxLine = new QComboBox;
        comboBoxLine->setEnabled(true);
        comboBoxLine->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
        //comboBoxLine->setMaximumHeight(20);
        comboBoxLine->setFont( font );
        comboBoxLine->setProperty("lineCombo",true);
        comboBoxLine->setAccessibleName( QString("L%1").arg(i) );
        connect( comboBoxLine,SIGNAL(activated(QString)),this, SLOT(handleLines(QString)) );

        /* Insert ready line into the 1st column */
        QLayout* layoutLine = new QHBoxLayout;
        layoutLine->setContentsMargins(QMargins(0,0,0,0));
        layoutLine->addWidget(labelLine);
        layoutLine->addWidget(comboBoxLine);
        lineBox->setLayout(layoutLine);
        column1->addWidget(lineBox);

        /* Add combobox to general list */
        lineCombosList.append(comboBoxLine);
    }
    mainLayoutWithColumns->addLayout(column1);

    if( devType==COOLMASTER )
    {
        column1->addStretch();
        column1->insertStretch(0);
        /* Prepare the head of switches diagram  */
        QLabel* dipSwithText = new QLabel;
        dipSwithText->setText("Required DIP Switches State");
        //dipSwithText->setFont(QFont("Neue Haas Unica Pro Medium",9 ));
        QWidget *line = new QWidget;
        line->setFixedHeight(1);
        line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line->setStyleSheet(QString("background-color: #b2bfc3;"));

        /* Add switches diagram to the 2nd column */
        column2->addStretch();
        column2->addWidget(dipSwithText);
        column2->addWidget(line);
        switchWidget = new SwitchesWidget;
        column2->addWidget(switchWidget);
        column2->addStretch();

        /* Add columns into the page and set tab layout*/

        mainLayoutWithColumns->insertSpacing(1,12);
        mainLayoutWithColumns->addLayout(column2);
    }
    else
    {
        column1->addStretch();
        column1->insertStretch(0);
        mainLayoutWithColumns->addStretch();
        mainLayoutWithColumns->insertStretch(0);
    }

    groupBoxLinesPage->setLayout(mainLayoutWithColumns);
    mainLayout->addWidget(groupBoxLinesPage);
    setLayout(mainLayout);
    setLinesInitialState();
}

/*
 * Handling with lines presentation
*/
void LinesTab::handleLines( QString str )
{
    QComboBox* senderBox = dynamic_cast<QComboBox*>( sender() );
    QString strLine = senderBox->accessibleName();
    comboBoxFilter();
    isHandlingLines = true;
    if( devType == COOLMASTER )
        createDipSwitches();

    /* send command */
    if(str.contains("---"))
        str = "Unused";
    else
    {
        QStringList strList = str.split("\t");
        str = strList[1].remove("(");
        str.remove(")");
    }
    commandBuffer->insert( "line type "+strLine+" ", str );
    emit bufferFilled(true);

}


/*
 * Devices list filtering
*/
void LinesTab::comboBoxFilter()
{
    int countGRTypes      = 0;
    int countUsedTypes    = 0;
    bool hasGreeLine = false;
    uint8_t dev;
    uint8_t selectedDevice;

    for( int i=0; i<lineCombosList.size(); i++ )
    {
        QComboBox* box = lineCombosList.at(i);
        //QListView* listVw = qobject_cast<QListView*>( box->view() );
        //listVw->setRowHidden(i,true);
        selectedDevice = line_types[(box->currentData()).value<int>()].ac_type;

        if ( (selectedDevice == LINE_AC_TYPE_UNUSED) || (selectedDevice == LINE_AC_TYPE_PBM)||
             (selectedDevice == LINE_AC_TYPE_SI))
            continue;

        if( selectedDevice==LINE_AC_TYPE_GR  )
        {
            countGRTypes++;
            if ( !hasGreeLine )
            {
                countUsedTypes++;
                hasGreeLine = true;
                continue;
            }
        }
        else countUsedTypes++;
    }

    if( countUsedTypes >= numbHVAC || countGRTypes>3 )
    {
        foreach( QComboBox* box, lineCombosList )
        {
            QListView* listVw = qobject_cast<QListView*>( box->view() );
            selectedDevice = line_types[(box->currentData()).value<int>()].ac_type;

            //qDebug()<<box->currentText();
            //qDebug()<<box->accessibleName()<<" "<<"items: "<<box->count();
            box->setMaxVisibleItems(box->count()+100);

            if( (selectedDevice == LINE_AC_TYPE_UNUSED) ||
                (selectedDevice == LINE_AC_TYPE_PBM)||
                (selectedDevice == LINE_AC_TYPE_SI) ||
                (( selectedDevice == LINE_AC_TYPE_GR ) && (countGRTypes>3)) )
            {
                for( int i=0; i<box->count(); i++ )
                {
                    dev = line_types[box->itemData( i,Qt::UserRole ).value<int>()].ac_type;
                    if ( (dev != LINE_AC_TYPE_UNUSED) && (dev != LINE_AC_TYPE_PBM) &&
                         (dev != LINE_AC_TYPE_SI))
                    {
                        if ( (dev != LINE_AC_TYPE_GR))
                            listVw->setRowHidden( i,true );
                        if((dev == LINE_AC_TYPE_GR))
                            if ((countGRTypes>3)||(!hasGreeLine) )
                                listVw->setRowHidden( i,true );
                    }
                }
            }
            else if ( (selectedDevice != LINE_AC_TYPE_UNUSED) && (selectedDevice != LINE_AC_TYPE_PBM) &&
                      (selectedDevice != LINE_AC_TYPE_SI)&&(selectedDevice!=LINE_AC_TYPE_GR) && (countGRTypes>3) )
                for( int i=0; i<box->count(); i++ )
                {
                    dev = line_types[box->itemData( i,Qt::UserRole ).value<int>()].ac_type;
                    if ( (dev == LINE_AC_TYPE_GR) )
                            listVw->setRowHidden( i,true );
                }
         }

    }
    else
    {
        foreach( QComboBox* box, lineCombosList )
        {
            if( !box->isEnabled() )
            {
                    box->setEnabled(true);
            }
            for( int i=0; i<box->count(); i++ )
            {
                 QListView* listView = qobject_cast<QListView*>( box->view() );
                 listView->setRowHidden( i,false );
            }
        }
    }
    if ( !(lineCombosList.at(0)->currentText().contains( "---",Qt::CaseInsensitive )) )
            lineCombosList.at(4)->setEnabled(false);
    else
        lineCombosList.at(4)->setEnabled(true);

    if ( !(lineCombosList.at(4)->currentText().contains( "---",Qt::CaseInsensitive )) )
        lineCombosList.at(0)->setEnabled(false);
    else
        lineCombosList.at(0)->setEnabled(true);
}

/*
 * Set lines combobox items
*/
void LinesTab::setLinesInitialState()
{
    int offset = 0;
    uint32_t lineType ;
    foreach( QComboBox* box, lineCombosList )
        box->clear();

    if ( devType == 1 )
        offset = 8;
    else if ( devType== 3 )
        offset = 16;

    for( int line = 0; line<nHVACLines; line++ )
    {
        QList<int> devLine;
        QList<int> devNonline;
        for( int i=0; i<static_cast<int>( sizeof( line_types )
                                      /sizeof( line_type_descr_t ) );i++ )
        {
            lineType = line_types[i].lines>>offset;
            if ( (lineType&( 1<<line ))!=0 )
            {
                if( QString(line_types[i].func_str).compare("BIST") )
                {
                    if ( line_types[i].ac_type ==LINE_AC_TYPE_UNUSED ||
                         line_types[i].ac_type ==LINE_AC_TYPE_PBM ||
                         line_types[i].ac_type == LINE_AC_TYPE_SI)
                    {
                       devNonline.append(i);
                    }
                    else
                    {
                       devLine.append(i) ;
                    }
                }
            }
        }
       sortDescrList(devLine);
       sortDescrList(devNonline);

       insertItemToLineCombo(devLine,lineCombosList.at( line ));
       lineCombosList.at( line )->insertSeparator( devLine.count() );
       insertItemToLineCombo(devNonline, lineCombosList.at( line ));
    }
}
void insertItemToLineCombo(QList<int> dev, QComboBox* box)
{
    foreach(int t, dev)
    {
        QString nameStr;
        QVariant var = QVariant::fromValue( t );

        if( QString(line_types[t].func_str).contains("Unused",Qt::CaseInsensitive))
            nameStr =  QString("---");
        else
            nameStr = QString(line_types[t].func_str)+"\t("+QString(line_types[t].ac_type_str)+")";
        box->addItem( nameStr,var );
    }
}

void sortDescrList(QList<int> dev)
{
    qSort( dev.begin(),dev.end(),comparatorLineTypeDescr );
}

bool comparatorLineTypeDescr(int t1, int t2)
{
    return strcmp(line_types[t1].ac_type_str,line_types[t2].ac_type_str);
}

/*
 * sort combobox list by device name
*/
void LinesTab::sortItems()
{
    foreach( QComboBox* box, lineCombosList )
    {
        if(box->count() < 2)
            continue;

        QStandardItemModel* model =
                dynamic_cast<QStandardItemModel*>(box->model());
            model->sort(0);
    }
}

/*
 * Set lines settings
 */
void LinesTab::fillLinesForm( QList<QStringList> rows )
{
    QStringList lineProperty;
    int         nLine;

    for( int i=0;i<rows.size();i=i+2 )
    {
        QString strKey = rows[i].at(0);
        strKey = strKey.trimmed();
        nLine = strKey.data()[1].digitValue(); /*set line number*/
        lineProperty = rows[i].at(1).split( " " );
        QString prop (lineProperty.at(0));
        if(prop.contains("Unused"))
            prop = "---";
        selectedDevices.append(prop);
        lineCombosList.at( nLine-1 )->setCurrentIndex( lineCombosList.at( nLine-1 )
                        ->findText( prop,Qt::MatchContains ) );
    }
    comboBoxFilter();
    if( devType==COOLMASTER )
        createDipSwitches();
}

/* -----------------------------------------------------------------------------
 * Operations with dip switches images
 * -----------------------------------------------------------------------------
*/
void LinesTab::createDipSwitches()
{
    line_type_descr_t descr;
    QVariant var;
    QComboBox* box;
    int code[4];

    /*P switches*/
    if ( lineCombosList.at(1)->currentText().contains( "---",Qt::CaseInsensitive )//L2 disabled
         && (!lineCombosList.at(5)->currentText().contains( "---",Qt::CaseInsensitive )))
        code[P_ROW] = SW_ALL_ON;
    else code[P_ROW] = SW_P_4_0;
    switchWidget->switchesHandler( P_ROW,code[P_ROW] );




    /* Q switches*/
    box = lineCombosList.at(0);
    var = box->currentData();
    descr = line_types[var.value<int>()];
    switch( descr.ac_type )
    {
    case LINE_AC_TYPE_DK:
        code[Q_ROW] = SW_DK;
        break;
    case LINE_AC_TYPE_ME:
        code[Q_ROW] = SW_ME;
        break;
    case LINE_AC_TYPE_TO:
        code[Q_ROW] = SW_TO;
        break;
    case LINE_AC_TYPE_SA:
        code[Q_ROW] = SW_SA;
        break;
    case LINE_AC_TYPE_HT:
        code[Q_ROW] = SW_HT;
        break;
    default:
        code[Q_ROW] = SW_UNUSED;
        break;
    }
    switchWidget->switchesHandler( Q_ROW,code[Q_ROW] );

    /* R switches*/
    box = lineCombosList.at(1);
    var = box->currentData();
    descr = line_types[var.value<int>()];
    switch( descr.ac_type )
    {
        case LINE_AC_TYPE_DK:
            code[R_ROW] = SW_DK;
            break;
        case LINE_AC_TYPE_ME:
            code[R_ROW] = SW_ME;
            break;
        case LINE_AC_TYPE_TO:
            code[R_ROW] = SW_TO;
            break;
        case LINE_AC_TYPE_SA:
            code[R_ROW] = SW_SA;
            break;
        case LINE_AC_TYPE_HT:
            code[R_ROW] = SW_HT;
            break;
        default:
            code[R_ROW] = SW_UNUSED;
            break;
    }
    switchWidget->switchesHandler( R_ROW,code[R_ROW] );

    /*S switches*/
    code[S_ROW] = 0;
    switchWidget->switchesHandler( S_ROW, code[S_ROW] );

    if( isHandlingLines )
    {
        int* st = switchWidget->getPrevState();
        for (int i=0; i<4; i++)
        {
            switchWidget->highlightChangedLabel(i,code[i] != *(st+i)?true:false);
            switchWidget->highlightChangedSwitchNumber(i,code[i]);
        }
        isHandlingLines = false;
    }
    else
    {   for (int i=0; i<4; i++)
        {
            switchWidget->setPrevState(i,code[i]);
        }
        switchWidget->switchNumbersToInitialState();
    }
}

void LinesTab::switchNumbersToInitialState()
{
    if (devType == COOLMASTER )
        switchWidget->switchNumbersToInitialState();
}

