import weakref
import medInria as med
import qt
from .selection_toolbox import SelectionToolBox


@med.workspace_interface("Pipeline", "Execute Python pipelines", "Methodology")
class Workspace(med.Workspace):

    def __init__(self, parent=None):
        weakSelf = weakref.ref(self)
        self.__class__.instance = lambda : weakSelf
        super().__init__(parent)
        self.removeToolBox(self.getMouseInteractionToolBox())
        self.removeToolBox(self.getNavigatorToolBox())
        self.removeToolBox(self.getLayersToolBox())
        self.selectionToolBox = SelectionToolBox()
        self.selectionToolBox.setTitle(self.selectionToolBox.name())
        self.addToolBox(self.selectionToolBox)
        self._createMainDropContainer()
        self.tabbedViewContainers().lockTabs()
        self.activeStudy = None

    def _createMainDropContainer(self):
        self.mainDropContainer = self.tabbedViewContainers().addContainerInTabNamed("Pipeline")
        self.mainDropContainer.setMultiLayered(False)
        self.mainDropContainer.setUserSplittable(False)
        self.mainDropContainer.setUserOpenable(False)
        self.mainDropContainer.setClosingMode(med.ViewContainer.CLOSE_BUTTON_HIDDEN)
        #viewContainer.connect('containerSelected(QUuid)', )

        dropSite = med.DropSite()
        dropSite.setStyleSheet("background-color: rgba(255, 255, 255, 0);")
        dropSite.setSizePolicy(qt.QSizePolicy.Expanding, qt.QSizePolicy.Preferred)
        dropSite.setText("Drag'n drop an existing study here\nOR\nCreate a new one with the button on the top right")
        dropSite.acceptDrops()
        dropSite.connect('objectDropped', self.loadData)
        
        self.mainDropContainer.setDefaultWidget(dropSite)

    def loadData(self, dataIndex):
        if dataIndex.isValidForSeries():
            self.mainDropContainer.addData(dataIndex)
        elif dataIndex.isValidForStudy():
            seriesList = med.DataManager.instance().getSeriesListFromStudy(dataIndex)
            meshList = []
            self.createMeshContainer()
            for seriesIndex in seriesList:
                seriesType = med.DataManager.instance().getDataType(seriesIndex)
                if seriesType:
                    print(seriesType.className())
                    if seriesType.inherits(med.AbstractMeshData.staticMetaObject):
                        self.meshContainer.addData(med.DataManager.instance().retrieveData(seriesIndex))

    def createMeshContainer(self):
        self.meshContainer = self.tabbedViewContainers().addContainerInTabNamed("Meshes")
        self.meshContainer.setUserSplittable(False)
        self.meshContainer.setClosingMode(med.ViewContainer.CLOSE_BUTTON_HIDDEN)
