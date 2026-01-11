#include "MindVisionCamera.h"
#include <QDebug>
#include <QElapsedTimer>
#include <cstdlib>

// =============================================================================
// CameraWorker Implementation
// =============================================================================

CameraWorker::CameraWorker(CameraHandle handle, int width, int height)
    : m_hCamera(handle), m_width(width), m_height(height), m_stopRequested(false), m_pRgbBuffer(nullptr)
{
    // Allocate buffer for RGB conversion (Width * Height * 3 bytes for RGB24)
    // Note: The SDK documentation recommends CameraAlignMalloc for better performance,
    // but standard malloc is used here for standard C++ compatibility.
    // Ensure you link against MVCAMSDK.lib/dll.
    int size = m_width * m_height * 3;
    m_pRgbBuffer = (unsigned char*)malloc(size);
}

CameraWorker::~CameraWorker()
{
    if (m_pRgbBuffer) {
        free(m_pRgbBuffer);
        m_pRgbBuffer = nullptr;
    }
}

void CameraWorker::stop()
{
    m_stopRequested = true;
}

void CameraWorker::process()
{
    unsigned char* pRawBuffer = nullptr;
    CameraSdkStatus status;
    
    QElapsedTimer fpsTimer;
    fpsTimer.start();
    int frameCount = 0;

    while (!m_stopRequested) {
        // 1. Get raw image buffer with a timeout (e.g., 1000ms)
        // CameraGetImageBuffer retrieves a pointer to the internal SDK buffer (Zero-Copy)
        status = CameraGetImageBuffer(m_hCamera, &m_frameHead, &pRawBuffer, 1000);

        if (status == CAMERA_STATUS_SUCCESS) {
            // Calculate FPS
            frameCount++;
            if (fpsTimer.elapsed() >= 1000) {
                double fps = frameCount * 1000.0 / fpsTimer.elapsed();
                emit fpsChanged(fps);
                
                fpsTimer.restart();
                frameCount = 0;
            }

            // 2. Process raw data to RGB
            // CameraImageProcess converts the RAW data (pRawBuffer) to the target format (m_pRgbBuffer).
            // We previously configured the camera to output RGB24 via CameraSetIspOutFormat.
            status = CameraImageProcess(m_hCamera, pRawBuffer, m_pRgbBuffer, &m_frameHead);

            if (status == CAMERA_STATUS_SUCCESS) {
                // 3. Create QImage from the processed RGB buffer
                // QImage::Format_RGB888 expects 3 bytes per pixel (R, G, B)
                QImage img(m_pRgbBuffer, m_frameHead.iWidth, m_frameHead.iHeight, QImage::Format_RGB888);
                
                // 4. Emit signal with a *copy* of the image
                // We must copy because m_pRgbBuffer will be reused in the next iteration.
                emit frameReady(img.copy()); 
            }

            // 5. Release the raw buffer back to the SDK so it can be reused for new frames
            CameraReleaseImageBuffer(m_hCamera, pRawBuffer);
        } else {
            // Handle timeout or error. 
            // status == CAMERA_STATUS_TIME_OUT if no frame arrived in 1000ms.
        }
    }
    
    emit finished();
}

// =============================================================================
// MindVisionCamera Implementation
// =============================================================================

MindVisionCamera::MindVisionCamera(QObject *parent)
    : QObject(parent), m_hCamera(0), m_isOpen(false), m_workerThread(nullptr), m_worker(nullptr)
{
    // Initialize the SDK
    // 0: English, 1: Chinese
    CameraSdkInit(0); 
}

MindVisionCamera::~MindVisionCamera()
{
    stop();
    close();
}

bool MindVisionCamera::open()
{
    if (m_isOpen) return true;

    int iCameraCounts = 1;
    tSdkCameraDevInfo tCameraList[1]; // Buffer for 1 camera info
    
    // Enumerate devices to find connected cameras
    if (CameraEnumerateDevice(tCameraList, &iCameraCounts) != CAMERA_STATUS_SUCCESS || iCameraCounts == 0) {
        emit errorOccurred("No MindVision camera found.");
        return false;
    }

    // Initialize the first available camera found
    if (CameraInit(&tCameraList[0], -1, -1, &m_hCamera) != CAMERA_STATUS_SUCCESS) {
        emit errorOccurred("Failed to initialize camera.");
        return false;
    }

    m_devInfo = tCameraList[0];
    m_isOpen = true;
    
    // Get Camera Capabilities
    CameraGetCapability(m_hCamera, &m_capInfo);

    // Set the ISP output format to RGB24. This ensures CameraImageProcess produces
    // data compatible with QImage::Format_RGB888.
    CameraSetIspOutFormat(m_hCamera, CAMERA_MEDIA_TYPE_RGB8);
    
    // Default to Auto Exposure
    setAutoExposure(true);

    return true;
}

void MindVisionCamera::close()
{
    if (m_isOpen) {
        stop(); // Stop capturing first
        
        // Release SDK resources for this camera
        CameraUnInit(m_hCamera);
        
        m_hCamera = 0;
        m_isOpen = false;
    }
}

bool MindVisionCamera::start()
{
    if (!m_isOpen) {
        emit errorOccurred("Camera is not open.");
        return false;
    }

    // Start the camera video stream
    if (CameraPlay(m_hCamera) != CAMERA_STATUS_SUCCESS) {
        emit errorOccurred("Failed to start camera play.");
        return false;
    }

    // Get current resolution to allocate the correct buffer size in the worker
    tSdkImageResolution tResolution;
    if (CameraGetImageResolution(m_hCamera, &tResolution) != CAMERA_STATUS_SUCCESS) {
         emit errorOccurred("Failed to get camera resolution.");
         return false;
    }

    // Create the worker thread
    m_workerThread = new QThread;
    m_worker = new CameraWorker(m_hCamera, tResolution.iWidth, tResolution.iHeight);
    m_worker->moveToThread(m_workerThread);

    // Connect signals and slots for thread management
    connect(m_workerThread, &QThread::started, m_worker, &CameraWorker::process);
    connect(m_worker, &CameraWorker::frameReady, this, &MindVisionCamera::frameReady);
    connect(m_worker, &CameraWorker::fpsChanged, this, &MindVisionCamera::fpsChanged);
    
    // Clean up worker and thread when finished
    connect(m_worker, &CameraWorker::finished, m_workerThread, &QThread::quit);
    connect(m_worker, &CameraWorker::finished, m_worker, &CameraWorker::deleteLater);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);

    // Start the thread
    m_workerThread->start();
    return true;
}

void MindVisionCamera::stop()
{
    // Stop the worker thread
    if (m_worker) {
        m_worker->stop(); // Tell the loop to break
        if (m_workerThread) {
            m_workerThread->quit();
            m_workerThread->wait(); // Wait for the thread to actually finish
        }
        m_worker = nullptr;
        m_workerThread = nullptr;
    }
    
    // Stop the camera SDK stream
    if (m_isOpen) {
        CameraStop(m_hCamera); 
    }
}

bool MindVisionCamera::setAutoExposure(bool enabled)
{
    if (!m_isOpen) return false;
    return CameraSetAeState(m_hCamera, enabled ? TRUE : FALSE) == CAMERA_STATUS_SUCCESS;
}

bool MindVisionCamera::setExposureTime(double exposureTimeMs)
{
    if (!m_isOpen) return false;
    // SDK takes microseconds
    double exposureTimeUs = exposureTimeMs * 1000.0;
    return CameraSetExposureTime(m_hCamera, exposureTimeUs) == CAMERA_STATUS_SUCCESS;
}

bool MindVisionCamera::setAnalogGain(int gain)
{
    if (!m_isOpen) return false;
    return CameraSetAnalogGain(m_hCamera, gain) == CAMERA_STATUS_SUCCESS;
}

bool MindVisionCamera::getAutoExposure()
{
    if (!m_isOpen) return false;
    BOOL state = FALSE;
    CameraGetAeState(m_hCamera, &state);
    return state == TRUE;
}

double MindVisionCamera::getExposureTime()
{
    if (!m_isOpen) return 0.0;
    double timeUs = 0;
    CameraGetExposureTime(m_hCamera, &timeUs);
    return timeUs / 1000.0; // Convert back to ms
}

int MindVisionCamera::getAnalogGain()
{
    if (!m_isOpen) return 0;
    int gain = 0;
    CameraGetAnalogGain(m_hCamera, &gain);
    return gain;
}

void MindVisionCamera::getExposureTimeRange(double &minMs, double &maxMs)
{
    if (!m_isOpen) {
        minMs = 0;
        maxMs = 0;
        return;
    }
    // sExposeDesc.uiExposeTimeMin/Max are in lines? No, uiExposeTimeMin is lines usually for rolling shutter,
    // but for SDK abstraction it might be exposed differently.
    // The PDF says:
    // UINT uiExposeTimeMin; // In manual mode, the minimum number of line exposure
    // Wait, the SDK struct says lines. But CameraSetExposureTime takes time.
    // Usually there's a conversion or we assume a safe range. 
    // Let's use a safe large range or check if there's a time-based capability.
    // Actually, uiExposeTimeMin is often lines * lineTime. 
    // For simplicity, let's hardcode a reasonable range for the UI 
    // or use the capability struct if it has microseconds.
    // PDF: "CameraGetCapability ... tSdkExpose sExposeDesc ... uiExposeTimeMin"
    // Since converting lines to time requires line time which varies, 
    // we'll return a generic range for the slider (e.g. 0.1ms to 1000ms) 
    // OR we could use CameraGetExposureLineTime to calculate.
    // Let's try to calculate.
    
    double lineTime = 0;
    CameraGetExposureLineTime(m_hCamera, &lineTime);
    
    // If lineTime is valid
    if (lineTime > 0) {
        minMs = m_capInfo.sExposeDesc.uiExposeTimeMin * lineTime / 1000.0;
        maxMs = m_capInfo.sExposeDesc.uiExposeTimeMax * lineTime / 1000.0;
    } else {
        // Fallback
        minMs = 0.1;
        maxMs = 1000.0;
    }
}

void MindVisionCamera::getAnalogGainRange(int &min, int &max)
{
    if (!m_isOpen) {
        min = 0;
        max = 0;
        return;
    }
    min = m_capInfo.sExposeDesc.uiAnalogGainMin;
    max = m_capInfo.sExposeDesc.uiAnalogGainMax;
}

bool MindVisionCamera::setRoi(bool enable)
{
    if (!m_isOpen) return false;

    // We must restart the stream if it's running because the worker buffer size depends on resolution
    bool wasRunning = (m_workerThread != nullptr);
    if (wasRunning) {
        stop();
    }

    tSdkImageResolution tResolution;
    // Initialize with current just in case, though we will overwrite
    CameraGetImageResolution(m_hCamera, &tResolution);

    if (enable) {
        // Set to 640x480 Custom ROI
        tResolution.iIndex = 0xFF; // Custom resolution index
        tResolution.iWidth = 640;
        tResolution.iHeight = 480;
        
        // Try to center the ROI
        int maxWidth = m_capInfo.pImageSizeDesc[0].iWidth;
        int maxHeight = m_capInfo.pImageSizeDesc[0].iHeight;
        
        if (maxWidth > 640 && maxHeight > 480) {
            tResolution.iHOffsetFOV = (maxWidth - 640) / 2;
            tResolution.iVOffsetFOV = (maxHeight - 480) / 2;
        } else {
            tResolution.iHOffsetFOV = 0;
            tResolution.iVOffsetFOV = 0;
        }
    } else {
        // Restore to Full Resolution (Preset 0)
        tResolution.iIndex = 0;
        tResolution.iWidth = m_capInfo.pImageSizeDesc[0].iWidth;
        tResolution.iHeight = m_capInfo.pImageSizeDesc[0].iHeight;
        tResolution.iHOffsetFOV = 0;
        tResolution.iVOffsetFOV = 0;
    }

    if (CameraSetImageResolution(m_hCamera, &tResolution) != CAMERA_STATUS_SUCCESS) {
        // If failed, try to restore state (restart if was running)
        if (wasRunning) start();
        return false;
    }

    if (wasRunning) {
        return start();
    }
    return true;
}
