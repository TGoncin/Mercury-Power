import sys
import os
from PIL import Image

def resource_path(relative_path):
    """Ruta correcta dentro del exe PyInstaller"""
    try:
        base_path = sys._MEIPASS
    except AttributeError:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

asa_log_path = resource_path("asa_log.dbs")
params_path = resource_path("params.json")
internal_path = resource_path(os.path.join("Mercury", "_internal"))