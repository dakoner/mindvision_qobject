#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h> // For std::function if needed, or signals/slots

#include "MindVisionCamera.h"
#include "VideoThread.h"
#include <QObject> // Required for Q_OBJECT classes

namespace py = pybind11;

// Define a trampoline class for MindVisionCamera to handle virtual functions
// if any (though none are immediately apparent for this class in .h) and Q_OBJECT.
// For direct binding of non-virtual methods, a trampoline is not strictly necessary,
// but if signals/slots are to be used from Python, a QObject-aware binding is needed.

// For simplicity, we'll start with direct binding of methods.
// To handle Q_OBJECT signals/slots properly, one would typically use a library
// like 'python-qtbind' or 'PySide/PyQt' or manually create wrappers that emit
// Python events when C++ signals are fired. For a pure pybind11 approach
// without external Qt binding libraries, direct signal/slot connection from Python
// is not straightforward.

PYBIND11_MODULE(_mindvision_qobject_py, m) {
    m.doc() = "pybind11 wrapper for MindVision QObject library"; // optional module docstring

    py::class_<MindVisionCamera, QObject>(m, "MindVisionCamera")
        .def(py::init<QObject *>(), py::arg("parent") = nullptr)
        .def("open", &MindVisionCamera::open)
        .def("close", &MindVisionCamera::close)
        .def("start", &MindVisionCamera::start)
        .def("stop", &MindVisionCamera::stop)
        .def("setAutoExposure", &MindVisionCamera::setAutoExposure)
        .def("setExposureTime", &MindVisionCamera::setExposureTime)
        .def("setAnalogGain", &MindVisionCamera::setAnalogGain)
        .def("getAutoExposure", &MindVisionCamera::getAutoExposure)
        .def("getExposureTime", &MindVisionCamera::getExposureTime)
        .def("getAnalogGain", &MindVisionCamera::getAnalogGain)
        .def("getExposureTimeRange", [](MindVisionCamera &self) {
            double minMs, maxMs;
            self.getExposureTimeRange(minMs, maxMs);
            return py::make_tuple(minMs, maxMs);
        })
        .def("getAnalogGainRange", [](MindVisionCamera &self) {
            int min, max;
            self.getAnalogGainRange(min, max);
            return py::make_tuple(min, max);
        })
        .def("setRoi", &MindVisionCamera::setRoi)
        // Signals are not directly bindable to Python callables via pybind11 alone
        // without additional Qt-specific pybind11 extensions or manual wrappers.
        // For demonstration, we'll omit direct signal binding for now.
        ;

    py::class_<VideoThread, QObject>(m, "VideoThread")
        .def(py::init<QObject *>(), py::arg("parent") = nullptr)
        .def("startRecording", &VideoThread::startRecording,
             py::arg("width"), py::arg("height"), py::arg("fps"), py::arg("filename"))
        .def("stopRecording", &VideoThread::stopRecording)
        // addFrame takes QImage, which needs special handling. Skipping for now for simplicity.
        // .def("addFrame", &VideoThread::addFrame)
        ;
}