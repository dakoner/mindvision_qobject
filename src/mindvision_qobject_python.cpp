#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h> 

#include "MindVisionCamera.h"
#include "VideoThread.h"
#include <QObject>
#include <QImage>
#include <QBuffer>
#include <QDebug>

namespace py = pybind11;

namespace pybind11 { namespace detail {
    template <> struct type_caster<QString> {
    public:
        PYBIND11_TYPE_CASTER(QString, _("str"));
        bool load(handle src, bool) {
            if (!src) return false;
            PyObject *source = src.ptr();
            if (PyUnicode_Check(source)) {
                 Py_ssize_t size;
                 const char *ptr = PyUnicode_AsUTF8AndSize(source, &size);
                 if (!ptr) return false;
                 value = QString::fromUtf8(ptr, (int)size);
                 return true;
            }
            return false;
        }
        static handle cast(QString const &src, return_value_policy /* policy */, handle /* parent */) {
            QByteArray utf8 = src.toUtf8();
            return PyUnicode_FromStringAndSize(utf8.data(), utf8.size());
        }
    };
}}

// Wrapper for MindVisionCamera to support callbacks
class PyMindVisionCamera : public MindVisionCamera {
public:
    using MindVisionCamera::MindVisionCamera;

    void registerFrameCallback(py::function callback) {
        m_frameCallback = callback;
        disconnect(this, &MindVisionCamera::frameReady, nullptr, nullptr);
        
        connect(this, &MindVisionCamera::frameReady, this, [this](QImage img) {
            if (m_frameCallback) {
                py::gil_scoped_acquire acquire;
                try {
                    // Extract data: width, height, bytesPerLine, format, data
                    py::bytes data((const char*)img.bits(), img.sizeInBytes());
                    m_frameCallback(img.width(), img.height(), img.bytesPerLine(), (int)img.format(), data);
                } catch (py::error_already_set &e) {
                    qDebug() << "Python error in frame callback:" << e.what();
                }
            }
        });
    }

    void registerFpsCallback(py::function callback) {
        m_fpsCallback = callback;
        disconnect(this, &MindVisionCamera::fpsChanged, nullptr, nullptr);
        
        connect(this, &MindVisionCamera::fpsChanged, this, [this](double fps) {
             py::gil_scoped_acquire acquire;
             if (m_fpsCallback) {
                 try {
                     m_fpsCallback(fps);
                 } catch (py::error_already_set &e) {
                     qDebug() << "Python error in fps callback:" << e.what();
                 }
             }
        });
    }
    
    void registerErrorCallback(py::function callback) {
        m_errorCallback = callback;
        disconnect(this, &MindVisionCamera::errorOccurred, nullptr, nullptr);
        
        connect(this, &MindVisionCamera::errorOccurred, this, [this](QString msg) {
             py::gil_scoped_acquire acquire;
             if (m_errorCallback) {
                 try {
                     m_errorCallback(msg);
                 } catch (py::error_already_set &e) {
                     qDebug() << "Python error in error callback:" << e.what();
                 }
             }
        });
    }

private:
    py::function m_frameCallback;
    py::function m_fpsCallback;
    py::function m_errorCallback;
};

// Wrapper for VideoThread
class PyVideoThread : public VideoThread {
public:
    using VideoThread::VideoThread;
    
    void addFrameBytes(int width, int height, int bytesPerLine, int format, py::bytes data) {
         std::string s = data; 
         // Create QImage from data. Deep copy for the thread queue.
         QImage img((const uchar*)s.data(), width, height, bytesPerLine, (QImage::Format)format);
         addFrame(img.copy());
    }
};

PYBIND11_MODULE(_mindvision_qobject_py, m) {
    m.doc() = "pybind11 wrapper for MindVision QObject library"; 

    py::class_<QObject>(m, "QObject");

    // Bind PyMindVisionCamera but expose as MindVisionCamera
    py::class_<PyMindVisionCamera, QObject>(m, "MindVisionCamera")
        .def(py::init<QObject *>(), py::arg("parent") = nullptr)
        .def("open", &MindVisionCamera::open)
        .def("close", &MindVisionCamera::close)
        .def("start", &MindVisionCamera::start)
        .def("stop", &MindVisionCamera::stop)
        .def("setAutoExposure", &MindVisionCamera::setAutoExposure)
        .def("setExposureTime", &MindVisionCamera::setExposureTime)
        .def("setAnalogGain", &MindVisionCamera::setAnalogGain)
        .def("setAeTarget", &MindVisionCamera::setAeTarget)
        .def("getAutoExposure", &MindVisionCamera::getAutoExposure)
        .def("getExposureTime", &MindVisionCamera::getExposureTime)
        .def("getAnalogGain", &MindVisionCamera::getAnalogGain)
        .def("getAeTarget", &MindVisionCamera::getAeTarget)
        .def("getExposureTimeStep", &MindVisionCamera::getExposureTimeStep)
        .def("getExposureTimeRange", [](PyMindVisionCamera &self) {
            double minMs, maxMs;
            self.getExposureTimeRange(minMs, maxMs);
            return py::make_tuple(minMs, maxMs);
        })
        .def("getAnalogGainRange", [](PyMindVisionCamera &self) {
            int min, max;
            self.getAnalogGainRange(min, max);
            return py::make_tuple(min, max);
        })
        .def("setRoi", &MindVisionCamera::setRoi)
        .def("setTriggerMode", &MindVisionCamera::setTriggerMode)
        .def("setTriggerCount", &MindVisionCamera::setTriggerCount)
        .def("setTriggerDelay", &MindVisionCamera::setTriggerDelay)
        .def("setTriggerInterval", &MindVisionCamera::setTriggerInterval)
        .def("setExternalTriggerSignalType", &MindVisionCamera::setExternalTriggerSignalType)
        .def("setExternalTriggerJitterTime", &MindVisionCamera::setExternalTriggerJitterTime)
        .def("setExternalTriggerShutterMode", &MindVisionCamera::setExternalTriggerShutterMode)
        .def("setStrobeMode", &MindVisionCamera::setStrobeMode)
        .def("setStrobePolarity", &MindVisionCamera::setStrobePolarity)
        .def("setStrobeDelayTime", &MindVisionCamera::setStrobeDelayTime)
        .def("setStrobePulseWidth", &MindVisionCamera::setStrobePulseWidth)
        .def("triggerSoftware", &MindVisionCamera::triggerSoftware)
        .def("registerFrameCallback", &PyMindVisionCamera::registerFrameCallback)
        .def("registerFpsCallback", &PyMindVisionCamera::registerFpsCallback)
        .def("registerErrorCallback", &PyMindVisionCamera::registerErrorCallback)
        ;

    py::class_<PyVideoThread, QObject>(m, "VideoThread")
        .def(py::init<QObject *>(), py::arg("parent") = nullptr)
        .def("startRecording", &VideoThread::startRecording,
             py::arg("width"), py::arg("height"), py::arg("fps"), py::arg("filename"))
        .def("stopRecording", &VideoThread::stopRecording)
        .def("addFrameBytes", &PyVideoThread::addFrameBytes)
        .def("isRunning", &VideoThread::isRunning)
        ;
}
