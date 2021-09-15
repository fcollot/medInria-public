import medInria as med


@med.toolbox("Pipeline selection", "Pipeline selection tool box", ["view"])
class SelectionToolBox(med.ToolBox):

    def __init__(self, parent=None):
        super().__init__(parent)
