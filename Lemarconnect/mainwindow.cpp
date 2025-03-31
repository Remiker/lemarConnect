#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion des signaux
    connect(ui->Add_device, &QPushButton::clicked, this, &MainWindow::onAddDeviceClicked);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onCellDoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

QJsonObject createPanel(const QString &title, const QString &type, int x, int y, int w, int h, const QString &query, const QString &refId) {
    QJsonObject panel;
    panel["title"] = title;
    panel["type"] = type;

    QJsonObject gridPos;
    gridPos["x"] = x;
    gridPos["y"] = y;
    gridPos["w"] = w;
    gridPos["h"] = h;
    panel["gridPos"] = gridPos;

    QJsonObject datasource;
    datasource["type"] = "influxdb";
    datasource["uid"] = "ee9kapeot0zcwc";
    panel["datasource"] = datasource;

    QJsonArray targets;
    QJsonObject target;
    target["query"] = query;
    target["refId"] = refId;
    targets.append(target);

    panel["targets"] = targets;

    return panel;
}

QJsonObject createDashboard() {
    QJsonObject dashboard;
    dashboard["title"] = "first_dashboard";
    dashboard["uid"] = "ae9ghttcr36rkd";
    dashboard["version"] = 6;
    dashboard["editable"] = true;
    dashboard["refresh"] = "10s";
    dashboard["schemaVersion"] = 40;
    dashboard["timezone"] = "browser";

    QJsonObject time;
    time["from"] = "now-5m";
    time["to"] = "now";
    dashboard["time"] = time;

    QJsonArray panels;

    // Adding panels (Humidity, Temperature, etc.)
    panels.append(createPanel("Humidity", "timeseries", 0, 1, 12, 8,
                              "from(bucket: \"lemarconnect\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"measurement1\" and r._field == \"esp-device\" and r.source == \"humidity\")", "A"));

    panels.append(createPanel("Temperature", "timeseries", 12, 1, 12, 8,
                              "from(bucket: \"lemarconnect\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"measurement1\" and r._field == \"esp-device\" and r.source == \"temperature\")", "B"));

    panels.append(createPanel("Bar Chart Example", "barchart", 0, 9, 12, 8,
                              "from(bucket: \"lemarconnect\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"measurement1\" and r._field == \"esp-device\")", "C"));

    panels.append(createPanel("Gauge Example", "gauge", 12, 9, 12, 8,
                              "from(bucket: \"lemarconnect\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"measurement1\" and r._field == \"esp-device\")", "D"));

    panels.append(createPanel("Pie Chart Example", "piechart", 0, 17, 12, 8,
                              "from(bucket: \"lemarconnect\") |> range(start: -1h) |> filter(fn: (r) => r._measurement == \"measurement1\" and r._field == \"esp-device\")", "E"));

    dashboard["panels"] = panels;

    return dashboard;
}

void writeDashboardToFile(const QJsonObject &dashboard) {
    QFile file("grafana_dashboard.json");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening file for writing.";
        return;
    }

    QJsonDocument doc(dashboard);
    file.write(doc.toJson());
    file.close();
    qDebug() << "Dashboard JSON saved successfully.";
}

void MainWindow::onAddDeviceClicked()
{
    // Demander le nom du dispositif
    bool okName;
    QString name = QInputDialog::getText(this, "Device Name", "Enter device name:", QLineEdit::Normal, "", &okName);
    if (!okName || name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Device Name", "Device name cannot be empty.");
        return;
    }

    // Demander l'ID
    bool okID;
    QString id;
    do {
        id = QInputDialog::getText(this, "ID Value", "Enter ID value (up to 20 digits):", QLineEdit::Normal, "", &okID);
        if (okID && !id.isEmpty()) {
            QRegularExpression re("^[0-9]{1,20}$");
            if (!re.match(id).hasMatch()) {
                QMessageBox::warning(this, "Invalid ID", "Please enter a valid ID (up to 20 digits).");
                okID = false;
            }
        }
    } while (!okID || id.isEmpty());

    // Demander la location
    bool okLocation;
    QString location = QInputDialog::getText(this, "Location", "Enter location:", QLineEdit::Normal, "", &okLocation);
    if (!okLocation || location.isEmpty()) {
        QMessageBox::warning(this, "Invalid Location", "Location cannot be empty.");
        return;
    }

    // Demander le nombre de capteurs
    bool okSensorCount;
    int sensorCount = QInputDialog::getInt(this, "Number of Sensors", "Enter the number of sensors:", 1, 1, 100, 1, &okSensorCount);

    // Configurer le tableau
    ui->tableWidget->setColumnCount(4 + sensorCount * 2); // 4 colonnes de base pour le dispositif et 2 par capteur
    QStringList headers;
    headers << "Device Name" << "ID" << "Location" << "Number of Sensors";
    for (int i = 0; i < sensorCount; ++i) {
        headers << QString("Sensor %1 Name").arg(i + 1) << QString("Sensor %1 Threshold").arg(i + 1);
    }
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Ajouter une nouvelle ligne au tableau
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rowCount); // Insérer une nouvelle ligne

    // Placer les informations de base dans le tableau
    ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(name));
    ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(id));
    ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(location));
    ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::number(sensorCount)));

    // Ajouter les capteurs
    sensors.clear(); // Réinitialiser le vecteur de capteurs
    for (int i = 0; i < sensorCount; ++i) {
        bool okSensorName;
        QString sensorName = QInputDialog::getText(this, "Sensor Name", QString("Enter name for sensor %1:").arg(i + 1), QLineEdit::Normal, "", &okSensorName);

        if (!okSensorName || sensorName.isEmpty()) {
            QMessageBox::warning(this, "Invalid Sensor Name", "Sensor name cannot be empty.");
            return;
        }

        bool okThreshold;
        double threshold = QInputDialog::getDouble(this, "Sensor Threshold", QString("Enter threshold for %1:").arg(sensorName), 0.0, 0.0, 1000.0, 2, &okThreshold);

        // Placer le nom du capteur et son seuil dans le tableau
        int sensorColumnIndex = 4 + (i * 2); // Commencer à la colonne 4
        ui->tableWidget->setItem(rowCount, sensorColumnIndex, new QTableWidgetItem(sensorName)); // Nom du capteur
        ui->tableWidget->setItem(rowCount, sensorColumnIndex + 1, new QTableWidgetItem(QString::number(threshold))); // Seuil du capteur

        // Ajouter le capteur au vecteur
        sensors.append({sensorName, threshold});
    }

    // Mettre à jour l'affichage
    ui->tableWidget->resizeColumnsToContents();

    QJsonObject dashboard = createDashboard();
    writeDashboardToFile(dashboard);
}

void MainWindow::onCellDoubleClicked(int row, int column)
{
    // Logique pour éditer la cellule
    QString currentValue = ui->tableWidget->item(row, column)->text();
    QString newValue = QInputDialog::getText(this, "Edit Value", "Enter new value:", QLineEdit::Normal, currentValue);

    if (!newValue.isEmpty()) {
        ui->tableWidget->item(row, column)->setText(newValue); // Mettre à jour la valeur
    }
}

void MainWindow::refreshSensorCount() {
    // Mettre à jour le nombre total de capteurs dans le tableau
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        int sensorCount = 0;

        // Compter le nombre de capteurs pour cette ligne
        for (int column = 4; column < ui->tableWidget->columnCount(); column += 2) {
            if (ui->tableWidget->item(row, column) != nullptr) {
                sensorCount++;
            }
        }

        // Mettre à jour le nombre de capteurs dans la colonne appropriée
        ui->tableWidget->item(row, 3)->setText(QString::number(sensorCount));
    }
}



void MainWindow::on_Add_sensor_clicked()
{

}

