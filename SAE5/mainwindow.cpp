#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initListview()
{
    // QStandardItemModel *model = new QStandardItemModel(this);

    // Set the column headers
    // model->setHorizontalHeaderLabels({"Name", "Age", "Country"});

    ui->listDeviceView.Columns.Add("Id", -2,HorizontalAlignment.Left);
    ui->listDeviceView.Columns.Add("Name", -2, HorizontalAlignment.Left);
    ui->listDeviceView.Columns.Add("Age", -2, HorizontalAlignment.Left);
}
void MainWindow::addDevice()
{

    ListViewItem item1 = new ListViewItem(new[] {"id123", "Tom", "24"});
    // var item2 = new ListViewItem(new[] {person.Id, person.Name, person.Age});
    ui->listDeviceView.Items.Add(item1);
}


