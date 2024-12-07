#include "notisys.h"
#include "ui_notisys.h"
bool ActiveWeather=false;
bool ActiveCurrency=false;
bool ActiveStock=false;
NotiSys::NotiSys(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::NotiSys)
{
    ui->setupUi(this);

    // Timers
    //---------------------------------------------------------------------------------------------
    weatherUpdateTimer = new QTimer(this);
    connect(weatherUpdateTimer, &QTimer::timeout, this, [this]() { getDataFromServer("weather"); });

    currencyUpdateTimer = new QTimer(this);
    connect(currencyUpdateTimer, &QTimer::timeout, this, [this]() { getDataFromServer("currency"); });

    stockUpdateTimer = new QTimer(this);
    connect(stockUpdateTimer, &QTimer::timeout, this, [this]() { getDataFromServer("stocks"); });
    //---------------------------------------------------------------------------------------------


    setFixedSize(550, 400);
    setStyleSheet("QMainWindow { background-color: #427D9D; }"); //4A628A

    weatherBox = new QGroupBox("", this);
    currencyBox = new QGroupBox("", this);
    stockBox = new QGroupBox("", this);

    weatherBox->setStyleSheet("QGroupBox { border: 0px solid #0078d7; border-radius: 10px; background-color: #D1E6F3; }");
    currencyBox->setStyleSheet("QGroupBox { border: 0px solid #0078d7; border-radius: 10px; background-color: #D1E6F3; }");  //B6D0E1
    stockBox->setStyleSheet("QGroupBox { border: 0px solid #0078d7; border-radius: 10px; background-color: #D1E6F3; }");  //D1E6F3


    // weatherBox backgroungIcon
    //--------------------------------------------------------
    QLabel* backgroundIconWeather = new QLabel(weatherBox); // Додаємо іконку до головного вікна
    QPixmap pixmapWeather(":/icon/weather_icon.png");

    // Масштабуємо іконку
    backgroundIconWeather->setPixmap(pixmapWeather.scaled(150, 150, Qt::KeepAspectRatio));

    int x_weather = weatherBox->x() + weatherBox->width() - 50; // Зміщуємо праворуч
    int y_weather = weatherBox->y() + (weatherBox->height() - backgroundIconWeather->height()) / 2 + 100; // Центруємо по вертикалі і вниз

    backgroundIconWeather->move(x_weather, y_weather);

    // Робимо іконку "прозорою" для подій, щоб не блокувати взаємодію з елементами
    backgroundIconWeather->setAttribute(Qt::WA_TransparentForMouseEvents);
    backgroundIconWeather->show();
    //--------------------------------------------------------


    // currencyBox backgroundIcon
    //--------------------------------------------------------
    QLabel* backgroundIconCurrency = new QLabel(currencyBox);
    QPixmap pixmapCurrency(":/icon/currency_icon.png");

    backgroundIconCurrency->setPixmap(pixmapCurrency.scaled(110, 110, Qt::KeepAspectRatio));

    int x_currency = currencyBox->x() + currencyBox->width() - 20;
    int y_currency = currencyBox->y() + (currencyBox->height() - backgroundIconCurrency->height()) / 2 + 125;

    backgroundIconCurrency->move(x_currency, y_currency);

    backgroundIconCurrency->setAttribute(Qt::WA_TransparentForMouseEvents);
    backgroundIconCurrency->show();
    //--------------------------------------------------------


    // stockBox backgroundIcon
    //--------------------------------------------------------
    QLabel* backgroundIconStock = new QLabel(stockBox);
    QPixmap pixmapStock(":/icon/stock_icon.png");

    backgroundIconStock->setPixmap(pixmapStock.scaled(110, 110, Qt::KeepAspectRatio));

    int x_stock = stockBox->x() + stockBox->width() - 25; // Зміщуємо праворуч
    int y_stock = stockBox->y() + (stockBox->height() - backgroundIconStock->height()) / 2 + 125;

    backgroundIconStock->move(x_stock, y_stock);

    backgroundIconStock->setAttribute(Qt::WA_TransparentForMouseEvents);
    backgroundIconStock->show();
    //--------------------------------------------------------


    // Заповнення всіх GroupBox полями та елементами
    // WeatherBox
    //-----------------------------------------------------------------------------------
    weatherLayout = new QVBoxLayout;

    QLabel* titleLabelWeather = new QLabel("Прогноз погоди", this);
    titleLabelWeather->setStyleSheet("font-size: 14px; font-weight: bold; text-align: center; text-decoration: underline; color: #000000;");
    titleLabelWeather->setAlignment(Qt::AlignCenter); // Вирівнювання по центру

    weatherLayout->addWidget(titleLabelWeather);

    // Надписи
    QLabel* weatherLabel = new QLabel("Погода:", this);
    QLabel* temperatureLabel = new QLabel("Температура:", this);
    QLabel* windLabel = new QLabel("Вітер:", this);
    QLabel* humidityLabel = new QLabel("Вологість:", this);
    QLabel* subscribeLabelWeather = new QLabel("Підписатися", this);

    weatherLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    temperatureLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    windLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    humidityLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    subscribeLabelWeather->setStyleSheet("font-size: 14px; font-weight: bold;");

    // Поля для виведення інформації
    weatherLine = new QLineEdit(this);
    temperatureLine = new QLineEdit(this);
    windLine = new QLineEdit(this);
    humidityLine = new QLineEdit(this);

    setUpLine(weatherLine, 120, 26);
    setUpLine(temperatureLine, 100, 26);
    setUpLine(windLine, 100, 26);
    setUpLine(humidityLine, 100, 26);

    QVBoxLayout* weatherLayoutBlock = createLayoutBlock(weatherLabel, weatherLine, 3);
    QVBoxLayout* temperatureLayoutBlock = createLayoutBlock(temperatureLabel, temperatureLine, 3);
    QVBoxLayout* windLayoutBlock = createLayoutBlock(windLabel, windLine, 3);
    QVBoxLayout* humidityLayoutBlock = createLayoutBlock(humidityLabel, humidityLine, 3);

    // для відступів між блоками
    QVBoxLayout* spaceBetweenBlocks0 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks1 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks2 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks3 = spacingBetweenBlock(10, 10);

    // Додаємо блоки та відступи
    weatherLayout->addLayout(spaceBetweenBlocks0);
    weatherLayout->addLayout(weatherLayoutBlock);
    weatherLayout->addLayout(spaceBetweenBlocks1);
    weatherLayout->addLayout(temperatureLayoutBlock);
    weatherLayout->addLayout(spaceBetweenBlocks2);
    weatherLayout->addLayout(windLayoutBlock);
    weatherLayout->addLayout(spaceBetweenBlocks3);
    weatherLayout->addLayout(humidityLayoutBlock);

    QHBoxLayout* subscribeLayoutWeather = new QHBoxLayout;
    QCheckBox* subscribeCheckBoxWeather = new QCheckBox(this);
    connect(subscribeCheckBoxWeather, &QCheckBox::stateChanged, this, &NotiSys::onSubscribeWeatherChanged);


    // Змінюємо стиль для збільшення розміру галочки
    subscribeCheckBoxWeather->setStyleSheet("QCheckBox::indicator {"
                                     "width: 15px;"   // Ширина галочки
                                     "height: 15px;"  // Висота галочки
                                     "border: 1px solid #427D9D;"  // Колір рамки
                                     "border-radius: 5px;"          // Заокруглені кути
                                     "background-color: white;"     // Колір фону галочки
                                     "}"
                                     "QCheckBox::indicator:checked {"
                                     "background-color: #A2C9E0;"  // Колір фону, коли чекбокс вибрано
                                     "}");

    subscribeLayoutWeather->addWidget(subscribeCheckBoxWeather, 0, Qt::AlignLeft);
    //subscribeLayout->addSpacing(1);
    subscribeLayoutWeather->addWidget(subscribeLabelWeather, 0, Qt::AlignLeft);
    subscribeLayoutWeather->addStretch();

    weatherLayout->addStretch();   // Додаємо відступ донизу
    weatherLayout->addLayout(subscribeLayoutWeather);

    weatherBox->setLayout(weatherLayout);
    //-----------------------------------------------------------------------------------



    // CurrencyBox
    //-----------------------------------------------------------------------------------
    currencyLayout = new QVBoxLayout;

    QLabel* titleLabelCurrency = new QLabel("Курс валют", this);
    titleLabelCurrency->setStyleSheet("font-size: 14px; font-weight: bold; text-align: center; text-decoration: underline; color: #000000;");
    titleLabelCurrency->setAlignment(Qt::AlignCenter);

    currencyLayout->addWidget(titleLabelCurrency);

    QLabel* dollarLabel = new QLabel("Долар:", this);
    QLabel* euroLabel = new QLabel("Євро:", this);
    QLabel* poundsLabel = new QLabel("Фунт:", this);
    QLabel* subscribeLabelCurrency = new QLabel("Підписатися", this);

    dollarLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    euroLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    poundsLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    subscribeLabelCurrency->setStyleSheet("font-size: 14px; font-weight: bold;");

    // Поля для виведення інформації
    dollarLine = new QLineEdit(this);
    euroLine = new QLineEdit(this);
    poundsLine = new QLineEdit(this);

    setUpLine(dollarLine, 100, 26);
    setUpLine(euroLine, 100, 26);
    setUpLine(poundsLine, 100, 26);

    QVBoxLayout* dollarLayoutBlock = createLayoutBlock(dollarLabel, dollarLine, 3);
    QVBoxLayout* euroLayoutBlock = createLayoutBlock(euroLabel, euroLine, 3);
    QVBoxLayout* poundsLayoutBlock = createLayoutBlock(poundsLabel, poundsLine, 3);

    // для відступів між блоками
    QVBoxLayout* spaceBetweenBlocks4 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks5 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks6 = spacingBetweenBlock(10, 10);

    // Додаємо блоки та відступи
    currencyLayout->addLayout(spaceBetweenBlocks4);
    currencyLayout->addLayout(dollarLayoutBlock);
    currencyLayout->addLayout(spaceBetweenBlocks5);
    currencyLayout->addLayout(euroLayoutBlock);
    currencyLayout->addLayout(spaceBetweenBlocks6);
    currencyLayout->addLayout(poundsLayoutBlock);


    QHBoxLayout* subscribeLayoutCurrency = new QHBoxLayout;
    QCheckBox* subscribeCheckBoxCurrency = new QCheckBox(this);
    connect(subscribeCheckBoxCurrency, &QCheckBox::stateChanged, this, &NotiSys::onSubscribeCurrencyChanged);

    // Змінюємо стиль для збільшення розміру галочки
    subscribeCheckBoxCurrency->setStyleSheet("QCheckBox::indicator {"
                                             "width: 15px;"   // Ширина галочки
                                             "height: 15px;"  // Висота галочки
                                             "border: 1px solid #427D9D;"  // Колір рамки
                                             "border-radius: 5px;"          // Заокруглені кути
                                             "background-color: white;"     // Колір фону галочки
                                             "}"
                                             "QCheckBox::indicator:checked {"
                                             "background-color: #A2C9E0;"  // Колір фону, коли чекбокс вибрано
                                             "}");

    subscribeLayoutCurrency->addWidget(subscribeCheckBoxCurrency, 0, Qt::AlignLeft);
    //subscribeLayout->addSpacing(1);
    subscribeLayoutCurrency->addWidget(subscribeLabelCurrency, 0, Qt::AlignLeft);
    subscribeLayoutCurrency->addStretch();

    currencyLayout->addStretch();   // Додаємо відступ донизу
    currencyLayout->addLayout(subscribeLayoutCurrency);

    currencyBox->setLayout(currencyLayout);
    //-----------------------------------------------------------------------------------



    // StockBox
    //-----------------------------------------------------------------------------------
    stockLayout = new QVBoxLayout;

    QLabel* titleLabelStock = new QLabel("Курс акцій", this);
    titleLabelStock->setStyleSheet("font-size: 14px; font-weight: bold; text-align: center; text-decoration: underline; color: #000000;");
    titleLabelStock->setAlignment(Qt::AlignCenter);

    stockLayout->addWidget(titleLabelStock);

    QLabel* appleLabel = new QLabel("Apple:", this);
    QLabel* microsoftLabel = new QLabel("Microsoft:", this);
    QLabel* paypalLabel = new QLabel("PayPal:", this);
    QLabel* subscribeLabelStock = new QLabel("Підписатися", this);

    appleLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    microsoftLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    paypalLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    subscribeLabelStock->setStyleSheet("font-size: 14px; font-weight: bold;");

    // Поля для виведення інформації
    appleLine = new QLineEdit(this);
    microsoftLine = new QLineEdit(this);
    paypalLine = new QLineEdit(this);

    setUpLine(appleLine, 100, 26);
    setUpLine(microsoftLine, 100, 26);
    setUpLine(paypalLine, 100, 26);

    QVBoxLayout* appleLayoutBlock = createLayoutBlock(appleLabel, appleLine, 3);
    QVBoxLayout* microsoftLayoutBlock = createLayoutBlock(microsoftLabel, microsoftLine, 3);
    QVBoxLayout* paypalLayoutBlock = createLayoutBlock(paypalLabel, paypalLine, 3);

    // для відступів між блоками
    QVBoxLayout* spaceBetweenBlocks7 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks8 = spacingBetweenBlock(10, 10);
    QVBoxLayout* spaceBetweenBlocks9 = spacingBetweenBlock(10, 10);

    // Додаємо блоки та відступи
    stockLayout->addLayout(spaceBetweenBlocks7);
    stockLayout->addLayout(appleLayoutBlock);
    stockLayout->addLayout(spaceBetweenBlocks8);
    stockLayout->addLayout(microsoftLayoutBlock);
    stockLayout->addLayout(spaceBetweenBlocks9);
    stockLayout->addLayout(paypalLayoutBlock);


    QHBoxLayout* subscribeLayoutStock = new QHBoxLayout;
    QCheckBox* subscribeCheckBoxStock = new QCheckBox(this);
    connect(subscribeCheckBoxStock, &QCheckBox::stateChanged, this, &NotiSys::onSubscribeStockChanged);


    // Змінюємо стиль для збільшення розміру галочки
    subscribeCheckBoxStock->setStyleSheet("QCheckBox::indicator {"
                                          "width: 15px;"   // Ширина галочки
                                          "height: 15px;"  // Висота галочки
                                          "border: 1px solid #427D9D;"  // Колір рамки
                                          "border-radius: 5px;"          // Заокруглені кути
                                          "background-color: white;"     // Колір фону галочки
                                          "}"
                                          "QCheckBox::indicator:checked {"
                                          "background-color: #A2C9E0;"  // Колір фону, коли чекбокс вибрано
                                          "}");

    subscribeLayoutStock->addWidget(subscribeCheckBoxStock, 0, Qt::AlignLeft);
    //subscribeLayout->addSpacing(1);
    subscribeLayoutStock->addWidget(subscribeLabelStock, 0, Qt::AlignLeft);
    subscribeLayoutStock->addStretch();

    stockLayout->addStretch();   // Додаємо відступ донизу
    stockLayout->addLayout(subscribeLayoutStock);

    stockBox->setLayout(stockLayout);
    //-----------------------------------------------------------------------------------










    // Layout для основного вікна (розміщення)
    mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(10);

    // Додавання QGroupBox до Layout
    mainLayout->addWidget(weatherBox);
    mainLayout->addWidget(currencyBox);
    mainLayout->addWidget(stockBox);

    // Встановлення рівномірного розподілу простору
    mainLayout->setStretchFactor(weatherBox, 1);
    mainLayout->setStretchFactor(currencyBox, 1);
    mainLayout->setStretchFactor(stockBox, 1);

    // Встановлення основного Layout для центрального віджету
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);


}


void NotiSys::setUpLine(QLineEdit* lineEdit, int width, int height) {
    // Встановлення фіксованих розмірів
    lineEdit->setFixedWidth(width);
    lineEdit->setFixedHeight(height);

    // Встановлення стилю через setStyleSheet
    lineEdit->setStyleSheet("QLineEdit {"
                            "background-color: #A2C9E0;"  // Колір фону
                            "border: 1px solid #427D9D;"   // Колір та стиль бордера
                            "border-radius: 5px;"          // Заокруглені кути
                            "padding: 5px;"                // Відступи всередині поля
                            "font-size: 14px;"             // Розмір шрифту
                            "color: #000000;"              // Колір тексту
                            "}");

    // Створення ефекту тіні
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(2, 2); // Зсув тіні
    shadowEffect->setBlurRadius(15); // Розмір розмиття тіні
    shadowEffect->setColor(QColor("#427D9D")); // Колір тіні (той самий, що й колір фону)

    lineEdit->setGraphicsEffect(shadowEffect);

    // Встановлення поля тільки для читання
    lineEdit->setReadOnly(true);
}


QVBoxLayout* NotiSys::createLayoutBlock(QLabel* label, QLineEdit* lineEdit, int spacing) {
    QVBoxLayout* layoutBlock = new QVBoxLayout();
    layoutBlock->addWidget(label);
    layoutBlock->addWidget(lineEdit);
    layoutBlock->setSpacing(spacing);  // Налаштовуємо відстань між Label та LineEdit
    return layoutBlock;
}


QVBoxLayout* NotiSys::spacingBetweenBlock(int topMargin, int spacing) {
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(spacing);  // Встановлюємо відстань між блоками
    layout->setContentsMargins(0, topMargin, 0, 0);  // Встановлюємо відступ згори
    return layout;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void NotiSys::getDataFromServer(const QString& requestIdentify) {
    QString response;
    QString request = requestIdentify + ":" + requestIdentify;

    // Надсилаємо запит до сервера
    bool success = sendRequestToServer(request, response);

    if (!success) {
        qCritical() << "Failed to get " << requestIdentify << " data.";
        QMessageBox::critical(nullptr, "Data request error", "Failed to get data");
        return; // Якщо запит на погоду не вдалося надіслати, вийшли з функції
    }

    qDebug() << requestIdentify << " data received: " << response;

    // Якщо відповідь не порожня, оновлюємо UI
    if (!response.isEmpty()) {
        if(requestIdentify == "weather"){
            updateWeatherUI(response);
        } else if (requestIdentify == "currency") {
            updateCurrencyUI(response);
        } else if (requestIdentify == "stocks"){
            updateStockUI(response);
        }
    } else {
        qDebug() << "Failed to retrieve " << requestIdentify << " data.";
        QMessageBox::critical(nullptr, "Data request error", "Failed to retrieve data");
    }
}



void NotiSys::subscribeToUpdates(const QString& ifSubscribe, const QString& requestIdentify) {
    QString request = ifSubscribe + requestIdentify;  // Запит на підписку
    QString response;

    bool success = sendRequestToServer(request, response);

    if (!success) {
        qCritical() << "Failed to subscribe (unsubscribed) to " << requestIdentify << " updates.";
        QMessageBox::critical(nullptr, "Subscribe/Unsubscribed Error", "Failed subscribe/unsubscribed to  updates.");
        return; // Якщо підписка не вдалася
    }

    qDebug() << ifSubscribe << " to " << requestIdentify << " updates.";
}



bool NotiSys::sendRequestToServer(const QString& request, QString& response) {
    QString pipeName = R"(\\.\pipe\server_pipe)";
    HANDLE hPipe = CreateFile(
        reinterpret_cast<LPCWSTR>(pipeName.utf16()), // Ім'я каналу в UTF-16
        GENERIC_READ | GENERIC_WRITE,               // Читання і запис
        0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        qCritical() << "Error: Unable to connect to the server.";
        return false;
    }

    // Надсилаємо запит до сервера
    DWORD bytesWritten;
    BOOL success = WriteFile(
        hPipe, request.toStdString().c_str(), request.size(), &bytesWritten, NULL);

    if (!success) {
        qCritical() << "Failed to send request to the server.";
        QMessageBox::critical(nullptr, "Server request error", "Failed to send request to the server.");
        CloseHandle(hPipe);
        return false;
    }

    // Читання відповіді від сервера
    char buffer[1024];
    DWORD bytesRead;
    success = ReadFile(hPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL);

    if (!success || bytesRead == 0) {
        qCritical() << "Failed to read response from the server.";
        QMessageBox::critical(nullptr, "Read error", "Failed to read response from the server.");
        CloseHandle(hPipe);
        return false;
    }

    buffer[bytesRead] = '\0'; // Завершуємо рядок
    response = QString::fromStdString(buffer);

    // Логування відповіді для діагностики
    qDebug() << "Received from server: " << response;

    CloseHandle(hPipe);
    return true;
}


void NotiSys::updateWeatherUI(const QString& weatherData) {
    // "Weather in Lviv:\nTemperature: 25°C\nHumidity: 60%\nDescription: Sunny\nWind Speed: 5 m/s"

    // Розбиваємо відповідь на рядки
    QStringList lines = weatherData.split('\n');

    QString weatherDescription;
    QString temperature;
    QString humidity;
    QString windSpeed;

    for (const QString& line : lines) {
        if (line.contains("Temperature:")) {
            temperature = line.split(":").at(1).trimmed();
             temperature.remove(QRegularExpression("[^0-9.]"));  // Видаляємо всі символи, крім цифр та крапки
            double tempValue = temperature.toDouble();  // Конвертуємо в число
            temperature = QString::number(tempValue, 'f', 2);  // Округляємо до 2 знаків після коми
        } else if (line.contains("Humidity:")) {
            humidity = line.split(":").at(1).trimmed();
        } else if (line.contains("Description:")) {
            weatherDescription = line.split(":").at(1).trimmed();
        } else if (line.contains("Wind Speed:")) {
            windSpeed = line.split(":").at(1).trimmed();
        }
    }

    if (weatherDescription.isEmpty() || temperature.isEmpty() || humidity.isEmpty() || windSpeed.isEmpty()) {
        QMessageBox::critical(nullptr, "Data Error", "Could not get the data for the LineEdit correctly (weather)");
        return;
    } else {
        weatherLine->setText(weatherDescription);
        temperatureLine->setText(temperature + " C");
        humidityLine->setText(humidity);
        windLine->setText(windSpeed);
    }
}


void NotiSys::updateCurrencyUI(const QString& currencyData){
    QStringList lines = currencyData.split('\n');

    QString usd;
    QString eur;
    QString gbp;

    for (const QString& line : lines) {
        if (line.contains("USD:")) {
            usd = line.split(":").at(1).trimmed();
            usd.remove(QRegularExpression("[^0-9.]"));  // Видаляємо всі символи, крім цифр та крапки
            double tempValue = usd.toDouble();  // Конвертуємо в число
            usd = QString::number(tempValue, 'f', 2);  // Округляємо до 2 знаків після коми
        } else if (line.contains("EUR:")) {
            eur = line.split(":").at(1).trimmed();
            eur.remove(QRegularExpression("[^0-9.]"));
            double tempValue = eur.toDouble();
            eur = QString::number(tempValue, 'f', 2);
        } else if (line.contains(":")) {
            gbp = line.split(":").at(1).trimmed();
            gbp.remove(QRegularExpression("[^0-9.]"));
            double tempValue = gbp.toDouble();
            gbp = QString::number(tempValue, 'f', 2);
        }
    }

    if (usd.isEmpty() || eur.isEmpty() || gbp.isEmpty()) {
        QMessageBox::critical(nullptr, "Data Error", "Could not get the data for the LineEdit correctly (currency)");
        return;
    } else {
        dollarLine->setText(usd + " UAH");
        euroLine->setText(eur + " UAH");
        poundsLine->setText(gbp + " UAH");
    }
}


void NotiSys::updateStockUI(const QString& stockData){
    QStringList lines = stockData.split('\n');

    QString msft;
    QString pypl;
    QString aapl;

    for (const QString& line : lines) {
        if (line.contains("MSFT:")) {
            msft = line.split(":").at(1).trimmed();
        } else if (line.contains("PYPL:")) {
            pypl = line.split(":").at(1).trimmed();
        } else if (line.contains(":")) {
            aapl = line.split(":").at(1).trimmed();
        }
    }
    if (aapl.isEmpty() || msft.isEmpty() || pypl.isEmpty()) {
        QMessageBox::critical(nullptr, "Data Error", "Could not get the data for the LineEdit correctly (currency)");
        return;
    } else {
        appleLine->setText(aapl);
        microsoftLine->setText(msft);
        paypalLine->setText(pypl);
    }

}



void NotiSys::onSubscribeWeatherChanged(int state) {
    QString requestIdentify = "weather";

    if (state == Qt::Checked) {
        qDebug() << "Subscribed to weather updates.";
        ActiveWeather=true;

        QString ifSubscribe = "subscribe:";

        subscribeToUpdates(ifSubscribe, requestIdentify);
        getDataFromServer(requestIdentify);

        weatherUpdateTimer->start(3600000);

    } else if (state == Qt::Unchecked) {
        qDebug() << "Unsubscribed from weather updates.";
        ActiveWeather=false;

        // Надсилаємо запит на відписку
        QString ifSubscribe = "unsubscribe:";
        subscribeToUpdates(ifSubscribe, requestIdentify);

        weatherLine->clear();
        temperatureLine->clear();
        humidityLine->clear();
        windLine->clear();

        weatherUpdateTimer->stop();
    }
}


void NotiSys::onSubscribeCurrencyChanged(int state) {
    QString requestIdentify = "currency";

    if (state == Qt::Checked) {
        qDebug() << "Subscribed to Currency updates.";
        ActiveCurrency=true;
        QString ifSubscribe = "subscribe:";

        subscribeToUpdates(ifSubscribe, requestIdentify);
        getDataFromServer(requestIdentify);

        currencyUpdateTimer->start(24 * 60 * 60 * 1000); // 24 години в мілісекундах


    } else if (state == Qt::Unchecked) {
        qDebug() << "Unsubscribed from Currency updates.";
        ActiveCurrency=false;
        QString ifSubscribe = "unsubscribe:";
        subscribeToUpdates(ifSubscribe, requestIdentify);

        dollarLine->clear();
        euroLine->clear();
        poundsLine->clear();
        currencyUpdateTimer->stop();
    }
}

void NotiSys::onSubscribeStockChanged(int state) {
    QString requestIdentify = "stocks";

    if (state == Qt::Checked) {
        qDebug() << "Subscribed to Stock updates.";
        ActiveStock=true;
        QString ifSubscribe = "subscribe:";

        subscribeToUpdates(ifSubscribe, requestIdentify);
        getDataFromServer(requestIdentify);

        stockUpdateTimer->start(60 * 1000); // щохвилини

    } else if (state == Qt::Unchecked) {
        qDebug() << "Unsubscribed from Stock updates.";
        ActiveStock=false;
        QString ifSubscribe = "unsubscribe:";
        subscribeToUpdates(ifSubscribe, requestIdentify);

        appleLine->clear();
        microsoftLine->clear();
        paypalLine->clear();
        stockUpdateTimer->stop();
    }
}


NotiSys::~NotiSys()
{
    delete ui;
}
