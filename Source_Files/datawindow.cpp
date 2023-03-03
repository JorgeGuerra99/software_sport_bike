#include "Header_Files/datawindow.h"
#include "ui_datawindow.h"

datawindow::datawindow(Session*& mySes, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::datawindow)
{
    ses = mySes;
    ui->setupUi( this );
    connect ( ui->pushButton_2, SIGNAL( clicked() ), this, SLOT( deleteLater() ) );
    connect ( ui->pushButton, SIGNAL( clicked() ), this, SLOT( ExportDataButton() ) );

    vector < double > velVect = mySes->GetAllData( 'V' );
    vector < double > pulVect = mySes->GetAllData( 'P' );
    vector < double > loadVect = mySes->GetAllData( 'L' );
    QVector < double > velocity ( velVect.begin(), velVect.end() );
    QVector < double > pulse ( pulVect.begin(), pulVect.end() );
    QVector < double > load ( loadVect.begin(), loadVect.end() );
    QVector < double > time ( mySes->GetAllData().size() );

    for ( int i = 0; i< ( int ) mySes->GetAllData().size(); i++ )
    {
        time[ i ] = i;
    }
    if ( time.size() != velocity.size() ) cout << "ERROR en los tamaños. El de time es: " << time.size() << "y el de vel es: " << velocity.size()<< endl;
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph( 0 )->setData( time, velocity );
    // give the axes some labels:
    ui->customPlot->xAxis->setLabel( "x" );
    ui->customPlot->yAxis->setLabel( "y" );
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange( 0, time.back() );
    ui->customPlot->yAxis->setRange( 0, 100 );
    ui->customPlot->replot();

    // create graph and assign data to it:
    ui->customPlot_2->addGraph();
    ui->customPlot_2->graph( 0 )->setData( time, pulse );
    // give the axes some labels:
    ui->customPlot_2->xAxis->setLabel( "Tiempo" );
    ui->customPlot_2->yAxis->setLabel( "Pulso" );
    // set axes ranges, so we see all data:
    ui->customPlot_2->xAxis->setRange( 0, time.back() );
    ui->customPlot_2->yAxis->setRange( 0, 100 );
    ui->customPlot_2->replot();

    // create graph and assign data to it:
    ui->customPlot_3->addGraph();
    ui->customPlot_3->graph( 0 )->setData( time, load );
    // give the axes some labels:
    ui->customPlot_3->xAxis->setLabel( "Tiempo" );
    ui->customPlot_3->yAxis->setLabel( "Carga" );
    // set axes ranges, so we see all data:
    ui->customPlot_3->xAxis->setRange( 0, time.back() );
    ui->customPlot_3->yAxis->setRange( 0, 10 );
    ui->customPlot_3->replot();

    int sec = mySes->timeSes % 60;
    int min = mySes->timeSes / 60;
    QString labTime = QString::number( min ) + " min. " + QString::number( sec ) + "seg.";
    ui->label_6->setText( labTime );
    ui->label_7->setText( QString::number( mySes->GetDistance() ) + " km" );
    ui->label_8->setText( QString::number( mySes->GetVelocMaxMed() ) );
    ui->label_9->setText( QString::number( mySes->GetVelocMaxMed( 1 ) ) );
    ui->label_10->setText( QString::number( mySes->GetCalories() ) );

}

datawindow::~datawindow()
{
    delete ui;
}

void datawindow::ExportDataButton()
{
    ses->WriteReport();
    diaWin = new DialogWin( "El archivo ha sido exportado con éxito" );
    diaWin->show();
}
