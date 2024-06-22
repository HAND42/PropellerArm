from PySide6.QtWidgets import QFileDialog, QApplication
import pandas as pd


class CSVFileLoader:
    @staticmethod
    def open_csv_file():
        options = QFileDialog.Options()
        options |= QFileDialog.ReadOnly
        file_dialog = QFileDialog()
        file_dialog.setNameFilter("CSV Files (*.csv)")
        file_dialog.setViewMode(QFileDialog.List)
        file_dialog.setFileMode(QFileDialog.ExistingFiles)

        if file_dialog.exec_():
            selected_files = file_dialog.selectedFiles()
            if selected_files:
                return selected_files[0]  # Return the first selected file

        return None

    @staticmethod
    def read_csv_data(file_path):
        dtype = {
            "T": float,
            "Ax": float,
            "Ay": float,
            "Az": float,
        }
        data = pd.read_csv(file_path, sep=";", dtype=dtype)
        return data

    @staticmethod
    def load_csv_data():
        file_path = CSVFileLoader.open_csv_file()
        if file_path:
            return CSVFileLoader.read_csv_data(file_path)
        return None
