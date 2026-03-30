#!/usr/bin/env python3
"""
Simple Python GUI for MindVision Camera with video recording capability.
Uses PySide6 for the GUI and the MindVisionCamera/VideoThread modules.
"""

import sys
import os
from datetime import datetime

# Add the release directory to the path
script_dir = os.path.dirname(__file__)
release_dir = os.path.join(script_dir, "release")
sys.path.insert(0, release_dir)

from PySide6.QtWidgets import (
    QApplication, QMainWindow, QLabel, QPushButton, QVBoxLayout, 
    QWidget, QMessageBox, QFileDialog, QStatusBar
)

from _mindvision_qobject_py import MindVisionCamera, VideoThread

from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QImage, QPixmap


class CameraGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        
        # Initialize camera and video thread
        self.camera = MindVisionCamera()
        self.video_thread = VideoThread()
        
        # Recording state
        self.is_recording = False
        self.current_fps = 0.0
        self.last_frame_size = None
        
        # Frame counter for status updates
        self.frame_count = 0
        
        self.init_ui()
        
    def init_ui(self):
        """Initialize the user interface."""
        self.setWindowTitle("MindVision Camera - Video Recorder")
        self.setGeometry(100, 100, 900, 700)
        
        # Central widget and layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        
        # Camera display label
        self.camera_label = QLabel("Camera feed will appear here")
        self.camera_label.setAlignment(Qt.AlignCenter)
        self.camera_label.setStyleSheet("QLabel { background-color: black; color: white; }")
        self.camera_label.setMinimumSize(640, 480)
        layout.addWidget(self.camera_label)
        
        # Control buttons
        button_layout = QVBoxLayout()
        
        self.open_button = QPushButton("Open Camera")
        self.open_button.clicked.connect(self.open_camera)
        button_layout.addWidget(self.open_button)
        
        self.record_button = QPushButton("Start Recording")
        self.record_button.clicked.connect(self.toggle_recording)
        self.record_button.setEnabled(False)
        button_layout.addWidget(self.record_button)
        
        self.close_button = QPushButton("Close Camera")
        self.close_button.clicked.connect(self.close_camera)
        self.close_button.setEnabled(False)
        button_layout.addWidget(self.close_button)
        
        layout.addLayout(button_layout)
        
        # Status bar
        self.status_bar = QStatusBar()
        self.setStatusBar(self.status_bar)
        self.status_bar.showMessage("Camera closed. Click 'Open Camera' to start.")
        
        # FPS update timer
        self.fps_timer = QTimer()
        self.fps_timer.timeout.connect(self.update_fps_display)
        self.fps_timer.start(100)  # Update every 100ms
        
    def open_camera(self):
        """Open the camera and start capturing frames."""
        if self.camera.open():
            self.camera.registerFrameViewCallback(self.on_frame)
            self.camera.registerFpsCallback(self.on_fps_change)
            self.video_thread.setFrameSource(self.camera)
            
            if self.camera.start():
                self.open_button.setEnabled(False)
                self.close_button.setEnabled(True)
                self.record_button.setEnabled(True)
                self.status_bar.showMessage("Camera opened and capturing frames.")
            else:
                QMessageBox.critical(self, "Error", "Failed to start camera capture.")
                self.camera.close()
        else:
            QMessageBox.critical(self, "Error", "Failed to open camera. Check if camera is connected.")
            
    def close_camera(self):
        """Close the camera and stop capturing."""
        if self.is_recording:
            self.toggle_recording()
            
        self.camera.stop()
        self.camera.close()
        self.video_thread.clearFrameSource()
        
        self.camera_label.clear()
        self.camera_label.setText("Camera feed will appear here")
        self.camera_label.setStyleSheet("QLabel { background-color: black; color: white; }")
        
        self.open_button.setEnabled(True)
        self.close_button.setEnabled(False)
        self.record_button.setEnabled(False)
        self.status_bar.showMessage("Camera closed.")
        
    def on_frame(self, width, height, bytes_per_line, format, data):
        """Callback for new camera frames."""
        try:
            image_format = QImage.Format(format)
        except ValueError:
            return

        self.last_frame_size = (width, height)

        # Copy the frame so it stays valid after the callback returns.
        image = QImage(data, width, height, bytes_per_line, image_format).copy()
        
        # Update the display
        if not image.isNull():
            self.frame_count += 1
            
            # Convert to QPixmap and scale to fit the label
            pixmap = QPixmap.fromImage(image)
            scaled_pixmap = pixmap.scaled(
                self.camera_label.size(), 
                Qt.KeepAspectRatio, 
                Qt.SmoothTransformation
            )
            self.camera_label.setPixmap(scaled_pixmap)
            
    def on_fps_change(self, fps):
        """Callback for FPS updates."""
        self.current_fps = fps
        
    def update_fps_display(self):
        """Update the FPS display in the status bar."""
        status_text = f"FPS: {self.current_fps:.1f} | Frames: {self.frame_count}"
        if self.is_recording:
            status_text += " | RECORDING"
        self.status_bar.showMessage(status_text)
        
    def toggle_recording(self):
        """Start or stop video recording."""
        if not self.is_recording:
            # Start recording
            file_dialog = QFileDialog()
            file_dialog.setFileMode(QFileDialog.AnyFile)
            file_dialog.setAcceptMode(QFileDialog.AcceptSave)
            file_dialog.setNameFilter("Raw RGB24 files (*.rgb);;All files (*)")
            
            # Generate default filename with timestamp
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            default_filename = f"recording_{timestamp}.rgb"
            file_dialog.selectFile(default_filename)
            
            if file_dialog.exec():
                filename = file_dialog.selectedFiles()[0]
                if not filename.lower().endswith('.rgb'):
                    filename += '.rgb'

                if self.last_frame_size is None:
                    QMessageBox.warning(self, "No Frame", "Wait for the first camera frame before starting recording.")
                    return

                width, height = self.last_frame_size
                fps = self.current_fps if self.current_fps > 0 else 30.0
                
                self.video_thread.startRecording(width, height, fps, filename)
                self.is_recording = True
                self.record_button.setText("Stop Recording")
                self.status_bar.showMessage(f"Recording started: {filename}")
            else:
                self.status_bar.showMessage("Recording cancelled.")
        else:
            # Stop recording
            self.video_thread.stopRecording()
            self.is_recording = False
            self.record_button.setText("Start Recording")
            self.status_bar.showMessage("Recording stopped.")
            
    def closeEvent(self, event):
        """Handle window close event."""
        self.close_camera()
        event.accept()


def main():
    """Main entry point."""
    app = QApplication(sys.argv)
    
    # Set application metadata
    app.setApplicationName("MindVision Camera Recorder")
    app.setOrganizationName("MicroTools")
    
    # Create and show the main window
    window = CameraGUI()
    window.show()

    # Run the application
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
