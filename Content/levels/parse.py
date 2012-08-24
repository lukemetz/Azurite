from PIL import Image
i = Image.open("test.png")

pixels = i.load()
width, height = i.size
tiles = []

empty = (255, 255, 255)
stone = (100, 100, 100)
woods = (0, 100, 50)
plains = (240, 200, 30)
heightDev = 100.0

tileTypeDict = {};
tileTypeDict[empty] = None
tileTypeDict[stone] = "StoneTile"
tileTypeDict[woods] = "WoodsTile"
tileTypeDict[plains] = "PlainsTile"

#parse initial list of tiles
for x in range(width):
  for y in range(height):
    pixel = pixels[x, y] 
    tile = {}
    rgb = pixel[0:3]
    tileType = tileTypeDict[tuple(rgb)]
    if tileType == None:
      continue
    tile["type"] = tileType
    tile["pos"] = [x, pixel[3]/float(heightDev), y]
    tile["neighbors"] = []
    tiles.append(tile);

import math
#Connect touching tiles

def posDistance(t1, t2):
  return (math.fabs(t1[0] - t2[0]), math.fabs(t1[1] - t2[1]), math.fabs(t1[2] - t2[2]));

def isNeighbor(p1, p2):
  vec = posDistance(p1, p2)
  return vec[0] + vec[2] == 1

for t1 in tiles:
  index = 0
  for t2 in tiles:
    if isNeighbor(t1["pos"], t2["pos"]):
      t1["neighbors"].append(index)
    index += 1


import json
f = open("level.json", 'w')
f.write(json.dumps(tiles, sort_keys=True, indent=2))
f.close()

