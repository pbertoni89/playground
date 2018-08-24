#!/usr/bin/python
# -*- coding: utf-8 -*-

import wx
# import wx.lib.mixins.inspection as wit


from noname import MyFrame1


class FrameMainConcrete(MyFrame1):

    def __init__(self, parent):
        """
            :param parent:
        """
        super(FrameMainConcrete, self).__init__(parent)


if __name__ == '__main__':
    """
        Use wx.App xor wit.InspectableApp (a great debug tool, see)
        http://wiki.wxpython.org/Widget%20Inspection%20Tool
    """
    # app = wit.InspectableApp()
    app = wx.App()

    frame = FrameMainConcrete(None)
    frame.Show()
    app.MainLoop()
