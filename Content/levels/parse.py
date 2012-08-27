from PIL import Image
def level(folderName):
  print("Parsing " + folderName)
  tiles = Image.open(folderName+"/tiles.png")
  height = Image.open(folderName+"/height.png")
  units = Image.open(folderName+"/units.png")

  tilesPixels = tiles.load()
  heightPixels = height.load()
  unitsPixels = units.load()
  width, height = [int(x) for x in tiles.size]
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
  for x in range(int(width)):
    for y in range(int(height)):
      tPixel = tilesPixels[x, y] 
      hPixel = heightPixels[x, y]

      tile = {}
      rgb = tPixel[0:3]
      tileType = tileTypeDict[tuple(rgb)]
      if tileType == None:
        continue
      tile["type"] = tileType
      
      tileHeight = (255-hPixel[0])/float(heightDev);
      tile["pos"] = [x, tileHeight, y]
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

  #dump to file
  import json
  f = open(folderName+"/level.json", 'w')
  f.write(json.dumps(tiles, sort_keys=True, indent=2))
  f.close()
  
  # parse the units
  units = []
  for x in range(int(width)):
    for y in range(int(height)):
      uPixel = unitsPixels[x, y]
      unit = {}
      if uPixel[3] != 0:
        unit["playerId"] = 0
        for tile in tiles:
          if tile["pos"][0] == x and tile["pos"][2] == y:
            unit["tile"] = tiles.index(tile)
            break
        units.append(unit)
  #dump to file
  f = open(folderName+"/units.json", 'w')
  f.write(json.dumps(units, sort_keys=True, indent=2))
  f.close()

