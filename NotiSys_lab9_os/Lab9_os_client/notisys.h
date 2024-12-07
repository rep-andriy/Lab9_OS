#ifndef NOTISYS_H
#define NOTISYS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QGraphicsDropShadowEffect>
#include <QPixmap>
#include <Windows.h>
#include <QRegularExpression>
#include <QTimer>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class NotiSys;
}
QT_END_NAMESPACE

class NotiSys : public QMainWindow
{
    Q_OBJECT

public:
    NotiSys(QWidget *parent = nullptr);
    ~NotiSys();

    //Ui
    //---------------------
    void setUpLine(QLineEdit* lineEdit, int width, int height);
    QVBoxLayout* createLayoutBlock(QLabel* label, QLineEdit* lineEdit, int spacing);
    QVBoxLayout* spacingBetweenBlock(int topMargin, int spacing);
    //---------------------
    void onSubscribeWeatherChanged(int state);
    void onSubscribeCurrencyChanged(int state);
    void onSubscribeStockChanged(int state);

    bool sendRequestToServer(const QString& request, QString& response);
    void subscribeToUpdates(const QString& ifSubscribe, const QString& requestIdentify);
    void getDataFromServer(const QString& request);

    void updateWeatherUI(const QString& weatherData);
    void updateCurrencyUI(const QString& currencyData);
    void updateStockUI(const QString& stockData);

private:
    Ui::NotiSys *ui;

    QTimer* weatherUpdateTimer;
    QTimer* currencyUpdateTimer;
    QTimer* stockUpdateTimer;

    QGroupBox* weatherBox;
    QGroupBox* currencyBox;
    QGroupBox* stockBox;

    QLineEdit* weatherLine;
    QLineEdit* temperatureLine;
    QLineEdit* windLine;
    QLineEdit* humidityLine;

    QLineEdit* dollarLine;
    QLineEdit* euroLine;
    QLineEdit* poundsLine;

    QLineEdit* appleLine;
    QLineEdit* microsoftLine;
    QLineEdit* paypalLine;

    QHBoxLayout* mainLayout;
    QVBoxLayout* weatherLayout;
    QVBoxLayout* currencyLayout;
    QVBoxLayout* stockLayout;
};
#endif // NOTISYS_H
