import medInria as med


@med.data_interface("JSON file", "A JSON formatted file.")
class JSONData(med.AbstractData):
    
    def __init__(self, data = {}):
        super().__init__(parent)
        self._data = data

    def clone(self):
        return type(self)(self._data)

    def output(self):
        return self.data()

    def data(self):
        return self._data

    def setData(self, data):
        self._data.update(data)

    
