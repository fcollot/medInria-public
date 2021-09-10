import sys
from . import qt_bindings as qt
from . import medInria_bindings as med
from .console import Console


console = None


def createConsole(title, size):
    global console
    console = Console(title, size=size)
    mainWindow = qt.qApp().getProperty('MainWindow')
    mainWindow.connect('destroyed', lambda _ : destroyConsole())
    return console


def destroyConsole():
    global console
    console.deleteLater()
    console = None
