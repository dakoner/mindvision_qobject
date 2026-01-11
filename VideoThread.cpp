#include "VideoThread.h"
#include <QDebug>

VideoThread::VideoThread(QObject *parent) 
    : QThread(parent), m_abort(false), m_isRecording(false), m_width(0), m_height(0), m_fps(30.0)
{
}

VideoThread::~VideoThread()
{
    m_mutex.lock();
    m_abort = true;
    m_condition.wakeOne();
    m_mutex.unlock();
    wait();
}

void VideoThread::startRecording(int width, int height, double fps, const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    m_width = width;
    m_height = height;
    m_fps = fps;
    m_filename = filename;
    m_isRecording = true;
    m_abort = false;
    m_queue.clear();
    
    if (!isRunning()) {
        start();
    }
}

void VideoThread::stopRecording()
{
    QMutexLocker locker(&m_mutex);
    m_isRecording = false;
    m_condition.wakeOne();
}

void VideoThread::addFrame(const QImage &image)
{
    QMutexLocker locker(&m_mutex);
    if (m_isRecording) {
        // Deep copy is implicit if the image is detached, 
        // but to be safe against SDK buffer reuse if not handled upstream:
        // However, CameraWorker already does copy(), so 'image' here acts as a shared pointer to that copy.
        // It's safe to enqueue.
        m_queue.enqueue(image);
        m_condition.wakeOne();
    }
}

void VideoThread::run()
{
    QProcess ffmpeg;
    
    m_mutex.lock();
    int width = m_width;
    int height = m_height;
    double fps = m_fps;
    QString filename = m_filename;
    m_mutex.unlock();

    QStringList args;
    // Reverted to raw video parameters as requested
    args << "-y" 
         << "-f" << "rawvideo" 
         << "-vcodec" << "rawvideo" 
         << "-s" << QString("%1x%2").arg(width).arg(height) 
         << "-r" << QString::number(fps) 
         << "-pix_fmt" << "rgb24" 
         << "-i" << "-" 
         << "-c:v" << "rawvideo" 
         << "-pix_fmt" << "yuv420p"
         << "-an" 
         << filename;

    qDebug() << "VideoThread: Starting ffmpeg with" << args.join(" ");
    
    // Forward ffmpeg stdout/stderr to the application's console
    ffmpeg.setProcessChannelMode(QProcess::ForwardedChannels);

    ffmpeg.start("ffmpeg", args);
    if (!ffmpeg.waitForStarted()) {
        qDebug() << "VideoThread: Failed to start ffmpeg:" << ffmpeg.errorString();
        return;
    }

    while (true) {
        m_mutex.lock();
        
        if (m_abort) {
            m_mutex.unlock();
            break;
        }

        if (m_queue.isEmpty()) {
            // If stopped and empty, we are done
            if (!m_isRecording) {
                m_mutex.unlock();
                break;
            }
            // Wait for more frames
            m_condition.wait(&m_mutex);
        }

        // Check again after wake
        if (m_abort) {
            m_mutex.unlock();
            break;
        }
        
        if (m_queue.isEmpty() && !m_isRecording) {
            m_mutex.unlock();
            break;
        }

        if (!m_queue.isEmpty()) {
            QImage img = m_queue.dequeue();
            m_mutex.unlock();
            
            if (ffmpeg.isOpen()) {
                if (img.bytesPerLine() != img.width() * 3) {
                    for (int y = 0; y < img.height(); ++y) {
                        ffmpeg.write((const char*)img.scanLine(y), img.width() * 3);
                    }
                } else {
                    ffmpeg.write((const char*)img.bits(), img.sizeInBytes());
                }
                ffmpeg.waitForBytesWritten();
            }
            
            static int frameCount = 0;
            if (++frameCount % 30 == 0) {
                 qDebug() << "VideoThread: Processed frame" << frameCount << "Queue size:" << m_queue.size();
            }
        } else {
            m_mutex.unlock();
        }
    }
    
    if (ffmpeg.isOpen()) {
        ffmpeg.closeWriteChannel();
        ffmpeg.waitForFinished();
        ffmpeg.close();
    }
    
    qDebug() << "VideoThread: Finished.";
}
