from PySide6.QtCore import Slot
from PySide6.QtGui import QAction, QKeySequence, QIcon
from PySide6.QtWidgets import QMainWindow, QStackedWidget, QMenu
from port_widget import PortWidget
from terminal_widget import TerminalWidget
from plot_widget import PlotWidget
from csv_loader import CSVFileLoader


class MainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.setWindowTitle("Accelerometer viewer")
        self.setWindowIcon(QIcon("data/accelerometer-sensor.png"))

        # Create a stacked widget as the central widget
        self.central_stack = QStackedWidget()
        self.setCentralWidget(self.central_stack)

        # Widgets
        self.port_widget = PortWidget()

        self.serial_plot_widget = PlotWidget(port_widget=self.port_widget)
        self.csv_plot_widget = None

        self.terminal_widget = TerminalWidget(self.port_widget)
        self.central_stack.addWidget(self.serial_plot_widget)
        self.central_stack.addWidget(self.terminal_widget)

        # Menu
        self.menu = self.menuBar()

        # File menu
        self.file_menu = self.menu.addMenu("File")

        open_serial_plot_action = QAction("Open Serial Plot", self)
        open_serial_plot_action.triggered.connect(self.open_serial_plot)
        self.file_menu.addAction(open_serial_plot_action)

        open_new_csv_action = QAction("Open new CSV", self)
        open_new_csv_action.triggered.connect(lambda: self.open_csv_plot(True))
        self.file_menu.addAction(open_new_csv_action)

        self.open_csv_action = QAction("Open CSV", self)
        self.open_csv_action.triggered.connect(self.open_csv_plot)
        self.open_csv_action.setDisabled(True)
        self.file_menu.addAction(self.open_csv_action)

        # View menu
        self.view_menu = self.menu.addMenu("View")

        # Plot view
        show_plot_action = QAction("Plot", self)
        show_plot_action.triggered.connect(self.show_plot_widget)
        self.view_menu.addAction(show_plot_action)

        # Terminal view
        show_terminal_action = QAction("Terminal", self)
        show_terminal_action.triggered.connect(self.show_terminal_widget)
        self.view_menu.addAction(show_terminal_action)

        # Settings menu
        self.serial_menu = self.menu.addMenu("Serial")

        # Select port
        self.port_menu = QMenu("Select Port", self)
        self.serial_menu.addMenu(self.port_menu)
        self.port_menu.aboutToShow.connect(self.show_ports)

        # Commands menu
        self.command_menu = QMenu("Commands", self)
        self.serial_menu.addMenu(self.command_menu)

        # Commands
        for command in self.port_widget.getCommands():
            command_action = QAction(command, self)
            command_action.setCheckable(True)
            command_action.triggered.connect(self.set_command)
            self.command_menu.addAction(command_action)

        # Exit QAction
        exit_action = QAction("Exit", self)
        exit_action.setShortcut(QKeySequence.Quit)
        exit_action.triggered.connect(self.close)
        self.serial_menu.addAction(exit_action)

        # Status Bar
        self.status = self.statusBar()
        self.status.showMessage("Data loaded and plotted")

        # Window dimensions
        geometry = self.screen().availableGeometry()
        self.setFixedSize(geometry.width() * 1.1, geometry.height() * 0.7)

    def hide_all(self):
        for i in range(self.central_stack.count()):
            self.central_stack.widget(i).hide()

    @Slot()
    def open_serial_plot(self):
        self.central_stack.setCurrentWidget(self.serial_plot_widget)

    @Slot()
    def open_csv_plot(self, open_new=False):
        if open_new:
            data = CSVFileLoader.load_csv_data()
            self.csv_plot_widget = plot_widget = PlotWidget(data=data)
            self.open_csv_action.setEnabled(True)

            self.central_stack.addWidget(plot_widget)
            self.central_stack.setCurrentWidget(plot_widget)
            return

        if self.csv_plot_widget:
            self.central_stack.setCurrentWidget(self.csv_plot_widget)
            # self.hide_all()
            # plot_widget.show()

    @Slot()
    def show_plot_widget(self):
        self.central_stack.setCurrentWidget(self.serial_plot_widget)

        # self.hide_all()
        # self.plot_widget.show()

    @Slot()
    def show_terminal_widget(self):
        self.central_stack.setCurrentIndex(
            self.central_stack.indexOf(self.terminal_widget)
        )
        self.hide_all()
        self.terminal_widget.show()

    @Slot()
    def show_ports(self):
        for port_action in self.port_menu.actions():
            port_action.deleteLater()

        ports = self.port_widget.serial_ports()

        if not ports:
            port_action = QAction("No ports found...", self)
            port_action.setDisabled(True)
            self.port_menu.addAction(port_action)
            return

        for port_name in ports:
            port_action = QAction(port_name, self)
            port_action.triggered.connect(self.select_port)
            port_action.setCheckable(True)

            if port_name == self.port_widget.selected_port:
                port_action.setChecked(True)

            self.status.showMessage(f"Connected to {port_name}")

            self.port_menu.addAction(port_action)

    @Slot()
    def select_port(self):
        port_action = self.sender()
        if port_action.isChecked():
            port_name = port_action.text()
            self.port_widget.connect_port(port_name)
            port_action.setChecked(True)
        else:
            self.port_widget.disconnect_port()
            port_action.setChecked(False)

    @Slot()
    def set_command(self):
        command_action = self.sender()
        for action in self.command_menu.actions():
            action.setChecked(False)
        command_action.setChecked(True)
        self.port_widget.send_command(command_action.text())
