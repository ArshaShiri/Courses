import folium
import pandas

def getColorBasedOnElevation(elevation):
    if elevation < 1000:
        return "green"
    elif 1000 < elevation < 3000:
        return "orange"
    else:
        return "red"


data = pandas.read_csv("Volcanoes.txt")
latitude = list(data["LAT"])
longitude = list(data["LON"])
elevations = list(data["ELEV"])

map = folium.Map(location=[38.58, -99.09], zoom_start=6, tiles="Stamen Terrain")
featureGroup = folium.FeatureGroup(name="My Map")

for lt, ln, el in zip(latitude, longitude, elevations):
    featureGroup.add_child(folium.CircleMarker(location=[lt, ln],
                                               radius=6,
                                               popup="Elevation: " + str(el) + "m",
                                               fill_color=getColorBasedOnElevation(el),
                                               color="grey",
                                               fill_opacity=0.7))
    
map.add_child(featureGroup)
map.save("map.html")