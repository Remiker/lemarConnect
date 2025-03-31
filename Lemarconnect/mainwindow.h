#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QSerialPort>
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Sensor {
    QString name;
    double threshold;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddDeviceClicked();      // Slot pour ajouter un dispositif

    void onCellDoubleClicked(int row, int column); // Slot pour double-cliquer sur une cellule
    void refreshSensorCount();      // Fonction pour rafraîchir le compteur de capteurs

    void on_Add_sensor_clicked();

private:
    Ui::MainWindow *ui;            // Interface utilisateur
    QVector<Sensor> sensors;       // Pour stocker les capteurs
};

#endif // MAINWINDOW_H
