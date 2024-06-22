import serial
import serial.tools.list_ports
import time

from PySide6.QtCore import Slot, QObject, Signal, QThread
from PySide6.QtWidgets import QWidget


class PortWidget(QWidget):
    data_received = Signal(str)
    command_symbol = "$"
    commands = [("Stop", 1), ("Read Angles", 2)]

    def __init__(self):
        super().__init__()
        self.selected_port = None
        self.serial_port = None
        self.baudrate = 9600

    def serial_ports(self):
        ports = list(serial.tools.list_ports.comports())
        return [port.device for port in ports]
        # return ports

    @Slot()
    def connect_port(self, port):
        self.selected_port = port
        self.serial_port = serial.Serial(
            port=port, baudrate=self.baudrate, timeout=0.1)

        # Start the worker thread to continuously read data
        self.worker = PortWorker(self)
        self.worker.start()

    @Slot()
    def disconnect_port(self):
        self.selected_port = None
        if self.serial_port:
            try:
                self.stop_worker()
                self.serial_port.close()
                print(f"port {self.serial_port.name} closed")
            except Exception as e:
                print(f"Error closing serial port: {str(e)}")
            finally:
                self.serial_port = None

    def __del__(self):
        self.disconnect_port()

    def write(self, command):
        if self.serial_port:
            self.serial_port.write(bytes(command, "utf-8"))
            # command.encode("utf-8")

    def read(self) -> str:
        if self.serial_port:
            if self.serial_port.readline():
                return self.serial_port.readline().decode("utf-8")

    def getCommands(self):
        return [command[0] for command in self.commands]

    @Slot()
    def send_command(self, command_to_send):
        for command, value in self.commands:
            if command_to_send == command:
                c = f"{self.command_symbol}{value}"
                self.write(f"{self.command_symbol}{value}")

    def read_serial_data(self):
        while True:
            if self.serial_port:
                data = self.read()
                if data:
                    self.data_received.emit(data)

    def stop_worker(self):
        if hasattr(self, "worker") and self.worker.isRunning():
            self.worker.terminate()
            self.worker.wait()


class PortWorker(QThread):
    def __init__(self, port_widget):
        super().__init__()
        self.port_widget = port_widget

    def run(self):
        self.port_widget.read_serial_data()
