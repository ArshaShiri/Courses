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
volcanoesFeatureGroup = folium.FeatureGroup(name="Volcanoes")

for lt, ln, el in zip(latitude, longitude, elevations):
    volcanoesFeatureGroup.add_child(folium.CircleMarker(location=[lt, ln],
                                               radius=6,
                                               popup="Elevation: " + str(el) + "m",
                                               fill_color=getColorBasedOnElevation(el),
                                               color="grey",
                                               fill_opacity=0.7))

populationFeatureGroup = folium.FeatureGroup(name="Population")                                               

populationFeatureGroup.add_child(folium.GeoJson(data=open("world.json", "r", encoding="utf-8-sig").read(), 
                                      style_function=lambda x: {"fillColor":"yellow" if x["properties"]["POP2005"] < 10000000 
                                                                                     else "orange" if 10000000 <= x["properties"]["POP2005"] < 20000000 
                                                                                     else "red"}))                                                                                     
    
map.add_child(volcanoesFeatureGroup)
map.add_child(populationFeatureGroup)
map.add_child(folium.LayerControl())

map.save("map.html")