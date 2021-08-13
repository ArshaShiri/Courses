import justpy as jp
from pandas.core.dtypes.common import classes

def app():
    webPage = jp.QuasarPage()

    # Search for Quasar style to see different styles that can be applied to divs.
    h1 = jp.QDiv(a=webPage, text="Analysis of Course Reviews", classes="text-h3 text-center q-pa-md")
    p1 = jp.QDiv(a=webPage, text="These graphs represnet course review analysis:")
    return webPage

jp.justpy(app)