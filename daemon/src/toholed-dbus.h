#ifndef TOHOLED_DBUS_H
#define TOHOLED_DBUS_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtDBus/QtDBus>

#include <QTime>
#include <QThread>
#include <QElapsedTimer>
#include "oled.h"
#include "worker.h"
#include "weather.h"

#include <contextproperty.h>

#define SERVICE_NAME "com.kimmoli.toholed"
#define DRAWINGMODELOCKTIMEOUT (30000)

#include "backgroundactivity.h"

/* main class */

class Toholed: public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", SERVICE_NAME)

public:
    Toholed();

    ~Toholed()
    {
        /* Disable everything */
        delete(activity);

        setInterruptEnable(false, false);
        deinitOled();
        setVddState(false);
        printf("Toholed terminated!\n");
    }

public slots:
    QString getVersion();
    QString draw(const QDBusMessage& msg);
    QString setSettings(const QDBusMessage& msg);
    QByteArray captureOled();
    QString drawPicture(const QDBusMessage& msg);

signals:
    void displayUpdated();
    void iDontWantToLiveOnThisPlanet();

private slots:
    void setVddState(bool turn);
    void enableOled();
    void reloadSettings();

    /* interrupts */
    void setInterruptEnable(bool enableGpio, bool enableProximity);
    void handleGpioInterrupt();
    void handleProxInterrupt();

    /* dBus Signal handlers */
    void handleDisplayStatus(const QDBusMessage& msg);
    void handleNotificationClosed(const QDBusMessage& msg);
    void handleCall(const QDBusMessage& msg);
    void handleTweetian(const QDBusMessage& msg);
    void handleCommuni(const QDBusMessage& msg);
    void handleNotificationActionInvoked(const QDBusMessage& msg);
    void handleProfileChanged(const QDBusMessage& msg);
    void handleAlarm(const QDBusMessage& msg);
    void handleNetworkRegistration(const QDBusMessage& msg);
    void handleBluetooth(const QDBusMessage& msg);
    void handleWifi(const QDBusMessage& msg);
    void handleCellular(const QDBusMessage& msg);
    void handleConnmanManager(const QDBusMessage& msg);

    void propertyAlarmPresentChanged();
    void propertyBatteryIsChargingChanged();

    void handleEmailNotify();
    void handleTwitterNotify();
    void handleFacebookNotify();
    void handleIrssiNotify();
    void handleImNotify();
    void handleSmsNotify();
    void handleSystemUpdateNotify();
    void handleOtherNotify();
    void handleCallMissedNotify();
    void handleMitakuuluu();
    void handleSailorgramNotify();

    void updateDisplay(bool timeUpdateOverride = false, int blinks = 0);
    void blinkTimerTimeout( );
    void notificationSend(QString summary, QString body);

    void heartbeatReceived();

    void weatherChanged(QString newWeather);

private:
    QThread *gpioThread;
    Worker *gpioWorker;
    QThread *proximityThread;
    Worker *proximityWorker;

    bool ScreenCaptureOnProximity;

    bool oledInitDone;
    bool vddEnabled;
    bool gpioInterruptEnabled;
    bool proximityInterruptEnabled;

    unsigned int prevBrightness;
    unsigned int prevProx;

    unsigned int ssNotifyReplacesId;
    QString ssFilename;
    QString ScreenCaptureOnProximityStorePath;

    int activeHighlights;

    int timerCount;
    QTime prevTime;

    int blinkTimerCount;
    QTimer *blinkTimer;
    bool blinkNow;

    QTimer *screenshotFilter;

    QMutex mutex;

    int gpio_fd;
    int proximity_fd;

    bool iconSMS;
    bool iconEMAIL;
    bool iconCALL;
    bool iconTWEET;
    bool iconIRC;
    bool iconMITAKUULUU;
    bool iconSAILORGRAM;
    bool systemUpdate;

    bool blinkOnNotification;

    bool chargerConnected;
    bool silentProfile;
    bool alarmsPresent;

    bool displayOffWhenMainActive;
    bool analogClockFace;
    bool showAlarmsPresent;
    bool showCurrentTemperature;

    QString getCurrentProfile();
    void getCurrentNetworkConnectionStates();

    QVariantMap getDbusProperties(const QString & service,
                                  const QString & path,
                                  const QString & interface,
                                  const QDBusConnection & connection = QDBusConnection::sessionBus());

    QString networkType;
    bool wifiPowered;
    bool bluetoothPowered;
    bool wifiConnected;
    bool bluetoothConnected;
    bool cellularConnected;
    bool cellularPowered;
    bool offlineModeActive;

    bool lockDrawingMode;
    QString lockDrawingModeAppName;
    QElapsedTimer lockDrawingModeTimer;

    QScopedPointer<ContextProperty> propertyAlarmPresent;
    QScopedPointer<ContextProperty> propertyBatteryIsCharging;

    QString lastTemperature;

    BackgroundActivity *activity;

    Weather *weather;

    int gpioInterruptCounter;
    QTime gpioInterruptFloodDetect;
};



#endif // TOHOLED-DBUS_H
