import qt
import medInria as med


@med.toolbox_interface("Pipeline selection", "Pipeline selection tool box", [])
class SelectionToolBox(med.ToolBox):

    def __init__(self, parent=None):
        super().__init__(parent)
        
        mainWidget = qt.QWidget()
        self.addWidget(mainWidget)
        mainWidget.setLayout(qt.QVBoxLayout())

        newPipelineButton = qt.QPushButton("New pipeline study")
        mainWidget.layout().addWidget(newPipelineButton)
        newPipelineButton.connect('clicked', self.newPipeline)

    def newPipeline(self, _):
        pass
