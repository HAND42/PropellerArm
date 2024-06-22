from PySide6.QtCore import Qt, QAbstractTableModel, QModelIndex
from PySide6.QtGui import QColor
from datetime import datetime
import pandas as pd


class CustomTableModel(QAbstractTableModel):
    def __init__(self, data=None):
        QAbstractTableModel.__init__(self)

        self.load_data(data)

    def load_data(self, data=None):
        self.column_count = 5
        self.row_count = 0

        if isinstance(data, pd.DataFrame) and not data.empty:
            self.input_data = data
        else:
            empty_dic = {"T": [], "Thetaacc": [],
                         "Thetaestim": [], "PWMg": [], "PWMd": []}
            data = pd.DataFrame(empty_dic)
            self.input_data = data

    def rowCount(self, parent=QModelIndex()):
        self.row_count = len(self.input_data)
        return self.row_count

    def columnCount(self, parent=QModelIndex()):
        return self.column_count

    def headerData(self, section, orientation, role):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Horizontal:
            return ("T", "Thetaacc", "Thetaestim", "PWMg", "PWMd")[section]
        else:
            return f"{section}"

    def data(self, index, role=Qt.DisplayRole):
        column = index.column()
        row = index.row()

        if role == Qt.DisplayRole:
            value = self.input_data.iloc[row, column]
            value = float(value)
            return f"{value:.2f}"

        elif role == Qt.BackgroundRole:
            return QColor(Qt.white)
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight

        return None

    def appendRow(self, row_data):
        # Check if the incoming row_data is a dictionary or a list of values
        correct_format = False
        if isinstance(row_data, pd.DataFrame):
            correct_format = True
            self.input_data = pd.concat(
                [self.input_data, row_data], ignore_index=True)

        elif isinstance(row_data, list):
            print(self.column_count)
            if len(row_data) == self.column_count:
                correct_format = True

                formatted_row = f"{row_data[0]};{row_data[1]};{
                    row_data[2]};{row_data[3]};{row_data[4]}"

                # Append the formatted row to the CSV file
                with open("input_data.csv", 'a') as file:
                    file.write(formatted_row)

                new_row = {}
                new_row["T"] = row_data[0]
                new_row["Thetaacc"] = row_data[1]
                new_row["Thetaestim"] = row_data[2]
                new_row["PWMg"] = row_data[3]
                new_row["PWMd"] = row_data[4]

                self.input_data = pd.concat(
                    [self.input_data, pd.DataFrame(new_row, index=[0])],
                    ignore_index=True,
                )

                # self.input_data = pd.concat(
                #     [self.input_data, pd.DataFrame([new_row])],
                #     ignore_index=True,
                # )

            else:
                print("Row data does not have the correct number of columns.")
        else:
            print(
                f"Invalid row data format. Should be pd.DataFrame or list but is {
                    type(row_data)}"
            )

        if not correct_format:
            return

        # Update self.row_count
        self.rowCount()

        # Emit signals to inform the view that the model has changed
        top_left = self.index(self.row_count - 1, 0)
        bottom_right = self.index(self.row_count - 1, self.column_count - 1)
        self.dataChanged.emit(top_left, bottom_right)
