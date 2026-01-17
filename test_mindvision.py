import sys
import os

# Add the directory containing the generated module to sys.path
# Assuming the module is built into a 'python_module' directory relative to the project root
script_dir = os.path.dirname(__file__)
release_dir = os.path.join(script_dir, "release")
sys.path.insert(0, release_dir)

os.add_dll_directory(release_dir)
# Add Qt bin directory
os.add_dll_directory(r"C:\Qt\6.10.1\msvc2022_64\bin")
# Add MindVision SDK directory
os.add_dll_directory(r"C:\Program Files (x86)\MindVision\SDK\X64")


from _mindvision_qobject_py import MindVisionCamera, VideoThread
print("Successfully imported _mindvision_qobject_py module.")

# Test MindVisionCamera
print("\nTesting MindVisionCamera:")
camera = MindVisionCamera()
print(f"MindVisionCamera instance created: {camera}")

camera.open()
# You would typically call camera.open() here, but it requires a physical camera
# and might block or fail if not present. We'll just call some setters/getters.
print(f"Initial AutoExposure: {camera.getAutoExposure()}")
camera.setAutoExposure(True)
print(f"AutoExposure after setting to True: {camera.getAutoExposure()}")
camera.setAutoExposure(False)

min_exp, max_exp = camera.getExposureTimeRange()
print(f"Exposure Time Range: min={min_exp}ms, max={max_exp}ms")


min_exp, max_exp = camera.getExposureTimeRange()
step_exp = camera.getExposureTimeStep()

print(f"Exposure Time Range: {min_exp} ms to {max_exp} ms")
print(f"Exposure Time Step: {step_exp} ms")

if step_exp > 0:
    print("Calculating and verifying ALL valid exposure times (this may take a moment)...")
    current = min_exp
    match_count = 0
    # Ensure manual exposure
    camera.setAutoExposure(False)
    
    while current <= max_exp:
        camera.setExposureTime(current)
        
        actual = camera.getExposureTime()
        if abs(actual - current) > 0.001:
             print(f"    MISMATCH: Set {current:.4f}, Got {actual:.4f}")
        else:
             match_count += 1

        current += step_exp

    print(f"Verification complete. Total valid (matched) exposure values: {match_count}")
else:
    print("Step is 0 or invalid, cannot calculate steps.")

        
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

print(f"Is running: {video_thread.isRunning()}")

print("\nPython wrapper test completed successfully (syntactically).")
