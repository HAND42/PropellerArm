# https://doc.qt.io/qtforpython-6/tutorials/datavisualize/index.html

import sys
import argparse
import pandas as pd

from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QDateTime, QTimeZone
from main_window import MainWindow


def read_data(fname):
    # Read the CSV content
    df = pd.read_csv(fname, sep=";", header=0)
    return df


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()

    window.show()
    sys.exit(app.exec())
