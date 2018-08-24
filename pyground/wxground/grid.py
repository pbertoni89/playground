import wx
import wx.grid
from wx.lib.scrolledpanel import ScrolledPanel
import numpy as np


dims_wh = (200, 640)


class TestPanel(ScrolledPanel):
    def __init__(self, parent):
        ScrolledPanel.__init__(self, parent, wx.ID_ANY, size=dims_wh)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self._create_table(), 1, wx.EXPAND | wx.ALL, 5)
        self.SetSizer(self.sizer)
        self.SetupScrolling()
        self.SetAutoLayout(1)

    def _create_table(self):
        _table = wx.grid.Grid(self, -1)
        _table.CreateGrid(0, 2)
        for i in xrange(512):  # Work normally If I use 1722 rows
            _table.AppendRows()
            _table.SetCellValue(i, 0, str(i))
            _table.SetCellValue(i, 1, str(np.random.rand()))
        return _table

class TestFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, wx.ID_ANY,
                title="Counts per seconds", size=dims_wh)
        self.fSizer = wx.BoxSizer(wx.VERTICAL)
        self.fSizer.Add(TestPanel(self), 1, wx.EXPAND)
        self.SetSizer(self.fSizer)
        self.Show()

if __name__ == "__main__":
    app = wx.App(False)
    frame = TestFrame()
    app.MainLoop()
