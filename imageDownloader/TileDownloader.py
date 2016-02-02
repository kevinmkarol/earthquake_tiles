__author__ = 'Kevin'

import csv
import sys
import urllib
import os
from PIL import Image

API_KEY = "AIzaSyAtnQcVcLLHTd0peQLvh9JGbetWMnAr_mw"

sys.path.insert(0, './eskriett-googleMapDownloader')

with open('earthquakes_all_month.csv', 'rb') as functionValues:
    reader = csv.reader(functionValues, delimiter= ',', quotechar='|')
    for row in reader:
      try:
        lattitude = float(row[1])
        longitude = float(row[2])
        try:
          url = "https://maps.googleapis.com/maps/api/staticmap?maptype=satellite&center="+ str(lattitude) + "," + str(longitude) + "&zoom=14&size=640x400&key=" + API_KEY
          fullPath = "lat_" + str(lattitude) + "_lon_" + str(longitude)
          urllib.urlretrieve(url, fullPath)
          im = Image.open(fullPath)
          os.remove(fullPath)

        except IOError:
          print("Could not generate the image - try adjusting the zoom level and checking your coordinates")
        else:
          #Save the image to disk
          im.save("map_images/" + fullPath + ".png")
          print("The map has successfully been created")
      except:
        print("skipped line")