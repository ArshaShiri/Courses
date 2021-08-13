import justpy as jp

import pandas
from datetime import datetime
from pytz import UTC

# Parse the dates under Timestamp column as dates not strings.
data = pandas.read_csv("reviews.csv", parse_dates=['Timestamp'])
data['Week'] = data['Timestamp'].dt.strftime('%Y-%U')
week_avarage = data.groupby(['Week']).mean()

chart_definition = """
{
    chart: {
        type: 'spline',
        inverted: false
    },
    title: {
        text: 'Atmosphere Temperature by Altitude'
    },
    subtitle: {
        text: 'According to the Standard Atmosphere Model'
    },
    xAxis: {
        reversed: false,
        title: {
            enabled: true,
            text: 'Date'
        },
        labels: {
            format: '{value}'
        },
        accessibility: {
            rangeDescription: 'Range: 0 to 80 km.'
        },
        maxPadding: 0.05,
        showLastLabel: true
    },
    yAxis: {
        title: {
            text: 'Average Rating'
        },
        labels: {
            format: '{value}'
        },
        accessibility: {
            rangeDescription: 'Range: -90°C to 20°C.'
        },
        lineWidth: 2
    },
    legend: {
        enabled: false
    },
    tooltip: {
        headerFormat: '<b>{series.name}</b><br/>',
        pointFormat: '{point.x} {point.y}'
    },
    plotOptions: {
        spline: {
            marker: {
                enable: false
            }
        }
    },
    series: [{
        name: 'Average Dating',
        data: [[0, 15], [10, -50], [20, -56.5], [30, -46.5], [40, -22.1],
            [50, -2.5], [60, -27.7], [70, -55.7], [80, -76.5]]
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
    highcharts.options.title.text = "Average Rating by Week"
    
    # x = [3, 6, 8]
    # y = [4, 7, 8]
    # highcharts.options.series[0].data = list(zip(x.index, y)) is the same as:
    # highcharts.options.series[0].data = [[3, 4], [6, 7], [8, 9]]
    highcharts.options.xAxis.categories = list(week_avarage.index) 
    highcharts.options.series[0].data = list(week_avarage['Rating']) 

    return webPage

jp.justpy(app)