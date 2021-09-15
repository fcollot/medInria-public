import medInria as med
from .workspace import Workspace
from .selection_toolbox import SelectionToolBox


med.WorkspaceFactory.instance().registerWorkspace(Workspace)
med.ToolBoxFactory.instance().registerToolBox(SelectionToolBox)
