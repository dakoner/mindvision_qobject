import sys
import os

# Add the directory containing the generated module to sys.path
# Assuming the module is built into a 'python_module' directory relative to the project root
script_dir = os.path.dirname(__file__)
sys.path.insert(0, os.path.join(script_dir, "release"))
sys.path.insert(0, os.path.join(script_dir, "debug"))
module_path = os.path.join(script_dir, "python_module")
sys.path.insert(0, module_path)

try:
    from _mindvision_qobject_py import MindVisionCamera, VideoThread
    print("Successfully imported _mindvision_qobject_py module.")

    # Test MindVisionCamera
    print("\nTesting MindVisionCamera:")
    camera = MindVisionCamera()
    print(f"MindVisionCamera instance created: {camera}")

    # You would typically call camera.open() here, but it requires a physical camera
    # and might block or fail if not present. We'll just call some setters/getters.
    print(f"Initial AutoExposure: {camera.getAutoExposure()}")
    camera.setAutoExposure(True)
    print(f"AutoExposure after setting to True: {camera.getAutoExposure()}")

    min_exp, max_exp = camera.getExposureTimeRange()
    print(f"Exposure Time Range: min={min_exp}ms, max={max_exp}ms")

    # Test VideoThread
    print("\nTesting VideoThread:")
    video_thread = VideoThread()
    print(f"VideoThread instance created: {video_thread}")

    # These methods are for controlling recording; without a camera feeding frames,
    # calling them won't do much, but we can verify they are callable.
    print("Attempting to call startRecording (dummy parameters)...")
    video_thread.startRecording(640, 480, 30.0, "test_video.avi")
    print("startRecording called.")

    print("Attempting to call stopRecording...")
    video_thread.stopRecording()
    print("stopRecording called.")

    print("\nPython wrapper test completed successfully (syntactically).")

except ImportError as e:
    print(f"Error importing _mindvision_qobject_py: {e}")
    print("Please ensure the C++ module is built and the 'python_module' directory is in sys.path.")
    print(f"Attempted module path: {module_path}")
except Exception as e:
    print(f"An unexpected error occurred: {e}")
