import importlib, sys
from .customImport import installCustomImportHook as _installCustomImportHook

_installCustomImportHook()

from .bindings import registerSettingsWidget as _registerSettingsWidget
from .pluginManager import loadPlugins, pluginsInfo
from .console import Console


def initialize():
    _registerSettingsWidget()
