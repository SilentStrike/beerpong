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
    QObject::connect(player, SIGNAL(ProcessedVideo(QImage)), this, SLOT(UpdateVideo(QImage)));
    QObject::connect(player, SIGNAL(ProcessedDepth(QImage)), this, SLOT(UpdateDepth(QImage)));

    // initialize values
    // depth
    player->SetMinDepth(ui->minDepthSlider->value());
    ui->minDepthValue->setText(QString::number(ui->minDepthSlider->value()));
    player->SetMaxDepth(ui->maxDepthSlider->value());
    ui->maxDepthValue->setText(QString::number(ui->maxDepthSlider->value()));

    // hsv
    player->SetHLo(ui->hueLoSlider->value());
    ui->hueLoValue->setText(QString::number(ui->hueLoSlider->value()));
    player->SetHHi(ui->hueHiSlider->value());
    ui->hueHiValue->setText(QString::number(ui->hueHiSlider->value()));

    player->SetSLo(ui->satLoSlider->value());
    ui->satLoValue->setText(QString::number(ui->satLoSlider->value()));
    player->SetSHi(ui->satHiSlider->value());
    ui->satHiValue->setText(QString::number(ui->satHiSlider->value()));

    player->SetVLo(ui->valLoSlider->value());
    ui->valLoValue->setText(QString::number(ui->valLoSlider->value()));
    player->SetVHi(ui->valHiSlider->value());
    ui->valHiValue->setText(QString::number(ui->valHiSlider->value()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateFeedback(float trans, float speed)
{

}

void MainWindow::UpdateVideo(const QImage image)
{
    foreach (QLabel *l, findChildren<QLabel*>())
    {
        if (l->objectName().toStdString().find("colorVideo") ==  0)
        {
            l->setAlignment(((Qt::AlignCenter)));
            l->setPixmap(QPixmap::fromImage(image).scaled(l->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        }
    }
}

void MainWindow::UpdateDepth(const QImage image)
{
    foreach (QLabel *l, findChildren<QLabel*>())
    {
        if (l->objectName().toStdString().find("depthVideo") ==  0)
        {
            l->setAlignment(((Qt::AlignCenter)));
            l->setPixmap(QPixmap::fromImage(image).scaled(l->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
        }
    }
}

void MainWindow::on_minDepthSlider_valueChanged(int value)
{
    player->SetMinDepth(value);
    ui->minDepthValue->setText(QString::number(value));
}

void MainWindow::on_maxDepthSlider_valueChanged(int value)
{
    player->SetMaxDepth(value);
    ui->maxDepthValue->setText(QString::number(value));
}

void MainWindow::on_hueLoSlider_valueChanged(int value)
{
    player->SetHLo(value);
    ui->hueLoValue->setText(QString::number(value));
}

void MainWindow::on_hueHiSlider_valueChanged(int value)
{
    player->SetHHi(value);
    ui->hueHiValue->setText(QString::number(value));
}

void MainWindow::on_satLoSlider_valueChanged(int value)
{
    player->SetSLo(value);
    ui->satLoValue->setText(QString::number(value));
}

void MainWindow::on_satHiSlider_valueChanged(int value)
{
    player->SetSHi(value);
    ui->satHiValue->setText(QString::number(value));
}

void MainWindow::on_valLoSlider_valueChanged(int value)
{
    player->SetVLo(value);
    ui->valLoValue->setText(QString::number(value));
}

void MainWindow::on_valHiSlider_valueChanged(int value)
{
    player->SetVHi(value);
    ui->valHiValue->setText(QString::number(value));
}

void MainWindow::on_pBox_returnPressed()
{
}
