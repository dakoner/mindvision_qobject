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

// Ensure the MindVision SDK header is in your include path
// Usually located in the SDK installation directory (e.g., MVSDK/include)
#include "CameraApi.h"
#include "mindvision_qobject_global.h"

// Worker class to handle the image capture loop in a separate thread
class CameraWorker : public QObject
{
    Q_OBJECT
public:
    explicit CameraWorker(CameraHandle handle, int width, int height);
    ~CameraWorker();

public slots:
    // Main capture loop
    void process();
    // Request the loop to stop
    void stop();

signals:
    // Emitted when a frame is processed and converted to QImage
    void frameReady(QImage image);
    // Emitted to report FPS
    void fpsChanged(double fps);
    // Emitted when the capture loop finishes
    void finished();

private:
    CameraHandle m_hCamera;
    int m_width;
    int m_height;
    bool m_stopRequested;
    unsigned char* m_pRgbBuffer; // Buffer for RGB conversion
    tSdkFrameHead m_frameHead;
};

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
    CameraHandle m_hCamera;
    tSdkCameraDevInfo m_devInfo;
    tSdkCameraCapbility m_capInfo; // Camera capability cache
    bool m_isOpen;
    
    QThread* m_workerThread;
    CameraWorker* m_worker;
};

#endif // MINDVISIONCAMERA_H
