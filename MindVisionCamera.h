#ifndef MINDVISIONCAMERA_H
#define MINDVISIONCAMERA_H

#include <QObject>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#ifdef _WIN32
#include <windows.h>
#endif

#include "mindvision_qobject_global.h"

// Forward declaration of private implementation class
class MindVisionCameraPrivate;

// Main interface class
class MINDVISION_QOBJECT_EXPORT MindVisionCamera : public QObject
{
    Q_OBJECT
public:
    explicit MindVisionCamera(QObject *parent = nullptr);
    ~MindVisionCamera();

    // Initializes the SDK and opens the first available camera
    Q_INVOKABLE bool open();
    
    // Closes the camera and releases resources
    Q_INVOKABLE void close();

    // Starts the capture thread
    Q_INVOKABLE bool start();

    // Stops the capture thread
    Q_INVOKABLE void stop();

    // Settings
    Q_INVOKABLE bool setAutoExposure(bool enabled);
    Q_INVOKABLE bool setExposureTime(double exposureTimeMs); // Milliseconds
    Q_INVOKABLE bool setAnalogGain(int gain);
    
    Q_INVOKABLE bool getAutoExposure();
    Q_INVOKABLE double getExposureTime(); // Milliseconds
    Q_INVOKABLE int getAnalogGain();

    // Capabilities
    Q_INVOKABLE void getExposureTimeRange(double &minMs, double &maxMs);
    Q_INVOKABLE void getAnalogGainRange(int &min, int &max);

    // ROI
    Q_INVOKABLE bool setRoi(bool enable);

signals:
    // Signal for UI to update (connect this to your QML or Widget)
    void frameReady(QImage image);
    // Signal for current FPS
    void fpsChanged(double fps);
    // Signal for error reporting
    void errorOccurred(QString message);

private:
    MindVisionCameraPrivate *d;
};

#endif // MINDVISIONCAMERA_H
