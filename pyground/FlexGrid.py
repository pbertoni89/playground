import wx


class Example(wx.Frame):

    def __init__(self, parent, title):
        super(Example, self).__init__(parent, title=title, size=(600, 800))

        self.InitUI()
        self.Centre()
        self.Show()

    def InitUI(self):
        panel = wx.Panel(self)

		# - - - Add sizers
        sz_vbox_main = wx.BoxSizer(wx.VERTICAL)
        sz_fg = wx.FlexGridSizer(rows=5, cols=2, vgap=10, hgap=10)
        sz_hbox_ctrl = wx.BoxSizer(wx.HORIZONTAL)

        title = wx.StaticText(panel, label="Title")
        author = wx.StaticText(panel, label="Name of the Author")
        review = wx.StaticText(panel, label="Review")

        tc1 = wx.TextCtrl(panel)
        tc2 = wx.TextCtrl(panel)
        tc3 = wx.TextCtrl(panel, style=wx.TE_MULTILINE)

        # - - - Create "HBox" row
        ctrl_reset = wx.Button(panel, label="Reset")
        ctrl_foo = wx.RadioBox(panel, label="Multi", choices=["Arava", "Fava"])
        # three because FGS cols = 5
        ctrl_pseudo1, ctrl_pseudo2, ctrl_pseudo3 = wx.StaticText(panel, label=""), wx.StaticText(panel, label=""), wx.StaticText(panel, label="")

		# - - - Add example fields
        sz_fg.AddMany([title, (tc1, 1, wx.EXPAND), author,
                       (tc2, 1, wx.EXPAND), (review, 1, wx.EXPAND), (tc3, 1, wx.EXPAND)])
        # third row (review +  tc3) can grow
        sz_fg.AddGrowableRow(2, 1)
        # second col (tc1 + tc2 + tc3) can grow
        sz_fg.AddGrowableCol(1, 1)

        # - - - Add "Huge" row
        btn_huge = wx.Button(panel, label="Huge", size=(30, 50))
        text_huge = wx.StaticText(panel, label="I complete Huge row")
        sz_fg.Add(btn_huge, 1, wx.EXPAND)
        sz_fg.Add(text_huge, 1, wx.EXPAND)
        sz_fg.AddGrowableRow(3, 1)

		# - - - Add "HBox" row
        # sz_hbox_ctrl.AddMany([(ctrl_reset, 1, wx.EXPAND), (ctrl_foo, 1, wx.EXPAND), (ctrl_pseudo1, 1, wx.EXPAND), (ctrl_pseudo2, 1, wx.EXPAND), (ctrl_pseudo3, 1, wx.EXPAND)])
        sz_hbox_ctrl.AddMany([(ctrl_reset, 1, wx.EXPAND), (ctrl_foo, 1, wx.EXPAND)])
        sz_fg.Add(sz_hbox_ctrl, proportion=2, flag=wx.ALL | wx.EXPAND, border=10)

        # Finalize adding FGS to main VBOX
        sz_vbox_main.Add(sz_fg, proportion=1, flag=wx.ALL | wx.EXPAND, border=15)

        panel.SetSizer(sz_vbox_main)


app = wx.App()
Example(None, title='FlexiGrid Demo')
app.MainLoop()
