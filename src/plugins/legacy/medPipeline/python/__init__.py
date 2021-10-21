import medInria as med
from .workspace import Workspace
from .selection_toolbox import SelectionToolBox
from .json_data import JSONData

med.WorkspaceFactory.instance().registerWorkspace(Workspace)
med.ToolBoxFactory.instance().registerToolBox(SelectionToolBox)
# Note: JSONData must be registered from C code for now
