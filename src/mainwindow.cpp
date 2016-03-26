#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("BeerPong");

    // create controller
    controller = new Controller();
    QObject::connect(controller, SIGNAL(ControllerFeedback(float, float)), this, SLOT(UpdateFeedback(float, float)));

    // create player
    player = new Player();
    QObject::connect(player, SIGNAL(ProcessedDepth(QImage)), this, SLOT(UpdateDepth(QImage)));
    QObject::connect(player, SIGNAL(ProcessedColor(QImage)), this, SLOT(UpdateColor(QImage)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateFeedback(float trans, float speed)
{

}

void MainWindow::UpdateDepth(const QImage image)
{

}

void MainWindow::UpdateColor(const QImage image)
{

}
