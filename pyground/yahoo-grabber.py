#!/usr/bin/python
# -*- coding: utf-8 -*-

from pandas_datareader import data as pdr
import plotly
import plotly.graph_objs as go
import fix_yahoo_finance as yf

"""
	https://github.com/pydata/pandas-datareader/issues/487
"""

yf.pdr_override()


def import_data(stock, start, end):
    """
        Fix yahoo warning
        https://github.com/pydata/pandas-datareader/issues/487
        :param stock:
        :param start:
        :param end:
        :return:
    """
    d = pdr.get_data_yahoo(stock, start=start, end=end)
    return d


def plot_data(d):
    d_candle = go.Candlestick(x=d.index,
                              high=d.High,
                              low=d.Low,
                              close=d.Close,
                              increasing=dict(line=dict(color='#00FF00')),
                              decreasing=dict(line=dict(color='#FF0000')))

    layout = go.Layout(plot_bgcolor='rgb(59,68,75)')
    deata = [d_candle]
    f = go.Figure(data=deata, layout=layout)
    return f


if __name__ == "__main__":
    #data = import_data(["JPM", "MCD"], start="2005-07-01", end="2015-07-31")
    data = import_data("MCD", start="2005-07-01", end="2015-07-31")

    fig = plot_data(data)

    plot_url = plotly.offline.plot(fig, filename='styling-names.html')
