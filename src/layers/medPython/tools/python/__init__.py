import importlib, sys
from .customImport import installCustomImportHook


installCustomImportHook()


from .bindings import registerSettingsWidget
from .pluginManager import loadPlugins, pluginsInfo
from .console import initialize as initializeConsole


def initialize():
    registerSettingsWidget()
    loadPlugins()
    initializeConsole()


def finalize():
    pass
