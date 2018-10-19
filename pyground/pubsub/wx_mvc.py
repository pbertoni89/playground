"""
Adapted from wxPython website at http://wiki.wxpython.org/ModelViewController/.
:copyright: Copyright since 2006 by Oliver Schoenborn, all rights reserved.
:license: BSD, see LICENSE.txt for details.
"""

import wx
import sys

from pubsub import pub
from pubsub.utils.notification import useNotifyByWriteFile


# the following two modules don't know about each other yet will
# exchange data via pubsub:
# 	from wx_win1 import View
# 	from wx_win2 import ChangerWidget
# was for demo only. Imported here for convenience

#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


# verbose debug
useNotifyByWriteFile(sys.stdout)


print('pubsub API version', pub.VERSION_API)

#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


class View(wx.Frame):
    def __init__(self, parent=None):
        wx.Frame.__init__(self, parent, -1, "Main View")

        sizer = wx.BoxSizer(wx.VERTICAL)
        text = wx.StaticText(self, -1, "My Money")
        ctrl = wx.TextCtrl(self, -1, "")
        sizer.Add(text, 0, wx.EXPAND | wx.ALL)
        sizer.Add(ctrl, 0, wx.EXPAND | wx.ALL)

        self.moneyCtrl = ctrl
        ctrl.SetEditable(False)
        self.SetSizer(sizer)

        # subscribe to all "MONEY CHANGED" messages from the Model
        # to subscribe to ALL messages (topics), omit the second argument below
        pub.subscribe(self.set_money, "money_changed")

    def set_money(self, money):
        self.moneyCtrl.SetValue(str(money))


#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


class ChangerWidget(wx.Frame):
    CHANGE = 10  # by how much money changes every time click

    def __init__(self, parent=None):
        wx.Frame.__init__(self, parent, -1, "Changer View")

        sizer = wx.BoxSizer(wx.VERTICAL)
        self.add = wx.Button(self, -1, "Add Money")
        self.remove = wx.Button(self, -1, "Remove Money")
        sizer.Add(self.add, 0, wx.EXPAND | wx.ALL)
        sizer.Add(self.remove, 0, wx.EXPAND | wx.ALL)
        self.SetSizer(sizer)

        self.add.Bind(wx.EVT_BUTTON, self.on_add)
        self.remove.Bind(wx.EVT_BUTTON, self.on_remove)

    def on_add(self, evt):
        print('-----')
        pub.sendMessage("money_changing", amount=self.CHANGE)

    def on_remove(self, evt):
        print('-----')
        pub.sendMessage("money_changing", amount=- self.CHANGE)

#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


class Model:
    """
        The business data structure
    """
    def __init__(self):
        self.myMoney = 0

    def add_money(self, value):
        self.myMoney += value
        # now tell anyone who cares that the value has been changed
        pub.sendMessage("money_changed", money=self.myMoney)

    def remove_money(self, value):
        self.myMoney -= value
        # now tell anyone who cares that the value has been changed
        pub.sendMessage("money_changed", money=self.myMoney)

#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


class Controller:
    """
        the controller owns 1 model and 2 views
         - one for reading the current value
         - one for changing the current value
    """
    def __init__(self):
        self.model = Model()

        # set up the first frame which displays the current Model value
        self.view1 = View()
        self.view1.set_money(self.model.myMoney)

        # set up the second frame which allows the user to modify the Model's value
        self.view2 = ChangerWidget()

        self.view1.Show()
        self.view2.Show()

        pub.subscribe(self.change_money, 'money_changing')

    def change_money(self, amount):
        if amount >= 0:
            self.model.add_money(amount)
        else:
            self.model.remove_money(-amount)

#  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -


if __name__ == "__main__":
    app = wx.App()
    c = Controller()
    # sys.stdout = sys.__stdout__

    print('---- Starting main event loop ----')
    app.MainLoop()

print('---- Exited main event loop ----')
