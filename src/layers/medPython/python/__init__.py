import sys
from console import *
from output_stream import *
from manager import *

sys.stdout = PythonOutputStream()
sys.stderr = PythonOutputStream()

PythonManager().getConsole().listenTo(sys.stdout)
PythonManager().getConsole().listenTo(sys.stderr)

logo = """
                            888 8888888                  d8b
                            888   888                    Y8P
                            888   888
88888b.d88b.   .d88b.   .d88888   888   88888b.  888d888 888  8888b.
888 "888 "88b d8P  Y8b d88" 888   888   888 "88b 888P"   888     "88b
888  888  888 88888888 888  888   888   888  888 888     888 .d888888
888  888  888 Y8b.     Y88b 888   888   888  888 888     888 888  888
888  888  888  "Y8888   "Y88888 8888888 888  888 888     888 "Y888888
"""

print("Python version:")
print(sys.version)
print(logo)
print("This is the Python console for medInria.\n\n")
