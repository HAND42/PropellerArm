import serial
import time

from PySide6.QtCore import Qt, Slot
from PySide6.QtWidgets import (
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QTextEdit,
    QLineEdit,
    QPushButton,
)
from PySide6.QtGui import QFont, QColor


class TerminalWidget(QWidget):
    def __init__(self, port_widget):
        super().__init__()

        self.port_widget = port_widget
        self.init_ui()

        self.port_widget.data_received.connect(self.print_)

    def init_ui(self):
        layout = QVBoxLayout()

        self.output_text = QTextEdit()
        self.output_text.setReadOnly(True)

        # Set line spacing and word wrap behavior
        self.output_text.setLineWrapMode(QTextEdit.NoWrap)
        self.output_text.setLineWrapColumnOrWidth(0)
        self.output_text.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)

        text_color = "#F99417"
        background_color = "#363062"
        self.output_text.setStyleSheet(
            f"QTextEdit {{ color: {text_color}; background-color: {background_color}; }}"
        )

        font = QFont("Monospace")
        font.setPointSize(12)
        font.setBold(True)
        self.output_text.setFont(font)

        layout.addWidget(self.output_text)

        input_layout = QHBoxLayout()

        send_button = QPushButton("Send")
        send_button.setShortcut("Return")
        send_button.clicked.connect(self.send_command)
        input_layout.addWidget(send_button)

        clear_button = QPushButton("Clear")
        clear_button.clicked.connect(self.clear_output)
        input_layout.addWidget(clear_button)

        self.input_line = QLineEdit()
        self.input_line.setPlaceholderText("Enter command...")
        input_layout.addWidget(self.input_line)

        layout.addLayout(input_layout)
        self.setLayout(layout)

    def clear_output(self):
        self.output_text.clear()

    @Slot()
    def print_(self, line: str):
        self.output_text.append(line)

    @Slot()
    def send_command(self):
        command = self.input_line.text()
        if not command:
            return

        try:
            # Send the command to the serial port
            self.port_widget.write(command)
            self.print_(f"> {command}")

            # Clear the input line
            self.input_line.clear()

        except Exception as e:
            self.print_(f"Error: {str(e)}")
