import justpy as jp

import pandas
from datetime import datetime
from pytz import UTC

# Parse the dates under Timestamp column as dates not strings.
data = pandas.read_csv("reviews.csv", parse_dates=['Timestamp'])
data['Month'] = data['Timestamp'].dt.strftime('%Y-%m')
month_avarage_course = data.groupby(['Month', 'Course Name'])['Rating'].count().unstack()

chart_definition = """
{
    chart: {
        type: 'spline'
    },
    title: {
        text: 'Average fruit consumption during one week'
    },
    legend: {
        layout: 'vertical',
        align: 'left',
        verticalAlign: 'top',
        x: 150,
        y: 100,
        floating: false,
        borderWidth: 1,
        backgroundColor:
            '#FFFFFF'
    },
    xAxis: {
        categories: [
            'Monday',
            'Tuesday',
            'Wednesday',
            'Thursday',
            'Friday',
            'Saturday',
            'Sunday'
        ],
        plotBands: [{ // visualize the weekend
            from: 4.5,
            to: 6.5,
            color: 'rgba(68, 170, 213, .2)'
        }]
    },
    yAxis: {
        title: {
            text: 'Fruit units'
        }
    },
    tooltip: {
        shared: true,
        valueSuffix: ' units'
    },
    credits: {
        enabled: false
    },
    plotOptions: {
        areaspline: {
            fillOpacity: 0.5
        }
    },
    series: [{
        name: 'John',
        data: [3, 4, 3, 5, 4, 10, 12]
    }, {
        name: 'Jane',
        data: [1, 3, 4, 3, 3, 5, 4]
    }]
}
"""

def app():
    webPage = jp.QuasarPage()

    # Search for Quasar style to see different styles that can be applied to divs.
    h1 = jp.QDiv(a=webPage, text="Analysis of Course Reviews", classes="text-h3 text-center q-pa-md")
    p1 = jp.QDiv(a=webPage, text="These graphs represnet course review analysis:")

    # Search for highcharts documentation to pick a chart.
    highcharts = jp.HighCharts(a=webPage, options=chart_definition)
    highcharts.options.xAxis.categories = list(month_avarage_course.index)
    highchartsData = [{"name":v1, "data":[v2 for v2 in month_avarage_course[v1]]} for v1 in month_avarage_course.columns] 
    highcharts.options.series = highchartsData

    return webPage

jp.justpy(app)