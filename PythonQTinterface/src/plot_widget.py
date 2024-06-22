from PySide6.QtCore import QDateTime, Qt, Slot
from PySide6.QtGui import QPainter
from PySide6.QtWidgets import (
    QHBoxLayout,
    QVBoxLayout,
    QHeaderView,
    QSizePolicy,
    QTableView,
    QWidget,
    QScrollBar,
)
from PySide6.QtCharts import QChart, QChartView, QLineSeries, QDateTimeAxis, QValueAxis

from table_model import CustomTableModel
from port_widget import PortWidget
import pandas as pd
import time
import math


class PlotWidget(QWidget):
    def __init__(self, data=None, port_widget: PortWidget = None):
        QWidget.__init__(self)

        self.model = CustomTableModel(data)
        self.table_view = QTableView()
        self.table_view.setModel(self.model)

        # QTableView Headers
        self.table_view.setHorizontalHeader(QHeaderView(Qt.Horizontal))
        self.table_view.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        self.x_min1 = None
        self.x_max1 = None
        self.y_min1 = None
        self.y_max1 = None

        self.x_min2 = None
        self.x_max2 = None

        self.y_min2 = 103
        self.y_max2 = 104

        # Creating QChart 1
        self.chart1 = QChart()
        self.chart1.setAnimationOptions(QChart.NoAnimation)
        self.chart1.setTitle("Theta acc (blue) and Theta estimation (green)")

        # Creating QChartView 1
        self.chart_view1 = QChartView(self.chart1)
        self.chart_view1.setRenderHint(QPainter.Antialiasing)
        self.chart_view1.setFixedSize(700, 500)

        # Creating QChart 2
        self.chart2 = QChart()
        self.chart2.setAnimationOptions(QChart.NoAnimation)
        self.chart2.setTitle("PWMd (blue) and PWMg (green)")

        # Creating QChartView 2
        self.chart_view2 = QChartView(self.chart2)
        self.chart_view2.setRenderHint(QPainter.Antialiasing)
        self.chart_view2.setFixedSize(700, 500)

        # QWidget Layout
        self.main_layout = QHBoxLayout()
        size = QSizePolicy(QSizePolicy.Preferred, QSizePolicy.Preferred)

        # Left layout for table view
        size.setHorizontalStretch(1)
        self.table_view.setSizePolicy(size)
        self.main_layout.addWidget(self.table_view)

        # Right Layout for charts
        size.setHorizontalStretch(1)
        self.chart_view1.setSizePolicy(size)
        self.chart_view2.setSizePolicy(size)
        self.main_layout.addWidget(self.chart_view1)
        self.main_layout.addWidget(self.chart_view2)

        # Set the layout to the QWidget
        self.setLayout(self.main_layout)

        self.start_time = time.perf_counter()

        self.series_list1 = []
        self.series_list2 = []
        self.axis_x1 = None
        self.axis_y1 = None
        self.axis_x2 = None
        self.axis_y2 = None

        self.x_range1 = 100  # seconds
        self.x_range_start1 = None
        self.x_range_end1 = None

        self.x_range2 = 100  # seconds
        self.x_range_start2 = None
        self.x_range_end2 = None

        if isinstance(data, pd.DataFrame) and not data.empty:
            print("it is instance of pdDataframe")
            self.model.appendRow(data)
            self.add_series("Thetaacc", chart_number=1, populate=True)
            self.add_series("Thetaestimated", chart_number=1, populate=True)
            self.add_series("PWMd", chart_number=2, populate=True)
            self.add_series("PWMg", chart_number=2, populate=True)

        if port_widget:
            print("it isnot instance of pdDataframe")
            self.add_series("Thetaacc", chart_number=1)
            self.add_series("Thetaestimated", chart_number=1)
            self.add_series("PWMd", chart_number=2)
            self.add_series("PWMg", chart_number=2)
            self.port_widget = port_widget
            self.port_widget.data_received.connect(self.update_from_serial)

        self.add_axis(chart_number=1)
        self.add_axis(chart_number=2)

    def update_row(self, row_i, series_i, chart_number):

        if (chart_number == 1):

            # Check if is number (int or float)

            t = self.model.index(row_i, 0).data()
            if not t.replace(".", "", 1).isdigit():
                return
            y1 = self.model.index(row_i, series_i+1).data()

            # Check if t is a valid number
            try:
                t = float(t)
                print(f"t :{t}")
            except ValueError:
                print(f"Invalid time value at row {
                      row_i}, series {series_i}: {t}")
                return

            # Check if y is a valid number
            try:
                y1 = float(y1)
                print(f"y1 :{y1}")
            except ValueError:
                print(f"Invalid y value at row {
                      row_i}, series {series_i}: {y1}")
                return

            self.series_list1[series_i].append(t, y1)
            self.update_ranges(t, y1, chart_number)

        if (chart_number == 2):
            # Check if is number (int or float)

            t = self.model.index(row_i, 0).data()
            if not t.replace(".", "", 1).isdigit():
                return
            y2 = self.model.index(row_i, series_i+3).data()

            # Check if t is a valid number
            try:
                t = float(t)
                print(f"t :{t}")
            except ValueError:
                print(f"Invalid time value at row {
                      row_i}, series {series_i}: {t}")
                return

            # Check if y is a valid number
            try:
                y2 = float(y2)
                print(f"y2 :{y2}")
            except ValueError:
                print(f"Invalid y value at row {
                      row_i}, series {series_i}: {y2}")
                return

            self.series_list2[series_i].append(t, y2)
            self.update_ranges(t, y2, chart_number)

    def update_series(self):
        if self.model.input_data.empty:
            return

        row_count = self.model.rowCount()
        for s_i in range(len(self.series_list1)):
            self.update_row(row_count - 1, s_i, chart_number=1)

        for s_i in range(len(self.series_list2)):
            self.update_row(row_count - 1, s_i, chart_number=2)

        self.update_axes()

    def update_axes(self):
        row_count = self.model.rowCount()
        # print(f"Row count: {row_count}")

        y_min1 = self.y_min1 - 0.5
        y_max1 = self.y_max1 + 0.5
        # print(f"y_min1: {self.y_min1}, y_max1: {self.y_max1}, updated y_min1: {
        #       y_min1}, updated y_max1: {y_max1}")
        self.axis_y1.setRange(y_min1, y_max1)

        y_min2 = self.y_min2 - 0.5
        y_max2 = self.y_max2 + 0.5
        # print(f"y_min2: {self.y_min2}, y_max2: {self.y_max2}, updated y_min2: {
        #       y_min2}, updated y_max2: {y_max2}")
        self.axis_y2.setRange(y_min2, y_max2)

        if self.x_max1 >= self.x_range_end1 - self.x_range1 * 0.1:
            self.x_range_end1 += self.x_range1 * 0.2
            # print(f"x_range_end1 updated: {self.x_range_end1}")
            if self.x_range_end1 - self.x_range_start1 > self.x_range1:
                self.x_range_start1 = self.x_range_end1 - self.x_range1
                # print(f"x_range_start1 updated: {self.x_range_start1}")

        new_x_min_datetime1 = QDateTime.fromMSecsSinceEpoch(
            int(self.x_range_start1))
        new_x_max_datetime1 = QDateTime.fromMSecsSinceEpoch(
            int(self.x_range_end1))
        # print(f"x_range_start1: {self.x_range_start1}, x_range_end1: {self.x_range_end1}, new_x_min_datetime1: {
        #       new_x_min_datetime1}, new_x_max_datetime1: {new_x_max_datetime1}")
        self.axis_x1.setRange(new_x_min_datetime1, new_x_max_datetime1)

        if self.x_max2 >= self.x_range_end2 - self.x_range2 * 0.1:
            self.x_range_end2 += self.x_range2 * 0.2
            # print(f"x_range_end2 updated: {self.x_range_end2}")
            if self.x_range_end2 - self.x_range_start2 > self.x_range2:
                self.x_range_start2 = self.x_range_end2 - self.x_range2
                # print(f"x_range_start2 updated: {self.x_range_start2}")

        new_x_min_datetime2 = QDateTime.fromMSecsSinceEpoch(
            int(self.x_range_start2))
        new_x_max_datetime2 = QDateTime.fromMSecsSinceEpoch(
            int(self.x_range_end2))
        # print(f"x_range_start2: {self.x_range_start2}, x_range_end2: {self.x_range_end2}, new_x_min_datetime2: {
        #   new_x_min_datetime2}, new_x_max_datetime2: {new_x_max_datetime2}")
        self.axis_x2.setRange(new_x_min_datetime2, new_x_max_datetime2)

    def add_series(self, name: str, chart_number, populate=False):
        series = QLineSeries()
        series.setName(name)

        if chart_number == 1:
            series_i1 = len(self.series_list1)

            self.series_list1.append(series)
        elif chart_number == 2:
            series_i2 = len(self.series_list2)
            self.series_list2.append(series)

        if populate:
            for row_i in range(self.model.rowCount()):
                if chart_number == 1:
                    self.update_row(row_i, series_i1, chart_number)
                elif chart_number == 2:
                    self.update_row(row_i, series_i2, chart_number)

        if chart_number == 1:
            self.chart1.addSeries(series)
        elif chart_number == 2:
            self.chart2.addSeries(series)

        color_name = series.pen().color().name()
        self.model.color = f"{color_name}"

    def update_ranges(self, x, y, chart_number):
        if chart_number == 1:
            if not self.x_min1 or x < self.x_min1:
                self.x_min1 = x
            if not self.x_max1 or x > self.x_max1:
                self.x_max1 = x
            if not self.y_min1 or y < self.y_min1:
                self.y_min1 = y - 1
            if not self.y_max1 or y > self.y_max1:
                self.y_max1 = y + 1
            if not self.x_range_start1:
                self.x_range_start1 = self.x_min1
            if not self.x_range_end1:
                self.x_range_end1 = self.x_max1

        if chart_number == 2:
            if not self.x_min2 or x < self.x_min2:
                self.x_min2 = x
            if not self.x_max2 or x > self.x_max2:
                self.x_max2 = x
            if not self.y_min2 or y < self.y_min2:
                self.y_min2 = y - 1
            if not self.y_max2 or y > self.y_max2:
                self.y_max2 = y + 1
            if not self.x_range_start2:
                self.x_range_start2 = self.x_min2
            if not self.x_range_end2:
                self.x_range_end2 = self.x_max2

    def add_axis(self, chart_number):
        if chart_number == 1:
            x_min1 = self.x_min1
            if not self.x_min1:
                x_min1 = 0
            x_max1 = self.x_max1
            if not self.x_max1:
                x_max1 = 0
            y_min1 = self.y_min1
            if not self.y_min1:
                y_min1 = 0
            y_max1 = self.y_max1
            if not self.y_max1:
                y_max1 = 0
        elif chart_number == 2:
            x_min2 = self.x_min2
            if not self.x_min2:
                x_min2 = 0
            x_max2 = self.x_max2
            if not self.x_max2:
                x_max2 = 0
            y_min2 = self.y_min2
            if not self.y_min2:
                y_min2 = 0
            y_max2 = self.y_max2
            if not self.y_max2:
                y_max2 = 0
        if chart_number == 1:
            axis_x1 = QDateTimeAxis()
            x_min_datetime1 = QDateTime.fromMSecsSinceEpoch(int(x_min1))
            x_max_datetime1 = QDateTime.fromMSecsSinceEpoch(int(x_max1))
            axis_x1.setRange(x_min_datetime1, x_max_datetime1)
            axis_x1.setTickCount(1)
            axis_x1.setFormat("ss.zzz")
            axis_x1.setTitleText("Time")

            axis_y1 = QValueAxis()
            axis_y1.setTickCount(1)
            axis_y1.setRange(y_min1, y_max1)
            axis_y1.setLabelFormat("%.2f")
            axis_y1.setTitleText("Angles (in degrees)")
        elif chart_number == 2:
            axis_x2 = QDateTimeAxis()
            x_min_datetime2 = QDateTime.fromMSecsSinceEpoch(int(x_min2))
            x_max_datetime2 = QDateTime.fromMSecsSinceEpoch(int(x_max2))
            axis_x2.setRange(x_min_datetime2, x_max_datetime2)
            axis_x2.setTickCount(1)
            axis_x2.setFormat("ss.zzz")
            axis_x2.setTitleText("Time")

            axis_y2 = QValueAxis()
            axis_y2.setTickCount(1)
            axis_y2.setRange(y_min2, y_max2)
            axis_y2.setLabelFormat("%.2f")
            axis_y2.setTitleText("Throttle (in microseconds)")

        if chart_number == 1:
            self.chart1.addAxis(axis_x1, Qt.AlignBottom)
            self.chart1.addAxis(axis_y1, Qt.AlignLeft)
            self.axis_x1 = axis_x1
            self.axis_y1 = axis_y1
            for i in range(len(self.series_list1)):
                self.series_list1[i].attachAxis(self.axis_x1)
                self.series_list1[i].attachAxis(self.axis_y1)
        elif chart_number == 2:
            self.chart2.addAxis(axis_x2, Qt.AlignBottom)
            self.chart2.addAxis(axis_y2, Qt.AlignLeft)
            self.axis_x2 = axis_x2
            self.axis_y2 = axis_y2
            for i in range(len(self.series_list2)):
                self.series_list2[i].attachAxis(self.axis_x2)
                self.series_list2[i].attachAxis(self.axis_y2)

    def clear_data(self):
        row_count = self.model.rowCount()
        for i in range(row_count):
            # Remove the first row until there are no more rows
            self.model.removeRow(0)

        self.chart1.removeAllSeries()
        self.chart2.removeAllSeries()

        self.x_min1 = None
        self.x_max1 = None

        self.y_min1 = None
        self.y_max1 = None

        self.x_min2 = None
        self.x_max2 = None
        self.y_min2 = None
        self.y_max2 = None
        self.series_list1.clear()
        self.series_list2.clear()
        self.axis_x1 = None
        self.axis_y1 = None
        self.axis_x2 = None
        self.axis_y2 = None

    @Slot()
    def update_from_serial(self, data):
        # High-resolution relative time in seconds
        current_time = time.perf_counter() - self.start_time

        data = data.split(";")
        if len(data) == 4:
            # Insert relative time as the first element
            data.insert(0, current_time)
            self.model.appendRow(data)
            self.update_series()

        if len(data) == 5:  # Assuming 5 data values (T, theta, theta_estim, PWMg, PWMd)
            # print(data[0])
            # data = data[1:5]
            # data.insert(0, current_time)
            self.model.appendRow(data)
            self.update_series()
